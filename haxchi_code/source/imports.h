#ifndef IMPORTS_H
#define IMPORTS_H

#include "types.h"
#include "constants.h"

#define BSS_PHYS_OFFSET (0x50000000 - 0x10000000)
#define BSS_VA2PA(a) ((void*)(((u32)a) + BSS_PHYS_OFFSET))
#define BSS_PA2VA(a) ((void*)(((u32)a) - BSS_PHYS_OFFSET))

extern void* _bss_start;
extern void* _bss_end;

extern int (*IOS_Open)(const char* dev, int flags);
extern int (*IOS_Ioctl)(int fd, u32 cmd, void* buf_in, u32 size_in, void* buf_out, u32 size_out);

extern void (*DCFlushRange)(void *buffer, uint32_t length);

extern int (*OSScreenClearBufferEx)(int bufferNum, u32 val);
extern int (*OSScreenPutFontEx)(int bufferNum, unsigned int posX, unsigned int line, void* buffer);

extern void (*OSFatal)(char*);
extern void (*OSForceFullRelaunch)(void);
extern int (*SYSLaunchMenu)(void);
extern int (*SYSRelaunchTitle)(int, void*);
extern int (*SYSLaunchSettings)(void*);
extern int (*SYSSwitchToBrowser)(char*);
extern int (*_SYSLaunchSettingsDirect)(void*);
extern void (*Exit)(void);
extern void (*_exit)(void);

extern void* (*OSAllocFromSystem)(u32 size, int align);
extern void (*OSFreeToSystem)(void *ptr);
extern int (*IM_Open)();
extern int (*IM_Close)(int fd);
extern int (*IM_SetDeviceState)(int fd, void *mem, int state, int a, int b);

extern void (*OSScreenInit)(void);
extern int (*OSScreenGetBufferSizeEx)(int bufferNum);
extern int (*OSScreenSetBufferEx)(int bufferNum, void* addr);
extern int (*OSScreenFlipBuffersEx)(int bufferNum);

// the following (temporarily) stolen from libwiiu
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
    float x,y;
} Vec2D;

typedef struct
{
    uint16_t x, y;               /* Touch coordinates */
    uint16_t touched;            /* 1 = Touched, 0 = Not touched */
    uint16_t validity;           /* 0 = All valid, 1 = X invalid, 2 = Y invalid, 3 = Both invalid? */
} VPADTPData;
 
typedef struct
{
    uint32_t btn_hold;           /* Held buttons */
    uint32_t btn_trigger;        /* Buttons that are pressed at that instant */
    uint32_t btn_release;        /* Released buttons */
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

extern int (*VPADRead)(int controller, VPADData *buffer, unsigned int num, int *error);

void init_imports();

#endif
