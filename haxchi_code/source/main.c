#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imports.h"

u32* framebuffer_tv;
u32* framebuffer_drc;
u32 framebuffer_tv_size;
u32 framebuffer_drc_size;

void gfxInit()
{
	OSScreenInit();

	framebuffer_tv_size = OSScreenGetBufferSizeEx(0);
	framebuffer_drc_size = OSScreenGetBufferSizeEx(1);

	framebuffer_tv = (u32*)0xF4000000;
	framebuffer_drc = (u32*)((void*)framebuffer_tv + framebuffer_tv_size);

	OSScreenSetBufferEx(0, framebuffer_tv);
	OSScreenSetBufferEx(1, framebuffer_drc);
}

void gfxFlip()
{
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void print(char* str, int x, int y)
{
	OSScreenPutFontEx(0, x, y, str);
	OSScreenPutFontEx(1, x, y, str);
}

void _main()
{
	// clear bss
	memset(&_bss_start, 0x00, &_bss_end - &_bss_start);

	// get all the functions
	init_imports();

	// init framebuffers and stuff
	gfxInit();

	while(1)
	{
		VPADData vpad_data;
		int vpad_error;
		VPADRead(0, &vpad_data, 1, &vpad_error);

		OSScreenClearBufferEx(0, 0x40404040);
		OSScreenClearBufferEx(1, 0x40404040);

		print("wii u got SPYND yo", 5, 5);

		int line = 7;
		if (vpad_data.btn_hold & BUTTON_A) print("A is held", 7, line++);
		if (vpad_data.btn_hold & BUTTON_B) print("B is held", 7, line++);
		if (vpad_data.btn_hold & BUTTON_X) print("X is held", 7, line++);
		if (vpad_data.btn_hold & BUTTON_Y) print("Y is held", 7, line++);
		if (vpad_data.btn_hold & BUTTON_HOME) print("HOME is held", 7, line++);

		gfxFlip();
	}

	while(1);
}
