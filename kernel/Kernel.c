/**
 * @file Kernel.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 * @todo Implement some sort of exit or abort function???
 */

#include "kernel.h"    // Include the kernel header file
#include "cm0plus.h"   // Include Cortex-M0+ header file
#include "cortex.h"    // Include Cortex-M0+ header file
#include "Scheduler.h" // Include scheduler header file
// #include "Components/Components.h" // Include components

uint64_t F_CPU_CURRENT; // This is the clock frequency that the device is currently configured for. It must be set by the application code whenever the clock frequency changes.

extern bool sched_enabled;

/** TODO: Move init() to startup() and replace user accessable loop() with main()??? */
int main() { // Main
	init(); // User setup

	/** TODO: Check if this core should start the scheduler not */
	sched_enabled = true; // Enable the scheduler

	while(1){ loop(); }
	return 0;
}


/**===========================================================================
 * System Interupts
 * ===========================================================================*/

__attribute__((used, weak))
void Default_Handler() { // Defined in assembly file. Can be overwritten by the user.
	return;
}

__attribute__((used, weak, alias("Default_Handler")))
void NMI_Handler();

__attribute__((used, weak, alias("Default_Handler")))
void HardFault_Handler();

__attribute__((used, weak, alias("Default_Handler")))
void SVCall_Handler();

extern volatile uint32_t Ticks, Uptime, Millis, Micros;
__attribute__((weak))
void systick_hook() { return; } // Allows users or libraries to run code in the systick interrupt handler

__attribute__((used))
void SysTick_Handler() { // Runs every 1ms as defined in startup.c
	// Increment Ticks, Uptime, Millis, and Micros for delays and other functions
	Ticks++; Uptime++; Millis++; Micros++;
	/** TODO: Increment Millis and Micros based off of ticks and the Systick interval */

	if (sched_enabled == true && ((SCB->ICSR & SCB_ICSR_PENDSVSET_Msk) >> SCB_ICSR_PENDSVSET_Pos) != 1) { // only run if scheduler is enabled and no PendSV interrupt is pending
	// if (Kernel::Sched->enabled == true) { // only run if scheduler is enabled and no PendSV interrupt is pending
		PendSV_Trigger(); // Call PendSV to switch context
	}

	systick_hook(); // Call the user defined systick hook
}

// extern "C" __attribute__((used, weak))
// void PendSV_Handler(); // DO NOT DEFINE!!! Defined in assembly file






// void Kernel::init() {

// 	// Set the ISR handler functions (Systick and SVC)
// 	Init_ISRHandler();

// 	// Create the idle thread
// 	Scheduler::create("idle", 8, &Idle_Thread, none, nonurgent);

// 	// Run user defined init hook
// 	Init_Hook(); /** TODO: Check for errors */
	
// }