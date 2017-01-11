ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq ($(filter $(DEVKITARM)/bin,$(PATH)),)
export PATH:=$(DEVKITARM)/bin:$(PATH)
endif

CC = arm-none-eabi-gcc
# LINK = arm-none-eabi-gcc
LINK = arm-none-eabi-ld
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
CFLAGS += -Wall -mbig-endian -std=c99 -mcpu=arm926ej-s -Os -s -mthumb -I$(DEVKITPRO)/libnds/include
LDFLAGS += --script=ccd00.ld -EB -L"$(DEVKITARM)/arm-none-eabi/lib"

CFILES = $(wildcard source/*.c)
BINFILES = $(wildcard data/*.*)
OFILES = $(CFILES:source/%.c=build/%.o)
DFILES = $(CFILES:source/%.c=build/%.d)
SFILES = $(wildcard source/*.s)
OFILES += $(SFILES:source/%.s=build/%.o)
OFILES += $(BINFILES:data/%=build/%.o)
PROJECTNAME = ${shell basename "$(CURDIR)"}
CWD = "$(CURDIR)""

#---------------------------------------------------------------------------------
# canned command sequence for binary data, taken from devkitARM
#---------------------------------------------------------------------------------
define bin2o
	bin2s $< | $(AS) -o $(@)
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > source/`(echo $(<F) | tr . _)`.h
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> source/`(echo $(<F) | tr . _)`.h
	echo "extern const u32" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> source/`(echo $(<F) | tr . _)`.h
endef

.PHONY:=all dirs

all: dirs $(PROJECTNAME).bin $(PROJECTNAME)_bin.h

dirs:
	@mkdir -p build

$(PROJECTNAME).bin: $(PROJECTNAME).elf
#	$(OBJCOPY) -O binary $< $@
	$(OBJCOPY) -j .text -j .rodata -S -O binary $< $@

$(PROJECTNAME)_bin.h: $(PROJECTNAME).bin
	xxd -i $< | sed "s/unsigned/static const unsigned/g;s/$(PROJECTNAME)$*/$(PROJECTNAME)/g" > $@

$(PROJECTNAME).elf: $(OFILES)
	$(LINK) $(LDFLAGS) -o $(PROJECTNAME).elf $(sort $(filter-out build/crt0.o, $(OFILES))) libgcc.a

clean:
	@rm -f build/*.o build/*.d
	@rmdir build 2>/nul; true
	@rm -f $(PROJECTNAME).elf $(PROJECTNAME).bin $(PROJECTNAME)_bin.h
	@echo "all cleaned up !"

-include $(DFILES)

build/%.o: source/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) -MM $< > build/$*.d

build/%.o: source/%.s
	$(CC) $(CFLAGS) -xassembler-with-cpp -c $< -o $@
	@$(CC) -MM $< > build/$*.d

build/%.bin.o: data/%.bin
	@echo $(notdir $<)
	@$(bin2o)

build/%.tga.o: data/%.tga
	@echo $(notdir $<)
	@$(bin2o)
