//Standard PMM for n-bit systems
//Should work with anything.
#pragma once
#include "../pmm.hpp"
#include <stdint.h>
#include <defines.h>
#define MAX_PHYS (1ull<<PHYS_BITS)
#define NO_PAGES (MAX_PHYS/PAGE_SIZE)
#define NO_ENTRIES (NO_PAGES/32)
namespace drivers {
namespace mm {
class _PMM: public MTGos::PMM {
private:
    uint32_t bitmap[NO_ENTRIES];
public:
    _PMM(multiboot_info_t* mb_info);
    virtual ~_PMM() {};
    virtual auto alloc(uint64_t) -> void*;
    virtual auto free(void*, uint64_t) -> void;
    virtual auto isFree(void*) -> bool;
    virtual auto markUsed(void*) -> bool;
};
}
}
