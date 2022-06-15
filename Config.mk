
# Name of the outputted binary files (<TARGET_NAME>.elf, <TARGET_NAME>.lst, etc)
TARGET_NAME := blink

# File path of the linker script
LINKER := rp2040.ld
MKUF2  := mkuf2


# Shared Libraries ------------------------------------------------------------
# For if you have a folder of libraries that you share across many projects
LIBS_SHARED_BASE :=
LIBS_SHARED      := # List the folder names of your shared libraries here
# Project Specific Libraries --------------------------------------------------
LIBS_LOCAL_BASE  := libs
LIBS_LOCAL       := # List the folder names of your local libraries here



# Compiler Optimization Level -------------------------------------------------
# -O<Opt> (Ex: Faster = -O2)
# Debug:   g
# Default: 0
# Fast:    1
# Faster:  2 (Recommended Default)
# Fastest: 3
# Small Code:    s (Enables all -O2 optimizations except those that often increase code size)
# Smallest Code: z (Optimize aggressively for size rather than speed. Similar to -Os)
# --- Uncomment One Line ---
# OPT := 3
OPT := 2
# OPT := 1
# OPT := 0
# OPT := g
# OPT := s


# Additional Symbol Defines -------------------------------------------------
CFG_DEFINES := 



# Object Dump Settings (*.lst output file) ------------------------------------
OBJDUMP_FLAGS := -d -C -r -S
# -d: Disassemble
# -r: Shows symbol names on relocations
# -R: Shows dynamic-linking relocations / symbol names (useful on shared libraries)
# -C: Demangles C++ symbol names
# -S: Interleave source lines with disassembly. Implies -d. (--no-show-raw-insn: don't show raw instructions)
# -w: "Wide" mode: don't line-wrap the machine-code bytes
# -Mintel: use GAS/binutils MASM-like .intel_syntax noprefix syntax instead of AT&T