PATH := $(DEVKITPPC)/bin:$(PATH)
PREFIX ?= powerpc-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CFLAGS = -std=gnu99 -O3 -nostdinc -fno-builtin -I$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2/include -I$(DEVKITPPC)/powerpc-eabi/include
ASFLAGS = -mregnames -x assembler-with-cpp
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
LDFLAGS=-Ttext 1808000 -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2 -L$(DEVKITPPC)/powerpc-eabi/lib -lgcc -lc
OBJDUMP ?= $(PREFIX)objdump
project	:=	.
root	:=	$(CURDIR)
build	:=	 $(root)/bin

CFLAGS += -DUSE_SD_LOADER
ASFLAGS += -DUSE_SD_LOADER
FIRMWARE = 550

all: clean setup main

setup:
	mkdir -p $(root)/bin/

main:
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) -c $(project)/main.c
	$(AS) $(ASFLAGS) -DVER=$(FIRMWARE) -c $(project)/crt0.S
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o code$(FIRMWARE).elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map code.map
	$(OBJCOPY) code$(FIRMWARE).elf -S -O binary ../code$(FIRMWARE).bin

clean:
	rm -rf $(build)
	rm -rf code$(FIRMWARE).elf code.map
