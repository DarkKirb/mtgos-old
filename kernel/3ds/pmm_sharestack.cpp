#include "../src/pmm.hpp"
#include "../src/base.hpp"
#include <stdint.h>
extern "C" int kernel_start;
extern "C" int kernel_end;
namespace drivers {
namespace mm {
PMMShareStack::PMMShareStack(multiboot_info_t* mb_info): MTGos::PMM(mb_info)
#ifdef _3DS9
, a9int(mb_info)
#endif
  {
    uintptr_t cmax=0x20000000;
    kout << MTGos::LogLevel::INFO << (uint64_t)cmax << " ";
    cmax -= sizeof(uintptr_t);
    kout << (uint64_t)cmax << " ";
    lock=(volatile uintptr_t*)(cmax);
    cmax-=sizeof(PMMList*);
    kout << (uint64_t)cmax << " ";
    head=(volatile PMMList**)cmax;
    cmax-=sizeof(PMMList*);
    kout << (uint64_t)cmax << "\n";
    tail=(volatile PMMList**)cmax;
    *head=*tail=nullptr;
#ifdef _3DS9
    *lock=false; //First CPU to reach this#
    while(!(*lock));
    while(*lock);
#else
    kout << MTGos::LogLevel::INFO << "Waiting for ARM9\n";
    while(*lock);
    *lock=true;
    volatile uintptr_t* tlock=lock;
    uintptr_t l=false;
    lock=&l;
    PMMStack temppmm(mb_info);
    head=tail=nullptr;
    for(uintptr_t i=PAGE_SIZE;(i!=0) && i < MAX_PHYS; i += PAGE_SIZE) {
        if(!temppmm.isFree((void*)i))
            continue;
        push(i);
    }
    lock=tlock;
    //Done!
    *lock=false;
#endif
}
auto PMMShareStack::isFree(void* ptr) -> bool{
    uintptr_t p=(uintptr_t)ptr;
#ifdef _3DS9
    if(p < 0x1FF80000) {
        return a9int.isFree(ptr);
    }
#endif
    kout << MTGos::LogLevel::INFO << "Waiting for other CPU\n";
    while(*lock);
    *lock=true;
    for(volatile PMMList *curr=*head;curr!=nullptr;curr=curr->next) {
        if((uintptr_t)curr == p) {
            *lock=false;
            return true;
        }
    }
    *lock=false;
    return false;
}
auto PMMShareStack::markUsed(void* ptr) -> bool {
    uintptr_t p=(uintptr_t)ptr;
    #ifdef _3DS9
        if(p < 0x1FF80000) {
            return a9int.markUsed(ptr);
        }
    #endif
    kout << MTGos::LogLevel::INFO << "Waiting for other CPU\n";
    while(*lock);
    *lock=true;
    kout << MTGos::LogLevel::INFO << "Marking " << (uint64_t)p << " as used\n";
    for(volatile PMMList *curr=*head;curr!=nullptr;curr=curr->next) {

        if((uintptr_t)curr == p) {
            if(curr==*head)
                *head=curr->next;
            if(curr==*tail)
                *tail=curr->last;
            if(curr->next)
                curr->next->last=curr->last;
            if(curr->last)
                curr->last->next=curr->next;
            *lock=false;
            return true;
        }
    }
    *lock=false;
    return false;
}
auto PMMShareStack::push(uintptr_t p) -> void {
    if((uint64_t)p>=(uint64_t)MAX_PHYS)
        return;
    volatile PMMList *curr=(PMMList*)p;
    curr->last=(PMMList*)(*tail);
    if(!(*head))
        *head=curr;
    if(tail)
        (*tail)->next=(PMMList*)curr;
    *tail=curr;
    kout << MTGos::LogLevel::DEBUG << "Freeing " << (uint64_t)((uintptr_t)p) << "\n";
}
auto PMMShareStack::pop() -> uintptr_t {
    volatile PMMList *curr=*head;
    *head=curr->next;
    if(curr->next)
        curr->next->last=nullptr;
    return (uintptr_t)curr;
}
auto PMMShareStack::free(void* ptr, uint64_t size) -> void {
    uintptr_t p=(uintptr_t)ptr;
    #ifdef _3DS9
        if(p < 0x1FF80000) {
            return a9int.free(ptr,size);
        }
    #endif
    kout << MTGos::LogLevel::INFO << "Waiting for other CPU\n";
    while(*lock);
    if((uintptr_t)ptr >= MAX_PHYS)
        return;
    *lock=true;
    for(uintptr_t i=(uintptr_t)ptr;i<((uintptr_t)ptr)+size;i+=PAGE_SIZE)
        push(i);
    *lock=false;
}
auto PMMShareStack::alloc(uint64_t size) -> void * {
    #ifdef _3DS9
        void* tptr=a9int.alloc(size);
        if(tptr)
            return tptr;
    #endif
    kout << MTGos::LogLevel::INFO << "Waiting for other CPU\n";
    while(*lock);
    *lock=true;
    uint64_t pages = ((size+PAGE_SIZE)/PAGE_SIZE)-1;
    if(!pages)
        pages=1;
    kout << pages;
    if(pages==1) {
        void* ptr=(void*)(pop());
        *lock=false;
        return ptr;
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
    *lock=true;
    return nullptr;
}
}
}
