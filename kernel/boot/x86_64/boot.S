/******************************************************************************
 * MTGos - A nanokernel in C++
 * Copyright (C) 2017 Morten Delenk
 *
 * it under the terms of the GNU General Public License as published by
 * This program is free software: you can redistribute it and/or modify
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
#define ASM_FILE
#include <multiboot.h>
.code32
.section .btext
_start:
jmp _start2
.align MULTIBOOT_HEADER_ALIGN
.int MULTIBOOT_HEADER_MAGIC
.int 0x7
.int -(MULTIBOOT_HEADER_MAGIC+0x7)
.int 0,0,0,0,0
.int 0
.int 1024, 768, 24
.align MULTIBOOT_HEADER_ALIGN
.global _start
.extern start
.extern platform_init
_start2:
    cli
    mov $mb_ptr, %edi
    stosl
    mov %ebx, %eax
    stosl //Store multiboot header pointer and magic
    finit //Init FPU
    mov %cr0, %eax
    and $0xFB, %al
    or $0x2, %al
    mov %eax, %cr0
    mov %cr4, %eax
    or $0x06, %ah
    mov %cr4, %eax
    mov $kernel_stack, %esp //Initialize Stack
    mov $0x80000001, %eax
    cpuid
    and $0x20000000, %edx //Check if long mode is supported
    jz x86_64_err
    jmp x86_64_OK
x86_64_err:
    cli
    hlt
    jmp x86_64_err
x86_64_OK:
    //Assume PAE is supported. AMD64 requires PAE
    mov $pmfill, %esi
    mov $pagemapL4, %edi
    movsl
    movsl
    mov $pagedirPT, %edi
    mov $1024, %ecx
    rep movsl //Create temporary pagetabes
    mov %cr4, %eax
    or $0x20, %eax
    mov %eax, %cr4 //Activate PAE
    mov $0xC0000080, %ecx
    rdmsr
    or $0x00000100, %eax
    wrmsr //Activate x86_64
    mov $pagemapL4, %eax
    mov %eax, %cr3 //Load Page table
    mov %cr0, %eax
    bswap %eax
    or $0x80, %al
    bswap %eax
    mov %eax, %cr0 //Activate paging
    lgdt gdtr
    mov $0x30, %ax
    ljmp $0x28, $__start //Jump to 64-bit code!
.code64
__start:
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    call platform_init
    xor %rax, %rax
    mov $mb_ptr, %rsi
    lodsl
    mov %rax, %rdi
    lodsl
    mov %rax, %rsi
    call start
_stop:
    cli
    hlt
    jmp _stop

.section .data
gdtr:
    .word 9 * 8
    .int gdt
gdt:
    .quad 0 //NULL
    //32-bit kernel code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0x9A
        .byte 0xCF
        .byte 0x00
    //32-bit kernel code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0x92
        .byte 0xCF
        .byte 00
    //32-bit user code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0xFA
        .byte 0xCF
        .byte 0x00
    //32-bit user data
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0xF2
        .byte 0xCF
        .byte 00
    //64-bit kernel code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0x9B
        .byte 0xAF
        .byte 0x00
    //64-bit kernel code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0x93
        .byte 0xCF
        .byte 00
    //64-bit user code
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0xFB
        .byte 0xAF
        .byte 0x00
    //64-bit user data
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0xF3
        .byte 0xCF
        .byte 00
pmfill:
    .int pagedirPT + 0x7
    .int 0
pdptfill:
.quad 0x87
.quad 0x40000087
.quad 0x80000087
.quad 0xc0000087
.quad 0x100000087
.quad 0x140000087
.quad 0x180000087
.quad 0x1c0000087
.quad 0x200000087
.quad 0x240000087
.quad 0x280000087
.quad 0x2c0000087
.quad 0x300000087
.quad 0x340000087
.quad 0x380000087
.quad 0x3c0000087
.quad 0x400000087
.quad 0x440000087
.quad 0x480000087
.quad 0x4c0000087
.quad 0x500000087
.quad 0x540000087
.quad 0x580000087
.quad 0x5c0000087
.quad 0x600000087
.quad 0x640000087
.quad 0x680000087
.quad 0x6c0000087
.quad 0x700000087
.quad 0x740000087
.quad 0x780000087
.quad 0x7c0000087
.quad 0x800000087
.quad 0x840000087
.quad 0x880000087
.quad 0x8c0000087
.quad 0x900000087
.quad 0x940000087
.quad 0x980000087
.quad 0x9c0000087
.quad 0xa00000087
.quad 0xa40000087
.quad 0xa80000087
.quad 0xac0000087
.quad 0xb00000087
.quad 0xb40000087
.quad 0xb80000087
.quad 0xbc0000087
.quad 0xc00000087
.quad 0xc40000087
.quad 0xc80000087
.quad 0xcc0000087
.quad 0xd00000087
.quad 0xd40000087
.quad 0xd80000087
.quad 0xdc0000087
.quad 0xe00000087
.quad 0xe40000087
.quad 0xe80000087
.quad 0xec0000087
.quad 0xf00000087
.quad 0xf40000087
.quad 0xf80000087
.quad 0xfc0000087
.quad 0x1000000087
.quad 0x1040000087
.quad 0x1080000087
.quad 0x10c0000087
.quad 0x1100000087
.quad 0x1140000087
.quad 0x1180000087
.quad 0x11c0000087
.quad 0x1200000087
.quad 0x1240000087
.quad 0x1280000087
.quad 0x12c0000087
.quad 0x1300000087
.quad 0x1340000087
.quad 0x1380000087
.quad 0x13c0000087
.quad 0x1400000087
.quad 0x1440000087
.quad 0x1480000087
.quad 0x14c0000087
.quad 0x1500000087
.quad 0x1540000087
.quad 0x1580000087
.quad 0x15c0000087
.quad 0x1600000087
.quad 0x1640000087
.quad 0x1680000087
.quad 0x16c0000087
.quad 0x1700000087
.quad 0x1740000087
.quad 0x1780000087
.quad 0x17c0000087
.quad 0x1800000087
.quad 0x1840000087
.quad 0x1880000087
.quad 0x18c0000087
.quad 0x1900000087
.quad 0x1940000087
.quad 0x1980000087
.quad 0x19c0000087
.quad 0x1a00000087
.quad 0x1a40000087
.quad 0x1a80000087
.quad 0x1ac0000087
.quad 0x1b00000087
.quad 0x1b40000087
.quad 0x1b80000087
.quad 0x1bc0000087
.quad 0x1c00000087
.quad 0x1c40000087
.quad 0x1c80000087
.quad 0x1cc0000087
.quad 0x1d00000087
.quad 0x1d40000087
.quad 0x1d80000087
.quad 0x1dc0000087
.quad 0x1e00000087
.quad 0x1e40000087
.quad 0x1e80000087
.quad 0x1ec0000087
.quad 0x1f00000087
.quad 0x1f40000087
.quad 0x1f80000087
.quad 0x1fc0000087
.quad 0x2000000087
.quad 0x2040000087
.quad 0x2080000087
.quad 0x20c0000087
.quad 0x2100000087
.quad 0x2140000087
.quad 0x2180000087
.quad 0x21c0000087
.quad 0x2200000087
.quad 0x2240000087
.quad 0x2280000087
.quad 0x22c0000087
.quad 0x2300000087
.quad 0x2340000087
.quad 0x2380000087
.quad 0x23c0000087
.quad 0x2400000087
.quad 0x2440000087
.quad 0x2480000087
.quad 0x24c0000087
.quad 0x2500000087
.quad 0x2540000087
.quad 0x2580000087
.quad 0x25c0000087
.quad 0x2600000087
.quad 0x2640000087
.quad 0x2680000087
.quad 0x26c0000087
.quad 0x2700000087
.quad 0x2740000087
.quad 0x2780000087
.quad 0x27c0000087
.quad 0x2800000087
.quad 0x2840000087
.quad 0x2880000087
.quad 0x28c0000087
.quad 0x2900000087
.quad 0x2940000087
.quad 0x2980000087
.quad 0x29c0000087
.quad 0x2a00000087
.quad 0x2a40000087
.quad 0x2a80000087
.quad 0x2ac0000087
.quad 0x2b00000087
.quad 0x2b40000087
.quad 0x2b80000087
.quad 0x2bc0000087
.quad 0x2c00000087
.quad 0x2c40000087
.quad 0x2c80000087
.quad 0x2cc0000087
.quad 0x2d00000087
.quad 0x2d40000087
.quad 0x2d80000087
.quad 0x2dc0000087
.quad 0x2e00000087
.quad 0x2e40000087
.quad 0x2e80000087
.quad 0x2ec0000087
.quad 0x2f00000087
.quad 0x2f40000087
.quad 0x2f80000087
.quad 0x2fc0000087
.quad 0x3000000087
.quad 0x3040000087
.quad 0x3080000087
.quad 0x30c0000087
.quad 0x3100000087
.quad 0x3140000087
.quad 0x3180000087
.quad 0x31c0000087
.quad 0x3200000087
.quad 0x3240000087
.quad 0x3280000087
.quad 0x32c0000087
.quad 0x3300000087
.quad 0x3340000087
.quad 0x3380000087
.quad 0x33c0000087
.quad 0x3400000087
.quad 0x3440000087
.quad 0x3480000087
.quad 0x34c0000087
.quad 0x3500000087
.quad 0x3540000087
.quad 0x3580000087
.quad 0x35c0000087
.quad 0x3600000087
.quad 0x3640000087
.quad 0x3680000087
.quad 0x36c0000087
.quad 0x3700000087
.quad 0x3740000087
.quad 0x3780000087
.quad 0x37c0000087
.quad 0x3800000087
.quad 0x3840000087
.quad 0x3880000087
.quad 0x38c0000087
.quad 0x3900000087
.quad 0x3940000087
.quad 0x3980000087
.quad 0x39c0000087
.quad 0x3a00000087
.quad 0x3a40000087
.quad 0x3a80000087
.quad 0x3ac0000087
.quad 0x3b00000087
.quad 0x3b40000087
.quad 0x3b80000087
.quad 0x3bc0000087
.quad 0x3c00000087
.quad 0x3c40000087
.quad 0x3c80000087
.quad 0x3cc0000087
.quad 0x3d00000087
.quad 0x3d40000087
.quad 0x3d80000087
.quad 0x3dc0000087
.quad 0x3e00000087
.quad 0x3e40000087
.quad 0x3e80000087
.quad 0x3ec0000087
.quad 0x3f00000087
.quad 0x3f40000087
.quad 0x3f80000087
.quad 0x3fc0000087
.quad 0x4000000087
.quad 0x4040000087
.quad 0x4080000087
.quad 0x40c0000087
.quad 0x4100000087
.quad 0x4140000087
.quad 0x4180000087
.quad 0x41c0000087
.quad 0x4200000087
.quad 0x4240000087
.quad 0x4280000087
.quad 0x42c0000087
.quad 0x4300000087
.quad 0x4340000087
.quad 0x4380000087
.quad 0x43c0000087
.quad 0x4400000087
.quad 0x4440000087
.quad 0x4480000087
.quad 0x44c0000087
.quad 0x4500000087
.quad 0x4540000087
.quad 0x4580000087
.quad 0x45c0000087
.quad 0x4600000087
.quad 0x4640000087
.quad 0x4680000087
.quad 0x46c0000087
.quad 0x4700000087
.quad 0x4740000087
.quad 0x4780000087
.quad 0x47c0000087
.quad 0x4800000087
.quad 0x4840000087
.quad 0x4880000087
.quad 0x48c0000087
.quad 0x4900000087
.quad 0x4940000087
.quad 0x4980000087
.quad 0x49c0000087
.quad 0x4a00000087
.quad 0x4a40000087
.quad 0x4a80000087
.quad 0x4ac0000087
.quad 0x4b00000087
.quad 0x4b40000087
.quad 0x4b80000087
.quad 0x4bc0000087
.quad 0x4c00000087
.quad 0x4c40000087
.quad 0x4c80000087
.quad 0x4cc0000087
.quad 0x4d00000087
.quad 0x4d40000087
.quad 0x4d80000087
.quad 0x4dc0000087
.quad 0x4e00000087
.quad 0x4e40000087
.quad 0x4e80000087
.quad 0x4ec0000087
.quad 0x4f00000087
.quad 0x4f40000087
.quad 0x4f80000087
.quad 0x4fc0000087
.quad 0x5000000087
.quad 0x5040000087
.quad 0x5080000087
.quad 0x50c0000087
.quad 0x5100000087
.quad 0x5140000087
.quad 0x5180000087
.quad 0x51c0000087
.quad 0x5200000087
.quad 0x5240000087
.quad 0x5280000087
.quad 0x52c0000087
.quad 0x5300000087
.quad 0x5340000087
.quad 0x5380000087
.quad 0x53c0000087
.quad 0x5400000087
.quad 0x5440000087
.quad 0x5480000087
.quad 0x54c0000087
.quad 0x5500000087
.quad 0x5540000087
.quad 0x5580000087
.quad 0x55c0000087
.quad 0x5600000087
.quad 0x5640000087
.quad 0x5680000087
.quad 0x56c0000087
.quad 0x5700000087
.quad 0x5740000087
.quad 0x5780000087
.quad 0x57c0000087
.quad 0x5800000087
.quad 0x5840000087
.quad 0x5880000087
.quad 0x58c0000087
.quad 0x5900000087
.quad 0x5940000087
.quad 0x5980000087
.quad 0x59c0000087
.quad 0x5a00000087
.quad 0x5a40000087
.quad 0x5a80000087
.quad 0x5ac0000087
.quad 0x5b00000087
.quad 0x5b40000087
.quad 0x5b80000087
.quad 0x5bc0000087
.quad 0x5c00000087
.quad 0x5c40000087
.quad 0x5c80000087
.quad 0x5cc0000087
.quad 0x5d00000087
.quad 0x5d40000087
.quad 0x5d80000087
.quad 0x5dc0000087
.quad 0x5e00000087
.quad 0x5e40000087
.quad 0x5e80000087
.quad 0x5ec0000087
.quad 0x5f00000087
.quad 0x5f40000087
.quad 0x5f80000087
.quad 0x5fc0000087
.quad 0x6000000087
.quad 0x6040000087
.quad 0x6080000087
.quad 0x60c0000087
.quad 0x6100000087
.quad 0x6140000087
.quad 0x6180000087
.quad 0x61c0000087
.quad 0x6200000087
.quad 0x6240000087
.quad 0x6280000087
.quad 0x62c0000087
.quad 0x6300000087
.quad 0x6340000087
.quad 0x6380000087
.quad 0x63c0000087
.quad 0x6400000087
.quad 0x6440000087
.quad 0x6480000087
.quad 0x64c0000087
.quad 0x6500000087
.quad 0x6540000087
.quad 0x6580000087
.quad 0x65c0000087
.quad 0x6600000087
.quad 0x6640000087
.quad 0x6680000087
.quad 0x66c0000087
.quad 0x6700000087
.quad 0x6740000087
.quad 0x6780000087
.quad 0x67c0000087
.quad 0x6800000087
.quad 0x6840000087
.quad 0x6880000087
.quad 0x68c0000087
.quad 0x6900000087
.quad 0x6940000087
.quad 0x6980000087
.quad 0x69c0000087
.quad 0x6a00000087
.quad 0x6a40000087
.quad 0x6a80000087
.quad 0x6ac0000087
.quad 0x6b00000087
.quad 0x6b40000087
.quad 0x6b80000087
.quad 0x6bc0000087
.quad 0x6c00000087
.quad 0x6c40000087
.quad 0x6c80000087
.quad 0x6cc0000087
.quad 0x6d00000087
.quad 0x6d40000087
.quad 0x6d80000087
.quad 0x6dc0000087
.quad 0x6e00000087
.quad 0x6e40000087
.quad 0x6e80000087
.quad 0x6ec0000087
.quad 0x6f00000087
.quad 0x6f40000087
.quad 0x6f80000087
.quad 0x6fc0000087
.quad 0x7000000087
.quad 0x7040000087
.quad 0x7080000087
.quad 0x70c0000087
.quad 0x7100000087
.quad 0x7140000087
.quad 0x7180000087
.quad 0x71c0000087
.quad 0x7200000087
.quad 0x7240000087
.quad 0x7280000087
.quad 0x72c0000087
.quad 0x7300000087
.quad 0x7340000087
.quad 0x7380000087
.quad 0x73c0000087
.quad 0x7400000087
.quad 0x7440000087
.quad 0x7480000087
.quad 0x74c0000087
.quad 0x7500000087
.quad 0x7540000087
.quad 0x7580000087
.quad 0x75c0000087
.quad 0x7600000087
.quad 0x7640000087
.quad 0x7680000087
.quad 0x76c0000087
.quad 0x7700000087
.quad 0x7740000087
.quad 0x7780000087
.quad 0x77c0000087
.quad 0x7800000087
.quad 0x7840000087
.quad 0x7880000087
.quad 0x78c0000087
.quad 0x7900000087
.quad 0x7940000087
.quad 0x7980000087
.quad 0x79c0000087
.quad 0x7a00000087
.quad 0x7a40000087
.quad 0x7a80000087
.quad 0x7ac0000087
.quad 0x7b00000087
.quad 0x7b40000087
.quad 0x7b80000087
.quad 0x7bc0000087
.quad 0x7c00000087
.quad 0x7c40000087
.quad 0x7c80000087
.quad 0x7cc0000087
.quad 0x7d00000087
.quad 0x7d40000087
.quad 0x7d80000087
.quad 0x7dc0000087
.quad 0x7e00000087
.quad 0x7e40000087
.quad 0x7e80000087
.quad 0x7ec0000087
.quad 0x7f00000087
.quad 0x7f40000087
.quad 0x7f80000087
.quad 0x7fc0000087

.section .bss
mb_ptr:
    // These 8 Bytes will never be used when the stack comes near it
    .space 16384
kernel_stack:
    .align 4096
pagemapL4:
    .space 4096
pagedirPT:
    .space 4096
