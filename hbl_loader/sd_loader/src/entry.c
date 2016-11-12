#include <gctypes.h>
#include "elf_abi.h"
#include "../../common.h"
#include "../../fs_defs.h"
#include "../../os_defs.h"
#include "kernel_defs.h"
#include "loader_defs.h"

#define EXPORT_DECL(res, func, ...)                     res (* func)(__VA_ARGS__);
#define OS_FIND_EXPORT(handle, funcName, func)          OSDynLoad_FindExport(handle, 0, funcName, &func)

typedef struct _private_data_t
{
    EXPORT_DECL(void *, MEMAllocFromDefaultHeapEx,int size, int align);
    EXPORT_DECL(void, MEMFreeToDefaultHeap,void *ptr);

    EXPORT_DECL(void*, memcpy, void *p1, const void *p2, unsigned int s);
    EXPORT_DECL(void*, memset, void *p1, int val, unsigned int s);
    EXPORT_DECL(void, OSFatal, const char* msg);
    EXPORT_DECL(unsigned int, OSEffectiveToPhysical, const void*);
    EXPORT_DECL(void, exit, int);

    EXPORT_DECL(int, FSInit, void);
    EXPORT_DECL(int, FSAddClientEx, void *pClient, int unk_zero_param, int errHandling);
    EXPORT_DECL(int, FSDelClient, void *pClient);
    EXPORT_DECL(void, FSInitCmdBlock, void *pCmd);
    EXPORT_DECL(int, FSGetMountSource, void *pClient, void *pCmd, int type, void *source, int errHandling);
    EXPORT_DECL(int, FSMount, void *pClient, void *pCmd, void *source, const char *target, uint32_t bytes, int errHandling);
    EXPORT_DECL(int, FSUnmount, void *pClient, void *pCmd, const char *target, int errHandling);
    EXPORT_DECL(int, FSOpenFile, void *pClient, void *pCmd, const char *path, const char *mode, int *fd, int errHandling);
    EXPORT_DECL(int, FSGetStatFile, void *pClient, void *pCmd, int fd, void *buffer, int error);
    EXPORT_DECL(int, FSReadFile, void *pClient, void *pCmd, void *buffer, int size, int count, int fd, int flag, int errHandling);
    EXPORT_DECL(int, FSCloseFile, void *pClient, void *pCmd, int fd, int errHandling);

    EXPORT_DECL(int, SYSRelaunchTitle, int argc, char** argv);
} private_data_t;

static void (*DCFlushRange)(void *addr, unsigned int size);
static void (*DCInvalidateRange)(void *addr, unsigned int size);
static void (*ICInvalidateRange)(void *addr, unsigned int size);
static unsigned int hook_LiWaitOneChunk;
static unsigned int addrphys_LiWaitOneChunk;

extern void SC0x25_KernelCopyData(unsigned int addr, unsigned int src, unsigned int len);
extern void my_PrepareTitle_hook(void);

/* Write a 32-bit word with kernel permissions */
static void __attribute__ ((noinline)) kern_write(void *addr, uint32_t value)
{
    asm volatile (
        "li 3,1\n"
        "li 4,0\n"
        "mr 5,%1\n"
        "li 6,0\n"
        "li 7,0\n"
        "lis 8,1\n"
        "mr 9,%0\n"
        "mr %1,1\n"
        "li 0,0x3500\n"
        "sc\n"
        "nop\n"
        "mr 1,%1\n"
        :
        :	"r"(addr), "r"(value)
        :	"memory", "ctr", "lr", "0", "3", "4", "5", "6", "7", "8", "9", "10",
            "11", "12"
        );
}

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
                mem_area = mem_area->next;
                mapOffset = 0;
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

