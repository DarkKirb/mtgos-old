.section .btext
.arm
b _IRQ
nop
b _FIQ
nop
b _SWI
nop
b _UND
nop
b _PREFETCH
nop
b _DATA
_IRQ:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #0
    b prepareHandler
_FIQ:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #1
    b prepareHandler
_SWI:
    push {r0-r12,r14}
    mov r0, #2
    b prepareHandler
_UND:
    push {r0-r12,r14}
    mov r0, #3
    b prepareHandler
_PREFETCH:
    sub r14,#4
    push {r0-r12,r14}
    mov r0, #4
    b prepareHandler
_DATA:
    sub r14,#8
    push {r0-r12,r14}
    mov r0, #5
    b prepareHandler
.section .text
.extern interruptHandler
prepareHandler:
    //Push VFP registers
    //vpush {s0-s31}
    mrs r2, spsr
    push {r2}
    mov r1, sp
    mrs r2, cpsr
    bic r2, #0x1F
    orr r2, #0x13
    msr cpsr, r2 //Enter SVC mode
    push {r0,r1}
    blx interruptHandler
    pop {r0,r1}
    add r1, #4
    //vldmia r1!, {s0-s31}
    ldmia r1, {r0-r12,pc}
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
.global panic
.extern panic2
panic:
    push {r0-r12,lr}
    mrs r2, cpsr
    push {r2}
    mov r1, sp
    b panic2
.section .bss
.space 4096
stack:
