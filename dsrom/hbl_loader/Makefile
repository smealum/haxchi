PATH := $(DEVKITPPC)/bin:$(PATH)
PREFIX ?= powerpc-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CFLAGS = -std=gnu99 -O3 -nostdinc -fno-builtin
ASFLAGS = -mregnames -x assembler-with-cpp
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
LDFLAGS=-Ttext 1800000 -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/6.2.0 -lgcc
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
	$(LD) -o hbl_loader.elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map hbl_loader.map
	$(OBJCOPY) hbl_loader.elf -S -O binary ../hbl_loader.bin

clean:
	rm -rf $(build)
	rm -rf sd_loader.h
	rm -rf hbl_loader.elf hbl_loader.map
	make clean -C sd_loader
