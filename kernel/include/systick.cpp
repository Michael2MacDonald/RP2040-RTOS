/** TODO: Move from C++ to C */

#include "ArmUtil.h"
#include "cm0plus.h"

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