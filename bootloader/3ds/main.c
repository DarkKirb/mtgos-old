

#include <ctr9/io.h>
#include <ctr9/io/ctr_drives.h>
#include <ctr9/ctr_system.h>
#include <ctr9/ctr_screen.h>
#include <ctr9/i2c.h>
#include <ctr9/sha.h>
#include <ctr9/ctr_elf_loader.h>
#include <ctr9/ctr_interrupt.h>
#include <ctr9/ctr_pxi.h>

#include <ctrelf.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include "multiboot.h"
int main()
{
    *((uint32_t*)0x18300000)=0xFFFFFF;
    ctr_drives_initialize();
    printf("hi!\n");
	//To open a file, use fopen, using the drive as the prefix for the path.
	FILE *file = fopen("SD:/kernel9.elf", "rb");
	if (!file)
	{
		printf("Couldn't open kernel9.elf!\n");
		for(;;);
	}
    printf("Filling the multiboot info.\n");
    multiboot_info_t mb_info={0};
    mb_info.flags=MULTIBOOT_INFO_MEM_MAP | MULTIBOOT_INFO_FRAMEBUFFER_INFO | MULTIBOOT_INFO_BOOT_LOADER_NAME;
    mb_info.mem_lower=0x100000; //Only access this memory
    mb_info.mem_upper=0x8080000; //Never access this memory
    multiboot_memory_map_t mmap[10];
    mb_info.mmap_length=sizeof(mmap);
    mb_info.mmap_addr=(uintptr_t)(&mmap);
    mb_info.boot_loader_name=(uintptr_t)(&"arm9loaderhax");
    mb_info.framebuffer_addr=0x18300000 + 0x46500;
    mb_info.framebuffer_width=320;
    mb_info.framebuffer_width=240;
    mb_info.framebuffer_bpp=24;
    mb_info.framebuffer_type=MULTIBOOT_FRAMEBUFFER_TYPE_RGB;
    printf("Filling mmap\n");
    mmap[0].size=sizeof(multiboot_memory_map_t);
    mmap[0].addr=0;
    mmap[0].len=0x8000;
    mmap[0].type=MULTIBOOT_MEMORY_AVAILABLE; //ITCM

    mmap[1].size=sizeof(multiboot_memory_map_t);
    mmap[1].addr=0x08000000;
    mmap[1].len=0x00100000;
    mmap[1].type=MULTIBOOT_MEMORY_AVAILABLE; //ARM9 RAM

    mmap[2].size=sizeof(multiboot_memory_map_t);
    mmap[2].addr=0x08100000;
    mmap[2].len=0x00080000;
    mmap[2].type=MULTIBOOT_MEMORY_ACPI_RECLAIMABLE; //N3DS ARM9 RAM

    mmap[3].size=sizeof(multiboot_memory_map_t);
    mmap[3].addr=0x10000000;
    mmap[3].len=0x08000000;
    mmap[3].type=MULTIBOOT_MEMORY_RESERVED; //IO memory

    mmap[4].size=sizeof(multiboot_memory_map_t);
    mmap[4].addr=0x18000000;
    mmap[4].len=0x00600000;
    mmap[4].type=MULTIBOOT_MEMORY_RESERVED; //VRAM

    mmap[5].size=sizeof(multiboot_memory_map_t);
    mmap[5].addr=0x1FF00000;
    mmap[5].len=0x00080000;
    mmap[5].type=MULTIBOOT_MEMORY_RESERVED; //DSP RAM

    mmap[6].size=sizeof(multiboot_memory_map_t);
    mmap[6].addr=0x1FF80000;
    mmap[6].len=0x00080000;
    mmap[6].type=MULTIBOOT_MEMORY_RESERVED; //AXIWRAM

    mmap[7].size=sizeof(multiboot_memory_map_t);
    mmap[7].addr=0x20000000;
    mmap[7].len=0x08000000;
    mmap[7].type=MULTIBOOT_MEMORY_RESERVED; //FCRAM

    mmap[8].size=sizeof(multiboot_memory_map_t);
    mmap[8].addr=0x28000000;
    mmap[8].len=0x08000000;
    mmap[8].type=MULTIBOOT_MEMORY_RESERVED; //N3DS FCRAM

    mmap[9].size=sizeof(multiboot_memory_map_t);
    mmap[9].addr=0xFFF00000;
    mmap[9].len=0x00004000;
    mmap[9].type=MULTIBOOT_MEMORY_AVAILABLE; //DTCM

    printf("Loading the kernel.\n");
    Elf32_Ehdr header;
    ctr_load_header(&header, file);
    if(!ctr_check_elf(&header)) {
        printf("ERROR! This isn't a valid ELF!\n");
        for(;;);
    }
    ctr_load_segments(&header, file);
    fclose(file);
    ((void(*)(uint32_t,multiboot_info_t*))(header.e_entry))(0x2BADB002,&mb_info);
    ctr_system_poweroff();
	return 0;
}
