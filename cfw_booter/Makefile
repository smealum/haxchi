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

CFLAGS += -DUSE_SD_LOADER
ASFLAGS += -DUSE_SD_LOADER
FIRMWARE = 550

all: clean setup main

$(CURDIR)/payload/arm_kernel_bin.h:  $(CURDIR)/payload/arm_user_bin.h
	@$(MAKE) --no-print-directory -C $(CURDIR)/arm_kernel -f  $(CURDIR)/arm_kernel/Makefile
	@-mkdir -p $(CURDIR)/payload
	@cp -p $(CURDIR)/arm_kernel/arm_kernel_bin.h $@
	
$(CURDIR)/payload/arm_user_bin.h:
	@$(MAKE) --no-print-directory -C $(CURDIR)/arm_user -f  $(CURDIR)/arm_user/Makefile
	@-mkdir -p $(CURDIR)/payload
	@cp -p $(CURDIR)/arm_user/arm_user_bin.h $@

setup:
	mkdir -p $(root)/bin/

main: $(CURDIR)/payload/arm_kernel_bin.h
	$(CC) $(CFLAGS) -DVER=$(FIRMWARE) -c $(project)/main.c
	$(AS) $(ASFLAGS) -DVER=$(FIRMWARE) -c $(project)/crt0.S
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	$(LD) -o code$(FIRMWARE).elf $(build)/crt0.o `find $(build) -name "*.o" ! -name "crt0.o"` $(LDFLAGS) -Map code.map
	$(OBJCOPY) code$(FIRMWARE).elf -S -O binary ../code$(FIRMWARE).bin

clean:
	rm -rf $(build) payload
	rm -rf code$(FIRMWARE).elf code.map
	$(MAKE) --no-print-directory -C $(CURDIR)/arm_user -f  $(CURDIR)/arm_user/Makefile clean
	$(MAKE) --no-print-directory -C $(CURDIR)/arm_kernel -f  $(CURDIR)/arm_kernel/Makefile clean

print_stats:
	@echo
	@echo "code size : loadiine   =>" `$(OBJDUMP) -h ../loadiine.elf | awk '/.kernel_code|.text|.menu_magic|.loader_magic|.fs_method_calls|.rodata|.data|.sdata|.bss|.sbss|.fs_magic/ { sum+=strtonum("0x"$$3) } END {print sum}'` / 7530312
