
# Top Level Folders


## /bin/
Where compiled binaries and outputs go

## /tools/
Contains Make and the GNU C/C++ compiler

## /hardware/
Contains hardware-specific files

## /kernel/
Contains the kernel

## /lib/
Contains libraries to be included for the user application

## /src/
Contains the user application


# Sub-Level Folders


## /hardware/arch

**Sub-Folders:**
- ./arm/`<processor architecture>`/common/
- ./arm/`<processor architecture>`/`<processor core>`/
Contains header files that define regesters (and more) for the processor architectures or cores (Ex. ARMv6-M, ARMv7E-M, Cortex-M0+, Cortex-M4). Each architecture is contained in its own folder (Ex. "/armv6-m", "/core_cm0"). Cores are in the folder of the architecture they belong to.

**Note:* Currently the kernel only supports the ARMv6-M architecture.*

## /hardware/packages/`<package>`/
- 
- ./variants/`<variant>`/
<!-- - ./core/`<core>`/ -->
Packages contain multiple variants that are related. They could from the same vendor or have the same/similar hardware, mcu, or pinouts. Variants are organized into
Contains header files that define regesters (and more) for the board or microcontroller (Ex. Teensy 4.1, ). For example, you might put vendor files here or a GPIO library for that mcu/board.

<!-- 
## /hardware/---
Contains header files that define regesters (and more) for the microcontroller (Ex. RP2040, STM32-L0). For example, you might put vendor files here.
-->