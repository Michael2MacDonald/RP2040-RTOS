#include "kernel.h"
#include "cm0plus.h"
#include "cortex.h"
#include <core.h> // MCU/system startup
// void core_startup(); // From core\startup.cpp

#include "Scheduler.h"

#include <vector>
#include <string>
#include <cstring>

extern "C" void SVC();
extern "C" int main();
extern "C" void enter_main();

// extern "C" Kernel::TCB* Kernel::CurrentTCB;

extern Kernel::Scheduler* Sched;
extern "C" {
	bool* enabled;
}

// extern void __libc_init_array();

// extern void (**__init_array_start)();
// extern void (**__init_array_end)();
// inline void static_init() {
// 	for (void (**p)() = __init_array_start; p < __init_array_end; ++p)
// 		(*p)();
// }

// extern "C" __attribute__((noreturn, section(".startup"), optimize("no-tree-loop-distribute-patterns")))
extern "C" __attribute__((noreturn, section(".startup")))
void _startup(void) {

	core_startup(); // Initialize the MCU dependent peripherals

	nvic_setpriority(PendSV_IRQn, 3);  // Set PendSV interrupt to the lowest priority
	nvic_setpriority(SVCall_IRQn, 1);  // Set SVCall interrupt to the second highest priority
	nvic_setpriority(SysTick_IRQn, 0); // Set Systick interrupt to the highest priority

	/** TODO: Remove F_CPU_CURRENT entirely??? */
	// F_CPU_CURRENT = (6*1000000); // Startup at 6 MHz

	/** Set the number of clock cycles between each systick interrupt
	 * 120MHz = 120000000 = (120*1000000)
	 * The largest possible value you can set is (2^24)-1 (16777215) which at 120MHz is about 0.1398 seconds or 139.8ms ( 16777215/(120*1000000) = ~0.1398 )
	 * We have the reset value set at 120000 which is 1ms at 120MHz ( 120000/(120*1000000) = ~0.001 )
	 * So the systick interrupt is set to trigger every 1ms
	 */
	systick_init(); // Initialize the systick timer
	systick_set(120000);

	Kernel::Sched = new Kernel::Scheduler();
	enabled = &(Kernel::Sched->enabled);

	// Kernel::Scheduler::init(); // Initialize the scheduler
	Kernel::Sched->create("_MAIN", 256, &main, Kernel::none); // Create the main thread
	Kernel::CurrentTCB = Kernel::Sched->thread("_MAIN");
	// CurrentTCB = Kernel::Sched->threads[0];


	// while(CurrentTCB == nullptr){asm volatile("nop");}
	// while(Kernel::Sched->thread("_MAIN")->func == &main){asm volatile("nop");}
	// while(Kernel::Sched->thread("_MAIN") == nullptr){asm volatile("nop");}
	// while(Kernel::Sched->threads[0] == nullptr){asm volatile("nop");}
	// while(Kernel::Sched->threads[0]->name == "_MAIN"){asm volatile("nop");}
	// main();

	/* Run constructors / initializers */
	// __libc_init_array();

	asm("dsb"); // Data synchronization barrier
	asm("isb"); // Instruction synchronization barrier

	enter_main(); // Enter the _MAIN thread
	while (1) asm volatile("WFI"); // DON'T RETURN!!!!

} /** END: _startup() */

