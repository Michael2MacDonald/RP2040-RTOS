#include "systick.h"
#include "rp2040.h"

#define CSR_COUNTFLAG 	(1 << 16)
#define CSR_CLKSRC 		(1 << 2)
#define CSR_TICKINT 	(1 << 1)
#define CSR_ENABLE 		(1 << 0)


void systick_init() {
	// Enable SysTick
	// Set SysTick Clock Source to processor clock
	// Enable SysTick IRQ
	SysTick->CSR = CSR_ENABLE | CSR_CLKSRC | CSR_TICKINT;
}

void systick_deinit() {
	SysTick->CSR = 0; // disable systick
}

void systick_set(uint32_t value) {
	SysTick->RVR = value; // set systick reload value
	SysTick->CVR = value; // write to systick current value register to clear the counter
}

uint32_t systick_get() {
	return SysTick->CVR;
}