#include "kernel.h"
#include "ArmUtil.h" // For IRQn_Type
#include "cm0plus.h" // For SCB and NVIC registers

/** TODO: Move from C++ to C */

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

/** nvic_setpriority:
 * Copy of __NVIC_SetPriority() stolen from CMSIS.
 */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int)(IRQn))         )      &  0x03UL) * 8UL)
#define _SHP_IDX(IRQn)           ( (((((uint32_t)(int)(IRQn)) & 0x0FUL)-8UL) >>    2UL)      )
#define _IP_IDX(IRQn)            (   (((uint32_t)(int)(IRQn))                >>    2UL)      )
void nvic_setpriority(IRQn_Type IRQn, uint32_t priority) {
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