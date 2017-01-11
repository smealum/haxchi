OUTPUT_ARCH(arm)

MEMORY
{
	RAMX (rx) : ORIGIN = 0x0510D988, LENGTH = 0x0000024
}

SECTIONS
{
	.text : {
		build/crt0.o(.init)
		*(.text)
		*(.rodata)
	}
}

