
# export PATH="C:\Users\Michael\Documents\Personal\Programing\Embedded Development\OSAB\OSAB-RP2040-Kernel\tools\arm\bin":$PATH
SHELL := cmd.exe
export SHELL


BASE := cd C:/Users/Michael/Documents/Personal/Programing/Embedded\ Development/RP2040-Blink-UART &&

# Name of the outputted binary files (<TARGET_NAME>.elf, <TARGET_NAME>.lst, etc)
TARGET_NAME := blink

LINKER := rp2040.ld
MKUF2  := mkuf2

# Compiler Optimization Level -------------------------------------------------
# -O<Opt> (Ex: Faster = -O2)
# Debug:   g
# Default: 0
# Fast:    1
# Faster:  2 (Recommended Default)
# Fastest: 3
# Small Code:    s (Enables all -O2 optimizations except those that often increase code size)
# Smallest Code: z (Optimize aggressively for size rather than speed. Similar to -Os)
# OPT := 3
# OPT := 2
# OPT := 1
# OPT := 0
OPT := g
# OPT := s

# -Og 12856 1296
# -O0 17580 1408
# -O1 12832 1296
# -O2 12652 1296
# -O3  6904 1232
# -Os 12500 1296

# -O3                           6904 1232
# -O3 with move-loop-invariants 6884 1232
# -O3 with gcse                 6920 1232


# Shared Libraries ------------------------------------------------------------
# For if you have a folder of libraries that you share across many projects
LIBS_SHARED_BASE :=
LIBS_SHARED      := 
# Project Specific Libraries --------------------------------------------------
LIBS_LOCAL_BASE  := libs
LIBS_LOCAL       :=

OBJDUMP_FLAGS    := -d -C #-S -r
# -d: Disassemble
# -r: Shows symbol names on relocations
# -R: Shows dynamic-linking relocations / symbol names (useful on shared libraries)
# -C: Demangles C++ symbol names
# -S: Interleave source lines with disassembly. Implies -d. (--no-show-raw-insn: don't show raw instructions)
# -w: "Wide" mode: don't line-wrap the machine-code bytes
# -Mintel: use GAS/binutils MASM-like .intel_syntax noprefix syntax instead of AT&T


#******************************************************************************
# Flags and Defines
#******************************************************************************
FLAGS_CPU := -mthumb -mcpu=cortex-m0plus -mabi=aapcs #-mfloat-abi=hard -mfpu=fpv5-d16
FLAGS_OPT := -O$(OPT) -fno-inline #-fno-gcse #-fno-move-loop-invariants
FLAGS_COM := -g -Wall -Wextra -Wno-unused-parameter -Wunsafe-loop-optimizations -Wstack-usage=256 -ffunction-sections -fdata-sections -MMD -ffixed-r9 -nostdlib #--verbose
FLAGS_LSP := 

FLAGS_S   := #-x assembler-with-cpp
FLAGS_C   := #-std=gnu11
FLAGS_CPP := -std=gnu++14 -fno-exceptions -fpermissive -fno-rtti -felide-constructors -Wno-error=narrowing #-fno-threadsafe-statics
FLAGS_LD  := -Wl,--print-memory-usage,--gc-sections -T$(LINKER) --specs=nano.specs --specs=nosys.specs -nostartfiles
# FLAGS_LD  += -Wl,--specs=rdimon.specs # Enable rdimon.specs for debugging (Enables semihosting which allows you to use system calls and printf by passing them to the host computer via the debugger)
# --specs=nano.specs: use newlib-nano
# --specs=nosys.specs: defines that system calls should be implemented as stubs that return errors when called
# (--relax) (-fpic = position independent code)

