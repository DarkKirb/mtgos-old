#include <stdint.h>

#include <multiboot.h>
extern "C" void(*start_ctors)(multiboot_info_t*);
extern "C" void(*end_ctors)(multiboot_info_t*);
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
#define PORT 0x3F8
void init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}
int serial_received() {
   return inb(PORT + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);

   return inb(PORT);
}
int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}
void puts(const char* s) {
    for(int i=0;s[i]!=0;i++) {
        write_serial(s[i]);
    }
}
void puti(uint64_t i) {
    uint64_t output=i;
    const char* chars="0123456789ABCDEF";
    char buf[65];
    buf[64]='\0';
    char* ptr=buf+63;
    do {
        *(ptr--)=chars[output&0xF];
        output>>=4;
    } while(output && (ptr!=buf));
    puts(ptr+1);
    int arr[1];
}
extern "C" void start(int eax, multiboot_info_t* ebx)
{
    for(auto ctor=start_ctors;ctor!=end_ctors;ctor++)
        (*ctor)(ebx);
    puts("MTGos is starting\nBootloader magic: 0x");
    puti(eax);
    write_serial('\n');
    if(eax!=MULTIBOOT_BOOTLOADER_MAGIC) {
        puts("Not loaded by Multiboot conformant loader. Halting.\n");
        for(;;);
    }
    puts("Loaded by: ");
    puts((char*)ebx->boot_loader_name);
    write_serial('\n');
    for(auto dtor=start_dtors;dtor!=end_dtors;dtor++)
        (*dtor)();
    for(int x=1;x>0;x++);
}
