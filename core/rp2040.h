#ifndef __RP2040_H
#define __RP2040_H

#include <kernel.h>

// #ifdef __cplusplus
// extern "C" {
// #endif



#define F_XOSC (12*1000000) // 12 MHz External Crystal Oscillator
// #define F_CPU (120*1000000) // (120 MHz) This is the target core clock frequency. This my not be the actual clock frequency before the clock is configured.



// Read/Write Registers Using Pointers
#define ADDR_RD(addr)               *((volatile uint32_t*)(addr))
#define ADDR_WR(addr, data)         *((volatile uint32_t*)(addr)) = (data)


// ROM
#define ROM_BASE                  0x00000000
// FLASH
#define XIP_BASE                  0x10000000
#define XIP_NOALLOC_BASE          0x11000000
#define XIP_NOCACHE_BASE          0x12000000
#define XIP_NOCACHE_NOALLOC_BASE  0x13000000
#define XIP_CTRL_BASE             0x14000000
#define XIP_SRAM_BASE             0x15000000
#define XIP_SRAM_END              0x15004000
#define XIP_SSI_BASE              0x18000000
// SRAM
#define SRAM_BASE                 0x20000000
#define SRAM_STRIPED_BASE         0x20000000
#define SRAM_STRIPED_END          0x20040000
#define SRAM4_BASE                0x20040000
#define SRAM5_BASE                0x20041000
#define SRAM_END                  0x20042000
#define SRAM0_BASE                0x21000000
#define SRAM1_BASE                0x21010000
#define SRAM2_BASE                0x21020000
#define SRAM3_BASE                0x21030000
// APB
#define SYSINFO_BASE              0x40000000
#define SYSCFG_BASE               0x40004000
#define CLOCKS_BASE               0x40008000
#define RESETS_BASE               0x4000C000
#define PSM_BASE                  0x40010000
#define IO_BANK0_BASE             0x40014000
#define IO_QSPI_BASE              0x40018000
#define PADS_BANK0_BASE           0x4001C000
#define PADS_QSPI_BASE            0x40020000
#define XOSC_BASE                 0x40024000
#define PLL_SYS_BASE              0x40028000
#define PLL_USB_BASE              0x4002C000
#define BUSCTRL_BASE              0x40030000
#define UART0_BASE                0x40034000
#define UART1_BASE                0x40038000
#define SPI0_BASE                 0x4003C000
#define SPI1_BASE                 0x40040000
#define I2C0_BASE                 0x40044000
#define I2C1_BASE                 0x40048000
#define ADC_BASE                  0x4004C000
#define PWM_BASE                  0x40050000
#define TIMER_BASE                0x40054000
#define WATCHDOG_BASE             0x40058000
#define RTC_BASE                  0x4005C000
#define ROSC_BASE                 0x40060000
#define VREG_AND_CHIP_RESET_BASE  0x40064000
#define TBMAN_BASE                0x4006C000
// AHB
#define DMA_BASE                  0x50000000
#define USBCTRL_BASE              0x50100000
#define USBCTRL_DPRAM_BASE        0x50100000
#define USBCTRL_REGS_BASE         0x50110000
#define PIO0_BASE                 0x50200000
#define PIO1_BASE                 0x50300000
#define XIP_AUX_BASE              0x50400000
// SIO
#define SIO_BASE                  0xD0000000



#define CLOCKS ((volatile struct clocks_hw*)CLOCKS_BASE)
struct clocks_hw {
	uint32_t NA[12];              // (0x0000 - 0x002c) 
	uint32_t CLK_REF_CTRL;        // (0x0030) Control and Status Register
	uint32_t CLK_REF_DIV;         // (0x0034) 
	uint32_t CLK_REF_SELECTED;    // (0x0038) 
	uint32_t CLK_SYS_CTRL;        // (0x003c) 
	uint32_t CLK_SYS_DIV;         // (0x0040) 
	uint32_t CLK_SYS_SELECTED;    // (0x0044) 
	uint32_t CLK_PERI_CTRL;       // (0x0048) 
	uint32_t RESERVED;            // (0x004c) 
	uint32_t CLK_PERI_SELECTED;   // (0x0050) 
	uint32_t CLK_USB_CTRL;        // (0x0054) 
	uint32_t CLK_USB_DIV;         // (0x0058) 
	uint32_t CLK_USB_SELECTED;    // (0x005c) 
	uint32_t CLK_ADC_CTRL;        // (0x0060) 
	uint32_t CLK_ADC_DIV;         // (0x0064) 
	uint32_t CLK_RTC_CTRL;        // (0x006c) 
	uint32_t CLK_RTC_DIV;         // (0x0070) 
};

