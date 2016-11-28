/*
 * Copyright (C) 2016 FIX94
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "system/memory.h"
#include "common/common.h"
#include "main.h"
#include "exploit.h"
#include "iosuhax.h"
#include "gameList.h"

static const char *sdCardVolPath = "/vol/storage_sdcard";

//just to be able to call async
void someFunc(void *arg)
{
	(void)arg;
}

static int mcp_hook_fd = -1;
int MCPHookOpen()
{
	//take over mcp thread
	mcp_hook_fd = MCP_Open();
	if(mcp_hook_fd < 0)
		return -1;
	IOS_IoctlAsync(mcp_hook_fd, 0x62, (void*)0, 0, (void*)0, 0, someFunc, (void*)0);
	//let wupserver start up
	sleep(1);
	if(IOSUHAX_Open() < 0)
		return -1;
	return 0;
}

void MCPHookClose()
{
	if(mcp_hook_fd < 0)
		return;
	//close down wupserver, return control to mcp
	IOSUHAX_Close();
	//wait for mcp to return
	sleep(1);
	MCP_Close(mcp_hook_fd);
	mcp_hook_fd = -1;
}

void println_noflip(int line, const char *msg)
{
	OSScreenPutFontEx(0,0,line,msg);
	OSScreenPutFontEx(1,0,line,msg);
}

void println(int line, const char *msg)
{
	int i;
	for(i = 0; i < 2; i++)
	{	//double-buffered font write
		println_noflip(line,msg);
		OSScreenFlipBuffersEx(0);
		OSScreenFlipBuffersEx(1);
	}
}

typedef struct _parsedList_t {
	uint32_t tid;
	char name[64];
	char path[64];
	u8 *romPtr;
	u32 romSize;
} parsedList_t;

int fsa_read(int fsa_fd, int fd, void *buf, int len)
{
	int done = 0;
	uint8_t *buf_u8 = (uint8_t*)buf;
	while(done < len)
	{
		size_t read_size = len - done;
		int result = IOSUHAX_FSA_ReadFile(fsa_fd, buf_u8 + done, 0x01, read_size, fd, 0);
		if(result < 0)
			return result;
		else
			done += result;
	}
	return done;
}

int fsa_write(int fsa_fd, int fd, void *buf, int len)
{
	int done = 0;
	uint8_t *buf_u8 = (uint8_t*)buf;
	while(done < len)
	{
		size_t write_size = len - done;
		int result = IOSUHAX_FSA_WriteFile(fsa_fd, buf_u8 + done, 0x01, write_size, fd, 0);
		if(result < 0)
			return result;
		else
			done += result;
	}
	return done;
}

int Menu_Main(void)
{
	InitOSFunctionPointers();
	InitSysFunctionPointers();
	InitVPadFunctionPointers();
    VPADInit();

    // Init screen
    OSScreenInit();
    int screen_buf0_size = OSScreenGetBufferSizeEx(0);
    int screen_buf1_size = OSScreenGetBufferSizeEx(1);
	uint8_t *screenBuffer = memalign(0x100, screen_buf0_size+screen_buf1_size);
    OSScreenSetBufferEx(0, screenBuffer);
    OSScreenSetBufferEx(1, (screenBuffer + screen_buf0_size));
    OSScreenEnableEx(0, 1);
    OSScreenEnableEx(1, 1);
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);

	int mcp_handle = MCP_Open();
	int count = MCP_TitleCount(mcp_handle);
	int listSize = count*0x61;
	char *tList = memalign(32, listSize);
	memset(tList, 0, listSize);
	int recievedCount = count;
	MCP_TitleList(mcp_handle, &recievedCount, tList, listSize);
	MCP_Close(mcp_handle);

	int gAvailCnt = 0;
	parsedList_t *gAvail = (parsedList_t*)malloc(recievedCount*sizeof(parsedList_t));
	memset(gAvail, 0, recievedCount*sizeof(parsedList_t));

	int i, j;
	for(i = 0; i < recievedCount; i++)
	{
		char *cListElm = tList+(i*0x61);
		if(memcmp(cListElm+0x56,"mlc",4) != 0 || *(uint32_t*)(cListElm) != 0x00050000)
			continue;
		for(j = 0; j < GameListSize; j++)
		{
			if(*(uint32_t*)(cListElm+4) == GameList[j].tid)
			{
				gAvail[gAvailCnt].tid = GameList[j].tid;
				memcpy(gAvail[gAvailCnt].name, GameList[j].name, 64);
				memcpy(gAvail[gAvailCnt].path, cListElm+0xC, 64);
				gAvail[gAvailCnt].romPtr = GameList[j].romPtr;
				gAvail[gAvailCnt].romSize = *(GameList[j].romSizePtr);
				gAvailCnt++;
				break;
			}
		}
	}

    int vpadError = -1;
    VPADData vpad;

	u32 redraw = 1;
	s32 PosX = 0;
	s32 ScrollX = 0;

	s32 ListMax = gAvailCnt;
	if( ListMax > 12 )
		ListMax = 12;

	u32 UpHeld = 0, DownHeld = 0;
	while(1)
	{
		usleep(25000);
		VPADRead(0, &vpad, 1, &vpadError);
		if(vpadError != 0)
			continue;

		if((vpad.btns_d | vpad.btns_h) & VPAD_BUTTON_HOME)
		{
			OSScreenEnableEx(0, 0);
			OSScreenEnableEx(1, 0);
			free(screenBuffer);
			return EXIT_SUCCESS;
		}
		if( vpad.btns_h & VPAD_BUTTON_DOWN )
		{
			if(DownHeld == 0 || DownHeld > 10)
			{
				if( PosX + 1 >= ListMax )
				{
					if( PosX + 1 + ScrollX < gAvailCnt)
						ScrollX++;
					else {
						PosX	= 0;
						ScrollX = 0;
					}
				} else {
					PosX++;
				}
				redraw = 1;
			}
			DownHeld++;
		}
		else
			DownHeld = 0;

		if( vpad.btns_h & VPAD_BUTTON_UP )
		{
			if(UpHeld == 0 || UpHeld > 10)
			{
				if( PosX <= 0 )
				{
					if( ScrollX > 0 )
						ScrollX--;
					else {
						PosX	= ListMax - 1;
						ScrollX = gAvailCnt - ListMax;
					}
				} else {
					PosX--;
				}
				redraw = 1;
			}
			UpHeld++;
		}
		else
			UpHeld = 0;

		if( vpad.btns_d & VPAD_BUTTON_A )
			break;

		if( redraw )
		{
			OSScreenClearBufferEx(0, 0);
			OSScreenClearBufferEx(1, 0);
			println_noflip(0,"Haxchi v2.0 by FIX94");
			println_noflip(1,"Credits to smea, plutoo, yellows8, naehrwert, derrek and dimok");
			println_noflip(2,"Please select the game for the Installation from the list below.");
			// Starting position.
			int gamelist_y = 4;
			for (i = 0; i < ListMax; ++i, gamelist_y++)
			{
				const parsedList_t *cur_gi = &gAvail[i+ScrollX];
				char printStr[64];
				sprintf(printStr,"%c %s", i == PosX ? '>' : ' ', cur_gi->name);
				println_noflip(gamelist_y, printStr);
			}
			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
			redraw = 0;
		}
	}
	const parsedList_t *SelectedGame = &gAvail[PosX + ScrollX];
	for(j = 0; j < 2; j++)
	{
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		println_noflip(0,"Haxchi v2.0 by FIX94");
		println_noflip(1,"Credits to smea, plutoo, yellows8, naehrwert, derrek and dimok");
		println_noflip(2,"You have selected the following game:");
		println_noflip(3,SelectedGame->name);
		println_noflip(4,"This will install Haxchi. To remove it you have to delete and");
		println_noflip(5,"re-install the game. If you are sure press A, else press HOME.");
		OSScreenFlipBuffersEx(0);
		OSScreenFlipBuffersEx(1);
		usleep(25000);
	}
	while(1)
	{
		usleep(25000);
		VPADRead(0, &vpad, 1, &vpadError);
		if(vpadError != 0)
			continue;
		//user aborted
		if((vpad.btns_d | vpad.btns_h) & VPAD_BUTTON_HOME)
		{
			OSScreenEnableEx(0, 0);
			OSScreenEnableEx(1, 0);
			free(screenBuffer);
			return EXIT_SUCCESS;
		}
		//lets go!
		if(vpad.btns_d & VPAD_BUTTON_A)
			break;
	}

	//will inject our custom mcp code
	int line = 6;
	println(line++,"Doing IOSU Exploit...");
	IOSUExploit();

	int fsaFd = -1;
	int sdMounted = 0;
	int sdFd = -1, mlcFd = -1;

	//done with iosu exploit, take over mcp
	if(MCPHookOpen() < 0)
	{
		println(line++,"MCP hook could not be opened!");
		goto prgEnd;
	}

	//mount with full permissions
	fsaFd = IOSUHAX_FSA_Open();
	if(fsaFd < 0)
	{
		println(line++,"FSA could not be opened!");
		goto prgEnd;
	}
	int ret = IOSUHAX_FSA_Mount(fsaFd, "/dev/sdcard01", sdCardVolPath, 2, (void*)0, 0);
	if(ret < 0)
	{
		println(line++,"Failed to mount SD!");
		goto prgEnd;
	}
	else
		sdMounted = 1;
	char path[256];
	sprintf(path,"%s/content/0010/rom.zip",SelectedGame->path);
	if(IOSUHAX_FSA_OpenFile(fsaFd, path, "rb", &mlcFd) < 0)
	{
		println(line++,"No existing rom.zip not found in the game!");
		println(line++,"Make sure to update your DS title and try again.");
		goto prgEnd;
	}
	else
		IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
	if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
	{
		println(line++,"Writing rom.zip...");
		fsa_write(fsaFd, mlcFd, SelectedGame->romPtr, SelectedGame->romSize);
		IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
		mlcFd = -1;
	}

	char sdHaxchiPath[256];
	sprintf(sdHaxchiPath,"%s/haxchi",sdCardVolPath);

	char sdPath[256];
	sprintf(sdPath,"%s/config.txt",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t cfgSize = stats.size;
		uint8_t *cfgBuf = malloc(cfgSize);
		fsa_read(fsaFd, sdFd, cfgBuf, cfgSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		//write to nand
		sprintf(path,"%s/content/config.txt",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
		{
			println(line++,"Writing config.txt...");
			fsa_write(fsaFd, mlcFd, cfgBuf, cfgSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
			//make it readable by game
			IOSUHAX_FSA_ChangeMode(fsaFd, path, 0x644);
		}
		free(cfgBuf);
	}

	sprintf(sdPath,"%s/title.txt",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t titleSize = stats.size;
		xmlChar *titleBuf = malloc(titleSize+1);
		memset(titleBuf, 0, titleSize+1);
		fsa_read(fsaFd, sdFd, titleBuf, titleSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		sprintf(path,"%s/meta/meta.xml",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "rb", &mlcFd) >= 0)
		{
			IOSUHAX_FSA_StatFile(fsaFd, mlcFd, &stats);
			size_t metaSize = stats.size;
			char *metaBuf = malloc(metaSize);
			fsa_read(fsaFd, mlcFd, metaBuf, metaSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
			//parse doc
			xmlDocPtr doc = xmlReadMemory(metaBuf, metaSize, "meta.xml", "utf-8", 0);
			//change title
			xmlNode *root_element = xmlDocGetRootElement(doc);
			xmlNode *cur_node = NULL;
			for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if(memcmp(cur_node->name, "longname_", 9) == 0 || memcmp(cur_node->name, "shortname_", 10) == 0)
					{
						if(xmlNodeGetContent(cur_node) == NULL || !strlen((char*)xmlNodeGetContent(cur_node))) continue;
						xmlNodeSetContent(cur_node, titleBuf);
					}
				}
			}
			//back to xml
			xmlChar *newXml = NULL;
			int newSize = 0;
			xmlSaveNoEmptyTags = 1; //keeps original style
			xmlDocDumpFormatMemoryEnc(doc, &newXml, &newSize, "utf-8", 0);
			xmlFreeDoc(doc);
			free(metaBuf);
			//libxml2 adds in extra \n at the end
			if(newXml != NULL && newSize > 0 && newXml[newSize-1] == '\n')
			{
				newXml[newSize-1] = '\0';
				newSize--;
			}
			//write back to nand
			if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
			{
				println(line++,"Changing game title...");
				//UTF-8 BOM
				char bom[3] = { 0xEF, 0xBB, 0xBF };
				if(memcmp(newXml, bom, 3) != 0)
					fsa_write(fsaFd, mlcFd, bom, 0x03);
				fsa_write(fsaFd, mlcFd, newXml, newSize);
				IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
				mlcFd = -1;
				free(newXml);
			}
		}
		free(titleBuf);
	}

	sprintf(sdPath,"%s/bootDrcTex.tga",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t bootDrcTexSize = stats.size;
		uint8_t *bootDrcTex = malloc(bootDrcTexSize);
		fsa_read(fsaFd, sdFd, bootDrcTex, bootDrcTexSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		//write to nand
		sprintf(path,"%s/meta/bootDrcTex.tga",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
		{
			println(line++,"Writing bootDrcTex.tga...");
			fsa_write(fsaFd, mlcFd, bootDrcTex, bootDrcTexSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
		}
		free(bootDrcTex);
	}

	sprintf(sdPath,"%s/bootTvTex.tga",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t bootTvTexSize = stats.size;
		uint8_t *bootTvTex = malloc(bootTvTexSize);
		fsa_read(fsaFd, sdFd, bootTvTex, bootTvTexSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		//write to nand
		sprintf(path,"%s/meta/bootTvTex.tga",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
		{
			println(line++,"Writing bootTvTex.tga...");
			fsa_write(fsaFd, mlcFd, bootTvTex, bootTvTexSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
		}
		free(bootTvTex);
	}

	sprintf(sdPath,"%s/iconTex.tga",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t iconTexSize = stats.size;
		uint8_t *iconTex = malloc(iconTexSize);
		fsa_read(fsaFd, sdFd, iconTex, iconTexSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		//write to nand
		sprintf(path,"%s/meta/iconTex.tga",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
		{
			println(line++,"Writing iconTex.tga...");
			fsa_write(fsaFd, mlcFd, iconTex, iconTexSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
		}
		free(iconTex);
	}

	sprintf(sdPath,"%s/bootSound.btsnd",sdHaxchiPath);
	if(IOSUHAX_FSA_OpenFile(fsaFd, sdPath, "rb", &sdFd) >= 0)
	{
		//read in sd file
		fileStat_s stats;
		IOSUHAX_FSA_StatFile(fsaFd, sdFd, &stats);
		size_t bootSoundSize = stats.size;
		uint8_t *bootSound = malloc(bootSoundSize);
		fsa_read(fsaFd, sdFd, bootSound, bootSoundSize);
		IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		sdFd = -1;
		//write to nand
		sprintf(path,"%s/meta/bootSound.btsnd",SelectedGame->path);
		if(IOSUHAX_FSA_OpenFile(fsaFd, path, "wb", &mlcFd) >= 0)
		{
			println(line++,"Writing bootSound.btsnd...");
			fsa_write(fsaFd, mlcFd, bootSound, bootSoundSize);
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
			mlcFd = -1;
		}
		free(bootSound);
	}

	println(line++,"Done installing Haxchi!");

prgEnd:
	if(tList)
		free(tList);
	if(gAvail)
		free(gAvail);
	//close down everything fsa related
	if(fsaFd >= 0)
	{
		if(mlcFd >= 0)
			IOSUHAX_FSA_CloseFile(fsaFd, mlcFd);
		if(sdFd >= 0)
			IOSUHAX_FSA_CloseFile(fsaFd, sdFd);
		if(sdMounted)
			IOSUHAX_FSA_Unmount(fsaFd, sdCardVolPath, 2);
		IOSUHAX_FSA_Close(fsaFd);
	}
	//close out old mcp instance
	MCPHookClose();
	sleep(5);
	//will do IOSU reboot
    OSForceFullRelaunch();
    SYSLaunchMenu();
    OSScreenEnableEx(0, 0);
    OSScreenEnableEx(1, 0);
	free(screenBuffer);
    return EXIT_RELAUNCH_ON_LOAD;
}
