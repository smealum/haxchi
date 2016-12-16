.section ".init"
.thumb
.syntax unified

.globl _start

_start:
	ldr r0, =#0x00050000
	cmp r0, r1
	bne nomatch
	ldr r0, =#0x00000000 @will be written in by CBHC
	cmp r0, r2
	beq match
nomatch:
	@orignal code
	movs r0, #0x9F
	movs r3, #0x9F
	bx lr
match:
	@return delete error
	movs r0, #0
	ldr r3, =#(0x05108236+1)
	bx r3
