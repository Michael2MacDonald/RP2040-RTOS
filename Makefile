
# export PATH="C:\Users\Michael\Documents\Personal\Programing\Embedded Development\OSAB\OSAB-RP2040-Kernel\tools\arm\bin":$PATH
SHELL := bash
export SHELL

include Config.mk
include Board.mk


# BASE := cd C:/Users/Michael/Documents/Personal/Programing/Embedded\ Development/RP2040-Blink-UART &&


#******************************************************************************
# Flags and Defines
#******************************************************************************
FLAGS_CPU := -mthumb -mcpu=cortex-m0plus -mabi=aapcs #-mfloat-abi=hard -mfpu=fpv5-d16
FLAGS_OPT := -O$(OPT)
FLAGS_COM := -g -MMD -Wall -Wextra -Wno-unused-parameter -Wunsafe-loop-optimizations -Wstack-usage=256 -ffunction-sections -fdata-sections #--verbose
FLAGS_LSP :=  

FLAGS_S   := -x assembler-with-cpp
FLAGS_C   := -std=gnu11
FLAGS_CPP := -std=gnu++14 -fno-exceptions -fpermissive -fno-rtti -felide-constructors -Wno-error=narrowing #-fno-threadsafe-statics
FLAGS_LD  := -T$(LINKER) -Wl,--print-memory-usage,--gc-sections --specs=nano.specs --specs=nosys.specs #-nostartfiles #-nostdlib 
# --specs=rdimon.specs: Enables semihosting which allows you to use system calls and printf by passing them to the host computer via the debugger
# --specs=nano.specs: use newlib-nano
# --specs=nosys.specs: defines that system calls should be implemented as stubs that return errors when called
# (--relax) (-fpic = position independent code)

LIBS      := -lm -lstdc++
#-larm_cortexM7lfsp_math

DEFINES   := $(CFG_DEFINES) $(BOARD_DEFINES) -DLOW_REGS_ONLY=1

CPP_FLAGS := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_CPP)
C_FLAGS   := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_C)
S_FLAGS   := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_S)
LD_FLAGS  := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_LSP) $(FLAGS_LD)
AR_FLAGS  := rcs
NM_FLAGS  := --numeric-sort --defined-only --demangle --print-size

#******************************************************************************
# Colors
#******************************************************************************

