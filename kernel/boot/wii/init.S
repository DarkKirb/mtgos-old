.section .text
.global _start
.extern start
_start:
    mflr 0
    lis 1, stack@ha
    addi 1, 1, stack@l
    bl start
.section .bss
.space 4096
stack:
