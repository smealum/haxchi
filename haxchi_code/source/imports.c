#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imports.h"

#define COREINIT_OFFSET (- 0xFE3C00)

// dynamically linked
int (*IOS_Open)(const char* dev, int flags) = NULL;
int (*IOS_Ioctl)(int fd, u32 cmd, void* buf_in, u32 size_in, void* buf_out, u32 size_out) = NULL;
void (*DCFlushRange)(void *buffer, uint32_t length) = NULL;
int (*OSScreenClearBufferEx)(int bufferNum, u32 val) = NULL;
int (*OSScreenPutFontEx)(int bufferNum, unsigned int posX, unsigned int line, void* buffer) = NULL;
void (*OSFatal)(char*) = NULL;
void (*OSForceFullRelaunch)(void) = NULL;
void (*OSRestartGame)(void*, void*) = NULL;
int (*SYSLaunchMenu)(void) = NULL;
int (*SYSRelaunchTitle)(int, void*) = NULL;
int (*SYSLaunchSettings)(void*) = NULL;
int (*SYSSwitchToBrowser)(char*) = NULL;
int (*_SYSLaunchSettingsDirect)(void*) = NULL;
void (*Exit)(void) = NULL;
void (*_exit)(void) = NULL;
void (*OSScreenInit)(void) = NULL;
int (*OSScreenGetBufferSizeEx)(int bufferNum) = NULL;
int (*OSScreenSetBufferEx)(int bufferNum, void* addr) = NULL;
int (*OSScreenFlipBuffersEx)(int bufferNum) = NULL;
void* (*OSAllocFromSystem)(u32 size, int align) = NULL;
void (*OSFreeToSystem)(void *ptr) = NULL;
int (*IM_Open)() = NULL;
int (*IM_Close)(int fd) = NULL;
int (*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b) = NULL;
int (*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);

// 5.5.x values
int (*const OSDynLoad_Acquire)(const char* lib_name, int* out_addr) = (void*)(COREINIT_OFFSET + 0x0200DFB4);
int (*const OSDynLoad_FindExport)(int lib_handle, int flags, const char* name, void* out_addr) = (void*)(COREINIT_OFFSET + 0x0200F428);

void init_imports()
{
	int coreinit_handle, sysapp_handle, vpad_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	OSDynLoad_Acquire("sysapp.rpl", &sysapp_handle);
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);

	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Open", &IOS_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Ioctl", &IOS_Ioctl);
	
	OSDynLoad_FindExport(coreinit_handle, 0, "DCFlushRange", &DCFlushRange);
	
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutFontEx", &OSScreenPutFontEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenClearBufferEx", &OSScreenClearBufferEx);

	OSDynLoad_FindExport(coreinit_handle, 0, "OSFatal", &OSFatal);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSForceFullRelaunch", &OSForceFullRelaunch);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSRestartGame", &OSRestartGame);
	OSDynLoad_FindExport(sysapp_handle, 0, "SYSLaunchMenu", &SYSLaunchMenu);
	OSDynLoad_FindExport(sysapp_handle, 0, "SYSRelaunchTitle", &SYSRelaunchTitle);
	OSDynLoad_FindExport(sysapp_handle, 0, "SYSLaunchSettings", &SYSLaunchSettings);
	OSDynLoad_FindExport(sysapp_handle, 0, "SYSSwitchToBrowser", &SYSSwitchToBrowser);
	OSDynLoad_FindExport(sysapp_handle, 0, "_SYSLaunchSettingsDirect", &_SYSLaunchSettingsDirect);
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &Exit);
	OSDynLoad_FindExport(coreinit_handle, 0, "exit", &_exit);

	OSDynLoad_FindExport(coreinit_handle, 0, "OSAllocFromSystem", &OSAllocFromSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSFreeToSystem", &OSFreeToSystem);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Open", &IM_Open);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_Close", &IM_Close);
	OSDynLoad_FindExport(coreinit_handle, 0, "IM_SetDeviceState", &IM_SetDeviceState);

	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenInit", &OSScreenInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenGetBufferSizeEx", &OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenFlipBuffersEx", &OSScreenFlipBuffersEx);

	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);
}

void* memset(void *d, int v, size_t size)
{
	int i;
	for(i = 0; i < size; i++) ((unsigned char*)d)[i] = v;
	return d;
}

void* memcpy(void *d, const void* s, size_t size)
{
	int i;
	for(i = 0; i < size; i++) ((unsigned char*)d)[i] = ((unsigned char*)s)[i];
	return d;
}
