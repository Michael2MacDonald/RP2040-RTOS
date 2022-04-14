#pragma once



// Data Types
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef _Bool bool;

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
// ========== Cortex-M0+ internal peripherals ==========
#define PPB_BASE                  0xE0000000
// #define PPB_SCB2_BASE               0xE000E008 // System Control Block
#define SysTick_BASE            0xE000E010 // SysTick (Optional Peripheral; Not available on all devices)
// #define NVIC_BASE               0xE000E100 // Nested Vectored Interrupt Controller
#define SCB_BASE                0xE000ED00 // System Control Block
#define MPU_BASE                0xE000ED90 // Memory Protection Unit (Optional Peripheral; Not available on all devices)
#define NVIC_BASE               0xE000EF00 // Nested Vectored Interrupt Controller


// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000
// #define PPB_BASE                  0xE0000000



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




// struct systick_hw {
// 	uint32_t CTRL;                   // (0x000) (R/W) SysTick Control and Status Register
// 	uint32_t LOAD;                   // (0x004) (R/W) SysTick Reload Value Register
// 	uint32_t VAL;                    // (0x008) (R/W) SysTick Current Value Register
// 	uint32_t CALIB;                  // (0x00C) (R/ ) SysTick Calibration Register
// };
struct systick_hw {
	uint32_t CSR;     // (0x000) (R/W) SysTick Control and Status Register
	uint32_t RVR;     // (0x004) (R/W) SysTick Reload Value Register
	uint32_t CVR;     // (0x008) (R/W) SysTick Current Value Register
	uint32_t CALIB;   // (0x00C) (R/ ) SysTick Calibration Register
};

#define SysTick ((volatile struct systick_hw*)SysTick_BASE)

#define SysTick_CSR_COUNTFLAG_Pos         16U
#define SysTick_CSR_COUNTFLAG_Msk         (1UL << SysTick_CSR_COUNTFLAG_Pos)
// Clock Source
#define SysTick_CSR_CLKSOURCE_Pos          2U
#define SysTick_CSR_CLKSOURCE_Msk         (1UL << SysTick_CSR_CLKSOURCE_Pos)
// Enable Systick Interupt
#define SysTick_CSR_TICKINT_Pos            1U
#define SysTick_CSR_TICKINT_Msk           (1UL << SysTick_CSR_TICKINT_Pos)
// Enable
#define SysTick_CSR_ENABLE_Pos             0U
#define SysTick_CSR_ENABLE_Msk            (1UL /*<< SysTick_CTRL_ENABLE_Pos*/)
// Reload Value
#define SysTick_RVR_RELOAD_Pos             0U
#define SysTick_RVR_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)
// Current Value
#define SysTick_CVR_CURRENT_Pos             0U
#define SysTick_CVR_CURRENT_Msk            (0xFFFFFFUL /*<< SysTick_VAL_CURRENT_Pos*/)


#define NVIC_ISER   (PPB_BASE + 0xE100)
#define NVIC_ICER   (PPB_BASE + 0xE180)
#define NVIC_ISPR   (PPB_BASE + 0xE200)
#define NVIC_ICPR   (PPB_BASE + 0xE280)
#define NVIC_IPR0   (PPB_BASE + 0xE400)
#define NVIC_IPR1   (PPB_BASE + 0xE404)
#define NVIC_IPR2   (PPB_BASE + 0xE408)
#define NVIC_IPR3   (PPB_BASE + 0xE40C)
#define NVIC_IPR4   (PPB_BASE + 0xE410)
#define NVIC_IPR5   (PPB_BASE + 0xE414)
#define NVIC_IPR6   (PPB_BASE + 0xE418)
#define NVIC_IPR7   (PPB_BASE + 0xE41C)

struct nvic_hw {
	uint32_t ISER;               // (0x000) (R/W) Interrupt Set Enable Register
	uint32_t RESERVED0[31U];
	uint32_t ICER;               // (0x080) (R/W) Interrupt Clear Enable Register
	uint32_t RESERVED1[31U];
	uint32_t ISPR;               // (0x100) (R/W) Interrupt Set Pending Register
	uint32_t RESERVED2[31U];
	uint32_t ICPR;               // (0x180) (R/W) Interrupt Clear Pending Register
	uint32_t RESERVED3[31U];
	uint32_t RESERVED4[64U];
	uint32_t IP[8U];                 // (0x300) (R/W) Interrupt Priority Register
};

#define NVIC ((volatile struct nvic_hw*)NVIC_BASE)


