.global loadIDT
.section .text
loadIDT:

.macro idtEntry offset cs
.short \offset&0xFFFF
.short \cs
.byte 0
.byte 0b11101110
.short \offset>>16
.endm
//idtEntry loadIDT 0
