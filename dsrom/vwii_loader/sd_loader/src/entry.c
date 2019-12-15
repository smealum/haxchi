#include <gctypes.h>
#include "elf_abi.h"
#include "../../common.h"
#include "../../fs_defs.h"
#include "../../os_defs.h"
#include "kernel_defs.h"
#include "loader_defs.h"

#define EXPORT_DECL(res, func, ...)                     res (* func)(__VA_ARGS__);
#define OS_FIND_EXPORT(handle, funcName, func)          OSDynLoad_FindExport(handle, 0, funcName, &func)

static void (*DCFlushRange)(void *addr, unsigned int size);
static void (*DCInvalidateRange)(void *addr, unsigned int size);
static void (*ICInvalidateRange)(void *addr, unsigned int size);
static unsigned int hook_LiWaitOneChunk;
static unsigned int addrphys_LiWaitOneChunk;

extern void SC0x25_KernelCopyData(unsigned int addr, unsigned int src, unsigned int len);
extern void my_PrepareTitle_hook(void);

static void KernelCopyData(unsigned int addr, unsigned int src, unsigned int len)
{
    /*
     * Setup a DBAT access with cache inhibited to write through and read directly from memory
     */
    unsigned int dbatu0, dbatl0, dbatu1, dbatl1;
    // save the original DBAT value
    asm volatile("mfdbatu %0, 0" : "=r" (dbatu0));
    asm volatile("mfdbatl %0, 0" : "=r" (dbatl0));
    asm volatile("mfdbatu %0, 1" : "=r" (dbatu1));
    asm volatile("mfdbatl %0, 1" : "=r" (dbatl1));

    unsigned int target_dbatu0 = 0;
    unsigned int target_dbatl0 = 0;
    unsigned int target_dbatu1 = 0;
    unsigned int target_dbatl1 = 0;

    unsigned int *dst_p = (unsigned int*)addr;
    unsigned int *src_p = (unsigned int*)src;

    // we only need DBAT modification for addresses out of our own DBAT range
    // as our own DBAT is available everywhere for user and supervisor
    // since our own DBAT is on DBAT5 position we don't collide here
    if(addr < 0x00800000 || addr >= 0x01000000)
    {
        target_dbatu0 = (addr & 0x00F00000) | 0xC0000000 | 0x1F;
        target_dbatl0 = (addr & 0xFFF00000) | 0x32;
        asm volatile("mtdbatu 0, %0" : : "r" (target_dbatu0));
        asm volatile("mtdbatl 0, %0" : : "r" (target_dbatl0));
        dst_p = (unsigned int*)((addr & 0xFFFFFF) | 0xC0000000);
    }
    if(src < 0x00800000 || src >= 0x01000000)
    {
        target_dbatu1 = (src & 0x00F00000) | 0xB0000000 | 0x1F;
        target_dbatl1 = (src & 0xFFF00000) | 0x32;

        asm volatile("mtdbatu 1, %0" : : "r" (target_dbatu1));
        asm volatile("mtdbatl 1, %0" : : "r" (target_dbatl1));
        src_p = (unsigned int*)((src & 0xFFFFFF) | 0xB0000000);
    }

    asm volatile("eieio; isync");

    unsigned int i;
    for(i = 0; i < len; i += 4)
    {
        // if we are on the edge to next chunk
        if((target_dbatu0 != 0) && (((unsigned int)dst_p & 0x00F00000) != (target_dbatu0 & 0x00F00000)))
        {
            target_dbatu0 = ((addr + i) & 0x00F00000) | 0xC0000000 | 0x1F;
            target_dbatl0 = ((addr + i) & 0xFFF00000) | 0x32;
            dst_p = (unsigned int*)(((addr + i) & 0xFFFFFF) | 0xC0000000);

            asm volatile("eieio; isync");
            asm volatile("mtdbatu 0, %0" : : "r" (target_dbatu0));
            asm volatile("mtdbatl 0, %0" : : "r" (target_dbatl0));
            asm volatile("eieio; isync");
        }
        if((target_dbatu1 != 0) && (((unsigned int)src_p & 0x00F00000) != (target_dbatu1 & 0x00F00000)))
        {
            target_dbatu1 = ((src + i) & 0x00F00000) | 0xB0000000 | 0x1F;
            target_dbatl1 = ((src + i) & 0xFFF00000) | 0x32;
            src_p = (unsigned int*)(((src + i) & 0xFFFFFF) | 0xB0000000);

            asm volatile("eieio; isync");
            asm volatile("mtdbatu 1, %0" : : "r" (target_dbatu1));
            asm volatile("mtdbatl 1, %0" : : "r" (target_dbatl1));
            asm volatile("eieio; isync");
        }

        *dst_p = *src_p;

        ++dst_p;
        ++src_p;
    }

    /*
     * Restore original DBAT value
     */
    asm volatile("eieio; isync");
    asm volatile("mtdbatu 0, %0" : : "r" (dbatu0));
    asm volatile("mtdbatl 0, %0" : : "r" (dbatl0));
    asm volatile("mtdbatu 1, %0" : : "r" (dbatu1));
    asm volatile("mtdbatl 1, %0" : : "r" (dbatl1));
    asm volatile("eieio; isync");
}