#define PLL_SYS ((volatile struct pll_hw*)PLL_SYS_BASE)
#define PLL_USB ((volatile struct pll_hw*)PLL_USB_BASE)
struct pll_hw {
	uint32_t CS;        // (0x0000) Control and Status Register
	uint32_t PWR;       // (0x0004) Controls the PLL power modes
	uint32_t FBDIV_INT; // (0x0008) Feedback divisor
	uint32_t PRIM;      // (0x000c) Controls the PLL post dividers for the primary output (output is driven from VCO divided by postdiv1*postdiv2)
};

typedef enum {
	CLOCKS_CLK_REF_CTRL_SRC_rosc_clksrc_ph = 0,     // rosc_clksrc_ph
	CLOCKS_CLK_REF_CTRL_SRC_clksrc_clk_ref_aux = 1, // clksrc_clk_ref_aux
	CLOCKS_CLK_REF_CTRL_SRC_xosc_clksrc  = 2,       // xosc_clksrc
} CLOCKS_CLK_REF_CTRL_SRC_Enum;

typedef enum {
	CLOCKS_CLK_SYS_CTRL_SRC_clk_ref = 0,            // clk_ref
	CLOCKS_CLK_SYS_CTRL_SRC_clksrc_clk_sys_aux = 1, // clksrc_clk_sys_aux
} CLOCKS_CLK_SYS_CTRL_SRC_Enum;

typedef enum {
	CLOCKS_CLK_SYS_CTRL_AUXSRC_clksrc_pll_sys = 0, // clksrc_pll_sys
	CLOCKS_CLK_SYS_CTRL_AUXSRC_clksrc_pll_usb = 1, // clksrc_pll_usb
	CLOCKS_CLK_SYS_CTRL_AUXSRC_rosc_clksrc = 2,    // rosc_clksrc
	CLOCKS_CLK_SYS_CTRL_AUXSRC_xosc_clksrc = 3,    // xosc_clksrc
	CLOCKS_CLK_SYS_CTRL_AUXSRC_clksrc_gpin0 = 4,   // clksrc_gpin0
	CLOCKS_CLK_SYS_CTRL_AUXSRC_clksrc_gpin1 = 5,   // clksrc_gpin1
} CLOCKS_CLK_SYS_CTRL_AUXSRC_Enum;

typedef enum {
	CLOCKS_CLK_PERI_CTRL_AUXSRC_clk_sys = 0,        // clk_sys
	CLOCKS_CLK_PERI_CTRL_AUXSRC_clksrc_pll_sys = 1, // clksrc_pll_sys
	CLOCKS_CLK_PERI_CTRL_AUXSRC_clksrc_pll_usb = 2, // clksrc_pll_usb
	CLOCKS_CLK_PERI_CTRL_AUXSRC_rosc_clksrc_ph = 3, // rosc_clksrc_ph
	CLOCKS_CLK_PERI_CTRL_AUXSRC_xosc_clksrc = 4,    // xosc_clksrc
	CLOCKS_CLK_PERI_CTRL_AUXSRC_clksrc_gpin0 = 5,   // clksrc_gpin0
	CLOCKS_CLK_PERI_CTRL_AUXSRC_clksrc_gpin1 = 6,   // clksrc_gpin1
} CLOCKS_CLK_PERI_CTRL_AUXSRC_Enum;

typedef enum {
	CLOCKS_CLK_USB_CTRL_AUXSRC_clksrc_pll_usb = 0, // clksrc_pll_usb
	CLOCKS_CLK_USB_CTRL_AUXSRC_clksrc_pll_sys = 1, // clksrc_pll_sys
	CLOCKS_CLK_USB_CTRL_AUXSRC_rosc_clksrc_ph = 2, // rosc_clksrc_ph
	CLOCKS_CLK_USB_CTRL_AUXSRC_xosc_clksrc = 3,    // xosc_clksrc
	CLOCKS_CLK_USB_CTRL_AUXSRC_clksrc_gpin0 = 4,   // clksrc_gpin0
	CLOCKS_CLK_USB_CTRL_AUXSRC_clksrc_gpin1 = 5,   // clksrc_gpin1
} CLOCKS_CLK_USB_CTRL_AUXSRC_Enum;

