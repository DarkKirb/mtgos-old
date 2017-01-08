#include <stdint.h>
#include <multiboot.h>
#include "output/display.hpp"
#include "output/serial.hpp"
extern "C" void(*start_ctors)();
extern "C" void(*end_ctors)();
extern "C" void(*start_dtors)();
extern "C" void(*end_dtors)();

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
multiboot_info_t* mb_info;
SERIAL kout(mb_info);
DISPLAY out(mb_info);
extern "C" void start(int eax, multiboot_info_t* ebx)
{
	mb_info=ebx;
    for(auto ctor=&start_ctors;ctor<&end_ctors;ctor++)
        (**ctor)();
    kout << "MTGos is starting\nBootloader magic: 0x" << (uint64_t)eax << "\n";
    if(eax!=MULTIBOOT_BOOTLOADER_MAGIC) {
        kout << "Not loaded by Multiboot conformant loader. Halting.\n";
        for(;;);
    }
    kout << "Loaded by: " << (char*)ebx->boot_loader_name << "\n";
	out << "TEST. lalalalaala\ntwo lines!\n";
    for(auto dtor=&start_dtors;dtor!=&end_dtors;dtor++)
        (**dtor)();
    for(int x=1;x>0;x++);
}