// This function is called every time after LiBounceOneChunk.
// It waits for the asynchronous call of LiLoadAsync for the IOSU to fill data to the RPX/RPL address
// and return the still remaining bytes to load.
// We override it and replace the loaded date from LiLoadAsync with our data and our remaining bytes to load.
static int LiWaitOneChunk(unsigned int * iRemainingBytes, const char *filename, int fileType)
{
    unsigned int result;
    register int core_id;
    int remaining_bytes = 0;

    int sgFileOffset;
    int sgBufferNumber;
    int *sgBounceError;
    int *sgGotBytes;
    int *sgTotalBytes;
    int *sgIsLoadingBuffer;
    int *sgFinishedLoadingBuffer;

    // get the current core
    asm volatile("mfspr %0, 0x3EF" : "=r" (core_id));

    // get the offset of per core global variable for dynload initialized (just a simple address + (core_id * 4))
    unsigned int gDynloadInitialized = *(volatile unsigned int*)(OS_SPECIFICS->addr_gDynloadInitialized + (core_id << 2));

    // Comment (Dimok):
    // time measurement at this position for logger  -> we don't need it right now except maybe for debugging
    //unsigned long long systemTime1 = Loader_GetSystemTime();

	if(OS_FIRMWARE == 550)
    {
        // pointer to global variables of the loader
        loader_globals_550_t *loader_globals = (loader_globals_550_t*)(0xEFE19E80);

        sgBufferNumber = loader_globals->sgBufferNumber;
        sgFileOffset = loader_globals->sgFileOffset;
        sgBounceError = &loader_globals->sgBounceError;
        sgGotBytes = &loader_globals->sgGotBytes;
        sgTotalBytes = &loader_globals->sgTotalBytes;
        sgFinishedLoadingBuffer = &loader_globals->sgFinishedLoadingBuffer;
        // not available on 5.5.x
        sgIsLoadingBuffer = NULL;
    }
    else
    {
        // pointer to global variables of the loader
        loader_globals_t *loader_globals = (loader_globals_t*)(OS_SPECIFICS->addr_sgIsLoadingBuffer);

        sgBufferNumber = loader_globals->sgBufferNumber;
        sgFileOffset = loader_globals->sgFileOffset;
        sgBounceError = &loader_globals->sgBounceError;
        sgGotBytes = &loader_globals->sgGotBytes;
        sgIsLoadingBuffer = &loader_globals->sgIsLoadingBuffer;
        // not available on < 5.5.x
        sgTotalBytes = NULL;
        sgFinishedLoadingBuffer = NULL;
    }

    // the data loading was started in LiBounceOneChunk() and here it waits for IOSU to finish copy the data
    if(gDynloadInitialized != 0) {
        result = OS_SPECIFICS->LiWaitIopCompleteWithInterrupts(0x2160EC0, &remaining_bytes);

    }
    else {
        result = OS_SPECIFICS->LiWaitIopComplete(0x2160EC0, &remaining_bytes);
    }

    // Comment (Dimok):
    // time measurement at this position for logger -> we don't need it right now except maybe for debugging
    //unsigned long long systemTime2 = Loader_GetSystemTime();

    //------------------------------------------------------------------------------------------------------------------
    // Start of our function intrusion:
    // After IOSU is done writing the data into the 0xF6000000/0xF6400000 address,
    // we overwrite it with our data before setting the global flag for IsLoadingBuffer to 0
    // Do this only if we are in the game that was launched by our method
    s_mem_area *mem_area = MEM_AREA_TABLE;
    if((ELF_DATA_ADDR == mem_area->address) && (fileType == 0))
    {
        unsigned int load_address = (sgBufferNumber == 1) ? 0xF6000000 : (0xF6000000 + 0x00400000);
        unsigned int load_addressPhys = (sgBufferNumber == 1) ? 0x1B000000 : (0x1B000000 + 0x00400000); // virtual 0xF6000000 and 0xF6400000

        remaining_bytes = ELF_DATA_SIZE - sgFileOffset;
        if (remaining_bytes > 0x400000)
            // truncate size
            remaining_bytes = 0x400000;

        DCFlushRange((void*)load_address, remaining_bytes);

        u32 rpxBlockPos = 0;
        u32 done = 0;
        u32 mapOffset = 0;

        while((done < (u32)sgFileOffset) && mem_area)
        {
            if((done + mem_area->size) > (u32)sgFileOffset)
            {
                mapOffset = sgFileOffset - done;
                done = sgFileOffset;
            }
            else
            {
                done += mem_area->size;
                mem_area = mem_area->next;
            }
        }

        while((done < ELF_DATA_SIZE) && (rpxBlockPos < 0x400000) && mem_area)
        {
            u32 address = mem_area->address + mapOffset;
            u32 blockSize = ELF_DATA_SIZE - done;

            if(blockSize > (0x400000 - rpxBlockPos))
            {
                blockSize = 0x400000 - rpxBlockPos;
            }
            if((mapOffset + blockSize) >= mem_area->size)
            {
                blockSize = mem_area->size - mapOffset;
                //! this value is incremented later by blockSize, so set it to -blockSize for it to be 0 after copy
                //! it makes smaller code then if(mapOffset == mem_area->size) after copy
                mapOffset = -blockSize;
                mem_area = mem_area->next;
            }

            SC0x25_KernelCopyData(load_addressPhys + rpxBlockPos, address, blockSize);
            done += blockSize;
            rpxBlockPos += blockSize;
            mapOffset += blockSize;
        }

        DCInvalidateRange((void*)load_address, remaining_bytes);

        if((u32)(sgFileOffset + remaining_bytes) == ELF_DATA_SIZE)
        {
            ELF_DATA_ADDR = 0xDEADC0DE;
            ELF_DATA_SIZE = 0;
            MAIN_ENTRY_ADDR = 0xC001C0DE;
        }
        // set result to 0 -> "everything OK"
        result = 0;
    }

    // end of our little intrusion into this function
    //------------------------------------------------------------------------------------------------------------------

    // set the result to the global bounce error variable
    if(sgBounceError) {
        *sgBounceError = result;
    }

    // disable global flag that buffer is still loaded by IOSU
	if(sgFinishedLoadingBuffer)
    {
        unsigned int zeroBitCount = 0;
        asm volatile("cntlzw %0, %0" : "=r" (zeroBitCount) : "r"(*sgFinishedLoadingBuffer));
        *sgFinishedLoadingBuffer = zeroBitCount >> 5;
    }
    else if(sgIsLoadingBuffer)
    {
        *sgIsLoadingBuffer = 0;
    }

    // check result for errors
    if(result == 0)
    {
        // the remaining size is set globally and in stack variable only
        // if a pointer was passed to this function
        if(iRemainingBytes) {
            if(sgGotBytes) {
                *sgGotBytes = remaining_bytes;
            }

            *iRemainingBytes = remaining_bytes;

            // on 5.5.x a new variable for total loaded bytes was added
            if(sgTotalBytes) {
                *sgTotalBytes += remaining_bytes;
            }
        }
        // Comment (Dimok):
        // calculate time difference and print it on logging how long the wait for asynchronous data load took
        // something like (systemTime2 - systemTime1) * constant / bus speed, did not look deeper into it as we don't need that crap
    }
    else {
        // Comment (Dimok):
        // a lot of error handling here. depending on error code sometimes calls Loader_Panic() -> we don't make errors so we can skip that part ;-P
    }
    return result;
}