static int LoadFileToMem(private_data_t *private_data, const char *filepath, unsigned char **fileOut, unsigned int * sizeOut)
{
    int iFd = -1;
    void *pClient = private_data->MEMAllocFromDefaultHeapEx(FS_CLIENT_SIZE, 4);
    if(!pClient)
        return 0;

    void *pCmd = private_data->MEMAllocFromDefaultHeapEx(FS_CMD_BLOCK_SIZE, 4);
    if(!pCmd)
    {
        private_data->MEMFreeToDefaultHeap(pClient);
        return 0;
    }

    int success = 0;
    private_data->FSInit();
    private_data->FSInitCmdBlock(pCmd);
    private_data->FSAddClientEx(pClient, 0, -1);

    do
    {
        char tempPath[FS_MOUNT_SOURCE_SIZE];
        char mountPath[FS_MAX_MOUNTPATH_SIZE];

        int status = private_data->FSGetMountSource(pClient, pCmd, 0, tempPath, -1);
        if (status != 0) {
            private_data->OSFatal("FSGetMountSource failed.");
        }
        status = private_data->FSMount(pClient, pCmd, tempPath, mountPath, FS_MAX_MOUNTPATH_SIZE, -1);
        if(status != 0) {
            private_data->OSFatal("SD mount failed.");
        }

        status = private_data->FSOpenFile(pClient, pCmd, filepath, "r", &iFd, -1);
        if(status != 0) {
            private_data->OSFatal("FSOpenFile failed.");
        }

        FSStat stat;
        stat.size = 0;

        void *pBuffer = NULL;

        private_data->FSGetStatFile(pClient, pCmd, iFd, &stat, -1);

        if(stat.size > 0)
            pBuffer = private_data->MEMAllocFromDefaultHeapEx((stat.size + 0x3F) & ~0x3F, 0x40);
        else
            private_data->OSFatal("ELF file empty.");

        unsigned int done = 0;

        while(done < stat.size)
        {
            int readBytes = private_data->FSReadFile(pClient, pCmd, pBuffer + done, 1, stat.size - done, iFd, 0, -1);
            if(readBytes <= 0) {
                break;
            }
            done += readBytes;
        }

        if(done != stat.size)
        {
            private_data->MEMFreeToDefaultHeap(pBuffer);
        }
        else
        {
            *fileOut = (unsigned char*)pBuffer;
            *sizeOut = stat.size;
            success = 1;
        }

        private_data->FSCloseFile(pClient, pCmd, iFd, -1);
        private_data->FSUnmount(pClient, pCmd, mountPath, -1);
    }
    while(0);

    private_data->FSDelClient(pClient);
    private_data->MEMFreeToDefaultHeap(pClient);
    private_data->MEMFreeToDefaultHeap(pCmd);
    return success;
}

static unsigned int load_elf_image (private_data_t *private_data, unsigned char *elfstart)
{
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdrs;
	unsigned char *image;
	int i;

	ehdr = (Elf32_Ehdr *) elfstart;

	if(ehdr->e_phoff == 0 || ehdr->e_phnum == 0)
		return 0;

	if(ehdr->e_phentsize != sizeof(Elf32_Phdr))
		return 0;

	phdrs = (Elf32_Phdr*)(elfstart + ehdr->e_phoff);

	for(i = 0; i < ehdr->e_phnum; i++)
    {
		if(phdrs[i].p_type != PT_LOAD)
			continue;

		if(phdrs[i].p_filesz > phdrs[i].p_memsz)
			continue;

		if(!phdrs[i].p_filesz)
			continue;

        unsigned int p_paddr = phdrs[i].p_paddr;
		image = (unsigned char *) (elfstart + phdrs[i].p_offset);

        private_data->memcpy ((void *) p_paddr, image, phdrs[i].p_filesz);
        DCFlushRange((void*)p_paddr, phdrs[i].p_filesz);

		if(phdrs[i].p_flags & PF_X)
			ICInvalidateRange ((void *) p_paddr, phdrs[i].p_memsz);
	}

    //! clear BSS
    Elf32_Shdr *shdr = (Elf32_Shdr *) (elfstart + ehdr->e_shoff);
    for(i = 0; i < ehdr->e_shnum; i++)
    {
        const char *section_name = ((const char*)elfstart) + shdr[ehdr->e_shstrndx].sh_offset + shdr[i].sh_name;
        if(section_name[0] == '.' && section_name[1] == 'b' && section_name[2] == 's' && section_name[3] == 's')
        {
            private_data->memset((void*)shdr[i].sh_addr, 0, shdr[i].sh_size);
            DCFlushRange((void*)shdr[i].sh_addr, shdr[i].sh_size);
        }
        else if(section_name[0] == '.' && section_name[1] == 's' && section_name[2] == 'b' && section_name[3] == 's' && section_name[4] == 's')
        {
            private_data->memset((void*)shdr[i].sh_addr, 0, shdr[i].sh_size);
            DCFlushRange((void*)shdr[i].sh_addr, shdr[i].sh_size);
        }
    }

    //! setup hooks
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x25 * 4)), (unsigned int)KernelCopyData);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x25 * 4)), (unsigned int)KernelCopyData);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x25 * 4)), (unsigned int)KernelCopyData);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x25 * 4)), (unsigned int)KernelCopyData);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x25 * 4)), (unsigned int)KernelCopyData);

    //! store physical address for later use
    addrphys_LiWaitOneChunk = private_data->OSEffectiveToPhysical((void*)OS_SPECIFICS->addr_LiWaitOneChunk);

    u32 addr_my_PrepareTitle_hook = ((u32)my_PrepareTitle_hook) | 0x48000003;
    DCFlushRange(&addr_my_PrepareTitle_hook, 4);

    //! create our copy syscall
    SC0x25_KernelCopyData(OS_SPECIFICS->addr_PrepareTitle_hook, private_data->OSEffectiveToPhysical(&addr_my_PrepareTitle_hook), 4);

	return ehdr->e_entry;
}

