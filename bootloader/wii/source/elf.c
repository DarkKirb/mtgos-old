#include "elf.h"
Elf32_Ehdr hdr;
int checkELFHeader(FILE* f) {
    fseek(f, 0, SEEK_SET);
    fread(&hdr, sizeof(Elf32_Ehdr), 1, f);
    if((hdr.e_ident[0] != ELFMAG0) || (hdr.e_ident[1] != ELFMAG1) || (hdr.e_ident[2] != ELFMAG2) || (hdr.e_ident[3] != ELFMAG3)) {
        printf("ELF magic is invalid...\n");
        return -1;
    }
    if(hdr.e_ident[EI_CLASS]!=ELFCLASS32) {
        printf("Only ELF32's are supported on this machine.\n");
        return -2;
    }
    if(hdr.e_ident[EI_DATA]!=ELFDATA2MSB) {
        printf("Only big-endian ELF files are supported on this machine.\n");
        return -3;
    }
    if((hdr.e_ident[EI_VERSION]!=EV_CURRENT)||(hdr.e_version!=EV_CURRENT)) {
        printf("Invalid ELF version!\n");
        return -4;
    }
    if(hdr.e_ident[EI_OSABI]!=ELFOSABI_NONE) {
        printf("Can only load a ELF application System V!\n");
        return -5;
    }
    if(hdr.e_type != ET_EXEC) {
        printf("Can only load executable ELF files!\n");
        return -6;
    }
    if(hdr.e_machine != EM_PPC) {
        printf("Can only load PPC ELFs!\n");
        return -7;
    }
    return 0;
}
void *loadElf(FILE *f) {
    Elf32_Phdr *phdr=malloc(hdr.e_phentsize*hdr.e_phnum);
    printf("phdr offset=0x%x\n",hdr.e_phoff);
    if(hdr.e_phoff-ftell(f)) {
        void* x=malloc(hdr.e_phoff-ftell(f));
        fread(x,hdr.e_phoff-ftell(f),1,f);
        free(x);
    }
    fread(phdr, hdr.e_phentsize, hdr.e_phnum, f);
    for(int i=0;i<hdr.e_phnum;i++) {
        if(phdr[i].p_type == PT_LOAD) {
            void* x=malloc(phdr[i].p_offset-ftell(f));
            fread(x,phdr[i].p_offset-ftell(f),1,f);
            free(x);
            Elf32_Addr addr=(phdr[i].p_vaddr)?(phdr[i].p_vaddr):(phdr[i].p_paddr);
            addr+=0x40000000;
            memset(addr,0,phdr[i].p_memsz);
            printf("Load segment %i. Offset 0x%x, size 0x%x to 0x%x. 0x%x\n",i,phdr[i].p_offset,phdr[i].p_filesz,addr,ftell(f));
            size_t size=phdr[i].p_filesz;
            size_t off=0;
            while(size>0) {
                fread(addr+off,0x1000,1,f);
                if(size<0x1000)
        			break;
                size-=0x1000;
                off+=0x1000;
                printf("b:%x\n",size);
            }
        }
    }
    return (void*)hdr.e_entry;
}