typedef enum {
	CLOCKS_CLK_ADC_CTRL_AUXSRC_clksrc_pll_usb = 0, // clksrc_pll_usb
	CLOCKS_CLK_ADC_CTRL_AUXSRC_clksrc_pll_sys = 1, // clksrc_pll_sys
	CLOCKS_CLK_ADC_CTRL_AUXSRC_rosc_clksrc_ph = 2, // rosc_clksrc_ph
	CLOCKS_CLK_ADC_CTRL_AUXSRC_xosc_clksrc = 3,    // xosc_clksrc
	CLOCKS_CLK_ADC_CTRL_AUXSRC_clksrc_gpin0 = 4,   // clksrc_gpin0
	CLOCKS_CLK_ADC_CTRL_AUXSRC_clksrc_gpin1 = 5,   // clksrc_gpin1
} CLOCKS_CLK_ADC_CTRL_AUXSRC_Enum;

typedef enum {
	CLOCKS_CLK_RTC_CTRL_AUXSRC_clksrc_pll_usb = 0, // clksrc_pll_usb
	CLOCKS_CLK_RTC_CTRL_AUXSRC_clksrc_pll_sys = 1, // clksrc_pll_sys
	CLOCKS_CLK_RTC_CTRL_AUXSRC_rosc_clksrc_ph = 2, // rosc_clksrc_ph
	CLOCKS_CLK_RTC_CTRL_AUXSRC_xosc_clksrc = 3,    // xosc_clksrc
	CLOCKS_CLK_RTC_CTRL_AUXSRC_clksrc_gpin0 = 4,   // clksrc_gpin0
	CLOCKS_CLK_RTC_CTRL_AUXSRC_clksrc_gpin1 = 5,   // clksrc_gpin1
} CLOCKS_CLK_RTC_CTRL_AUXSRC_Enum;

/** REGISTERS: CLOCKS_CLK
 * CLOCKS_CLK_REF
 * CLOCKS_CLK_SYS
 * CLOCKS_CLK_PERI
 * CLOCKS_CLK_USB
 * CLOCKS_CLK_ADC
 * CLOCKS_CLK_RTC
 */

