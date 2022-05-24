#ifndef __CM0PLUS_H
#define __CM0PLUS_H

#include "kernel.h"


// ========== Cortex-M0+ internal peripherals ==========
#define PPB_BASE                0xE0000000
// #define PPB_SCB2_BASE           0xE000E008 // System Control Block
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

#define SysTick_CSR_COUNTFLAG_Pos     (16)
#define SysTick_CSR_COUNTFLAG_Msk     (1 << SysTick_CSR_COUNTFLAG_Pos)
// Clock Source
#define SysTick_CSR_CLKSOURCE_Pos     (2)
#define SysTick_CSR_CLKSOURCE_Msk     (1 << SysTick_CSR_CLKSOURCE_Pos)
// Enable Systick Interupt
#define SysTick_CSR_TICKINT_Pos       (1)
#define SysTick_CSR_TICKINT_Msk       (1 << SysTick_CSR_TICKINT_Pos)
// Enable
#define SysTick_CSR_ENABLE_Pos        (0)
#define SysTick_CSR_ENABLE_Msk        (1 << SysTick_CTRL_ENABLE_Pos)
// Reload Value
#define SysTick_RVR_RELOAD_Pos        (0)
#define SysTick_RVR_RELOAD_Msk        (0xFFFFFF << SysTick_LOAD_RELOAD_Pos)
// Current Value
#define SysTick_CVR_CURRENT_Pos       (0)
#define SysTick_CVR_CURRENT_Msk       (0xFFFFFF << SysTick_VAL_CURRENT_Pos)


// #define NVIC_ISER   (PPB_BASE + 0xE100)
// #define NVIC_ICER   (PPB_BASE + 0xE180)
// #define NVIC_ISPR   (PPB_BASE + 0xE200)
// #define NVIC_ICPR   (PPB_BASE + 0xE280)
// #define NVIC_IPR0   (PPB_BASE + 0xE400)
// #define NVIC_IPR1   (PPB_BASE + 0xE404)
// #define NVIC_IPR2   (PPB_BASE + 0xE408)
// #define NVIC_IPR3   (PPB_BASE + 0xE40C)
// #define NVIC_IPR4   (PPB_BASE + 0xE410)
// #define NVIC_IPR5   (PPB_BASE + 0xE414)
// #define NVIC_IPR6   (PPB_BASE + 0xE418)
// #define NVIC_IPR7   (PPB_BASE + 0xE41C)

struct nvic_hw {
	uint32_t ISER;               // (0x000) (R/W) Interrupt Set Enable Register
	uint32_t RESERVED0[31];
	uint32_t ICER;               // (0x080) (R/W) Interrupt Clear Enable Register
	uint32_t RESERVED1[31];
	uint32_t ISPR;               // (0x100) (R/W) Interrupt Set Pending Register
	uint32_t RESERVED2[31];
	uint32_t ICPR;               // (0x180) (R/W) Interrupt Clear Pending Register
	uint32_t RESERVED3[31];
	uint32_t RESERVED4[64];
	uint32_t IP[8];              // (0x300) (R/W) Interrupt Priority Register
};

#define NVIC ((volatile struct nvic_hw*)NVIC_BASE)


struct scb_hw {
	uint32_t CPUID;        // (0x000) (R/ ) CPUID Base Register
	uint32_t ICSR;         // (0x004) (R/W) Interrupt Control and State Register
	uint32_t VTOR;         // (0x008) (R/W) Vector Table Offset Register
	uint32_t AIRCR;        // (0x00C) (R/W) Application Interrupt and Reset Control Register
	uint32_t SCR;          // (0x010) (R/W) System Control Register
	uint32_t CCR;          // (0x014) (R/W) Configuration Control Register
	uint32_t RESERVED1;    // (0x018) (R/ ) Reserved - SHPR1
	uint32_t SHP[2];       // (0x01C) (R/W) System Handlers Priority Registers. [0] is RESERVED */
	uint32_t SHCSR;        // (0x024) (R/W) System Handler Control and State Register */
};

#define SCB ((volatile struct scb_hw*)SCB_BASE)

/** BITS: [19:16] Architecture (Also called 'Constant'?)
 * 0xC60 = ARMv6-M
 */
#define SCB_CPUID_ARCHITECTURE_Pos    (16)
#define SCB_CPUID_ARCHITECTURE_Msk    (0xF << SCB_CPUID_ARCHITECTURE_Pos)
/** BITS: [15:4] Partno
 * 0xC20 = Cortex-M0
 * 0xC60 = Cortex-M0+
 */
#define SCB_CPUID_PARTNO_Pos          (4)
#define SCB_CPUID_PARTNO_Msk          (0xFFF << SCB_CPUID_PARTNO_Pos)


/* SCB Interrupt Control State Register Definitions */
#define SCB_ICSR_NMIPENDSET_Pos       (31)
#define SCB_ICSR_NMIPENDSET_Msk       (1 << SCB_ICSR_NMIPENDSET_Pos)

#define SCB_ICSR_PENDSVSET_Pos        (28)
#define SCB_ICSR_PENDSVSET_Msk        (1 << SCB_ICSR_PENDSVSET_Pos)

#define SCB_ICSR_PENDSVCLR_Pos        (27)
#define SCB_ICSR_PENDSVCLR_Msk        (1 << SCB_ICSR_PENDSVCLR_Pos)

#define SCB_ICSR_PENDSTSET_Pos        (26)
#define SCB_ICSR_PENDSTSET_Msk        (1 << SCB_ICSR_PENDSTSET_Pos)

#define SCB_ICSR_PENDSTCLR_Pos        (25)
#define SCB_ICSR_PENDSTCLR_Msk        (1 << SCB_ICSR_PENDSTCLR_Pos)

#define SCB_ICSR_ISRPREEMPT_Pos       (23)
#define SCB_ICSR_ISRPREEMPT_Msk       (1 << SCB_ICSR_ISRPREEMPT_Pos)

#define SCB_ICSR_ISRPENDING_Pos       (22)
#define SCB_ICSR_ISRPENDING_Msk       (1 << SCB_ICSR_ISRPENDING_Pos)

#define SCB_ICSR_VECTPENDING_Pos      (12)
#define SCB_ICSR_VECTPENDING_Msk      (0x1FF << SCB_ICSR_VECTPENDING_Pos)

#define SCB_ICSR_VECTACTIVE_Pos       (0)
#define SCB_ICSR_VECTACTIVE_Msk       (0x1FF << SCB_ICSR_VECTACTIVE_Pos)


// Priority of system handler 11, SVCall
#define SCB_SHPR2_PRI_11_Pos          (30)
#define SCB_SHPR2_PRI_11_Msk          (2 << SCB_SHPR2_PRI_11_Pos)
// Priority of system handler 15, SysTick
#define SCB_SHPR3_PRI_15_Pos          (30)
#define SCB_SHPR3_PRI_15_Msk          (2 << SCB_SHPR3_PRI_15_Pos)
// Priority of system handler 14, PendSV
#define SCB_SHPR3_PRI_14_Pos          (22)
#define SCB_SHPR3_PRI_14_Msk          (2 << SCB_SHPR3_PRI_14_Pos)

/* SCB System Handler Control and State Register Definitions */
#define SCB_SHCSR_SYSTICKACT_Pos      (11)
#define SCB_SHCSR_SYSTICKACT_Msk      (1 << SCB_SHCSR_SYSTICKACT_Pos)


#endif /** END: __CM0PLUS_H */