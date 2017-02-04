#include "../base.hpp"
#include <stdint.h>
extern "C" int kernel_start;
extern "C" int kernel_end;
namespace drivers {
namespace mm {
PMMStack::PMMStack(multiboot_info_t* mb_info): MTGos::PMM(mb_info) {
    auto isUsed=[mb_info](uintptr_t addr) -> bool {
        if(addr == 0)
            return true;
        else if(addr == (uintptr_t)mb_info)
            return true;
        else if((addr >= (uintptr_t)(&kernel_start)) && (addr < (uintptr_t)(&kernel_end)))
            return true;
        else
            return false;
    };
    head=tail=nullptr;
    multiboot_mmap_entry* mmap=(multiboot_mmap_entry*)((uintptr_t)mb_info->mmap_addr);
    for(uint32_t i=0;i<(mb_info->mmap_length/sizeof(multiboot_mmap_entry));i++) {
        if(mmap[i].type==MULTIBOOT_MEMORY_AVAILABLE) {
            for(uintptr_t p=(uintptr_t)(mmap[i].addr); p<(uintptr_t)(mmap[i].addr+mmap[i].len); p+= PAGE_SIZE) {
                if(isUsed(p))
                    continue;
                push(p); //Mark as free
            }
        }
    }
}
auto PMMStack::isFree(void* ptr) -> bool{
    uintptr_t p=(uintptr_t)ptr;
    for(PMMList *curr=head;curr!=nullptr;curr=curr->next) {
        if((uintptr_t)curr == p)
            return true;
    }
    return false;
}
auto PMMStack::markUsed(void* ptr) -> bool {
    uintptr_t p=(uintptr_t)ptr;
    kout << MTGos::LogLevel::DEBUG << "Marking " << (uint64_t)p << " as used\n";
    for(PMMList *curr=head;curr!=nullptr;curr=curr->next) {

        if((uintptr_t)curr == p) {
            if(curr==head)
                head=curr->next;
            if(curr==tail)
                tail=curr->last;
            if(curr->next)
                curr->next->last=curr->last;
            if(curr->last)
                curr->last->next=curr->next;
            return true;
        }
    }
    return false;
}
auto PMMStack::push(uintptr_t p) -> void {
    if((uint64_t)p>=(uint64_t)MAX_PHYS)
        return;
    PMMList *curr=(PMMList*)p;
    curr->last=tail;
    curr->next=nullptr;
    if(!head)
        head=curr;
    if(tail)
        tail->next=curr;
    tail=curr;
    kout << MTGos::LogLevel::DEBUG << "Freeing " << (uint64_t)((uintptr_t)p) << "\n";
}
auto PMMStack::pop() -> uintptr_t {
    if(!(head)) {
#ifndef _3DS9
        panic("Oops. We just ran out of memory!\n");
#endif
        return 0;
    }
    PMMList *curr=head;
    head=curr->next;
    if(curr->next)
        curr->next->last=nullptr;
    return (uintptr_t)curr;
}
auto PMMStack::free(void* ptr, uint64_t size) -> void {
    if((uintptr_t)ptr >= MAX_PHYS)
        return;
    for(uintptr_t i=(uintptr_t)ptr;i<((uintptr_t)ptr)+size;i+=PAGE_SIZE)
        push(i);
}
auto PMMStack::alloc(uint64_t size) -> void * {
    uint64_t pages = ((size+PAGE_SIZE)/PAGE_SIZE)-1;
    if(!pages)
        pages=1;
    if(pages==1) {
        return (void*)(pop());
    }
    for(uintptr_t i=0;i<=MAX_PHYS;i+=PAGE_SIZE) {
        if(!isFree((void*)i))
            continue;
        if(!isFree((void*)(i+pages*PAGE_SIZE))) {
            i+=pages*PAGE_SIZE;
            continue;
        }
        bool found=true;
        for(uintptr_t j=i+PAGE_SIZE;j<i+pages*PAGE_SIZE;j+=PAGE_SIZE) {
            if(!isFree((void*)j)) {
                i=j+=PAGE_SIZE;
                found=false;
                break;
            }
        }
        if(!found)
            continue;

        for(uintptr_t j=i; j<i+pages*PAGE_SIZE; j+=PAGE_SIZE) {
            markUsed((void*)j);
        }
        return (void*)i;
    }
    return nullptr;
}
}
}