COL_CORE    := [38;2;187;206;251m
COL_KERNEL  := [38;2;100;206;251m
COL_LIB     := [38;2;206;244;253m
COL_SRC     := [38;2;100;149;237m
COL_LINK    := [38;2;255;255;202m
COL_ERR     := [38;2;255;159;159m
COL_OK      := [38;2;179;255;179m
COL_RESET   := [0m


#******************************************************************************
# Folders and Files
#******************************************************************************

# USR_SRC         := $(BASE)/src
# LIB_SRC         := $(BASE)/libs
# KERNEL_SRC      := $(BASE)/kernel
# CORE_SRC        := $(BASE)/core
# # CORE_SRC        := $(BASE)/$(CORE_BASE)
USR_SRC         := src
LIB_SRC         := libs
KERNEL_SRC      := kernel
CORE_SRC        := core
# CORE_SRC        := $(CORE_BASE)

# BIN             := $(BASE)/bin
BIN             := bin
USR_BIN         := $(BIN)/$(USR_SRC)
LIB_BIN         := $(BIN)/$(LIB_SRC)
KERNEL_BIN      := $(BIN)/$(KERNEL_SRC)
KERNEL_LIB      := $(BIN)/kernel.a
CORE_BIN        := $(BIN)/$(CORE_SRC)
CORE_LIB        := $(BIN)/core.a
TARGET_HEX      := $(BIN)/$(TARGET_NAME).hex
TARGET_BIN      := $(BIN)/$(TARGET_NAME).bin
TARGET_ELF      := $(BIN)/$(TARGET_NAME).elf
TARGET_LST      := $(BIN)/$(TARGET_NAME).lst
TARGET_SYM      := $(BIN)/$(TARGET_NAME).sym
TARGET_UF2      := $(BIN)/$(TARGET_NAME).uf2


#******************************************************************************
# BINARIES
#******************************************************************************
GCC_BASE = tools\arm\bin
# GCC_BASE = C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin
# CC              := $(GCC_BASE)/arm-none-eabi-gcc
# CXX             := $(GCC_BASE)/arm-none-eabi-g++
# AR              := $(GCC_BASE)/arm-none-eabi-gcc-ar
# NM              := $(GCC_BASE)/arm-none-eabi-gcc-nm
# OBJDUMP         := $(GCC_BASE)/arm-none-eabi-objdump
# OBJCOPY         := $(GCC_BASE)/arm-none-eabi-objcopy
CC              := arm-none-eabi-gcc
CXX             := arm-none-eabi-g++
AR              := arm-none-eabi-gcc-ar
NM              := arm-none-eabi-gcc-nm
OBJDUMP         := arm-none-eabi-objdump
OBJCOPY         := arm-none-eabi-objcopy
# CC              := cd $(BASE) && arm-none-eabi-gcc
# CXX             := cd $(BASE) && arm-none-eabi-g++
# AR              := cd $(BASE) && arm-none-eabi-gcc-ar
# NM              := cd $(BASE) && arm-none-eabi-gcc-nm
# OBJDUMP         := cd $(BASE) && arm-none-eabi-objdump
# OBJCOPY         := cd $(BASE) && arm-none-eabi-objcopy


#******************************************************************************
# Source and Include Files
#******************************************************************************
# Recursively create list of source and object files in USR_SRC and CORE_SRC
# and corresponding subdirectories.
# The function rwildcard is taken from http://stackoverflow.com/a/12959694)

rwildcard =$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#User Sources -----------------------------------------------------------------
USR_C_FILES     := $(call rwildcard,$(USR_SRC)/,*.c)
USR_CPP_FILES   := $(call rwildcard,$(USR_SRC)/,*.cpp)
USR_S_FILES     := $(call rwildcard,$(USR_SRC)/,*.S)
USR_OBJ         := $(USR_S_FILES:$(USR_SRC)/%.S=$(USR_BIN)/%.s.o) $(USR_C_FILES:$(USR_SRC)/%.c=$(USR_BIN)/%.c.o) $(USR_CPP_FILES:$(USR_SRC)/%.cpp=$(USR_BIN)/%.cpp.o)

# Core library sources --------------------------------------------------------
CORE_CPP_FILES  := $(call rwildcard,$(CORE_SRC)/,*.cpp)
CORE_C_FILES    := $(call rwildcard,$(CORE_SRC)/,*.c)
CORE_S_FILES    := $(call rwildcard,$(CORE_SRC)/,*.S)
CORE_OBJ        := $(CORE_S_FILES:$(CORE_SRC)/%.S=$(CORE_BIN)/%.s.o) $(CORE_C_FILES:$(CORE_SRC)/%.c=$(CORE_BIN)/%.c.o) $(CORE_CPP_FILES:$(CORE_SRC)/%.cpp=$(CORE_BIN)/%.cpp.o)

# Kernel library sources --------------------------------------------------------
KERNEL_CPP_FILES  := $(call rwildcard,$(KERNEL_SRC)/,*.cpp)
KERNEL_C_FILES    := $(call rwildcard,$(KERNEL_SRC)/,*.c)
KERNEL_S_FILES    := $(call rwildcard,$(KERNEL_SRC)/,*.S)
KERNEL_OBJ        := $(KERNEL_S_FILES:$(KERNEL_SRC)/%.S=$(KERNEL_BIN)/%.s.o) $(KERNEL_C_FILES:$(KERNEL_SRC)/%.c=$(KERNEL_BIN)/%.c.o) $(KERNEL_CPP_FILES:$(KERNEL_SRC)/%.cpp=$(KERNEL_BIN)/%.cpp.o)

# User library sources (see https://github.com/arduino/arduino/wiki/arduino-ide-1.5:-library-specification)
LIB_DIRS_SHARED := $(foreach d, $(LIBS_SHARED), $(LIBS_SHARED_BASE)/$d/ $(LIBS_SHARED_BASE)/$d/utility/)      # base and /utility
LIB_DIRS_SHARED += $(foreach d, $(LIBS_SHARED), $(LIBS_SHARED_BASE)/$d/src/ $(dir $(call rwildcard,$(LIBS_SHARED_BASE)/$d/src/,*/.)))                          # src and all subdirs of base

LIB_DIRS_LOCAL  := $(foreach d, $(LIBS_LOCAL), $(LIBS_LOCAL_BASE)/$d/ $(LIBS_LOCAL_BASE)/$d/utility/ )        # base and /utility
LIB_DIRS_LOCAL  += $(foreach d, $(LIBS_LOCAL), $(LIBS_LOCAL_BASE)/$d/src/ $(dir $(call rwildcard,$(LIBS_LOCAL_BASE)/$d/src/,*/.)))                          # src and all subdirs of base

LIB_CPP_SHARED  := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.cpp))
LIB_C_SHARED    := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.c))
LIB_S_SHARED    := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.S))

LIB_CPP_LOCAL   := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.cpp))
LIB_C_LOCAL     := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.c))
LIB_S_LOCAL     := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.S))

