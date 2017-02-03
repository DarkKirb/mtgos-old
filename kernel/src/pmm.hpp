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
#ifdef PMM_STANDARD
#include "drivers/pmm.hpp"
#define PMMD drivers::mm::_PMM
#else
#ifdef PMM_STACK
#include "drivers/pmm_stack.hpp"
#define PMMD drivers::mm::PMMStack
#endif
#endif
