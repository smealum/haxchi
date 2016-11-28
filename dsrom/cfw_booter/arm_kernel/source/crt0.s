.section ".init"
.arm
.align 4

.extern _main
.type _main, %function

.extern memset
.type memset, %function

_start:
	b _main
