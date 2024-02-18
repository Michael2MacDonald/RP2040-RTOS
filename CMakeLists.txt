cmake_minimum_required(VERSION 3.16)

# include(arm-none-eabi.cmake)
set(CMAKE_TOOLCHAIN_FILE arm-none-eabi.cmake)

set(PROJECT_NAME RP2040-Blink-UART)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Inlcude device configurations
#include(device_config.cmake)

set(LINKER "\"${CMAKE_CURRENT_SOURCE_DIR}\"/rp2040.ld")


# Compiler Optimization Level -------------------------------------------------
# -O<Opt> (Ex: Faster = -O2)
# Debug:   g
# Default: 0
# Fast:    1
# Faster:  2 (Recommended Default)
# Fastest: 3
# Small Code:    s (Enables all -O2 optimizations except those that often increase code size)
# Smallest Code: z (Optimize aggressively for size rather than speed. Similar to -Os)
set(OPT 2)

set(F_CPU "(120*1000000)") # (120 MHz) This is the target core clock frequency. This my not be the actual clock frequency before the clock is configured.
# # USB device vendor ID
# VID_USB := 0x239A # 0x239A = Adafruit
# # USB device product ID
# PID_USB := 0x80f1 # 0x80f1 = Adafruit Feather RP2040
# # USB version that the device conforms to
# BCD_USB := 0x0200 # 0x0200 = USB 2.0
# USB_DEFINES := -DUSB_VID=$(VID_USB) -DUSB_PID=$(PID_USB) -DUSB_BCD=$(BCD_USB)
set(BOARD_DEFINES "-DF_CPU=\"${F_CPU}\"") #$(USB_DEFINES) #-D$(MCU_DEF) -D$(BOARD_DEF)
set(CFG_DEFINES "")
set(DEFINES "${CFG_DEFINES} ${BOARD_DEFINES} -DLOW_REGS_ONLY=1")

# set(LIBS "-lm -lstdc++")

set(FLAGS_CPU "-mthumb -mcpu=cortex-m0plus -mabi=aapcs") #-mfloat-abi=hard -mfpu=fpv5-d16
set(FLAGS_OPT "-O${OPT}")
set(FLAGS_COM "-g -MMD -Wall -Wextra -Wno-unused-parameter -Wunsafe-loop-optimizations -Wstack-usage=256 -fstack-usage -ffunction-sections -fdata-sections") #--verbose -save-temps
set(FLAGS_LSP "")

set(FLAGS_S "-x assembler-with-cpp")
set(FLAGS_C "-std=gnu11")
set(FLAGS_CPP "-std=gnu++14 -fno-exceptions -fpermissive -fno-rtti -felide-constructors -Wno-error=narrowing")
set(FLAGS_LD "-T${LINKER} -Wl,--print-memory-usage,--gc-sections --specs=nano.specs --specs=nosys.specs")

set(CPP_FLAGS "${FLAGS_CPU} ${FLAGS_OPT} ${FLAGS_COM} ${DEFINES} ${FLAGS_CPP}")
set(C_FLAGS   "${FLAGS_CPU} ${FLAGS_OPT} ${FLAGS_COM} ${DEFINES} ${FLAGS_C}")
set(S_FLAGS   "${FLAGS_CPU} ${FLAGS_OPT} ${FLAGS_COM} ${DEFINES} ${FLAGS_S}")
set(LD_FLAGS  "${FLAGS_CPU} ${FLAGS_OPT} ${FLAGS_LSP} ${FLAGS_LD}")
set(AR_FLAGS  "rcs")
set(NM_FLAGS  "--numeric-sort --defined-only --demangle --print-size")

set(CMAKE_EXE_LINKER_FLAGS "${LD_FLAGS}" CACHE INTERNAL "")

set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed")


set(CMAKE_C_FLAGS "${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CPP_FLAGS}")
set(CMAKE_ASM_FLAGS "${S_FLAGS}")


add_executable(${PROJECT_NAME}
	core/boot2_w25x10cl_4_padded_checksum.S
	core/resets.c
	core/clock.c
	core/gpio.c
	core/uart.c
	core/startup.c

	kernel/vectors.S
	kernel/vectors_core1.S
	kernel/enter_main.S
	kernel/PendSVHandler.S
	kernel/core1.c
	kernel/reset.S
	kernel/list.c
	kernel/cortex.c
	kernel/startup.c
	kernel/delay.c
	kernel/Scheduler.c
	kernel/Kernel.c

	src/itoa.c
	src/compass.c
	# src/print/Print.cpp

	src/main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC kernel core src src/print)
# target_link_libraries(${PROJECT_NAME} m stdc++)
target_link_libraries(${PROJECT_NAME} m)


