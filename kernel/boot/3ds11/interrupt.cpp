#include <interrupts.hpp>
#include "../src/base.hpp"
extern "C" void interruptHandler(uint32_t number, cpu_state* state) {
    out << "Interrupt " << (uint64_t)number << " occurred\n";
    if(number >= 4) {
        out << "Exception at " << (uint64_t)(state->lr) << "\nHalting!\n";
        for(;;);
    }
}
