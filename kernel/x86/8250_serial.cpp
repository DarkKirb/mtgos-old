#include "../src/output/serial.hpp"
#include "8250_serial.hpp"
#define PORT 0x3F8
namespace drivers {
namespace serial {
namespace {
    static inline void outb(uint16_t port, uint8_t val) {
    	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
    }
    static inline uint8_t inb(uint16_t port) {
    	uint8_t ret;
    	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    	return ret;
    }
}
PC_Serial::PC_Serial(multiboot_info_t* mb_info): MTGos::Serial(mb_info) {
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}
PC_Serial::~PC_Serial() {}
bool PC_Serial::isTransmitEmpty() {
    return inb(PORT+5) & 0x20;
}
auto PC_Serial::putChar(char c) -> void {
    while (!isTransmitEmpty());
    outb(PORT,(uint8_t)c);
}
}
}
