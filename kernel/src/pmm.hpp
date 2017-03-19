#pragma once
#include <multiboot.h>
#include <stdint.h>
namespace MTGos {
class PMM {
protected:
    multiboot_info_t* mb_info;
public:
    PMM(multiboot_info_t* mb_info): mb_info(mb_info) {};
    virtual ~PMM() {};
    virtual auto alloc(uint64_t) -> void* = 0;
    virtual auto free(void*, uint64_t) -> void = 0;
    virtual auto isFree(void*) -> bool = 0;
    virtual auto markUsed(void*) -> bool = 0;
};
}
#ifndef __EXTERNAL
#include "drivers/pmm_stack.hpp"
#if defined(_3DS9) || defined(_3DS11)
#include "../3ds/pmm_sharestack.hpp"
#endif
#ifdef PMM_STACK
#define PMMD drivers::mm::PMMStack
#else
#ifdef PMM_SHAREDSTACK
#define PMMD drivers::mm::PMMShareStack
#endif
#endif
#else
#include "drivers/pmm_stack.hpp"
#define PMMD drivers::mm::PMMStack
#endif
