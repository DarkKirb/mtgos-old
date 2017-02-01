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
    mov $8, %ecx
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
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x28, $__start //Jump to 64-bit code!
.code64
__start:
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
    .word 4 * 8
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
    .quad 0xC0000087
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
