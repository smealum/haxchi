#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os_defs.h"

#define HBL_VERSION                 "v1.4"

#define CAFE_OS_SD_PATH             "/vol/external01"
#define SD_PATH                     "sd:"
#define WIIU_PATH                   "/wiiu"

#ifndef MEM_BASE
#define MEM_BASE                    (0x00800000)
#endif

#define ELF_DATA_ADDR               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define HBL_CHANNEL                 (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x08))
#define RPX_MAX_SIZE                (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x0C))
#define RPX_MAX_CODE_SIZE           (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x10))
#define MAIN_ENTRY_ADDR             (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))
#define OS_FIRMWARE                 (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x04))

#define OS_SPECIFICS                ((OsSpecifics*)(MEM_BASE + 0x1500))

#define MEM_AREA_TABLE              ((s_mem_area*)(MEM_BASE + 0x1600))

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */

