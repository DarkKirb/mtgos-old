//Standard PMM for n-bit systems
//Should work with anything.
#pragma once
#include "../pmm.hpp"
#include <stdint.h>
#include <defines.h>
#define MAX_PHYS (1ull<<PHYS_BITS)
#define NO_PAGES (MAX_PHYS/PAGE_SIZE)
namespace drivers {
namespace mm {
struct PMMList {
PMMList *last;
PMMList *next;
};
class PMMStack: public MTGos::PMM {
private:
    PMMList *head,*tail;
    virtual auto push(uintptr_t) -> void;
    virtual auto pop() -> uintptr_t;
public:
    PMMStack(multiboot_info_t* mb_info);
    virtual ~PMMStack() {};
    virtual auto alloc(uint64_t) -> void*;
    virtual auto free(void*, uint64_t) -> void;
    virtual auto isFree(void*) -> bool;
    virtual auto markUsed(void*) -> bool;
};
}
}
