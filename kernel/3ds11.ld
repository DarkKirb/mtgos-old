ENTRY(_start)
SECTIONS
{
    . = 0x1FF80000;
        kernel_start = .;
        __exidx_start = .;
        .text : {
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
        __exidx_end=.;
        .bss : {
                *(.bss)
                *(COMMON)
        }
        . = 0x1FFFF000;
        .text.boot : {
            KEEP(*(.text.boot));
        }
        kernel_end=.;
}