// CLOCKS_CLK_REF Register
#define CLOCKS_CLK_REF_CTRL_SRC_Pos 0
#define CLOCKS_CLK_REF_CTRL_SRC_Msk (1 << CLOCKS_CLK_REF_CTRL_SRC_Pos)
// CLOCKS_CLK_SYS Register
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos 5
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_Msk (1 << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_SRC_Pos 0
#define CLOCKS_CLK_SYS_CTRL_SRC_Msk (1 << CLOCKS_CLK_SYS_CTRL_SRC_Pos)
// CLOCKS_CLK_PERI Register
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos 5
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_Msk (1 << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_ENABLE_Pos 11
#define CLOCKS_CLK_PERI_CTRL_ENABLE_Msk (1 << CLOCKS_CLK_PERI_CTRL_ENABLE_Pos)
// CLOCKS_CLK_USB Register
#define CLOCKS_CLK_USB_CTRL_ENABLE_Pos 11
#define CLOCKS_CLK_USB_CTRL_ENABLE_Msk (1 << CLOCKS_CLK_USB_CTRL_ENABLE_Pos)
#define CLOCKS_CLK_USB_CTRL_AUXSRC_Pos 5
#define CLOCKS_CLK_USB_CTRL_AUXSRC_Msk (1 << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)
// CLOCKS_CLK_ADC Register
#define CLOCKS_CLK_ADC_CTRL_ENABLE_Pos 11
#define CLOCKS_CLK_ADC_CTRL_ENABLE_Msk (1 << CLOCKS_CLK_ADC_CTRL_ENABLE_Pos)
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos 5
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_Msk (1 << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
// CLOCKS_CLK_RTC Register
#define CLOCKS_CLK_RTC_DIV_INT_Pos 8
#define CLOCKS_CLK_RTC_DIV_INT_Msk (1 << CLOCKS_CLK_RTC_DIV_INT_Pos)
#define CLOCKS_CLK_RTC_CTRL_ENABLE_Pos 11
#define CLOCKS_CLK_RTC_CTRL_ENABLE_Msk (1 << CLOCKS_CLK_RTC_CTRL_ENABLE_Pos)


struct xosc_hw {
	uint32_t ctrl;
	uint32_t status;
	uint32_t dormant;
	uint32_t startup;
	uint32_t count;
};

#define xosc ((volatile struct xosc_hw*)XOSC_BASE)

#define XOSC_CTRL_ENABLE_Pos           12
#define XOSC_CTRL_ENABLE_Msk           (1 << XOSC_CTRL_ENABLE_Pos)
#define XOSC_STATUS_STABLE_Pos         31
#define XOSC_STATUS_STABLE_Msk         (1 << XOSC_STATUS_STABLE_Pos)
#define XOSC_STATUS_BADWRITE_Pos       24
#define XOSC_STATUS_BADWRITE_Msk       (1 << XOSC_STATUS_BADWRITE_Pos)
#define XOSC_STATUS_ENABLED_Pos        12
#define XOSC_STATUS_ENABLED_Msk        (1 << XOSC_STATUS_ENABLED_Pos)

// typedef struct { // (0x40024000) XOSC Structure
// 	// (0x000) Crystal Oscillator Control
// 	union {
// 		uint32_t CTRL;
// 		struct {
// 			uint32_t FREQ_RANGE : 12; // [11,0] Frequency range. This resets to 0xAA0 and cannot be changed.
// 			uint32_t ENABLE     : 12; // [23,12] On power-up this field is initialised to DISABLE and the chip runs from the ROSC. If the chip has subsequently been programmed to run from the XOSC then setting this field to DISABLE may lock-up the chip. If this is a concern then run the clk_ref from the ROSC and enable the clk_sys RESUS feature. The 12-bit code is intended to give some protection against accidental writes. An invalid setting will enable the oscillator.
// 			uint32_t            : 8;  // Reserved
// 		} CTRL_b;
// 	} ;
// 	// (0x004) Crystal Oscillator Status
// 	union {
// 		uint32_t STATUS;
// 		struct {
// 			uint32_t FREQ_RANGE : 2;  // [1,0] The current frequency range setting, always reads 0
// 			uint32_t            : 10; // Reserved
// 			uint32_t ENABLED    : 1;  // [12,12] Oscillator is enabled but not necessarily running and stable, resets to 0
// 			uint32_t            : 11; // Reserved
// 			uint32_t BADWRITE   : 1;  // [24,24] An invalid value has been written to CTRL_ENABLE or CTRL_FREQ_RANGE or DORMANT
// 			uint32_t            : 6;  // Reserved
// 			uint32_t STABLE     : 1;  // [31,31] Oscillator is running and stable
// 		} STATUS_b;
// 	} ;
// 	// (0x008) Crystal Oscillator pause control This is used to save power by pausing the XOSC On power-up this field is initialised to WAKE An invalid write will also select WAKE WARNING: stop the PLLs before selecting dormant mode WARNING: setup the irq before selecting dormant mode
// 	uint32_t DORMANT;
// 	// (0x00C) Controls the startup delay of the XOSC
// 	union {
// 		uint32_t STARTUP; 
// 		struct {
// 			uint32_t DELAY      : 14; // [13,0] in multiples of 256*xtal_period. The reset value of 0xc4 corresponds to approx 50 000 cycles.
// 			uint32_t            : 6;  // Reserved
// 			uint32_t X4         : 1;  // [20,20] Multiplies the startup_delay by 4. This is of little value to the user given that the delay can be programmed directly.
// 			uint32_t            : 11; // Reserved
// 		} STARTUP_b;
// 	} ;
// 	uint32_t RESERVED[3];
// 	// (0x01C) A down counter running at the xosc frequency which counts to zero and stops. To start the counter write a non-zero value. Can be used for short software pauses when setting up time sensitive hardware.
// 	union {
// 		uint32_t COUNT;
// 		struct {
// 			uint32_t COUNT      : 8;  // [7,0] COUNT
// 			uint32_t            : 24; // Reserved
// 		} COUNT_b;
// 	} ;
// } XOSC_Type; // Size = 32 (0x20)



/** REGISTERS: PLL_SYS
 * PLL_SYS_PRIM
 * PLL_SYS_PWR
 * PLL_SYS_CS
 */

// PLL_SYS_PRIM Register
#define PLL_SYS_PRIM_POSTDIV1_Pos 16
#define PLL_SYS_PRIM_POSTDIV2_Pos 12
// PLL_SYS_PWR Register
#define PLL_SYS_PWR_VCOPD_Pos 5
#define PLL_SYS_PWR_VCOPD_Msk (1 << 5) // 0x20
#define PLL_SYS_PWR_POSTDIVPD_Pos 3
#define PLL_SYS_PWR_POSTDIVPD_Msk (1 << PLL_SYS_PWR_POSTDIVPD_Pos) // 0x8
#define PLL_SYS_PWR_PD_Pos    0
#define PLL_SYS_PWR_PD_Msk    (1 << 0) // 0x1
// PLL_SYS_CS Register
#define PLL_SYS_CS_REFDIV_Pos 0
#define PLL_SYS_CS_LOCK_Pos 31
#define PLL_SYS_CS_LOCK_Msk (1 << PLL_SYS_CS_LOCK_Pos)



// #ifdef __cplusplus
// }; /** END: __cplusplus */
// #endif


#endif /** END: __RP2040_H */