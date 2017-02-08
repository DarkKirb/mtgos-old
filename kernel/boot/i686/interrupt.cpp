#include <stdint.h>
#include <interrupts.hpp>
#include "../src/base.hpp"
struct IDTent {
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset2;
}__attribute__((packed));
struct IDT {
    uint16_t size;
    uint32_t off;
}__attribute__((packed));
static IDT idt;
static IDTent entries[256];
void idt_setEntry(int id,uintptr_t addr) {
    entries[id].offset1=(uint16_t)addr;
    entries[id].selector=0x08;
    entries[id].zero=0;
    entries[id].type=0b11101110;
    entries[id].offset2=(uint16_t)(addr>>16);
}
extern "C" void * intr_stub_0;
extern "C" void loadIDT() {
    uintptr_t p=(uintptr_t)&intr_stub_0;
    for(int i=0;i<256;i++) {
        idt_setEntry(i,p+i*16);
    }
    idt.size=sizeof(entries);
    idt.off=(uint32_t)((uintptr_t)(&entries));
    asm volatile("lidt %0" : : "m"(idt));
}
extern "C" void panic2(char* text, cpu_state* cpu) {
    out << "=== KERNEL PANIC ===\n";
    out << "MTGos encountered a problem it couldn't resolve\n";
    out << "Please open a issue at bit.ly/mtgos if you cannot resolve this on your own\n";
    out << "eax: " << (uint64_t)cpu->eax << ", ebx: " << (uint64_t)cpu->ebx << "\n";
    out << "ecx: " << (uint64_t)cpu->ecx << ", edx: " << (uint64_t)cpu->edx << "\n";
    out << "esi: " << (uint64_t)cpu->esi << ", edi: " << (uint64_t)cpu->edi << "\n";
    out << "ebp: " << (uint64_t)cpu->ebp << ", eip: " << (uint64_t)cpu->intr << "\n";
    out << "i686-" << (uint64_t) BUILDID << "\n";
    out << text << "\nHalting...\n";
    for(;;);
}
extern "C" cpu_state* handleINT(cpu_state* cpu) {
    out << "Interrupt " << (uint64_t)cpu->intr << " occurred\n";
    out << "cs:eip: " << (uint64_t)cpu->cs << ":" << (uint64_t)cpu->eip << "\n";
    out << "ss:esp: " << (uint64_t)cpu->ss << ":" << (uint64_t)cpu->esp << "\n";
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
