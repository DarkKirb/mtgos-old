ENTRY(_start)
SECTIONS
{
    . = 0x1FF80000;
        kernel_start = .;
        .text : {
                *(.text)
        }
        .constructors : {
                start_ctors = .;
                        KEEP(*( .init_array ));
                        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
                        KEEP(*(.ctors ));
                end_ctors = .;
                start_dtors = .;
                        KEEP(*(.dtors));
                end_dtors = .;
                start_eh_frame = .;
                KEEP(*(.eh_frame));
                KEEP(*(.eh_frame*));
                QUAD(0);
                KEEP(*(.gcc_except_table));
                KEEP(*(.gcc_except_table*));
                *(.data)
        }
        ARM.extab : {
            KEEP(*(.ARM.extab* .gnu.linkonce.armextab.*));
        }
        __exidx_start = .;
        ARM.exidx : {
            KEEP(*(.ARM.exidx* .gnu.linkonce.armexidx.*));
        }
        __exidx_end = .;
        .rodata : {
                *(.rodata)
        }
        .bss : {
                *(.bss)
                *(COMMON)
        }
        . = 0x1FFFF000;
        .text.boot : {
            KEEP(*(.text.boot));
        }
        .data : {
            *(.data)
            *(.data.*)
        }
        kernel_end=.;
}