void my_PrepareTitle(CosAppXmlInfo *xmlKernelInfo)
{
    if(ELF_DATA_ADDR == MEM_AREA_TABLE->address)
    {
        xmlKernelInfo->max_size = RPX_MAX_SIZE;
        xmlKernelInfo->max_codesize = RPX_MAX_CODE_SIZE;
        //! setup our hook to LiWaitOneChunk for RPX loading
        hook_LiWaitOneChunk = ((u32)LiWaitOneChunk) | 0x48000002;
        KernelCopyData(addrphys_LiWaitOneChunk, (u32) &hook_LiWaitOneChunk, 4);
        asm volatile("icbi 0, %0" : : "r" (OS_SPECIFICS->addr_LiWaitOneChunk & ~31));
    }
    else if((MAIN_ENTRY_ADDR == 0xC001C0DE) && (*(u32*)xmlKernelInfo->rpx_name == 0x66666c5f)) // ffl_
    {
        //! restore original LiWaitOneChunk instruction as our RPX is done
        MAIN_ENTRY_ADDR = 0xDEADC0DE;
        KernelCopyData(addrphys_LiWaitOneChunk, (u32)&OS_SPECIFICS->orig_LiWaitOneChunkInstr, 4);
        asm volatile("icbi 0, %0" : : "r" (OS_SPECIFICS->addr_LiWaitOneChunk & ~31));
    }
}

