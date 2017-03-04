#include <gccore.h>
#include <wiiuse/wpad.h>
#include "elf.h"
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "multiboot.h"


static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
    
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
    printf("\n\n");
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    if (!fatInitDefault()) {
		printf("fatInitDefault failure: terminating\n");
		for(;;);
	}
    FILE* kernel=fopen("/wii.elf","rb");
    if(!kernel){
        printf("Could not open file\n");
        for(;;);
    }
    if(checkELFHeader(kernel)<0) {
        printf("This is not a valid ELF file!\n");
        for(;;);
    }
    FILE* font=fopen("/font.big","rb");
    if(!font) {
        printf("Could not open font\n");
        for(;;);
    }
    size_t size=2539456; //Because seeking is apperantly for losers?
	while(size>0) {
		fread(0xD0000000+ftell(font), 0x1000, 1, font); //Read to beginning of MEM1
		if(size<0x1000)
			break;
		size-=0x1000;
		//printf("a:%x\n",size);
	}
    multiboot_info_t mb_info;
    mb_info.mem_lower=24*1024*1024;
    mb_info.mem_upper=64*1024*1024;
    multiboot_memory_map_t mmap[2];
    mb_info.mmap_length=sizeof(mmap);
    mb_info.mmap_addr=(uint32_t)&mmap;
    mb_info.framebuffer_pitch=((uint32_t)xfb);
    mb_info.framebuffer_width=rmode->fbWidth;
    mb_info.framebuffer_height=rmode->xfbHeight;
    mb_info.framebuffer_bpp=32;
    mb_info.framebuffer_type=3; //YUY2 to save HW costs…but not to safe software costs
    mmap[0].size=sizeof(multiboot_memory_map_t);
    mmap[0].addr=0x80000000;
    mmap[0].len=0x1800000;
    mmap[0].type=MULTIBOOT_MEMORY_AVAILABLE;
    mmap[1].size=sizeof(multiboot_memory_map_t);
    mmap[1].addr=0x90000000;
    mmap[1].len=0x40000000;
    mmap[1].type=MULTIBOOT_MEMORY_AVAILABLE;
    multiboot_module_t mods[1];
    mods[0].mod_start=0xD0000000;
    mods[0].mod_end=0xD0000000+2539456;
    mb_info.mods_count=1;
    mb_info.mods_addr=(uint32_t)&mods;
	mb_info.boot_loader_name=(uintptr_t)(&"Wiiloader");
    void(*entry)(uint32_t, multiboot_info_t*)=loadElf(kernel);
	printf("%x\n",(uint32_t)mb_info.framebuffer_pitch);
    printf("%x\n",(uint32_t)xfb);
    VIDEO_WaitVSync();
    for(int i=0;i<1000;i++) {
        ((uint32_t*)(xfb))[i]=0xFFFFFFFF;
    }
    VIDEO_WaitVSync();
    for(;;);
    entry(MULTIBOOT_BOOTLOADER_MAGIC,&mb_info);
	while(1);
    return 0;
}
