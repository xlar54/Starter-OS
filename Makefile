# Makefile for Commodore OS

BUILD_DIR = bin
SOURCE_DIR = src
INCLUDE_DIR=include

CFLAGS = -nostdlib -nostdinc -fno-builtin -m32 -ffreestanding
LDFLAGS = -Tlink.ld -melf_i386
ASFLAGS=-felf

CSOURCE := $(wildcard $(SOURCE_DIR)/*.c)
ASOURCE := $(wildcard $(SOURCE_DIR)/*.s)

_COBJECT := $(patsubst %.c,%.o, $(CSOURCE))
_AOBJECT := $(patsubst %.s,%.o, $(ASOURCE))
AOBJECT = $(addprefix $(BUILD_DIR)/, $(notdir $(_AOBJECT)))
COBJECT = $(addprefix $(BUILD_DIR)/, $(notdir $(_COBJECT)))


all: link

# Link all of the objects
link: $(AOBJECT) $(COBJECT)
	ld $(LDFLAGS) $(AOBJECT) $(COBJECT) -L. -Map $(BUILD_DIR)/kernel.map -o $(BUILD_DIR)/kernel.bin

#build c files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	gcc -c $< -o $@ $(CFLAGS) -I $(INCLUDE_DIR)

#build s files (Assembly)
.s.o:
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.s
	nasm $(ASFLAGS) $< -o $@

clean:
	rm $(BUILD_DIR)/*.bin
	rm $(BUILD_DIR)/*.o
	rm $(BUILD_DIR)/*.map
	


