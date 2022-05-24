#include "kernel.h"
#include <rp2040.h>

/** TODO: Move from C++ to C */


// System Tick Counter: Increments every systick interrupt (1ms). Used by timing functions like delay().
volatile uint32_t Ticks = 0; /** TODO: Rename */  // Ticks is incremented every systick interrupt (Can be reset)
volatile uint32_t Uptime = 0; /** TODO: Rename */ // Milliseconds since the system was started (Never reset except on system reset)
volatile uint32_t Millis = 0; // Counts up in milliseconds (May be reset)
volatile uint32_t Micros = 0; // Counts up in microseconds (May be reset)


void delay(uint32_t msec) {
	if (msec == 0) return;
	uint32_t start = millis();
	// uint32_t start = Ticks;
	do {
		// yield(); // Not yet implemented
		asm volatile ("nop");
	// } while (Ticks-start < msec);
	} while (millis()-start < msec);
}

void delay_nop(uint32_t count) {
	while (count--) asm volatile ("nop");
}

void delay_notick(uint32_t msec) { // Delay millis without needing systick interrupt
	// Calculate the number of cycles needed to delay the specified number of milliseconds
	// One cycle is one loop of the while loop below

	// 'nop' is two clock cycles and a for loop takes about 24 cycles (I think)
	const uint32_t cycleLength = (24+2); // 24 instruction cycles

	// (cycles * cycleLength)/F_CPU = seconds
	// (seconds * F_CPU) / cycleLength = cycles
	// So ((millis/1000) * F_CPU) / cycleLength = cycles
	uint32_t cycles = ( (msec/1000) * F_CPU ) / cycleLength;

	for (unsigned int i = 0; i<cycles; i++) asm volatile ("nop");
}

// __attribute__((weak)) // Allows the user to override the default implementation
// void sleep(uint32_t msec) {
// 	uint32_t start = millis();
// 	do {
// 		asm("wfi");
// 	} while(millis()-start > msec) ;
// }

// __attribute__((always_inline))
uint32_t ticks() {
	// asm volatile("cpsid i");
	return Ticks;
	// asm volatile("cpsie i");
}
// __attribute__((always_inline))
uint32_t millis() {
	// asm volatile("cpsid i");
	return Millis;
	// asm volatile("cpsie i");
}
// __attribute__((always_inline))
uint32_t micros() {
	// asm volatile("cpsid i");
	return Micros;
	// asm volatile("cpsie i");
}

void reset_ticks(uint32_t ticks) {
	/** TODO: Ensure that reseting Ticks does not effect Uptime, Millis, or Micros */
	Ticks = ticks;
}
void reset_millis(uint32_t millis) {
	/** TODO: Ensure that reseting millis does not effect any timing code */
	Millis = millis;
}
void reset_micros(uint32_t micros) {
	/** TODO: Ensure that reseting micros does not effect any timing code */
	Micros = micros;
}

// volatile uint32_t systick_millis_count = 0;
// volatile uint32_t systick_cycle_count = 0;
// volatile uint32_t scale_cpu_cycles_to_microseconds = 0;
// uint32_t systick_safe_read; // micros() synchronization



// void delay(uint32_t msec) {
// 	if (msec == 0) return; // Return if there is no delay

// 	uint32_t start = micros(); // Save the current ms count since boot
// 	while (1) {
// 		// Wait until the defined delay time in ms has passed
// 		while ((micros() - start) >= 1000) {
// 			if (--msec == 0) return;
// 			start += 1000;
// 		}
// 		// yield(); // Not yet implemented
// 	}
// 	// TODO...
// }


/** micros
 * @short Return the number of microseconds since the program started.
 */
