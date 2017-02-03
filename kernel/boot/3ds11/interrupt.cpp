#include <interrupts.hpp>
#include "../src/base.hpp"
extern "C" void interruptHandler(uint32_t number, cpu_state* state) {
    out << "Interrupt " << (uint64_t)number << " occurred\n";
    out << (uint64_t)state->spsr << ";" << (uint64_t)state->lr << "\n";
    out << (uint64_t)state->r0 << ";" << (uint64_t)state->r1 << "\n";
    out << (uint64_t)state->r2 << ";" << (uint64_t)state->r3 << "\n";
    out << (uint64_t)state->r4 << ";" << (uint64_t)state->r5 << "\n";
    out << (uint64_t)state->r6 << ";" << (uint64_t)state->r7 << "\n";
    out << (uint64_t)state->r8 << ";" << (uint64_t)state->r9 << "\n";
    out << (uint64_t)state->r10 << ";" << (uint64_t)state->r11 << "\n";
    out << (uint64_t)state->r12 << ";" << (uint64_t)((uintptr_t)(state)) << "\n";
    if(number >= 3) {
        out << "Exception at " << (uint64_t)(state->lr) << "\nHalting!\n";
        for(;;);
    }
}
