OUTPUT_ARCH(arm)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x08135000, LENGTH = 0x000B000
}

SECTIONS
{
	.text : ALIGN(0x100) {
		__file_start = .;
		build/crt0.o(.init)
		*(.text)
	}
	.rodata : {
		*(.rodata*)
		__file_end = .;
	}
}

