

#include <ctr9/io.h>
#include <ctr9/io/ctr_drives.h>
#include <ctr9/ctr_system.h>
#include <ctr9/ctr_screen.h>
#include <ctr9/i2c.h>
#include <ctr9/sha.h>
#include <ctr9/ctr_elf_loader.h>
#include <ctr9/ctr_interrupt.h>
#include <ctr9/ctr_pxi.h>
#include <ctr9/ctr_cache.h>

#include <ctrelf.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include "multiboot.h"
char stack[1024];
void arm11main() {
    asm volatile("mov sp, %0" : : "r"((uint32_t)(&stack)));
    //Filling multiboot info
    multiboot_info_t mb_info={0};
    mb_info.flags=MULTIBOOT_INFO_MEM_MAP | MULTIBOOT_INFO_FRAMEBUFFER_INFO | MULTIBOOT_INFO_BOOT_LOADER_NAME;
    mb_info.mem_lower=0x80000;
    mb_info.mem_upper=0x8000000;
    multiboot_memory_map_t mmap[7];
    mb_info.mmap_length=sizeof(mmap);
    mb_info.mmap_addr=(uintptr_t)(&mmap);
    mb_info.boot_loader_name=(uintptr_t)(&"arm9loaderhax");
    mb_info.framebuffer_addr=0x18300000;
    mb_info.framebuffer_width=400;
    mb_info.framebuffer_width=240;
    mb_info.framebuffer_bpp=24;
    mb_info.framebuffer_type=MULTIBOOT_FRAMEBUFFER_TYPE_RGB;
    multiboot_module_t mods[1];
    mods[0].mod_start=0x21000000;
    mb_info.mods_count=1;
    mb_info.mods_addr=(uint32_t)(&mods);
    //Fill mmap
    mmap[0].size=sizeof(multiboot_memory_map_t);
    mmap[0].addr=0x10000000;
    mmap[0].len=0x08000000;
    mmap[0].type=MULTIBOOT_MEMORY_RESERVED; //IO

    mmap[1].size=sizeof(multiboot_memory_map_t);
    mmap[1].addr=0x18000000;
    mmap[1].len=0x00600000;
    mmap[1].type=MULTIBOOT_MEMORY_RESERVED; //VRAM

    mmap[2].size=sizeof(multiboot_memory_map_t);
    mmap[2].addr=0x1F000000;
    mmap[2].len=0x00400000;
    mmap[2].type=MULTIBOOT_MEMORY_RESERVED; //N3DS VRAM

    mmap[3].size=sizeof(multiboot_memory_map_t);
    mmap[3].addr=0x1FF00000;
    mmap[3].len=0x00080000;
    mmap[3].type=MULTIBOOT_MEMORY_RESERVED; //DSP memory

    mmap[4].size=sizeof(multiboot_memory_map_t);
    mmap[4].addr=0x1FF80000;
    mmap[4].len=0x0007F000;
    mmap[4].type=MULTIBOOT_MEMORY_AVAILABLE; //AXIWRAM (Except for the vector location)

    mmap[5].size=sizeof(multiboot_memory_map_t);
    mmap[5].addr=0x20000000;
    mmap[5].len=0x08000000;
    mmap[5].type=MULTIBOOT_MEMORY_AVAILABLE; //FCRAM

    mmap[6].size=sizeof(multiboot_memory_map_t);
    mmap[6].addr=0x28000000;
    mmap[6].len=0x08000000;
    mmap[6].type=MULTIBOOT_MEMORY_ACPI_RECLAIMABLE; //N3DS FCRAM

    while(1) {
        if(*((uint32_t*)0x1FFFFFF8))
            break;
    }
    (*((void(**)(uint32_t,multiboot_info_t*))0x1FFFFFF8))(0x2BADB002,&mb_info);
}
int main()
{
    ctr_drives_initialize();
    ctr_libctr9_init();;
    printf("hi!\n");
	//To open a file, use fopen, using the drive as the prefix for the path.
	FILE *file = fopen("SD:/kernel9.elf", "rb");
	if (!file)
	{
		printf("Couldn't open kernel9.elf!\n");
		for(;;);
	}
    printf("Owning ARM11.\n");
    *((uint32_t*)0x1FFFFFF8)=(uint32_t)(&arm11main);
    printf("loading multiboot modules\n");
    FILE *f2 = fopen("SD:/font.bin", "rb");
    if(f2) {
        fseek(f2,0,SEEK_END);
        size_t size=ftell(f2);
        fseek(f2,0,SEEK_SET);
        fread((void*)0x21000000,size,1,f2);
    }
    printf("Loaded the font\n");
    ctr_cache_drain_write_buffer();
    ctr_cache_clean_and_flush_all();
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
    multiboot_module_t mods[1];
    mods[0].mod_start=0x21000000;
    mb_info.mods_count=1;
    mb_info.mods_addr=(uint32_t)(&mods);
    printf("Filling mmap\n");
    mmap[0].size=sizeof(multiboot_memory_map_t);
    mmap[0].addr=0;
    mmap[0].len=0x8000;
    mmap[0].type=MULTIBOOT_MEMORY_RESERVED; //ITCM

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
    mmap[9].type=MULTIBOOT_MEMORY_RESERVED; //DTCM

    Elf32_Ehdr header;
    ctr_load_header(&header, file);
    if(!ctr_check_elf(&header)) {
        printf("This isn't a valid ELF!\n");
        for(;;);
    }
    ctr_load_segments(&header, file);
    printf("Loaded kernel9\n");
    fclose(file);
    file=fopen("SD:/kernel11.elf","rb");
    if(file) {
        Elf32_Ehdr header11;
        ctr_load_header(&header11, file);
        if(!ctr_check_elf(&header11)) {
            printf("This isn't a valid ELF!\n");
            for(;;);
        } else {
            ctr_load_segments(&header11,file);
            printf("Kernel11 loaded\n");
            ctr_cache_drain_write_buffer();
            ctr_cache_clean_and_flush_all();
            printf("Starting kernel11...\n");
            *((uint32_t*)0x1FFFFFF8)=(uint32_t)(header11.e_entry);
        }
        fclose(file);

    }
    ctr_cache_drain_write_buffer();
    ctr_cache_clean_and_flush_all();
    printf("Starting kernel9...\n");
    ((void(*)(uint32_t,multiboot_info_t*))(header.e_entry))(0x2BADB002,&mb_info);
    ctr_system_poweroff();
	return 0;
}
