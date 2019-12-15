.section ".text"
.arm
.align 4

.globl disable_mmu
.type disable_mmu, %function
disable_mmu:
	mrc p15, 0, r0, c1, c0, 0
	ldr r1, =#0xFFFFEFFA
	and r1, r0, r1
	mcr p15, 0, r1, c1, c0, 0
	bx lr

.globl restore_mmu
.type restore_mmu, %function
restore_mmu:
	mcr p15, 0, r0, c1, c0, 0
	bx lr
