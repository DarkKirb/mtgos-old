ENTRY(_start)
SECTIONS
{
        . = 0x08000000;
        kernel_start = .;
        __exidx_start = .;
        .text : {
                KEEP(*(.btext));
                *(.text)
        }
        .data : {
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
        .rodata : {
                *(.rodata)
        }
        __exidx_end = .;
        .bss : {
                *(.bss)
                *(COMMON)
        }
        kernel_end = .;
}
