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
#include <multiboot2.h>

.section .text
jmp $_start
.align MULTIBOOT_TAG_ALIGN
.long MULTIBOOT2_HEADER_MAGIC
.long MULTIBOOT_ARCHITECTURE_I386
.long 0x10 //size
.long -(MULTIBOOT2_HEADER_MAGIC+MULTIBOOT_ARCHITECTURE_I386+0x10)

.global _start
.extern start
_start:
    mov $kernel_stack, %esp //Initialize Stack
    push %ebx
    push %eax //Push arguments to start()
    call start

.section .bss
.space 16384
kernel_stack:
