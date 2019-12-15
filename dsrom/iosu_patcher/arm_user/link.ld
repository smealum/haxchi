OUTPUT_ARCH(arm)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x101312D0, LENGTH = 0x000BF00
}

SECTIONS
{
	.text : ALIGN(0x04) {
		build/crt0.o(.init)
		*(.text)
	}
	.rodata : {
		*(.rodata*)
	}
}

