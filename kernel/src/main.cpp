#include <stdint.h>
#include <multiboot.h>
#include "output/display.hpp"
#include "output/serial.hpp"
#include "pmm.hpp"
#include "base.hpp"
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
DISPLAY out(mb_info);
SERIAL kout(mb_info);
PMMD pmm(mb_info);

extern "C" void start(int eax, multiboot_info_t* ebx)
{
	mb_info=ebx;
    for(auto ctor=&start_ctors;ctor<&end_ctors;ctor++)
        (**ctor)();
    kout << MTGos::LogLevel::INFO << "MTGos is starting\n"<< MTGos::LogLevel::INFO<<"Bootloader magic: 0x" << (uint64_t)eax << "\n";
    if(eax!=MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Not loaded by Multiboot conformant loader!\n");
    }
    kout << MTGos::LogLevel::INFO << "Loaded by: " << (char*)ebx->boot_loader_name << "\n";
	out << "Triggering interrupt\n";
#ifdef __arm__
	asm volatile("SVC #0");
#else
	asm volatile("int $0x20");
#endif
	out << "Am I still there?\n";
	while(true) {
		pmm.alloc(1);
		//out << (uint64_t)((uintptr_t)(pmm.alloc(1))) << "\n";
	}
    for(auto dtor=&start_dtors;dtor!=&end_dtors;dtor++)
        (**dtor)();
    for(int x=1;x>0;x++);
}