struct scb_hw {
	uint32_t CPUID;        // (0x000) (R/ ) CPUID Base Register
	uint32_t ICSR;         // (0x004) (R/W) Interrupt Control and State Register
	uint32_t VTOR;         // (0x008) (R/W) Vector Table Offset Register
	uint32_t AIRCR;        // (0x00C) (R/W) Application Interrupt and Reset Control Register
	uint32_t SCR;          // (0x010) (R/W) System Control Register
	uint32_t CCR;          // (0x014) (R/W) Configuration Control Register
	uint32_t RESERVED1;
	uint32_t SHP[2U];      // (0x01C) (R/W) System Handlers Priority Registers. [0] is RESERVED */
	uint32_t SHCSR;        // (0x024) (R/W) System Handler Control and State Register */
};

#define SCB ((volatile struct scb_hw*)SCB_BASE)

/** BITS: [19:16] Architecture (Also called 'Constant'?)
 * 0xC60 = ARMv6-M
 */
#define SCB_CPUID_ARCHITECTURE_Pos         16U
#define SCB_CPUID_ARCHITECTURE_Msk         (0xFUL << SCB_CPUID_ARCHITECTURE_Pos)
/** BITS: [15:4] Partno
 * 0xC20 = Cortex-M0
 * 0xC60 = Cortex-M0+
 */
#define SCB_CPUID_PARTNO_Pos                4U
#define SCB_CPUID_PARTNO_Msk               (0xFFFUL << SCB_CPUID_PARTNO_Pos)


/* SCB Interrupt Control State Register Definitions */
#define SCB_ICSR_NMIPENDSET_Pos            31U                                            /*!< SCB ICSR: NMIPENDSET Position */
#define SCB_ICSR_NMIPENDSET_Msk            (1UL << SCB_ICSR_NMIPENDSET_Pos)               /*!< SCB ICSR: NMIPENDSET Mask */

#define SCB_ICSR_PENDSVSET_Pos             28U                                            /*!< SCB ICSR: PENDSVSET Position */
#define SCB_ICSR_PENDSVSET_Msk             (1UL << SCB_ICSR_PENDSVSET_Pos)                /*!< SCB ICSR: PENDSVSET Mask */

#define SCB_ICSR_PENDSVCLR_Pos             27U                                            /*!< SCB ICSR: PENDSVCLR Position */
#define SCB_ICSR_PENDSVCLR_Msk             (1UL << SCB_ICSR_PENDSVCLR_Pos)                /*!< SCB ICSR: PENDSVCLR Mask */

#define SCB_ICSR_PENDSTSET_Pos             26U                                            /*!< SCB ICSR: PENDSTSET Position */
#define SCB_ICSR_PENDSTSET_Msk             (1UL << SCB_ICSR_PENDSTSET_Pos)                /*!< SCB ICSR: PENDSTSET Mask */

#define SCB_ICSR_PENDSTCLR_Pos             25U                                            /*!< SCB ICSR: PENDSTCLR Position */
#define SCB_ICSR_PENDSTCLR_Msk             (1UL << SCB_ICSR_PENDSTCLR_Pos)                /*!< SCB ICSR: PENDSTCLR Mask */

#define SCB_ICSR_ISRPREEMPT_Pos            23U                                            /*!< SCB ICSR: ISRPREEMPT Position */
#define SCB_ICSR_ISRPREEMPT_Msk            (1UL << SCB_ICSR_ISRPREEMPT_Pos)               /*!< SCB ICSR: ISRPREEMPT Mask */

#define SCB_ICSR_ISRPENDING_Pos            22U                                            /*!< SCB ICSR: ISRPENDING Position */
#define SCB_ICSR_ISRPENDING_Msk            (1UL << SCB_ICSR_ISRPENDING_Pos)               /*!< SCB ICSR: ISRPENDING Mask */

#define SCB_ICSR_VECTPENDING_Pos           12U                                            /*!< SCB ICSR: VECTPENDING Position */
#define SCB_ICSR_VECTPENDING_Msk           (0x1FFUL << SCB_ICSR_VECTPENDING_Pos)          /*!< SCB ICSR: VECTPENDING Mask */

#define SCB_ICSR_VECTACTIVE_Pos             0U                                            /*!< SCB ICSR: VECTACTIVE Position */
#define SCB_ICSR_VECTACTIVE_Msk            (0x1FFUL /*<< SCB_ICSR_VECTACTIVE_Pos*/)       /*!< SCB ICSR: VECTACTIVE Mask */

#define SCB_SHCSR_SYSTICKACT_Pos   11
#define SCB_SHCSR_SYSTICKACT_Msk   (1 << SCB_SHCSR_SYSTICKACT_Pos)