static void loadFunctionPointers(private_data_t * private_data)
{
    unsigned int coreinit_handle;

    EXPORT_DECL(int, OSDynLoad_Acquire, const char* rpl, u32 *handle);
    EXPORT_DECL(int, OSDynLoad_FindExport, u32 handle, int isdata, const char *symbol, void *address);

    OSDynLoad_Acquire = (int (*)(const char*, u32 *))OS_SPECIFICS->addr_OSDynLoad_Acquire;
    OSDynLoad_FindExport = (int (*)(u32, int, const char *, void *))OS_SPECIFICS->addr_OSDynLoad_FindExport;

    OSDynLoad_Acquire("coreinit", &coreinit_handle);

    unsigned int *functionPtr = 0;

    OSDynLoad_FindExport(coreinit_handle, 1, "MEMAllocFromDefaultHeapEx", &functionPtr);
    private_data->MEMAllocFromDefaultHeapEx = (void * (*)(int, int))*functionPtr;
    OSDynLoad_FindExport(coreinit_handle, 1, "MEMFreeToDefaultHeap", &functionPtr);
    private_data->MEMFreeToDefaultHeap = (void (*)(void *))*functionPtr;

    OS_FIND_EXPORT(coreinit_handle, "memcpy", private_data->memcpy);
    OS_FIND_EXPORT(coreinit_handle, "memset", private_data->memset);
    OS_FIND_EXPORT(coreinit_handle, "OSFatal", private_data->OSFatal);
    OS_FIND_EXPORT(coreinit_handle, "DCFlushRange", DCFlushRange);
    OS_FIND_EXPORT(coreinit_handle, "DCInvalidateRange", DCInvalidateRange);
    OS_FIND_EXPORT(coreinit_handle, "ICInvalidateRange", ICInvalidateRange);
    OS_FIND_EXPORT(coreinit_handle, "OSEffectiveToPhysical", private_data->OSEffectiveToPhysical);
    OS_FIND_EXPORT(coreinit_handle, "exit", private_data->exit);

    OS_FIND_EXPORT(coreinit_handle, "FSInit", private_data->FSInit);
    OS_FIND_EXPORT(coreinit_handle, "FSAddClientEx", private_data->FSAddClientEx);
    OS_FIND_EXPORT(coreinit_handle, "FSDelClient", private_data->FSDelClient);
    OS_FIND_EXPORT(coreinit_handle, "FSInitCmdBlock", private_data->FSInitCmdBlock);
    OS_FIND_EXPORT(coreinit_handle, "FSGetMountSource", private_data->FSGetMountSource);
    OS_FIND_EXPORT(coreinit_handle, "FSMount", private_data->FSMount);
    OS_FIND_EXPORT(coreinit_handle, "FSUnmount", private_data->FSUnmount);
    OS_FIND_EXPORT(coreinit_handle, "FSOpenFile", private_data->FSOpenFile);
    OS_FIND_EXPORT(coreinit_handle, "FSGetStatFile", private_data->FSGetStatFile);
    OS_FIND_EXPORT(coreinit_handle, "FSReadFile", private_data->FSReadFile);
    OS_FIND_EXPORT(coreinit_handle, "FSCloseFile", private_data->FSCloseFile);

    unsigned int sysapp_handle;
    OSDynLoad_Acquire("sysapp.rpl", &sysapp_handle);
    OS_FIND_EXPORT(sysapp_handle, "SYSRelaunchTitle", private_data->SYSRelaunchTitle);
}