### -01
# FLAGS_COM += -fauto-inc-dec -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fguess-branch-probability -fif-conversion2 -fif-conversion -fipa-pure-const -fipa-reference -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time
# FLAGS_COM += -fauto-inc-dec -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fguess-branch-probability -fif-conversion2 -fif-conversion -fipa-pure-const -fipa-reference                         -ftree-ccp -ftree-ch -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time        -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fforward-propagate -finline-functions-called-once -fipa-profile -fmerge-constants -fshrink-wrap -fsplit-wide-types -ftree-bit-ccp -fssa-phiopt -ftree-copy-prop -ftree-sink -ftree-slsr    # -fmove-loop-invariants
### -02
# FLAGS_COM += -fthread-jumps -falign-functions -falign-jumps -falign-loops  -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fexpensive-optimizations -fgcse-lm -finline-small-functions -findirect-inlining -fipa-sra -foptimize-sibling-calls -fpeephole2 -fregmove -freorder-blocks -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstrict-aliasing -fstrict-overflow -ftree-switch-conversion -ftree-pre -ftree-vrp #-fgcse
# FLAGS_COM += -fthread-jumps -falign-functions -falign-jumps -falign-loops  -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fexpensive-optimizations -fgcse-lm -finline-small-functions -findirect-inlining -fipa-sra -foptimize-sibling-calls -fpeephole2           -freorder-blocks -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstrict-aliasing -fstrict-overflow -ftree-switch-conversion -ftree-pre -ftree-vrp       -fdevirtualize -fdevirtualize-speculatively -fhoist-adjacent-loads -fipa-cp -fipa-cp-alignment -fipa-icf -fisolate-erroneous-paths-dereference -flra-remat -foptimize-strlen -fpartial-inlining    -ftree-builtin-call-dce -ftree-tail-merge -fipa-ra #-fgcse    # not supported by this arch: -freorder-blocks-and-partition
### -02 -fno-gcse -fno-move-loop-invariants
# FLAGS_COM += -fauto-inc-dec -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fguess-branch-probability -fif-conversion2 -fif-conversion -fipa-pure-const -fipa-reference                         -ftree-ccp -ftree-ch -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time        -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fforward-propagate -finline-functions-called-once -fipa-profile -fmerge-constants -fshrink-wrap -fsplit-wide-types -ftree-bit-ccp -fssa-phiopt -ftree-copy-prop -ftree-sink -ftree-slsr    # -fmove-loop-invariants
# FLAGS_COM += -fthread-jumps -falign-functions -falign-jumps -falign-loops  -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fexpensive-optimizations -fgcse-lm -finline-small-functions -findirect-inlining -fipa-sra -foptimize-sibling-calls -fpeephole2           -freorder-blocks -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstrict-aliasing -fstrict-overflow -ftree-switch-conversion -ftree-pre -ftree-vrp       -fdevirtualize -fdevirtualize-speculatively -fhoist-adjacent-loads -fipa-cp -fipa-cp-alignment -fipa-icf -fisolate-erroneous-paths-dereference -flra-remat -foptimize-strlen -fpartial-inlining    -ftree-builtin-call-dce -ftree-tail-merge -fipa-ra #-fgcse    # not supported by this arch: -freorder-blocks-and-partition
# FLAGS_KERNEL := -Og -fauto-inc-dec -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fguess-branch-probability -fif-conversion2 -fif-conversion -fipa-pure-const -fipa-reference                         -ftree-ccp -ftree-ch -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time        -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fforward-propagate -finline-functions-called-once -fipa-profile -fmerge-constants -fshrink-wrap -fsplit-wide-types -ftree-bit-ccp -fssa-phiopt -ftree-copy-prop -ftree-sink -ftree-slsr    # -fmove-loop-invariants
# FLAGS_KERNEL += -fthread-jumps -falign-functions -falign-jumps -falign-loops  -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps  -fcse-skip-blocks -fdelete-null-pointer-checks -fexpensive-optimizations -fgcse-lm -finline-small-functions -findirect-inlining -fipa-sra -foptimize-sibling-calls -fpeephole2           -freorder-blocks -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstrict-aliasing -fstrict-overflow -ftree-switch-conversion -ftree-pre -ftree-vrp       -fdevirtualize -fdevirtualize-speculatively -fhoist-adjacent-loads -fipa-cp -fipa-cp-alignment -fipa-icf -fisolate-erroneous-paths-dereference -flra-remat -foptimize-strlen -fpartial-inlining    -ftree-builtin-call-dce -ftree-tail-merge -fipa-ra #-fgcse    # not supported by this arch: -freorder-blocks-and-partition
# FLAGS_KERNEL := -fno-gcse -fno-move-loop-invariants

