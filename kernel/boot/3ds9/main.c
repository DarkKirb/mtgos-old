#include <multiboot.h>
#include <stdint.h>
void start(int eax, multiboot_info_t* ebx);
void n3ds_init(int eax, multiboot_info_t* ebx) {
    if((*((uint32_t*)0x10140FFC))&2==0)
        return; //Old 3DS
    //New 3ds. Activate the 512KiB of ram we additionally get.
    *((uint32_t*)0x10000200) |= 1;
    //Activate the 512KiB mmap
    multiboot_memory_map_t *mmap=(multiboot_memory_map_t*)(ebx->mmap_addr);
    mmap[2].type=MULTIBOOT_MEMORY_AVAILABLE;
}
