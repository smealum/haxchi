PATH := $(DEVKITPPC)/bin:$(PATH)
PREFIX ?= powerpc-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CFLAGS = -std=gnu99 -O3 -nostdinc -fno-builtin
ASFLAGS = -mregnames -x assembler-with-cpp
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
LDFLAGS=-Ttext 1800000 -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2 -lgcc
OBJDUMP ?= $(PREFIX)objdump
project	:=	.
root	:=	$(CURDIR)
build	:=	 $(root)/bin

sd_loader_elf := sd_loader/sd_loader.elf

CFLAGS += -DUSE_SD_LOADER
ASFLAGS += -DUSE_SD_LOADER
FIRMWARE = 550

all: clean setup main

sd_loader.h: $(sd_loader_elf)
	xxd -i $< | sed "s/unsigned/static const unsigned/g;s/loader/loader/g;s/build_//g" > $@

$(sd_loader_elf):
	make -C sd_loader

setup:
	mkdir -p $(root)/bin/

main: sd_loader.h
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) -c $(project)/launcher.c
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) -c $(project)/kexploit.c
	$(AS) $(ASFLAGS) -DVER=$(FIRMWARE) -c $(project)/kernel_patches.S
	$(AS) $(ASFLAGS) -DVER=$(FIRMWARE) -c $(project)/crt0.S
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o code$(FIRMWARE).elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map code.map
	$(OBJCOPY) code$(FIRMWARE).elf -S -O binary ../code$(FIRMWARE).bin

clean:
	rm -rf $(build)
	rm -rf sd_loader.h
	rm -rf code$(FIRMWARE).elf code.map
	make clean -C sd_loader

print_stats:
	@echo
	@echo "code size : loadiine   =>" `$(OBJDUMP) -h ../loadiine.elf | awk '/.kernel_code|.text|.menu_magic|.loader_magic|.fs_method_calls|.rodata|.data|.sdata|.bss|.sbss|.fs_magic/ { sum+=strtonum("0x"$$3) } END {print sum}'` / 7530312
