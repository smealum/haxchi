.PHONY := all code550.bin

ifeq ($(Windows_NT), 1)
	ZIP = zip/zip.exe
else
	ZIP = zip
endif

all: setup brainage kirby yoshids brainage.zip kirby.zip yoshids.zip

brainage: setup_brainage brainage.nds

kirby: setup_kirby kirby.nds

yoshids: setup_yoshids yoshids.nds

setup:
	@cd option_select && make && cd ..
	@cd hbl_loader && make && cd ..
	@cd cfw_booter && make && cd ..

setup_brainage:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f brainage_defs.s defines.s

setup_kirby:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f kirby_defs.s defines.s

setup_yoshids:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f yoshids_defs.s defines.s

brainage.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds brainage.nds

kirby.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds kirby.nds

yoshids.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds yoshids.nds
	@cp yoshids.nds wwtouched.nds

brainage_cfw.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds brainage_cfw.nds

kirby_cfw.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds kirby_cfw.nds

yoshids_cfw.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds yoshids_cfw.nds
	@cp yoshids_cfw.nds wwtouched_cfw.nds

brainage.zip:
	$(ZIP) -JXjq9 brainage.zip brainage.nds

kirby.zip:
	$(ZIP) -JXjq9 kirby.zip kirby.nds

yoshids.zip:
	$(ZIP) -JXjq9 yoshids.zip yoshids.nds
	$(ZIP) -JXjq9 wwtouched.zip wwtouched.nds

clean:
	@rm -f *.bin defines.s brainage.nds brainage.zip kirby.nds kirby.zip wwtouched.nds wwtouched.zip yoshids.nds yoshids.zip
	@cd option_select && make clean && cd ..
	@cd hbl_loader && make clean && cd ..
	@cd cfw_booter && make clean && cd ..
	@echo "all cleaned up !"
