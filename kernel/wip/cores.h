/** TODO: Move from C++ to C */

#include "kernel.h"

#define CORE_COUNT 1

// namespace Kernel {

struct core_t {

	// Used by assembly startup code
	void* stack_pointer; // Pointer to the top of the stack for this core
	void* startup_func; // Pointer to the function to call on startup

	void* sched;

	/** TODO: Implement this */
	void (*sleep)(uint32_t ms); // Put the core to sleep for ms milliseconds

	/** TODO: Implement other levels of sleep */

	/** TODO: Implement this */
	void (*stop)(); // Stop the CPU core

};

uint32_t core0_sp;
uint32_t core1_sp;
void core0_startup();
void core1_startup();
Kernel::Scheduler core0_sched;
Kernel::Scheduler core1_sched;
void core0_sleep(uint32_t ms);
void core1_sleep(uint32_t ms);
void core0_stop();
void core1_stop();

core_t core0, core1;

core_t core0 = {
	.stack_pointer = core0_sp,
	.startup_func = (void*)&core0_startup,
	.sched = (void*)&core1_sched,
	.sleep = (void*)&core0_sleep,
	.stop = (void*)&core0_stop,
};

core_t core1 = {
	.stack_pointer = core1_sp,
	.startup_func = (void*)&core1_startup,
	.sched = (void*)&core1_sched,
	.sleep = (void*)&core1_sleep,
	.stop = (void*)&core1_stop,
};


extern "C" __attribute__((section(".syslut")))
const core_t* cores[2] = { &core0, &core1 }; // holds pointers to each core object

/** TODO: Implement GetCore() function */
__attribute__((weak)) // overridden by the 
uint8_t GetCore() { // Returns the core number of the current core
	return 0;
}


// } /** END: namespace Kernel */