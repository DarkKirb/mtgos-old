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
#ifdef I686
struct cpu_state {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    uint32_t intr;
    uint32_t error;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};
#else
#ifdef X86_64
struct cpu_state {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;

    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t intr;
    uint64_t error;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};
#else
#endif
#endif
#endif
#endif
