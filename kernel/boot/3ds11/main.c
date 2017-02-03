#include <multiboot.h>
#include <stdint.h>
void start(int eax, multiboot_info_t* ebx);
void n3ds_init(int eax, multiboot_info_t* ebx) {
    if((*((uint32_t*)0x10140FFC))!=7)
        return; //Old 3DS
    //New 3ds. Bit 2 says whether we can use 3x or 2x clock speed.
    *((uint16_t*)0x10141300) = 1;
    if((*((uint32_t*)0x10140FFC))&4==0)
        *((uint16_t*)0x10141300) |= 3; //500 MHz
    else
        *((uint16_t*)0x10141300) |= 5; //800 MHz
    //Activate the Additional 128MB MMAP
    multiboot_memory_map_t *mmap=(multiboot_memory_map_t*)(ebx->mmap_addr);
    mmap[6].type=MULTIBOOT_MEMORY_AVAILABLE;
}
