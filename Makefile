.PHONY := all code550.bin

all: brainage kirby yoshids brainage.zip kirby.zip yoshids.zip

brainage: setup_brainage brainage.nds

kirby: setup_kirby kirby.nds

yoshids: setup_yoshids yoshids.nds

setup_brainage:
	rm -f *.bin
	@cd hbl_loader && make && cd ..
	@cp -f brainage_defs.s defines.s

setup_kirby:
	rm -f *.bin
	@cd hbl_loader && make && cd ..
	@cp -f kirby_defs.s defines.s

setup_yoshids:
	rm -f *.bin
	@cd hbl_loader && make && cd ..
	@cp -f yoshids_defs.s defines.s

brainage.nds:
	armips haxchi_rop.s
	armips haxchi.s

kirby.nds:
	armips haxchi_rop.s
	armips haxchi.s

yoshids.nds:
	armips haxchi_rop.s
	armips haxchi.s

brainage.zip:
	zip -JXjq9 brainage.zip brainage.nds

kirby.zip:
	zip -JXjq9 kirby.zip kirby.nds

yoshids.zip:
	zip -JXjq9 yoshids.zip yoshids.nds

clean:
	@rm -f *.bin brainage.nds brainage.zip kirby.nds kirby.zip yoshids.nds yoshids.zip
	@cd hbl_loader && make clean && cd ..
	@echo "all cleaned up !"
