//Taken from libwiius coreinit.h

#ifndef COREINIT_H
#define COREINIT_H

#define OSDynLoad_Acquire ((void (*)(char* rpl, unsigned int *handle))0x0102A3B4)
#define OSDynLoad_FindExport ((void (*)(unsigned int handle, int isdata, char *symbol, void *address))0x0102B828)
#define OSFatal ((void (*)(char* msg))0x01031618)
#define __os_snprintf ((int(*)(char* s, int n, const char * format, ... ))0x0102F160)

typedef struct OSContext
{
	/* OSContext identifier */
    uint32_t tag1;
    uint32_t tag2;

    /* GPRs */
    uint32_t gpr[32];

	/* Special registers */
    uint32_t cr;
    uint32_t lr;
    uint32_t ctr;
    uint32_t xer;

    /* Initial PC and MSR */
    uint32_t srr0;
    uint32_t srr1;
} OSContext;

#endif /* COREINIT_H */