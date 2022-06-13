#include "kernel.h"

extern "C" int main() { // Main
	/** TODO: Setup RTOS */
	init(); // User setup

	/** TODO: Check if this core should start the scheduler not */
	Kernel::Sched->enabled = true; // Enable the scheduler

	while(1){
		loop();
	}

	return 0;
}

/** TODO: Implement some sort of exit or abort function */
__attribute__((noreturn))
void main_exit() {
	while(1) {
		/** TODO: Implement MCU/system and core reset functions */
		// reset();
	}
}