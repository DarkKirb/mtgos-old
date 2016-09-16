#include <moduleinterface.h>
#include <interrupts.h>
#include <modstubs.h>
#include <stdint.h>
#include <x86/io.h>
#include <x86/cpu_state.h>
#define IDT_INTERRUPT_GATE 0x06
#define IDT_TRAP_GATE      0x07
#define IDT_TASK_GATE      0x05
#define IDT_SEG_32_BIT     0x08
#define IDT_RING_0         0x00
#define IDT_RING_3         0x60
#define IDT_USED           0x80
#define SEG_KERNEL         0x08
#define SEG_USER           0x08 /*NEVER USE!!*/
#define SEG_DBL_FAULT 0x28 /*Only use for double fault handler!!*/
extern uint64_t intr_0;
uint64_t idt[256]; //Number of interrupts

class Interrupts: public MTGos::Interrupt {
private:
	auto setEntry(int i, void* offset, uint16_t seg, uint8_t flags) -> void {
        idt[i]=(uint16_t)((uint32_t)offset);
        idt[i]|=(uint64_t)(((uint32_t)offset)>>16)<<48;
        idt[i]|=((uint64_t)seg)<<16;
        idt[i]|=((uint64_t)flags)<<40;
	}
    auto initPIC() -> void{
        //Init PIC
        outb(0x20, 0x11); //Master PIC
        outb(0x21, 0x20); //Interrupt number for IRQ0
        outb(0x21, 0x04); //IRQ is the slave
        outb(0x21, 0x01); //ICW 4
        outb(0xA0, 0x11); //Init Master-PIC
        outb(0xA1, 0x28); //Interrupt number for IRQ8
        outb(0xA1, 0x02); //IRQ is the slave
        outb(0xA1, 0x01); //ICW 4
        //Activate IRQs
        outb(0x21, 0x00);
        outb(0xA1, 0x00);
    }
    auto apply() -> void {
        struct { uint16_t limit; uint64_t* ptr; } __attribute__((packed))
          idtptr = {256*8-1, (uint64_t*)&idt };
        asm volatile("lidt %0" : : "m"(idtptr));
    }
public:
    Interrupts() {};
    ~Interrupts() {};
    virtual auto registerHandler() -> void {
        cli();
        uint64_t* intr = &intr_0;
        for(int i=0;i<256;i++) {
            setEntry(i, intr+i, SEG_KERNEL, IDT_INTERRUPT_GATE | IDT_SEG_32_BIT | IDT_RING_0 | IDT_USED);
        }
        setEntry(48, intr+48, SEG_KERNEL, IDT_TRAP_GATE | IDT_SEG_32_BIT | IDT_RING_3 | IDT_USED);
        initPIC();
        apply();
        sti();
    }
};
Interrupts *instance;
extern "C" cpu_state* intra_common_handler(cpu_state* cpu) {
    for(;;);
    return cpu;
}
void(*tbl[3])()={(void(*)())&getType,(void(*)())&size_of,(void(*)())&spawnAt};
table_type getTable() {
    doCtors();
    return (void(**)())&tbl;
}
auto getType() -> ModType {
    return ModType::interrupt;
}
auto spawnAt(void* pos) -> bool {
    new(pos) Interrupts;
    return true;
}
auto size_of() -> size_t {
    return sizeof(Interrupts);
    return 0;
}