int _start(int argc, char **argv)
{
    private_data_t private_data;

    if(MAIN_ENTRY_ADDR != 0xC001C0DE)
    {
        loadFunctionPointers(&private_data);

        while(1)
        {
            if(ELF_DATA_ADDR != 0xDEADC0DE && ELF_DATA_SIZE > 0)
            {
                //! copy data to safe area before processing it
                unsigned char * pElfBuffer = (unsigned char *)private_data.MEMAllocFromDefaultHeapEx(ELF_DATA_SIZE, 4);
                if(pElfBuffer)
                {
                    private_data.memcpy(pElfBuffer, (unsigned char*)ELF_DATA_ADDR, ELF_DATA_SIZE);
                    MAIN_ENTRY_ADDR = load_elf_image(&private_data, pElfBuffer);
                    private_data.MEMFreeToDefaultHeap(pElfBuffer);

                }
                ELF_DATA_ADDR = 0xDEADC0DE;
                ELF_DATA_SIZE = 0;
            }

            if(MAIN_ENTRY_ADDR == 0xDEADC0DE || MAIN_ENTRY_ADDR == 0)
            {
                if(HBL_CHANNEL)
                {
                    break;
                }
                else
                {
                    unsigned char *pElfBuffer = NULL;
                    unsigned int uiElfSize = 0;

                    LoadFileToMem(&private_data, CAFE_OS_SD_PATH WIIU_PATH "/apps/homebrew_launcher/homebrew_launcher.elf", &pElfBuffer, &uiElfSize);

                    if(!pElfBuffer)
                    {
                        private_data.OSFatal("Failed to load homebrew_launcher.elf");
                    }
                    else
                    {
                        MAIN_ENTRY_ADDR = load_elf_image(&private_data, pElfBuffer);
                        if(MAIN_ENTRY_ADDR == 0)
                        {
                            private_data.OSFatal("Failed to load homebrew_launcher.elf");
                        }
                        else
                        {
                            private_data.MEMFreeToDefaultHeap(pElfBuffer);
                        }
                    }
                }
            }
            else
            {
                int returnVal = ((int (*)(int, char **))MAIN_ENTRY_ADDR)(argc, argv);
                //! exit to miimaker and restart application on re-enter of another application
                if(returnVal == (int)EXIT_RELAUNCH_ON_LOAD)
                {
                    break;
                }
                //! exit to homebrew launcher in all other cases
                else
                {
                    MAIN_ENTRY_ADDR = 0xDEADC0DE;
                    private_data.SYSRelaunchTitle(0, 0);
                    private_data.exit(0);
                    break;
                }
            }
        }
    }

    int ret = ( (int (*)(int, char **))(*(unsigned int*)OS_SPECIFICS->addr_OSTitle_main_entry) )(argc, argv);

    //! if an application returns and was an RPX launch then launch HBL again
    if(MAIN_ENTRY_ADDR == 0xC001C0DE)
    {
        private_data.SYSRelaunchTitle(0, 0);
        private_data.exit(0);
    }
    return ret;
}
