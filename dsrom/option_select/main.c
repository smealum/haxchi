/*
 * Copyright (C) 2016 FIX94
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <string.h>
#include "types.h"
#include "coreinit.h"
#include "../global.h"

typedef struct
{
    float x,y;
} Vec2D;

typedef struct
{
    uint16_t x, y;               /* Touch coordinates */
    uint16_t touched;            /* 1 = Touched, 0 = Not touched */
    uint16_t invalid;            /* 0 = All valid, 1 = X invalid, 2 = Y invalid, 3 = Both invalid? */
} VPADTPData;

typedef struct
{
    uint32_t btns_h;                  /* Held buttons */
    uint32_t btns_d;                  /* Buttons that are pressed at that instant */
    uint32_t btns_r;                  /* Released buttons */
    Vec2D lstick, rstick;        /* Each contains 4-byte X and Y components */
    char unknown1c[0x52 - 0x1c]; /* Contains accelerometer and gyroscope data somewhere */
    VPADTPData tpdata;           /* Normal touchscreen data */
    VPADTPData tpdata1;          /* Modified touchscreen data 1 */
    VPADTPData tpdata2;          /* Modified touchscreen data 2 */
    char unknown6a[0xa0 - 0x6a];
    uint8_t volume;
    uint8_t battery;             /* 0 to 6 */
    uint8_t unk_volume;          /* One less than volume */
    char unknowna4[0xac - 0xa4];
} VPADData;

#define BUTTON_A        0x8000
#define BUTTON_B        0x4000
#define BUTTON_X        0x2000
#define BUTTON_Y        0x1000
#define BUTTON_LEFT     0x0800
#define BUTTON_RIGHT    0x0400
#define BUTTON_UP       0x0200
#define BUTTON_DOWN     0x0100
#define BUTTON_ZL       0x0080
#define BUTTON_ZR       0x0040
#define BUTTON_L        0x0020
#define BUTTON_R        0x0010
#define BUTTON_PLUS     0x0008
#define BUTTON_MINUS    0x0004
#define BUTTON_HOME     0x0002
#define BUTTON_SYNC     0x0001

typedef struct
{
	int val;
	char txt[12];
} config_select;

static const config_select sel[17] = {
	{BUTTON_A,"a="},
	{BUTTON_B,"b="},
	{BUTTON_X,"x="},
	{BUTTON_Y,"y="},
	{BUTTON_LEFT,"left="},
	{BUTTON_RIGHT,"right="},
	{BUTTON_UP,"up="},
	{BUTTON_DOWN,"down="},
	{BUTTON_ZL,"zl="},
	{BUTTON_ZR,"zr="},
	{BUTTON_L,"l="},
	{BUTTON_R,"r="},
	{BUTTON_PLUS,"plus="},
	{BUTTON_MINUS,"minus="},
	{BUTTON_HOME,"home="},
	{BUTTON_SYNC,"sync="},
	{0,"default="},
};

typedef struct
{
    uint32_t flag;
    uint32_t permission;
    uint32_t owner_id;
    uint32_t group_id;
    uint32_t size;
    uint32_t alloc_size;
    uint64_t quota_size;
    uint32_t ent_id;
    uint64_t ctime;
    uint64_t mtime;
    uint8_t attributes[48];
} __attribute__((packed)) FSStat;

#define MIN(a, b) (((a)>(b))?(b):(a))

