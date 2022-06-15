#include "cortex.h"
/** TODO: Remove dependency on kernel.h??? */
#include "kernel.h"
#include "cm0plus.h" // For SCB and NVIC registers

/** TODO: Move from C++ to C */


/**===========================================================================
 * Systick Functions
 * ===========================================================================*/

#define CSR_COUNTFLAG  (1 << 16)
#define CSR_CLKSRC     (1 << 2)
#define CSR_TICKINT    (1 << 1)
#define CSR_ENABLE     (1 << 0)

void systick_init() {
	// Enable SysTick, set SysTick clock source to processor clock, enable SysTick IRQ
	SysTick->CSR = CSR_ENABLE | CSR_CLKSRC | CSR_TICKINT;
}

void systick_deinit() {
	SysTick->CSR = 0; // Disable systick
}

void systick_set(uint32_t value) {
	SysTick->RVR = value; // Set systick reload value
	SysTick->CVR = value; // Write to systick current value register to clear the counter
}

uint32_t systick_get() {
	return SysTick->CVR; // Get the current value of the Systick counter
}



/**===========================================================================
 * Nested Vectored Interrupt Controller (NVIC) Functions And Structs
 * ===========================================================================*/

extern uint32_t vector_table[];

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

void PendSV_Trigger() {
	SCB->ICSR |= (1 << 28); // Set PendSV to pending
}

/** nvic_setpriority:
 * Copy of __NVIC_SetPriority() stolen from CMSIS.
 */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int)(IRQn))         )      &  0x03UL) * 8UL)
#define _SHP_IDX(IRQn)           ( (((((uint32_t)(int)(IRQn)) & 0x0FUL)-8UL) >>    2UL)      )
#define _IP_IDX(IRQn)            (   (((uint32_t)(int)(IRQn))                >>    2UL)      )
void nvic_setpriority(IRQn_t IRQn, uint32_t priority) {
	if ((int)(IRQn) >= 0) {
		NVIC->IP[_IP_IDX(IRQn)]  = ((uint32_t)(NVIC->IP[_IP_IDX(IRQn)]  & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
			(((priority << (8U - 2)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
	} else {
		SCB->SHP[_SHP_IDX(IRQn)] = ((uint32_t)(SCB->SHP[_SHP_IDX(IRQn)] & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
			(((priority << (8U - 2)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
	}
}
// void nvic_set_priority(uint32_t num, uint8_t hardware_priority) {
// 	// Note: Only 32 bit writes are supported
// 	uint32_t *p = (uint32_t *)( (&(NVIC->IP)) + (num & ~3u) );
// 	*p = (*p & ~(0xffu << (8 * (num & 3u)))) | (((uint32_t) hardware_priority) << (8 * (num & 3u)));
// }