unsigned int _main(int argc, char **argv)
{
    if(MAIN_ENTRY_ADDR != 0xC001C0DE)
    {
		EXPORT_DECL(int, OSDynLoad_Acquire, const char* rpl, u32 *handle);
		EXPORT_DECL(int, OSDynLoad_FindExport, u32 handle, int isdata, const char *symbol, void *address);

		OSDynLoad_Acquire = (int (*)(const char*, u32 *))OS_SPECIFICS->addr_OSDynLoad_Acquire;
		OSDynLoad_FindExport = (int (*)(u32, int, const char *, void *))OS_SPECIFICS->addr_OSDynLoad_FindExport;

		unsigned int coreinit_handle;
		OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

		OS_FIND_EXPORT(coreinit_handle, "DCFlushRange", DCFlushRange);
		OS_FIND_EXPORT(coreinit_handle, "DCInvalidateRange", DCInvalidateRange);
		OS_FIND_EXPORT(coreinit_handle, "ICInvalidateRange", ICInvalidateRange);

		EXPORT_DECL(void *, MEMAllocFromDefaultHeapEx,int size, int align);
		EXPORT_DECL(void, MEMFreeToDefaultHeap,void *ptr);

		unsigned int *functionPtr = 0;

		OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeapEx", &functionPtr);
		MEMAllocFromDefaultHeapEx = (void * (*)(int, int))*functionPtr;
		OSDynLoad_FindExport(coreinit_handle, 1, "MEMFreeToDefaultHeap", &functionPtr);
		MEMFreeToDefaultHeap = (void (*)(void *))*functionPtr;

		//get all the CMPT functions
		unsigned int cmpt_handle;
		OSDynLoad_Acquire("nn_cmpt.rpl", &cmpt_handle);

		int (*CMPTLaunchTitle)(void* CMPTConfigure, int ConfigSize, int titlehigh, int titlelow);
		int (*CMPTAcctSetScreenType)(int screenType);
		int (*CMPTGetDataSize)(int* dataSize);
		int (*CMPTCheckScreenState)();

		OSDynLoad_FindExport(cmpt_handle, 0, "CMPTLaunchTitle", &CMPTLaunchTitle);
		OSDynLoad_FindExport(cmpt_handle, 0, "CMPTAcctSetScreenType", &CMPTAcctSetScreenType);
		OSDynLoad_FindExport(cmpt_handle, 0, "CMPTGetDataSize", &CMPTGetDataSize);
		OSDynLoad_FindExport(cmpt_handle, 0, "CMPTCheckScreenState", &CMPTCheckScreenState);

		//1 = TV Only, 2 = GamePad Only, 3 = Both
		CMPTAcctSetScreenType(3); 
		if(CMPTCheckScreenState() < 0)
		{
			CMPTAcctSetScreenType(2);
			if(CMPTCheckScreenState() < 0)
				CMPTAcctSetScreenType(1);
		}

		int datasize;
		CMPTGetDataSize(&datasize);

		//needed for CMPT to work
		unsigned int padscore_handle;
		OSDynLoad_Acquire("padscore.rpl", &padscore_handle);
		void(*KPADInit)();
		OSDynLoad_FindExport(padscore_handle, 0, "KPADInit", &KPADInit);
		KPADInit();

		void *databuf = MEMAllocFromDefaultHeapEx(datasize, 0x40);
		CMPTLaunchTitle(databuf, datasize, VWII_TITLE_ID_LOW, VWII_TITLE_ID_HIGH);
		MEMFreeToDefaultHeap(databuf);
		MAIN_ENTRY_ADDR = 0xC001C0DE;
	}

    unsigned int entry = *(unsigned int*)OS_SPECIFICS->addr_OSTitle_main_entry;

    //! launch the original title with clean stack
    return entry;
}
