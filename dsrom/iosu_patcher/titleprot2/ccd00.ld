OUTPUT_ARCH(arm)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x0510D9AC, LENGTH = 0x0000110
}

SECTIONS
{
	.text : {
		build/crt0.o(.init)
		*(.text)
		*(.rodata)
	}
}

