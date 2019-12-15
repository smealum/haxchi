.include "defines.s"
.create "rom.nds", 0

.nds

.org 0x000
	.ascii "HAXCHI" ; Game Title

.org 0x00C
	.ascii "HAXX" ; Gamecode
	.ascii "01" ; Makercode
	.byte 0x00 ; Unitcode

.org 0x020
	.word arm9_data ; ARM9 rom_offset
	.word 0x2000800 ; ARM9 entry_address
	.word 0x2000000 ; ARM9 ram_address
	.word arm9_data_end - arm9_data ; ARM9 size
	.word arm7_data ; ARM7 rom_offset
	.word 0x2000000 ; ARM7 entry_address
	.word ARM7_ROM_MEM2_START + HAX_TARGET_ADDRESS ; ARM7 ram_address
	.word arm7_data_end - arm7_data ; ARM7 size

.org 0x080
	.word total_size ; Total Used ROM size
	.word 0x4000 ; ROM Header Size

.org 0x8000
arm9_data:
	.incbin "haxchi_rop.bin"
arm9_data_end:

.align 0x1000
arm7_data:
	.incbin "haxchi_rop_hook.bin"
arm7_data_end:

total_size:

.Close