uint32_t __main(void)
{
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	unsigned int *pMEMAllocFromDefaultHeapEx;
	unsigned int *pMEMAllocFromDefaultHeap;
	unsigned int *pMEMFreeToDefaultHeap;
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeapEx", &pMEMAllocFromDefaultHeapEx);
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeap", &pMEMAllocFromDefaultHeap);
	OSDynLoad_FindExport(coreinit_handle, 1, "MEMFreeToDefaultHeap", &pMEMFreeToDefaultHeap);
	void*(*MEMAllocFromDefaultHeapEx)(int size, int align) = (void*)(*pMEMAllocFromDefaultHeapEx);
	void*(*MEMAllocFromDefaultHeap)(int size) = (void*)(*pMEMAllocFromDefaultHeap);
	void(*MEMFreeToDefaultHeap)(void *ptr) = (void*)(*pMEMFreeToDefaultHeap);

	int launchmode = LAUNCH_HBL;
	//default path goes to HBL
	strcpy((void*)0xF5E70000,"/vol/external01/wiiu/apps/homebrew_launcher/homebrew_launcher.elf");

	int iFd = -1;
	void *pClient = MEMAllocFromDefaultHeapEx(0x1700,4);
	void *pCmd = MEMAllocFromDefaultHeapEx(0xA80,4);
	void *pBuffer = NULL;

	void (*DCStoreRange)(void *buffer, uint32_t length);
	OSDynLoad_FindExport(coreinit_handle, 0, "DCStoreRange", &DCStoreRange);

	int(*FSInit)(void);
	int(*FSAddClientEx)(void *pClient, int unk_zero_param, int errHandling);
	int(*FSDelClient)(void *pClient);
	void(*FSInitCmdBlock)(void *pCmd);
	int(*FSOpenFile)(void *pClient, void *pCmd, const char *path, const char *mode, int *fd, int errHandling);
	int(*FSGetStatFile)(void *pClient, void *pCmd, int fd, void *buffer, int error);
	int(*FSReadFile)(void *pClient, void *pCmd, void *buffer, int size, int count, int fd, int flag, int errHandling);
	int(*FSCloseFile)(void *pClient, void *pCmd, int fd, int errHandling);

	OSDynLoad_FindExport(coreinit_handle, 0, "FSInit", &FSInit);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSInitCmdBlock", &FSInitCmdBlock);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSAddClientEx", &FSAddClientEx);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSDelClient", &FSDelClient);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSOpenFile", &FSOpenFile);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSGetStatFile", &FSGetStatFile);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSReadFile", &FSReadFile);
	OSDynLoad_FindExport(coreinit_handle, 0, "FSCloseFile", &FSCloseFile);

	FSInit();
	FSInitCmdBlock(pCmd);
	FSAddClientEx(pClient, 0, -1);
	FSOpenFile(pClient, pCmd, "/vol/content/config.txt", "r", &iFd, -1);
	if(iFd < 0)
		goto fileEnd;
	FSStat stat;
	stat.size = 0;

	FSGetStatFile(pClient, pCmd, iFd, &stat, -1);

	if(stat.size > 0)
	{
		pBuffer = MEMAllocFromDefaultHeapEx(stat.size+1,0x40);
		memset(pBuffer,0,stat.size+1);
	}
	else
		goto fileEnd;

	unsigned int done = 0;

	while(done < stat.size)
	{
		int readBytes = FSReadFile(pClient, pCmd, pBuffer + done, 1, stat.size - done, iFd, 0, -1);
		if(readBytes <= 0) {
			break;
		}
		done += readBytes;
	}

	unsigned int vpad_handle;
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);
	int(*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);

	char *fList = (char*)pBuffer;

	int error;
	VPADData vpad_data;
	VPADRead(0, &vpad_data, 1, &error);
	char FnameChar[256];
	memset(FnameChar,0,256);
	int i;
	for(i = 0; i < 17; i++)
	{
		if((vpad_data.btns_h & sel[i].val) || (sel[i].val == 0))
		{
			char *n = strstr(fList,sel[i].txt);
			if(n)
			{
				char *fEnd = NULL;
				char *fName = n+strlen(sel[i].txt);
				char *fEndR = strchr(fName,'\r');
				char *fEndN = strchr(fName,'\n');
				if(fEndR)
				{
					if(fEndN && fEndN < fEndR)
						fEnd = fEndN;
					else
						fEnd = fEndR;
				}
				else if(fEndN)
				{
					if(fEndR && fEndR < fEndN)
						fEnd = fEndR;
					else
						fEnd = fEndN;
				}
				else
					fEnd = fName+strlen(fName);
				if(fEnd && fName < fEnd)
				{
					int fLen = MIN(fEnd-fName,255);
					memcpy(FnameChar, fName, fLen);
					if(memcmp(FnameChar+fLen-6,"fw.img",7) == 0)
					{
						if(FnameChar[0] == '/' && fLen > 7 && *(FnameChar+fLen-7) == '/')
						{
							*(FnameChar+fLen-7) = '\0';
							__os_snprintf((void*)0xF5E70000,32,"/vol/sdcard%s",FnameChar);
						}
						else if(FnameChar[0] != '/' && fLen > 6 && *(FnameChar+fLen-7) == '/')
						{
							*(FnameChar+fLen-7) = '\0';
							__os_snprintf((void*)0xF5E70000,32,"/vol/sdcard/%s",FnameChar);
						}
						else
							__os_snprintf((void*)0xF5E70000,32,"/vol/sdcard");
						launchmode = LAUNCH_CFW_IMG;
						break;
					}
					else if(memcmp(FnameChar+fLen-4,".elf",5) == 0)
					{
						if(FnameChar[0] == '/')
							__os_snprintf((void*)0xF5E70000,250,"/vol/external01%s",FnameChar);
						else
							__os_snprintf((void*)0xF5E70000,250,"/vol/external01/%s",FnameChar);
						launchmode = LAUNCH_HBL;
						break;
					}
					else if(memcmp(FnameChar+fLen-7,"sysmenu",8) == 0)
					{
						launchmode = LAUNCH_SYSMENU;
						break;
					}
				}
			}
		}
	}
fileEnd:
	if(pClient && pCmd)
	{
		if(iFd >= 0)
			FSCloseFile(pClient, pCmd, iFd, -1);
		FSDelClient(pClient);
		MEMFreeToDefaultHeap(pClient);
		MEMFreeToDefaultHeap(pCmd);
	}
	if(pBuffer)
		MEMFreeToDefaultHeap(pBuffer);

	if(launchmode == LAUNCH_HBL)
		return 0x01800000;

	//store path to sd fw.img for arm_kernel
	if(launchmode == LAUNCH_CFW_IMG)
		DCStoreRange((void*)0xF5E70000,0x100);

	unsigned int sysapp_handle;
	OSDynLoad_Acquire("sysapp.rpl", &sysapp_handle);
	void (*SYSLaunchMenu)(void);
	OSDynLoad_FindExport(sysapp_handle, 0,"SYSLaunchMenu", &SYSLaunchMenu);

	int (*OSForceFullRelaunch)(void);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSForceFullRelaunch", &OSForceFullRelaunch);

	void (*OSExitThread)(int);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSExitThread", &OSExitThread);

	//do iosu patches
	void (*patch_iosu)(unsigned int coreinit_handle, unsigned int sysapp_handle, int launchmode, int from_cbhc) = (void*)0x01804000;
	patch_iosu(coreinit_handle, sysapp_handle, launchmode, 0);

	if(launchmode == LAUNCH_CFW_IMG)
		OSForceFullRelaunch();
	SYSLaunchMenu();

	OSExitThread(0);
	return 0;
}
