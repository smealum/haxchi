.section ".init"
.arm
.align 4

.extern _main
.type _main, %function

.extern memset
.type memset, %function

_start:
	b _main

    .global IOS_DCFlushAllCache
IOS_DCFlushAllCache:
    MOV R15, R0
clean_loop:
    MRC p15, 0, r15, c7, c10, 3
    BNE clean_loop
    MCR p15, 0, R0, c7, c10, 4
