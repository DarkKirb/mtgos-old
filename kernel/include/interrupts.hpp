#pragma once
#include <stdint.h>
enum class Interrupts {
    INVALID_OPCODE, //Raised when an invalid opcode exception occurs
    PAGE_FAULT, //Raised when an data exception occurs while paging is activated
    GENERAL_PROTECTION, //Raised when an data exception occurs while paging is deactivated
    TIMER_IRQ, //Used to initiate a task switch
    KEYBOARD_IRQ, //Used to tell the keyboard driver that a character has arrived
    IPC_IRQ, //Used to tell the CPU that another CPU is sending you data.
    DISK_IRQ1,
    DISK_IRQ2, //Used to tell the system that the disk finished reading data.
    SYSCALL
};
union vfpfloat {
    struct {float s1; float s2;};
    double d;
};
#ifdef _3DS11
struct cpu_state {
    uint32_t spsr;
    vfpfloat d0;
    vfpfloat d1;
    vfpfloat d2;
    vfpfloat d3;
    vfpfloat d4;
    vfpfloat d5;
    vfpfloat d6;
    vfpfloat d7;
    vfpfloat d8;
    vfpfloat d9;
    vfpfloat d10;
    vfpfloat d11;
    vfpfloat d12;
    vfpfloat d13;
    vfpfloat d14;
    vfpfloat d15;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;
};
#else
#ifdef _3DS9
struct cpu_state {
    uint32_t spsr;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;
};
#else
#endif
#endif
