OUTPUT(sd_loader.elf);

ENTRY(_start);

SECTIONS {
	. = 0x00800000;
	.text : {
		*(.kernel_code*);
		*(.text*);
		/* Tell linker to not garbage collect this section as it is not referenced anywhere */
		KEEP(*(.kernel_code*));
	}
	.data : {
		*(.rodata) *(.rodata.*);
		*(.data*);
		*(.bss*);
	}
	.rodata1 : {
		*(.rodata1) *(.rodata1.*)
	}
	/DISCARD/ : {
		*(*);
	}
}

ASSERT((SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.rodata1)) <= 0x1300, "Memory overlapping with main elf.");
