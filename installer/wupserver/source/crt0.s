.section ".init"
.arm
.align 4

.globl _start

.extern ipc_thread
.type ipc_thread, %function

_start:
	@wupserver
	mov r0, #0
	bl ipc_thread
	@jump to original code
	ldr r1,[pc]
	bx r1
	.word (0x05027954)+1