// uint32_t micros(void) {
// 	uint32_t smc, scc;
// 	do {
// 		__LDREXW(&systick_safe_read);
// 		smc = systick_millis_count;
// 		scc = systick_cycle_count;
// 	} while ( __STREXW(1, &systick_safe_read));
// 	uint32_t cyccnt = ARM_DWT_CYCCNT;
// 	asm volatile("" : : : "memory");
// 	uint32_t ccdelta = cyccnt - scc;
// 	uint32_t frac = ((uint64_t)ccdelta * scale_cpu_cycles_to_microseconds) >> 32;
// 	if (frac > 1000) frac = 1000;
// 	uint32_t usec = 1000*smc + frac;
// 	return usec;
// }


// void delay_ticks(unsigned ticks) {
// 	PPB_SYST->LOAD = ticks;
// 	PPB_SYST->VAL = 0;
// 	PPB_SYST->CTRL = PPB_SYST_CSR_ENABLE_Msk;

// 	// COUNTFLAG is a bit that is set to 1 when counter reaches 0.
// 	// It's automatically cleared when read.
// 	while ((PPB_SYST->CTRL & PPB_SYST_CSR_COUNTFLAG_Msk) == 0);
// 	PPB_SYST->CTRL = 0;
// }



/**
 *  \brief Configures the SAM3 SysTick & reset tickCount.
 *  Systick interrupt handler will generates 1ms interrupt and increase a
 *  tickCount.
 *  \param dwNew_MCK  Current master clock.
 */
// extern uint32_t TimeTick_Configure( uint32_t dwNew_MCK ) {
// 	Ticks = 0 ;
// 	return SysTick_Config( dwNew_MCK/1000 ) ;
// }

// static __INLINE uint32_t SysTick_Config(uint32_t ticks) {
// 	if (ticks > PPB_SYST_RVR_RELOAD_Msk)  return (1);            /* Reload value impossible */

// 	PPB_SYST->LOAD  = (ticks & PPB_SYST_RVR_RELOAD_Msk) - 1;      /* set reload register */
// 	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
// 	PPB_SYST->VAL   = 0;                                          /* Load the SysTick Counter Value */
// 	PPB_SYST->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
// 							SysTick_CTRL_TICKINT_Msk   |
// 							SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
// 	return (0);                                                  /* Function successful */
// }

// Interrupt-compatible version of micros
// Theory: repeatedly take readings of SysTick counter, millis counter and SysTick interrupt pending flag.
// When it appears that millis counter and pending is stable and SysTick hasn't rolled over, use these 
// values to calculate micros. If there is a pending SysTick, add one to the millis counter in the calculation.
// unsigned long micros( void ) {
// 	uint32_t ticks, ticks2;
// 	uint32_t pend, pend2;
// 	uint32_t count, count2;

// 	ticks2  = PPB_SYST->VAL;
// 	pend2   = !!((SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)||((SCB->SHCSR & SCB_SHCSR_SYSTICKACT_Msk)))  ;
// 	count2  = GetTickCount();

// 	do {
// 		ticks=ticks2;
// 		pend=pend2;
// 		count=count2;
// 		ticks2  = PPB_SYST->VAL;
// 		pend2   = !!((SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)||((SCB->SHCSR & SCB_SHCSR_SYSTICKACT_Msk)))  ;
// 		count2  = GetTickCount();
// 	} while ((pend != pend2) || (count != count2) || (ticks < ticks2));

// 	return ((count+pend) * 1000) + (((PPB_SYST->LOAD  - ticks)*(1048576/(F_CPU/1000000)))>>20) ; 
// 	// this is an optimization to turn a runtime division into two compile-time divisions and 
// 	// a runtime multiplication and shift, saving a few cycles
// }

// original function:
// uint32_t micros( void )
// {
//     uint32_t ticks ;
//     uint32_t count ;
// 
//     SysTick->CTRL;
//     do {
//         ticks = SysTick->VAL;
//         count = GetTickCount();
//     } while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk);
// 
//     return count * 1000 + (SysTick->LOAD + 1 - ticks) / (SystemCoreClock/1000000) ;
// }

