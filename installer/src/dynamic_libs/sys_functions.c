/****************************************************************************
 * Copyright (C) 2015
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include "os_functions.h"

 unsigned int sysapp_handle __attribute__((section(".data"))) = 0;

EXPORT_DECL(int, _SYSLaunchTitleByPathFromLauncher, const char* path, int len, int zero);
EXPORT_DECL(int, SYSRelaunchTitle, int argc, char** argv);
EXPORT_DECL(int, SYSLaunchMenu, void);
EXPORT_DECL(int, SYSCheckTitleExists, u64 titleId);
EXPORT_DECL(int, SYSLaunchTitle, u64 titleId);
EXPORT_DECL(int, SYSLaunchSettings, int unk);

void InitAcquireSys(void)
{
    OSDynLoad_Acquire("sysapp.rpl", &sysapp_handle);
}

void InitSysFunctionPointers(void)
{
    unsigned int *funcPointer = 0;
    InitAcquireSys();

    OS_FIND_EXPORT(sysapp_handle, _SYSLaunchTitleByPathFromLauncher);
    OS_FIND_EXPORT(sysapp_handle, SYSRelaunchTitle);
    OS_FIND_EXPORT(sysapp_handle, SYSLaunchMenu);
    OS_FIND_EXPORT(sysapp_handle, SYSCheckTitleExists);
    OS_FIND_EXPORT(sysapp_handle, SYSLaunchTitle);
    OS_FIND_EXPORT(sysapp_handle, SYSLaunchSettings);
}
