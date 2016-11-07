OUTPUT_ARCH(powerpc)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x01800000, LENGTH = 0x00010000
	RAMRW (rw!i) : ORIGIN = 0x20000000, LENGTH = 0x00002000
}

SECTIONS
{
	.text : ALIGN(0x100) {
		build/crt0.o(.init)
		*(.text)
		*(.rodata)
		*(.sdata)
	}

	.bss : {
		_bss_start = .;
		*(.bss);
		*(.sbss);
	}
	_bss_end = .;
}

