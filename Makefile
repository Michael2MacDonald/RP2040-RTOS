
# export PATH="C:\Users\Michael\Documents\Personal\Programing\Embedded Development\OSAB\OSAB-RP2040-Kernel\tools\arm\bin":$PATH


GCC_BASE = tools\arm\bin
CC              := $(GCC_BASE)/arm-none-eabi-gcc
AR              := $(GCC_BASE)/arm-none-eabi-gcc-ar
NM              := $(GCC_BASE)/arm-none-eabi-gcc-nm
OBJDUMP         := $(GCC_BASE)/arm-none-eabi-objdump
OBJCOPY         := $(GCC_BASE)/arm-none-eabi-objcopy

TARGET_NAME = blink

LINKER = rp2040.ld
MKUF2  = mkuf2

# -fpic position independent code

FLAGS_CPU := -mthumb -mcpu=cortex-m0plus
FLAGS_OPT := -O2
FLAGS_COM := -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD
FLAGS_LSP := 

FLAGS_S := 
FLAGS_C := 
FLAGS_LD := -Wl,--print-memory-usage,--gc-sections -T$(LINKER) --specs=nano.specs --specs=nosys.specs #-nostartfiles
#--relax

C_FLAGS     := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_C)
S_FLAGS     := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_S)
LD_FLAGS    := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_LSP) $(FLAGS_LD)

ASM  = $(wildcard src/*.S)
SRC  = $(wildcard src/*.c)
INC  = $(wildcard inc/*.h)
OBJA = $(ASM:src/%.S=$(BIN)/%.s.o)
OBJC = $(SRC:src/%.c=$(BIN)/%.c.o)
# LIBS = -lstdc++


BIN             := bin
USR_BIN         := $(BIN)/src
LIB_BIN         := $(BIN)/libs
KERNEL_BIN      := $(BIN)/kernel
KERNEL_LIB      := $(BIN)/kernel.a
CORE_BIN        := $(BIN)/core
CORE_LIB        := $(BIN)/core.a
TARGET_HEX      := $(BIN)/$(TARGET_NAME).hex
TARGET_BIN      := $(BIN)/$(TARGET_NAME).bin
TARGET_ELF      := $(BIN)/$(TARGET_NAME).elf
TARGET_LST      := $(BIN)/$(TARGET_NAME).lst
TARGET_SYM      := $(BIN)/$(TARGET_NAME).sym
TARGET_UF2      := $(BIN)/$(TARGET_NAME).uf2


all: build $(TARGET_BIN) $(TARGET_LST) $(TARGET_SYM) $(TARGET_HEX) $(TARGET_UF2)

clean:
	@echo Clean
	@if exist $(BIN) rd /s/q "$(BIN)"
	@if exist $(TARGET_NAME).* del $(TARGET_NAME).*


$(OBJC): $(BIN)/%.c.o: src/%.c $(INC)
	@echo Compile $<
	@"$(CC)" $(C_FLAGS) -Iinc -c $< -o "$@"

$(OBJA): $(BIN)/%.s.o: src/%.S $(INC)
	@echo Compile $<
	@"$(CC)" $(S_FLAGS) -c $< -o "$@"

$(TARGET_ELF): $(LINKER) $(OBJA) $(OBJC)
	@echo Create $@
	@"$(CC)" $(LD_FLAGS) $(OBJA) $(OBJC) -o $@


%.uf2: %.elf
	@echo Create $@
	@tools/elf2uf2 $< $@
	@echo Successfully built project &&echo.

%.lst: %.elf
	@echo [LST] $@
	@"$(OBJDUMP)" -d -S --demangle --no-show-raw-insn "$<" > "$@"

%.sym: %.elf
	@echo [SYM] $@
	@"$(NM)" $(NM_FLAGS) "$<" > "$@"

%.hex: %.elf
	@echo [HEX] $@
	@"$(OBJCOPY)" -O ihex -R.eeprom "$<" "$@"

%.bin: %.elf
	@echo Create $@
	@"$(OBJCOPY)" -O binary $< $@




build:
	@echo Create $@
	@mkdir $(BIN)