LIB_OBJ         := $(LIB_CPP_SHARED:$(LIBS_SHARED_BASE)/%.cpp=$(LIB_BIN)/%.cpp.o)  $(LIB_CPP_LOCAL:$(LIBS_LOCAL_BASE)/%.cpp=$(LIB_BIN)/%.cpp.o)
LIB_OBJ         += $(LIB_C_SHARED:$(LIBS_SHARED_BASE)/%.c=$(LIB_BIN)/%.c.o)  $(LIB_C_LOCAL:$(LIBS_LOCAL_BASE)/%.c=$(LIB_BIN)/%.c.o)
LIB_OBJ         += $(LIB_S_SHARED:$(LIBS_SHARED_BASE)/%.S=$(LIB_BIN)/%.s.o)  $(LIB_S_LOCAL:$(LIBS_LOCAL_BASE)/%.S=$(LIB_BIN)/%.s.o)

# Includes -------------------------------------------------------------
INCLUDE         := -I$(USR_SRC) -I$(CORE_SRC) -I$(KERNEL_SRC)
INCLUDE         += $(foreach d, $(LIB_DIRS_SHARED), -I$d)
INCLUDE         += $(foreach d, $(LIB_DIRS_LOCAL), -I$d)

# Generate directories --------------------------------------------------------
DIRECTORIES     := $(sort $(dir $(CORE_OBJ) $(KERNEL_OBJ) $(USR_OBJ) $(LIB_OBJ)))
generateDirs    := $(foreach d, $(DIRECTORIES), $(shell if not exist "$d" mkdir "$d"))

# $(info dirs: $(DIRECTORIES))

#******************************************************************************
# Rules:
#******************************************************************************

.PHONY: all rebuild clean cleanUser cleanCore cleanKernel cleanLib

all: $(TARGET_BIN) $(TARGET_LST) $(TARGET_SYM) $(TARGET_HEX) $(TARGET_UF2)

rebuild: clean all

clean: cleanUser cleanCore cleanKernel cleanLib
	@echo Cleaning Done!


