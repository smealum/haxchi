.PHONY := all code550.bin

ifeq ($(Windows_NT), 1)
	ZIP = zip/zip.exe
else
	ZIP = zip
endif

ifeq ($(CB), 1)
	HAXCHI_S = haxchi_cb.s
	ROP_S = haxchi_rop_cb.s
	ROP_BIN = haxchi_rop_cb.bin
else
	HAXCHI_S = haxchi.s
	ROP_S = haxchi_rop.s
	ROP_BIN = haxchi_rop.bin
endif

all: setup animalcrossing brainage dkjclimber guardiansigns kirby kirbymassattack mariokartds masterofdisguise newsmb_eur partnersintime \
	pokemonranger sfcommand sm64ds yoshids zeldaph zeldast \
	animalcrossing.zip brainage.zip dkjclimber.zip guardiansigns.zip kirby.zip kirbymassattack.zip mariokartds.zip masterofdisguise.zip \
	newsmb_eur.zip partnersintime.zip pokemonranger.zip sfcommand.zip sm64ds.zip yoshids.zip zeldaph.zip zeldast.zip

animalcrossing: setup_animalcrossing animalcrossing.nds

brainage: setup_brainage brainage.nds

dkjclimber: setup_dkjclimber dkjclimber.nds

guardiansigns: setup_guardiansigns guardiansigns.nds

kirby: setup_kirby kirby.nds

kirbymassattack: setup_kirbymassattack kirbymassattack.nds

mariokartds: setup_mariokartds mariokartds.nds

masterofdisguise: setup_masterofdisguise masterofdisguise.nds

newsmb_eur: setup_newsmb_eur newsmb_eur.nds

partnersintime: setup_partnersintime partnersintime.nds

pokemonranger: setup_pokemonranger pokemonranger.nds

sfcommand: setup_sfcommand sfcommand.nds

sm64ds: setup_sm64ds sm64ds.nds

yoshids: setup_yoshids yoshids.nds

zeldaph: setup_zeldaph zeldaph.nds

zeldast: setup_zeldast zeldast.nds

setup:
	@cd cbhc_menu && make && cd ..
	@cd hbl_loader && make && cd ..
	@cd iosu_patcher && make && cd ..
	@cd option_select && make && cd ..

setup_animalcrossing:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f animalcrossing_defs.s defines.s

setup_brainage:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f brainage_defs.s defines.s

setup_dkjclimber:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f dkjclimber_defs.s defines.s

setup_guardiansigns:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f guardiansigns_defs.s defines.s

setup_kirby:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f kirby_defs.s defines.s

setup_kirbymassattack:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f kirbymassattack_defs.s defines.s

setup_mariokartds:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f mariokartds_defs.s defines.s

setup_masterofdisguise:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f masterofdisguise_defs.s defines.s

setup_newsmb_eur:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f newsmb_eur_defs.s defines.s

setup_pokemonranger:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f pokemonranger_defs.s defines.s

setup_partnersintime:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f partnersintime_defs.s defines.s

setup_sfcommand:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f sfcommand_defs.s defines.s

setup_sm64ds:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f sm64ds_defs.s defines.s

setup_yoshids:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f yoshids_defs.s defines.s

setup_zeldaph:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f zeldaph_defs.s defines.s

setup_zeldast:
	@rm -f defines.s $(ROP_BIN) haxchi_rop_hook.bin
	@cp -f zeldast_defs.s defines.s

animalcrossing.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds animalcrossing.nds

brainage.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds brainage.nds
	@cp brainage.nds yoshitouchandgo.nds

dkjclimber.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds dkjclimber.nds

guardiansigns.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds guardiansigns.nds

kirby.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds kirby.nds

kirbymassattack.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds kirbymassattack.nds

mariokartds.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds mariokartds.nds
	@cp mariokartds.nds newsmb.nds

masterofdisguise.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds masterofdisguise.nds

newsmb_eur.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds newsmb_eur.nds

pokemonranger.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds pokemonranger.nds

partnersintime.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds partnersintime.nds

sfcommand.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds sfcommand.nds

sm64ds.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds sm64ds.nds
	@cp sm64ds.nds kirbycanvascurse.nds

yoshids.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds yoshids.nds
	@cp yoshids.nds wwtouched.nds
	@cp yoshids.nds bigbrainacademy.nds

zeldaph.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds zeldaph.nds

zeldast.nds:
	@armips $(ROP_S)
	@armips $(HAXCHI_S)
	@mv rom.nds zeldast.nds
	@cp zeldast.nds explorersofsky.nds
	@cp zeldast.nds shadowsofalmia.nds

animalcrossing.zip:
	$(ZIP) -JXjq9 ../installer/data/animalcrossing.zip animalcrossing.nds

brainage.zip:
	$(ZIP) -JXjq9 ../installer/data/brainage.zip brainage.nds
	$(ZIP) -JXjq9 ../installer/data/yoshitouchandgo.zip yoshitouchandgo.nds

dkjclimber.zip:
	$(ZIP) -JXjq9 ../installer/data/dkjclimber.zip dkjclimber.nds

guardiansigns.zip:
	$(ZIP) -JXjq9 ../installer/data/guardiansigns.zip guardiansigns.nds

kirby.zip:
	$(ZIP) -JXjq9 ../installer/data/kirby.zip kirby.nds

kirbymassattack.zip:
	$(ZIP) -JXjq9 ../installer/data/kirbymassattack.zip kirbymassattack.nds

mariokartds.zip:
	$(ZIP) -JXjq9 ../installer/data/mariokartds.zip mariokartds.nds
	$(ZIP) -JXjq9 ../installer/data/newsmb.zip newsmb.nds

masterofdisguise.zip:
	$(ZIP) -JXjq9 ../installer/data/masterofdisguise.zip masterofdisguise.nds

newsmb_eur.zip:
	$(ZIP) -JXjq9 ../installer/data/newsmb_eur.zip newsmb_eur.nds

pokemonranger.zip:
	$(ZIP) -JXjq9 ../installer/data/pokemonranger.zip pokemonranger.nds

partnersintime.zip:
	$(ZIP) -JXjq9 ../installer/data/partnersintime.zip partnersintime.nds

sfcommand.zip:
	$(ZIP) -JXjq9 ../installer/data/sfcommand.zip sfcommand.nds

sm64ds.zip:
	$(ZIP) -JXjq9 ../installer/data/sm64ds.zip sm64ds.nds
	$(ZIP) -JXjq9 ../installer/data/kirbycanvascurse.zip kirbycanvascurse.nds

yoshids.zip:
	$(ZIP) -JXjq9 ../installer/data/yoshids.zip yoshids.nds
	$(ZIP) -JXjq9 ../installer/data/wwtouched.zip wwtouched.nds
	$(ZIP) -JXjq9 ../installer/data/bigbrainacademy.zip bigbrainacademy.nds

zeldaph.zip:
	$(ZIP) -JXjq9 ../installer/data/zeldaph.zip zeldaph.nds

zeldast.zip:
	$(ZIP) -JXjq9 ../installer/data/zeldast.zip zeldast.nds
	$(ZIP) -JXjq9 ../installer/data/explorersofsky.zip explorersofsky.nds
	$(ZIP) -JXjq9 ../installer/data/shadowsofalmia.zip shadowsofalmia.nds

clean:
	@rm -f *.bin *.nds ../installer/data/*.zip defines.s
	@cd cbhc_menu && make clean && cd ..
	@cd hbl_loader && make clean && cd ..
	@cd iosu_patcher && make clean && cd ..
	@cd option_select && make clean && cd ..
	@echo "all cleaned up !"
