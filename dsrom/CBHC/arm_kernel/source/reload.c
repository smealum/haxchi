//kernel relaunch hook, thanks to dimok
#include "types.h"
#include "utils.h"
#include "reload.h"
#include "elf_patcher.h"
#include "getbins.h"

extern char __file_start, __file_end;

void kernel_launch_ios(u32 launch_address, u32 L, u32 C, u32 H)
{
    void (*kernel_launch_bootrom)(u32 launch_address, u32 L, u32 C, u32 H) = (void*)0x0812A050;

    if(*(u32*)(launch_address - 0x300 + 0x1AC) == 0x00DFD000)
    {
        int level = disable_interrupts();
        unsigned int control_register = disable_mmu();

		u32 ios_elf_start = launch_address + 0x804 - 0x300;

		// nop out memcmp hash checks
		section_write_word(ios_elf_start, 0x040017E0, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0x040019C4, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0x04001BB0, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0x04001D40, 0xE3A00000); // mov r0, #0

		// patch OS launch sig check
		section_write_word(ios_elf_start, 0x0500A818, 0x20002000); // mov r0, #0; mov r0, #0

		// patch MCP authentication check
		section_write_word(ios_elf_start, 0x05014CAC, 0x20004770); // mov r0, #0; bx lr

		// jump over overwritten MCP debug thread start function
		section_write_word(ios_elf_start, 0x0501FEE0, 0x20002000); //mov r0, #0; mov r0, #0

		// fix 10 minute timeout that crashes MCP after 10 minutes of booting
		section_write_word(ios_elf_start, 0x05022474, 0xFFFFFFFF); // NEW_TIMEOUT

		// replace ioctl 0x62 code with jump to wupserver
		section_write_word(ios_elf_start, 0x05026BA8, 0x47780000); // bx pc
		section_write_word(ios_elf_start, 0x05026BAC, 0xE59F1000); // ldr r1, [pc]
		section_write_word(ios_elf_start, 0x05026BB0, 0xE12FFF11); // bx r1
		section_write_word(ios_elf_start, 0x05026BB4, wupserver_addr); // wupserver code

		// patch cert verification
		section_write_word(ios_elf_start, 0x05052A90, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0x05052A94, 0xE12FFF1E); // bx lr

		// patch IOSC_VerifyPubkeySign to always succeed
		section_write_word(ios_elf_start, 0x05052C44, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0x05052C48, 0xE12FFF1E); // bx lr

		// patch cached cert check
		section_write_word(ios_elf_start, 0x05054D6C, 0xE3A00000); // mov r0, 0
		section_write_word(ios_elf_start, 0x05054D70, 0xE12FFF1E); // bx lr

		// change system.xml to syshax.xml
		section_write_word(ios_elf_start, 0x050600F0, 0x79736861); // ysha
		section_write_word(ios_elf_start, 0x050600F4, 0x782E786D); // x.xm

		section_write_word(ios_elf_start, 0x05060114, 0x79736861); // ysha
		section_write_word(ios_elf_start, 0x05060118, 0x782E786D); // x.xm

		// jump to titleprot code (titleprot_addr+4)
		section_write_word(ios_elf_start, 0x05107F70, 0xF005FD0A); //bl (titleprot_addr+4)
		// overwrite mcp_d_r code with titleprot
		section_write_word(ios_elf_start, titleprot_addr, 0x20004770); // mov r0, #0; bx lr
		section_write(ios_elf_start, titleprot_addr+4, get_titleprot_bin(), get_titleprot_bin_len());

		// overwrite mcp_d_r code with wupserver
		section_write_word(ios_elf_start, 0x0510E56C, 0x47700000); // bx lr
		section_write(ios_elf_start, wupserver_addr, get_wupserver_bin(), get_wupserver_bin_len());

		// apply IOS ELF launch hook (thanks dimok!)
		section_write_word(ios_elf_start, 0x0812A120, ARM_BL(0x0812A120, kernel_launch_ios));

		// Put arm_kernel file back where it is now
		section_write(ios_elf_start, (u32)&__file_start, &__file_start, &__file_end - &__file_start);

		// allow any region title launch
		section_write_word(ios_elf_start, 0xE0030498, 0xE3A00000); // mov r0, #0

		// allow custom bootLogoTex and bootMovie.h264
		section_write_word(ios_elf_start, 0xE0030D68, 0xE3A00000); // mov r0, #0
		section_write_word(ios_elf_start, 0xE0030D34, 0xE3A00000); // mov r0, #0

        restore_mmu(control_register);
        enable_interrupts(level);
    }

    kernel_launch_bootrom(launch_address, L, C, H);
}
