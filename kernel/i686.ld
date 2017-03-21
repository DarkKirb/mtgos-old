ENTRY(_start)
SECTIONS
{
        . = 0x100000;
        kernel_start = .;
        .text : {
                KEEP(*(.btext));
                *(.text)
        }
        .data : {
                start_ctors = .;
                        KEEP(*( .init_array ));
                        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
                        KEEP(*(.ctors));
                end_ctors = .;
                start_dtors = .;
                        KEEP(*(.dtors));
                end_dtors = .;
                start_eh_frame = .;
                *(.eh_frame);
                . += 4;
                *(.data)
        }
        .rodata : {
                *(.rodata)
        }
        .bss : {
                *(.bss)
                *(COMMON)
        }
        kernel_end = .;
}
