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
    lgdt gdtr
    mov $0x10, %cx
    mov %cx, %ds
    mov %cx, %es
    mov %cx, %fs
    mov %cx, %gs
    mov %cx, %ss
    ljmp $0x08, $_start3
_start3:
    mov $kernel_stack, %esp //Initialize Stack
    push %ebx
    push %eax //Push arguments to start()
    call platform_init
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
.section .bss
.space 16384
kernel_stack:
