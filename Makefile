.PHONY := all code550.bin

all: WUP-N-DAAP.nds

code550.bin:
	@cd hbl_loader && make && cd ..

haxchi_rop_hook.bin haxchi_rop.bin: code550.bin haxchi_rop.s
	armips haxchi_rop.s

WUP-N-DAAP.nds: haxchi_rop_hook.bin haxchi_rop.bin haxchi.s
	armips haxchi.s
	zip -JXjq9 rom.zip WUP-N-DAAP.nds

clean:
	@rm -f *.bin WUP-N-DAAP.nds rom.zip
	@cd hbl_loader && make clean && cd ..
	@echo "all cleaned up !"