# Core library ----------------------------------------------------------------
$(CORE_BIN)/%.s.o: $(CORE_SRC)/%.S
	@echo CORE [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(CORE_BIN)/%.c.o: $(CORE_SRC)/%.c
	@echo CORE [CC]  $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(CORE_BIN)/%.cpp.o: $(CORE_SRC)/%.cpp
	@echo CORE [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(CORE_LIB) : $(CORE_OBJ)
	@echo CORE [AR] $@
	@$(AR) $(AR_FLAGS) $@ $^
	@echo Core built successfully &&echo.

# Kernel library ----------------------------------------------------------------
$(KERNEL_BIN)/%.s.o: $(KERNEL_SRC)/%.S
	@echo Kernel [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(KERNEL_BIN)/%.c.o: $(KERNEL_SRC)/%.c
	@echo Kernel [CC]  $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(KERNEL_BIN)/%.cpp.o: $(KERNEL_SRC)/%.cpp
	@echo Kernel [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(KERNEL_LIB) : $(KERNEL_OBJ)
	@echo Kernel [AR] $@
	@$(AR) $(AR_FLAGS) $@ $^
	@echo Kernel built successfully &&echo.

# Shared Libraries ------------------------------------------------------------
$(LIB_BIN)/%.s.o: $(LIBS_SHARED_BASE)/%.S
	@echo LIB [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.cpp.o: $(LIBS_SHARED_BASE)/%.cpp
	@echo LIB [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.c.o: $(LIBS_SHARED_BASE)/%.c
	@echo LIB [CC]  $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

# Local Libraries -------------------------------------------------------------
$(LIB_BIN)/%.s.o: $(LIBS_LOCAL_BASE)/%.S
	@echo LIB [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.cpp.o: $(LIBS_LOCAL_BASE)/%.cpp
	@echo LIB [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.c.o: $(LIBS_LOCAL_BASE)/%.c
	@echo LIB [CC] $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

# Handle user sources ---------------------------------------------------------
$(USR_BIN)/%.s.o: $(USR_SRC)/%.S
	@echo USER [ASM] $<
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(USR_BIN)/%.c.o: $(USR_SRC)/%.c
	@echo USER [CC] $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(USR_BIN)/%.cpp.o: $(USR_SRC)/%.cpp
	@echo USER [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

# Linking ---------------------------------------------------------------------
# $(TARGET_ELF): $(CORE_LIB) $(KERNEL_LIB) $(LIB_OBJ) $(USR_OBJ)
$(TARGET_ELF): $(USR_OBJ) $(LIB_OBJ) $(CORE_OBJ) $(KERNEL_OBJ)
# $(TARGET_ELF): $(USR_OBJ) $(LIB_OBJ) $(CORE_LIB) $(KERNEL_LIB) #$(CORE_LIB)
	@echo [LD] $@
# LIBS MUST BE THE LAST ITEM OR IT WILL NOT LINK CORRECTLY!!!!
	@"$(CC)" $(LD_FLAGS) -o "$@" $(USR_OBJ) $(LIB_OBJ) $(CORE_OBJ) $(KERNEL_OBJ) $(LIBS)
# @"$(CC)" $(LD_FLAGS) -o "$@" $(USR_OBJ) $(LIB_OBJ) $(CORE_LIB) $(KERNEL_LIB) $(LIBS)
	@echo User code built and linked to libraries &&echo.

%.uf2: %.elf
	@echo [UF2] $@
	@tools/elf2uf2 "$<" "$@"
	@echo Successfully built project &&echo.

%.lst: %.elf
	@echo [LST] $@
	@"$(OBJDUMP)" $(OBJDUMP_FLAGS) "$<" > "$@"

%.sym: %.elf
	@echo [SYM] $@
	@"$(NM)" $(NM_FLAGS) "$<" > "$@"

%.hex: %.elf
	@echo [HEX] $@
	@"$(OBJCOPY)" -O ihex -R.eeprom "$<" "$@"

%.bin: %.elf
	@echo [BIN] $@
	@"$(OBJCOPY)" -O binary "$<" "$@"

# Cleaning --------------------------------------------------------------------
cleanUser:
	@echo "$(COL_LINK)"Cleaning user binaries..."$(COL_RESET)"
	@if [ -f $(USR_BIN) ]; then rm "$(USR_BIN)"; fi
	@if [ -f "$(TARGET_LST)" ]; then rm $(TARGET_LST); fi

cleanCore:
	@echo "$(COL_LINK)"Cleaning core binaries..."$(COL_RESET)"
	@if [ -f $(CORE_BIN) ]; then rm "$(CORE_BIN)"; fi
	@if [ -f $(CORE_LIB) ]; then rm  $(CORE_LIB); fi

cleanKernel:
	@echo "$(COL_LINK)"Cleaning kernel binaries..."$(COL_RESET)"
	@if [ -f $(KERNEL_BIN) ]; then rm "$(KERNEL_BIN)"; fi
	@if [ -f $(KERNEL_LIB) ]; then rm  $(KERNEL_LIB); fi

cleanLib:
	@echo "$(COL_LINK)"Cleaning user library binaries..."$(COL_RESET)"
	@if [ -f $(LIB_BIN) ]; then rm "$(LIB_BIN)"; fi

# compiler generated dependency info ------------------------------------------
-include $(CORE_OBJ:.o=.d)
-include $(KERNEL_OBJ:.o=.d)
-include $(USR_OBJ:.o=.d)
-include $(LIB_OBJ:.o=.d)