LIBS      := -lm -lstdc++ #-larm_cortexM7lfsp_math

DEFINES   := -DLOW_REGS_ONLY=1 #-D$(MCU_DEF) -D$(BOARD_DEF)

CPP_FLAGS := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_CPP)
C_FLAGS   := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_C)
S_FLAGS   := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_S)
LD_FLAGS  := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_LSP) $(FLAGS_LD)
AR_FLAGS  := rcs
NM_FLAGS  := --numeric-sort --defined-only --demangle --print-size


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


#******************************************************************************
# BINARIES
#******************************************************************************
GCC_BASE = tools\arm\bin
# GCC_BASE = C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin
CC              := $(GCC_BASE)/arm-none-eabi-gcc
CXX             := $(GCC_BASE)/arm-none-eabi-g++
AR              := $(GCC_BASE)/arm-none-eabi-gcc-ar
NM              := $(GCC_BASE)/arm-none-eabi-gcc-nm
OBJDUMP         := $(GCC_BASE)/arm-none-eabi-objdump
OBJCOPY         := $(GCC_BASE)/arm-none-eabi-objcopy
# CC              := arm-none-eabi-gcc
# CXX             := arm-none-eabi-g++
# AR              := arm-none-eabi-gcc-ar
# NM              := arm-none-eabi-gcc-nm
# OBJDUMP         := arm-none-eabi-objdump
# OBJCOPY         := arm-none-eabi-objcopy
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
INCLUDE         := -I$(USR_SRC) -I$(CORE_SRC) -I$(KERNEL_SRC) #-include kernel/common.h
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
	@echoLIB [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.cpp.o: $(LIBS_SHARED_BASE)/%.cpp
	@echoLIB [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.c.o: $(LIBS_SHARED_BASE)/%.c
	@echo LIB [CC]  $(notdir $<)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -c "$<" -o "$@"

# Local Libraries -------------------------------------------------------------
$(LIB_BIN)/%.s.o: $(LIBS_LOCAL_BASE)/%.S
	@echoLIB [ASM] $(notdir $<)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.cpp.o: $(LIBS_LOCAL_BASE)/%.cpp
	@echoLIB [CPP] $(notdir $<)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -c "$<" -o "$@"

$(LIB_BIN)/%.c.o: $(LIBS_LOCAL_BASE)/%.c
	@echoLIB [CC]  $(notdir $<)
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
$(TARGET_ELF): $(USR_OBJ) $(LIB_OBJ) $(CORE_OBJ) $(KERNEL_LIB) #$(CORE_LIB)
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
	@echo Cleaning user binaries...
	@if exist $(USR_BIN) rd /s/q "$(USR_BIN)"
	@if exist $(TARGET_LST) del $(subst /,\,$(TARGET_LST))

cleanCore:
	@echo Cleaning core binaries...
	@if exist $(CORE_BIN) rd /s/q "$(CORE_BIN)"
	@if exist $(CORE_LIB) del  $(subst /,\,$(CORE_LIB))

cleanKernel:
	@echo Cleaning kernel binaries...
	@if exist $(KERNEL_BIN) rd /s/q "$(KERNEL_BIN)"
	@if exist $(KERNEL_LIB) del  $(subst /,\,$(KERNEL_LIB))

cleanLib:
	@echo Cleaning user library binaries...
	@if exist $(LIB_BIN) rd /s/q "$(LIB_BIN)"

# compiler generated dependency info ------------------------------------------
-include $(CORE_OBJ:.o=.d)
-include $(KERNEL_OBJ:.o=.d)
-include $(USR_OBJ:.o=.d)
-include $(LIB_OBJ:.o=.d)