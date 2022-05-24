/**
 * @file Kernel.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 */

#include "kernel.h"    // Include the kernel header file
#include "cm0plus.h"   // Include Cortex-M0+ header file
#include "Scheduler.h" // Include scheduler header file
// #include "Components/Components.h" // Include components

uint64_t F_CPU_CURRENT; // This is the clock frequency that the device is currently configured for. It must be set by the application code whenever the clock frequency changes.


void PendSV_Trigger() {
	SCB->ICSR |= (1 << 28); // Set PendSV to pending
}


// Kernel::Scheduler Sched; // created in Scheduler.h
// Kernel::Scheduler* Sched = Kernel::Scheduler::initialize(); // Create Scheduler object
// namespace Kernel {
// 	Scheduler* Sched = Scheduler::initialize(); // Create Scheduler object
// }


// using namespace Kernel;

// void Kernel::init() {

// 	// Set the ISR handler functions (Systick and SVC)
// 	Init_ISRHandler();

// 	// Create the idle thread
// 	Scheduler::create("idle", 8, &Idle_Thread, none, nonurgent);

// 	// Run user defined init hook
// 	Init_Hook(); /** TODO: Check for errors */
	
// }

// __attribute__((always_inline))
// inline void Kernel::PendSV_Trigger() {
// 	// Set the PendSV exception as pending by setting bit 28 of the ICSR register
// 	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
// }


// extern "C" void SwitchContext() { // Rename?? Move??
// 	/** TODO:
// 	 * - Increment systick (Not needed; done by systick handler)
// 	 * - Check for stack overflow
// 	 * - Check for suspended threads
// 	 * 	- vDelays
// 	 * 	- waiting for resources
// 	 * 	- etc
// 	 * - Check tasks that are waiting
// 	 * 	- delay
// 	 * 	- interupt
// 	 * 	- event
// 	 * - check events???
// 	 * - Find the highest priority thread and set CurrentTCB to that thread
// 	 * 
// 	 */
// }