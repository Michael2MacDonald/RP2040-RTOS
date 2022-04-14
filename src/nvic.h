#pragma once
#include "rp2040.h"

void nvic_init();
void nvic_register_irq(uint32_t irq, void (*handler)());
void nvic_enable(uint32_t irq);
void nvic_disable(uint32_t irq);
void nvic_setpending(uint32_t irq);
void nvic_clearpending(uint32_t irq);


typedef enum IRQn {
	// ==== Cortex-M0 Processor Exceptions ====
	Reset_IRQn             = -15, // 1 Reset Vector
	NonMaskableInt_IRQn    = -14, // 2 Non maskable Interrupt
	HardFault_IRQn         = -13, // 3 Hard Fault all classes of Fault */
	MemoryManagement_IRQn  = -12, // 4 Memory Management
	BusFault_IRQn          = -11, // 5 Bus Fault
	UsageFault_IRQn        = -10, // 6 Usage Fault
	SecureFault_IRQn       =  -9, // 7 Secure Fault Interrupt
	SVCall_IRQn            =  -5, // 11 System Service Call via SVC instruction
	DebugMonitor_IRQn      =  -4, // 12 Debug Monitor
	PendSV_IRQn            =  -2, // 14 Pendable request for system service
	SysTick_IRQn           =  -1, // 15 System Tick Timer
	// ==== Device Specific Interrupt Numbers ====
	Device0_IRQn = 0,
	Device1_IRQn,
	Device2_IRQn,
	Device3_IRQn,
	Device4_IRQn,
	Device5_IRQn,
	Device6_IRQn,
	Device7_IRQn,
	Device8_IRQn,
	Device9_IRQn,
	Device10_IRQn,
	Device11_IRQn,
	Device12_IRQn,
	Device13_IRQn,
	Device14_IRQn,
	Device15_IRQn,
	Device16_IRQn,
	Device17_IRQn,
	Device18_IRQn,
	Device19_IRQn,
	Device20_IRQn,
	Device21_IRQn,
	Device22_IRQn,
	Device23_IRQn,
	Device24_IRQn,
	Device25_IRQn,
	Device26_IRQn,
	Device27_IRQn,
	Device28_IRQn,
	Device29_IRQn,
	Device30_IRQn,
	Device31_IRQn
} IRQn_Type;

#define IRQ_TIMER0      0
#define IRQ_TIMER1      1
#define IRQ_TIMER2      2
#define IRQ_TIMER3      3
#define IRQ_PWM_WRAP    4
#define IRQ_USBCTRL     5
#define IRQ_XIP         6
#define IRQ_0_PIO0      7
#define IRQ_1_PIO0      8
#define IRQ_0_PIO1      9
#define IRQ_1_PIO1      10
#define IRQ_DMA_0       11
#define IRQ_DMA_1       12
#define IRQ_IO_BANK0    13
#define IRQ_IO_QSPI     14
#define IRQ_SIO_PROC0   15
#define IRQ_SIO_PROC1   16
#define IRQ_CLOCKS      17
#define IRQ_SPI0        18
#define IRQ_SPI1        19
#define IRQ_UART0       20
#define IRQ_UART1       21
#define IRQ_ADC_FIFO    22
#define IRQ_I2C0        23
#define IRQ_I2C1        24
#define IRQ_RTC         25
#define IRQ_COUNT       26


// #define NVIC_PRIO_BITS 2

// /* Interrupt Priorities are WORD accessible only under ARMv6M                   */
// /* The following MACROS handle generation of the register offset and byte masks */
// #define _BIT_SHIFT(IRQn)         (  (((uint32_t)(IRQn)       )    &  0x03) * 8 )
// #define _SHP_IDX(IRQn)           ( ((((uint32_t)(IRQn) & 0x0F)-8) >>    2)     )
// #define _IP_IDX(IRQn)            (   ((uint32_t)(IRQn)            >>    2)     )

// static inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority) {
// 	if (IRQn < 0) {
// 		SCB->SHP[_SHP_IDX(IRQn)] = (SCB->SHP[_SHP_IDX(IRQn)] & ~(0xFF << _BIT_SHIFT(IRQn))) |
// 			(((priority << (8 - NVIC_PRIO_BITS)) & 0xFF) << _BIT_SHIFT(IRQn));
// 	} else {
// 		NVIC->IP[_IP_IDX(IRQn)] = (NVIC->IP[_IP_IDX(IRQn)] & ~(0xFF << _BIT_SHIFT(IRQn))) |
// 			(((priority << (8 - NVIC_PRIO_BITS)) & 0xFF) << _BIT_SHIFT(IRQn));
// 	}
// }
