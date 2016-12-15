OUTPUT_ARCH(arm)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x08134100, LENGTH = 0x000BF00
}

SECTIONS
{
	.text : ALIGN(0x100) {
		build/crt0.o(.init)
		*(.text)
	}
	.rodata : {
		*(.rodata*)
	}
}

