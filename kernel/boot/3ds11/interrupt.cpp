#include <interrupts.hpp>
#include "../src/base.hpp"
extern "C" void interruptHandler(uint32_t number, cpu_state* state) {
    out << "Interrupt " << (uint64_t)number << " occurred\n";
    out << "CPSR: " << (uint64_t)state->spsr << ", PC: " << (uint64_t)state->lr << "\n";
    out << "R0: " << (uint64_t)state->r0 << ", R1: " << (uint64_t)state->r1 << "\n";
    out << "R2: " << (uint64_t)state->r2 << ", R3: " << (uint64_t)state->r3 << "\n";
    out << "R4: " << (uint64_t)state->r4 << ", R5: " << (uint64_t)state->r5 << "\n";
    out << "R6: " << (uint64_t)state->r6 << ", R7: " << (uint64_t)state->r7 << "\n";
    out << "R8: " << (uint64_t)state->r8 << ", R9: " << (uint64_t)state->r9 << "\n";
    out << "R10: " << (uint64_t)state->r10 << ", R11: " << (uint64_t)state->r11 << "\n";
    out << "R12: " <<(uint64_t)state->r12 << ", SP: " << (uint64_t)((uintptr_t)(state)) << "\n";
    if(number >= 3) {
        out << "Exception at " << (uint64_t)(state->lr) << "\nHalting!\n";
        for(;;);
    }
}
