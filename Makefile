.PHONY := all code550.bin

ifeq ($(Windows_NT), 1)
	ZIP = zip/zip.exe
else
	ZIP = zip
endif

all: setup brainage dkjclimber kirby kirbymassattack mariokartds masterofdisguise newsmb_eur partnersintime sfcommand sm64ds yoshids zeldaph \
	brainage.zip dkjclimber.zip kirby.zip kirbymassattack.zip mariokartds.zip masterofdisguise.zip newsmb_eur.zip partnersintime.zip \
	sfcommand.zip sm64ds.zip yoshids.zip yoshitouchandgo.zip zeldaph.zip

brainage: setup_brainage brainage.nds

dkjclimber: setup_dkjclimber dkjclimber.nds

kirby: setup_kirby kirby.nds

kirbymassattack: setup_kirbymassattack kirbymassattack.nds

mariokartds: setup_mariokartds mariokartds.nds

masterofdisguise: setup_masterofdisguise masterofdisguise.nds

newsmb_eur: setup_newsmb_eur newsmb_eur.nds

partnersintime: setup_partnersintime partnersintime.nds

sfcommand: setup_sfcommand sfcommand.nds

sm64ds: setup_sm64ds sm64ds.nds

yoshids: setup_yoshids yoshids.nds

zeldaph: setup_zeldaph zeldaph.nds

setup:
	@cd option_select && make && cd ..
	@cd hbl_loader && make && cd ..
	@cd cfw_booter && make && cd ..

setup_brainage:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f brainage_defs.s defines.s

setup_dkjclimber:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f dkjclimber_defs.s defines.s

setup_kirby:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f kirby_defs.s defines.s

setup_kirbymassattack:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f kirbymassattack_defs.s defines.s

setup_mariokartds:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f mariokartds_defs.s defines.s

setup_masterofdisguise:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f masterofdisguise_defs.s defines.s

setup_newsmb_eur:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f newsmb_eur_defs.s defines.s

setup_partnersintime:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f partnersintime_defs.s defines.s

setup_sfcommand:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f sfcommand_defs.s defines.s

setup_sm64ds:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f sm64ds_defs.s defines.s

setup_yoshids:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f yoshids_defs.s defines.s

setup_zeldaph:
	@rm -f defines.s haxchi_rop.bin haxchi_rop_hook.bin
	@cp -f zeldaph_defs.s defines.s

brainage.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds brainage.nds
	@cp brainage.nds yoshitouchandgo.nds

dkjclimber.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds dkjclimber.nds

kirby.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds kirby.nds

kirbymassattack.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds kirbymassattack.nds

mariokartds.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds mariokartds.nds
	@cp mariokartds.nds newsmb.nds

masterofdisguise.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds masterofdisguise.nds

newsmb_eur.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds newsmb_eur.nds

partnersintime.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds partnersintime.nds

sfcommand.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds sfcommand.nds

sm64ds.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds sm64ds.nds
	@cp sm64ds.nds kirbycanvascurse.nds

yoshids.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds yoshids.nds
	@cp yoshids.nds wwtouched.nds
	@cp yoshids.nds bigbrainacademy.nds

zeldaph.nds:
	@armips haxchi_rop.s
	@armips haxchi.s
	@mv rom.nds zeldaph.nds

brainage.zip:
	$(ZIP) -JXjq9 brainage.zip brainage.nds
	$(ZIP) -JXjq9 yoshitouchandgo.zip yoshitouchandgo.nds

dkjclimber.zip:
	$(ZIP) -JXjq9 dkjclimber.zip dkjclimber.nds

kirby.zip:
	$(ZIP) -JXjq9 kirby.zip kirby.nds

kirbymassattack.zip:
	$(ZIP) -JXjq9 kirbymassattack.zip kirbymassattack.nds

mariokartds.zip:
	$(ZIP) -JXjq9 mariokartds.zip mariokartds.nds
	$(ZIP) -JXjq9 newsmb.zip newsmb.nds

masterofdisguise.zip:
	$(ZIP) -JXjq9 masterofdisguise.zip masterofdisguise.nds

newsmb_eur.zip:
	$(ZIP) -JXjq9 newsmb_eur.zip newsmb_eur.nds

partnersintime.zip:
	$(ZIP) -JXjq9 partnersintime.zip partnersintime.nds

sfcommand.zip:
	$(ZIP) -JXjq9 sfcommand.zip sfcommand.nds

sm64ds.zip:
	$(ZIP) -JXjq9 sm64ds.zip sm64ds.nds
	$(ZIP) -JXjq9 kirbycanvascurse.zip kirbycanvascurse.nds

yoshids.zip:
	$(ZIP) -JXjq9 yoshids.zip yoshids.nds
	$(ZIP) -JXjq9 wwtouched.zip wwtouched.nds
	$(ZIP) -JXjq9 bigbrainacademy.zip bigbrainacademy.nds

zeldaph.zip:
	$(ZIP) -JXjq9 zeldaph.zip zeldaph.nds

clean:
	@rm -f *.bin *.nds *.zip defines.s
	@cd option_select && make clean && cd ..
	@cd hbl_loader && make clean && cd ..
	@cd cfw_booter && make clean && cd ..
	@echo "all cleaned up !"
