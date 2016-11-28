
	.extern _main
    .globl _start

_start:
	mflr 0
	stwu 1,-0x18(1)
	stw 0,0x1C(1)
	stw 3,8(1)
	stw 4,0xC(1)
    # jump to our main
    bl _main
	# launch original title
	mtctr 3
	lwz 3,8(1)
	lwz 4,0xC(1)
	lwz 0,0x1C(1)
	mtlr 0
	addi 1,1,0x18
	bctr
