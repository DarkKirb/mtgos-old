ENTRY(_start)
SECTIONS
{
    . = 0x200000;
    kernel_start = .;
    .text : {
        *(.btext)
        *(.text)
    }
    .data ALIGN(0x200000) : {
        start_ctors = .;
            KEEP(*( .init_array ));
            KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
            *(.ctors)
        end_ctors = .;
        start_dtors = .;
            *(.dtors)
        end_dtors = .;
        *(.data)
    }
    .rodata ALIGN(0x200000) : {
        *(.rodata)
    }
    .bss ALIGN(0x200000) : {
        *(.bss)
    }
    . = ALIGN(0x200000);
    kernel_end = .;
}
