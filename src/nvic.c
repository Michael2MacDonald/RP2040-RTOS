#include "nvic.h"
#include "delay.h"
#include "gpio.h"
#include "rp2040.h"

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

// #define CPUID       (PPB_BASE + 0xED00)
// #define ICSR        (PPB_BASE + 0xED04)
// #define VTOR        (PPB_BASE + 0xED08)
// #define AICR        (PPB_BASE + 0xED0C)

extern uint32_t vector_table[];

__attribute__((used))
void NMI_Handler() {

}
__attribute__((used))
void HardFault_Handler() {

}
__attribute__((used))
void SVCall_Handler() {

}
__attribute__((used))
void PendSV_Handler() {

}

extern void systick_hook();

// __attribute__((interrupt("IRQ"), optimize("-O3")))
void SysTick_Handler() { // Runs every 1ms as defined in startup.c

	TickCount++; // Increment TickCount for delays and other functions

	systick_hook(); // Call the user defined systick hook

}


void nvic_init() {
	// int i;
	// for (i = 0; i < (16 + IRQ_COUNT); ++i)
	// 	vector_table[i] = 0;

	// vector_table[1] = (uint)&reset;
	// vector_table[2] = (uint)nmi_handler;
	// vector_table[3] = (uint)hardfault_handler;
	// vector_table[11] = (uint)svccall_handler;
	// vector_table[14] = (uint)pendsv_handler;
	// vector_table[15] = (uint)systick_handler;
	// *((volatile uint*)VTOR) = (uint)vector_table;
}

void nvic_register_irq(uint32_t irq, void (*handler)()) {
	vector_table[16 + irq] = (uint32_t)handler;
}

void nvic_enable(uint32_t irq) {
	NVIC->ISER = 1 << irq;
}

void nvic_disable(uint32_t irq) {
	NVIC->ICER = 1 << irq;
}

void nvic_setpending(uint32_t irq) {
	NVIC->ISPR = 1 << irq;
}

void nvic_clearpending(uint32_t irq) {
	NVIC->ICPR = 1 << irq;
}