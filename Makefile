.PHONY := all haxchi/haxchi_code.bin

all: haxchi.srl

haxchi/haxchi_code.bin:
	@cd haxchi_code && make clean && make && cd ..

haxchi_rop_hook.bin haxchi_rop.bin: haxchi/haxchi_code.bin haxchi_rop.s
	armips haxchi_rop.s

haxchi.srl: haxchi_rop_hook.bin haxchi_rop.bin haxchi.s
	armips haxchi.s

clean:
	@rm -f *.bin haxchi.srl
	@echo "all cleaned up !"
