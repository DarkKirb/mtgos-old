#include <multiboot.h>
void start(int eax, multiboot_info_t* ebx);
void _start(int eax, multiboot_info_t* ebx) {
    start(eax,ebx);
}
