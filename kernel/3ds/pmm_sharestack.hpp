//Standard PMM for n-bit systems
//Should work with anything.
#pragma once
#include "../src/drivers/pmm_stack.hpp"
#include <stdint.h>
#include <defines.h>
#define MAX_PHYS (1ull<<PHYS_BITS)
#define NO_PAGES (MAX_PHYS/PAGE_SIZE)
namespace drivers {
namespace mm {
class PMMShareStack: public MTGos::PMM {
private:
#ifdef _3DS9
    PMMStack a9int;
#endif
    volatile uintptr_t * lock;
    volatile PMMList **head;
    volatile PMMList **tail;
    virtual auto push(uintptr_t) -> void;
    virtual auto pop() -> uintptr_t;
public:
    PMMShareStack(multiboot_info_t* mb_info);
    virtual ~PMMShareStack() {};
    virtual auto alloc(uint64_t) -> void*;
    virtual auto free(void*, uint64_t) -> void;
    virtual auto isFree(void*) -> bool;
    virtual auto markUsed(void*) -> bool;
};
}
}
