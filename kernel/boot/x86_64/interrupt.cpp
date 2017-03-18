#include <stdint.h>
#include <interrupts.hpp>
#include <base.hpp>
struct IDTent {
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t zero2;
}__attribute__((packed));
struct IDT {
    uint16_t size;
    uint32_t off;
}__attribute__((packed));
static IDT idt;
static IDTent entries[256];
void idt_setEntry(int id,uintptr_t addr) {
    entries[id].offset1=(uint16_t)addr;
    entries[id].selector=0x28;
    entries[id].zero=0;
    entries[id].type=0b11101110;
    entries[id].offset2=(uint16_t)(addr>>16);
    entries[id].offset3=(uint32_t)(addr>>32);
    entries[id].zero2=0;
}
extern "C" void * intr_stub_0;
extern "C" void loadIDT() {
    uintptr_t p=(uintptr_t)&intr_stub_0;
    for(int i=0;i<256;i++) {
        idt_setEntry(i,p+i*32);
    }
    idt.size=sizeof(entries);
    idt.off=(uint32_t)((uintptr_t)(&entries));
    asm volatile("lidt %0" : : "m"(idt));
}
extern "C" void panic2(char* text, cpu_state* cpu) {
    out << "=== KERNEL PANIC ===\n";
    out << "MTGos encountered a problem it couldn't resolve\n";
    out << "Please open a issue at bit.ly/mtgos if you cannot resolve this on your own\n";
    out << "rax: " << (uint64_t)cpu->rax << ", rbx: " << (uint64_t)cpu->rbx << "\n";
    out << "rcx: " << (uint64_t)cpu->rcx << ", rdx: " << (uint64_t)cpu->rdx << "\n";
    out << "rsi: " << (uint64_t)cpu->rsi << ", rdi: " << (uint64_t)cpu->rdi << "\n";
    out << "rbp: " << (uint64_t)cpu->rbp << ", rip: " << (uint64_t)cpu->intr << "\n";
    out << "r8: " << (uint64_t)cpu->r8 << ", r9: " << (uint64_t)cpu->r9 << "\n";
    out << "r10: " << (uint64_t)cpu->r10 << ", r11: " << (uint64_t)cpu->r11 << "\n";
    out << "r12: " << (uint64_t)cpu->r12 << ", r13: " << (uint64_t)cpu->r13 << "\n";
    out << "r14: " << (uint64_t)cpu->r14 << ", r15: " << (uint64_t)cpu->r15 << "\n";
    out << "x86_64-" << (uint64_t) BUILDID << "\n";
    out << text << "\nHalting...";
    for(;;);
}
extern "C" cpu_state* handleINT(cpu_state* cpu) {
    out << "Interrupt " << (uint64_t)cpu->intr << " occurred\n";
    out << "cs:rip: " << (uint64_t)cpu->cs << ":" << (uint64_t)cpu->rip << "\n";
    out << "ss:rsp: " << (uint64_t)cpu->ss << ":" << (uint64_t)cpu->rsp << "\n";
    if(cpu->intr < 0x20) {
        char txts[0x20][4] = {
            "#DE", "#DB", "NMI", "#BP", "#OF", "#BR", "#UD", "#NM",
            "#DF", "CSO", "#TS", "#NP", "#SS", "#GP", "#PF", "x0F",
            "#MF", "#AC", "#MC", "#XF", "#VE", "x15", "x16", "x17",
            "x18", "x19", "x1A", "x1B", "x1C", "x1D", "#SX", "x1F"
        };
        panic2(txts[cpu->intr],cpu);
        for(;;);
    }
    return cpu;
}
