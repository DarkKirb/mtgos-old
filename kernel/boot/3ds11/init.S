.section .btext
.arm
b _IRQ
b _FIQ
b _SWI
b _UND
b _PREFETCH
b _DATA
interruptHandler:
    bx lr
_IRQ:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #0
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
_FIQ:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #1
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
_SWI:
    push {r0-r12,r14}
    mov r0, #2
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
_UND:
    push {r0-r12,r14}
    mov r0, #3
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
_PREFETCH:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #4
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
_DATA:
    sub r14,#8
    push {r0-r12,r14}
    mov r0, #5
    mov r1, sp
    blx interruptHandler
    pop {r0-r12,pc}
.global _start
.extern start
.extern n3ds_init
_start:
    mov r4, r0
    mov r5, r1
    ldr r0, =stack
    mov sp, r0 //Supervisor
    mrs r2, cpsr
    bic r2, #0x1F
    orr r1, r2, #0x11
    msr cpsr, r1
    mov sp, r0 //FIQ
    orr r1, r2, #0x12
    msr cpsr, r1
    mov sp, r0 //IRQ
    orr r1, r2, #0x17
    msr cpsr, r1
    mov sp, r0 //ABT
    orr r1, r2, #0x1B
    msr cpsr, r1
    mov sp, r0 //UND
    orr r1, r2, #0x13
    msr cpsr, r1
    mov r0, r4
    mov r1, r5
    push {r0,r1}
    blx n3ds_init
    pop {r0,r1}
    blx start

.section .bss
.space 4096
stack:
