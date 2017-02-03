#include "pmm.hpp"
#include "../base.hpp"
#include <stdint.h>
extern "C" int kernel_start;
extern "C" int kernel_end;
namespace drivers {
namespace mm {
PMMStack::PMMStack(multiboot_info_t* mb_info): MTGos::PMM(mb_info) {
    multiboot_mmap_entry* mmap=(multiboot_mmap_entry*)((uintptr_t)mb_info->mmap_addr);
    uintptr_t cmax=0;
    for(uint32_t i=0;i<(mb_info->mmap_length/sizeof(multiboot_mmap_entry));i++) {
        if(mmap[i].type==MULTIBOOT_MEMORY_AVAILABLE) {
            cmax=mmap[i].addr+mmap[i].len > cmax ? mmap[i].addr+mmap[i].len : cmax;
        }
    }
    max=sp=(uintptr_t*)cmax;
    sp-=sizeof(uintptr_t);
    //First, free all free memory
    for(uint32_t i=0;i<(mb_info->mmap_length/sizeof(multiboot_mmap_entry));i++) {
        if(mmap[i].type==MULTIBOOT_MEMORY_AVAILABLE)
            free((void*)((uintptr_t*)mmap[i].addr),mmap[i].len);
    }
    //Mark the stack
    for(uintptr_t i=((uintptr_t)sp)&(~(PAGE_SIZE-1));i<cmax;i+=PAGE_SIZE) {
        markUsed((void*)i);
    }
    //Next, mark the mb_struct as used
    markUsed((void*)(((uintptr_t)mb_info)&(~(PAGE_SIZE-1))));
    markUsed(nullptr);
    //Then, all of the kernel
    for(uintptr_t i=(uintptr_t)(&kernel_start);i<(uintptr_t)(&kernel_end);i+=PAGE_SIZE) {
        markUsed((void*)i);
    }
    //Done!
}
auto PMMStack::isFree(void* ptr) -> bool{
    uintptr_t p;
    for(uintptr_t* i=sp;i<max;i++) {
        if(p==*i)
            return true;
    }
    return false;
}
auto PMMStack::markUsed(void* ptr) -> bool {
    uintptr_t p=(uintptr_t)ptr;
    kout << MTGos::LogLevel::DEBUG << "Marking " << (uint64_t)p << " as used\n";
    for(uintptr_t* i=sp;i<max;i++) {
        if(p==*i) {
            for(uintptr_t* j=i; j>sp; --j) {
                *j=j[-1];
            }
            sp++;
            return true;
        }
    }
    return false;
}
auto PMMStack::push(uintptr_t p) -> void {
    if((uint64_t)p>=(uint64_t)MAX_PHYS)
        return;
    *sp=p;
    sp--;
    kout << MTGos::LogLevel::DEBUG << "Freeing " << (uint64_t)((uintptr_t)p) << "\n";
}
auto PMMStack::pop() -> uintptr_t {
    sp++;
    kout << MTGos::LogLevel::DEBUG << "Allocating " << (uint64_t)(*sp) << "\n";
    return *sp;
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
    kout << pages;
    if(pages==1)
        return (void*)(pop());
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
