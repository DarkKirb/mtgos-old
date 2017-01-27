#include "pmm.hpp"
#include "../base.hpp"
#include <stdint.h>
extern "C" int kernel_start;
extern "C" int kernel_end;
namespace drivers {
namespace mm {
_PMM::_PMM(multiboot_info_t* mb_info): MTGos::PMM(mb_info) {

    //Mark all memory as used
    for(int i=0;i<NO_ENTRIES;i++)
        bitmap[i]=0;
    multiboot_mmap_entry* mmap=(multiboot_mmap_entry*)((uintptr_t)mb_info->mmap_addr);
    //First, free all free memory
    for(uint32_t i=0;i<mb_info->mmap_length;i++) {
        if(mmap[i].type==MULTIBOOT_MEMORY_AVAILABLE)
            free((void*)((uintptr_t*)mmap[i].addr),mmap[i].len);
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
auto _PMM::isFree(void* ptr) -> bool{
    uintptr_t p=(uintptr_t)ptr;
    p&=~(PAGE_SIZE-1);
    uint64_t entry=p/PAGE_SIZE;
    uint8_t bit = entry % 32;
    entry/=32;
    return !(!(bitmap[entry]&(1<<bit)));
}
auto _PMM::markUsed(void* ptr) -> bool {
    uintptr_t p=(uintptr_t)ptr;
    p&=~(PAGE_SIZE-1);
    uint64_t entry=p/PAGE_SIZE;
    uint8_t bit = entry % 32;
    entry/=32;
    if(!(bitmap[entry]&(1<<bit)))
        return false;
    bitmap[entry] &= ~(1<<bit);
    return true;
}
auto _PMM::free(void* ptr, uint64_t size) -> void {
    if((uintptr_t)ptr >= MAX_PHYS)
        return;
    uint64_t pages = (size+PAGE_SIZE-1)/PAGE_SIZE;
    for(uintptr_t i=0,p = ((uintptr_t)ptr)&(~(PAGE_SIZE-1));i<pages;i++,p+=PAGE_SIZE) {
        //kout << MTGos::LogLevel::INFO << (uint64_t)p << "\n";
        uint64_t entry=p/PAGE_SIZE;
        uint8_t bit = entry % 32;
        entry/=32;
        if(entry>=NO_ENTRIES)
            return;
        bitmap[entry] |= 1<<bit;
    }
}
auto _PMM::alloc(uint64_t size) -> void * {
    uint64_t pages = (size+PAGE_SIZE-1)/PAGE_SIZE;
    for(uintptr_t p=PAGE_SIZE;p&&(p<MAX_PHYS-1);p+=PAGE_SIZE) {
        if(!isFree((void*)p))
            continue;
        //Found a free initial page!
        if(!isFree((void*)(p+pages*PAGE_SIZE))) {
            //But it doesn't fit.
            p+=pages*PAGE_SIZE;
            p-=PAGE_SIZE;
            continue;
        }
        bool broke=false;
        for(uintptr_t q=p;q<p+pages*PAGE_SIZE;q+=PAGE_SIZE) {
            if(!isFree((void*)q)) {
                //Not free in the middle.
                broke=true;
                p=q;
                break;
            }
        }
        if(broke)
            continue;
        //We found one!
        for(uintptr_t q=p;q<p+pages*PAGE_SIZE;q+=PAGE_SIZE) {
            markUsed((void*)q);
        }
        return (void*)p;
    }
    return nullptr;
}
}
}
