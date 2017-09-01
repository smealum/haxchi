PATH := $(DEVKITPPC)/bin:$(PATH)
PREFIX ?= powerpc-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CFLAGS = -std=gnu89 -O3 -Wall -nostdinc -fno-builtin -I$(DEVKITPPC)/lib/gcc/powerpc-eabi/6.2.0/include -I$(DEVKITPPC)/powerpc-eabi/include
ASFLAGS = -mregnames -x assembler-with-cpp
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
LDFLAGS=-Ttext 1808000 -L$(DEVKITPPC)/lib/gcc/powerpc-eabi/6.2.0 -L$(DEVKITPPC)/powerpc-eabi/lib -lgcc -lc
OBJDUMP ?= $(PREFIX)objdump
project	:=	.
root	:=	$(CURDIR)
build	:=	 $(root)/bin

FIRMWARE = 550

all: clean setup main

setup:
	mkdir -p $(root)/bin/

main:
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) -c $(project)/main.c
	$(AS) $(ASFLAGS) -DVER=$(FIRMWARE) -c $(project)/crt0.S
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o cbhc_menu.elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map cbhc_menu.map
	$(OBJCOPY) cbhc_menu.elf -S -O binary ../cbhc_menu.bin

clean:
	rm -rf $(build) payload
	rm -rf cbhc_menu.elf cbhc_menu.map
