PATH := $(DEVKITPPC)/bin:$(PATH)
PREFIX ?= powerpc-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CFLAGS = -std=gnu99 -O3 -nostdinc -fno-builtin -I$(DEVKITPPC)/lib/gcc/powerpc-eabi/6.2.0/include -I$(DEVKITPPC)/powerpc-eabi/include
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
	$(LD) -o option_select.elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map option_select.map
	$(OBJCOPY) option_select.elf -S -O binary ../option_select.bin

clean:
	rm -rf $(build)
	rm -rf option_select.elf option_select.map
