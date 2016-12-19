
	.extern __main
    .globl _start

_start:
    # jump to our main
    bl __main
	mtlr r3
	blr
