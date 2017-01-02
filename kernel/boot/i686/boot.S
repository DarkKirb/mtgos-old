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
.int 1024, 768, 32
.align MULTIBOOT_HEADER_ALIGN
.global _start
.extern start
.extern platform_init
_start2:
    mov $kernel_stack, %esp //Initialize Stack
    push %ebx
    push %eax //Push arguments to start()
    call platform_init
    call start

.section .bss
.space 16384
kernel_stack:
