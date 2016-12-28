.section ".init"
.thumb
.syntax unified

.globl _start

_start:
	ldr r0, [r4,#0x10]
	ldr r1, =#0x00050000
	cmp r0, r1
	bne exit_ok
	ldr r0, [r4,#0x14]
	ldr r1, =#0x00000000 @will be written in by CBHC
	cmp r0, r1
	bne exit_ok
	@return our error message
	ldr r0, =#0x99999999
	ldr r3, =#(0x050146A4+1)
	bx r3
exit_ok:
	@original code
	lsrs r2, r3, #0x10
	lsls r3, r2, #0x10
	bx lr
