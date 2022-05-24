/**
 * @file Scheduler.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 */

#include "kernel.h"
#include "Scheduler.h"

#include <algorithm>
#include <vector>

extern "C" void HardFault_Handler(void);

// Extern "C" allows the variable to be visable by the 'PendSVHandler.S' assembly file
extern "C" {
	volatile Kernel::TCB* CurrentTCB; // Pointer to the current TCB
}
extern "C" Kernel::TCB_t* _FAST;
extern "C" Kernel::TCB_t* _MAIN;
extern void PendSV_Trigger();

// extern inline uint32_t millis();

extern volatile bool switch_signal;

namespace Kernel {

// Core_t Core; // Create Core object
Scheduler* Sched; // Create Scheduler object

// Declare outside of class
// std::vector<TCB*> Scheduler::threads; // Holds pointers to all threads
// TCB* Scheduler::activeThread;
// bool Scheduler::enabled;
// unsigned int Scheduler::lastId;


// void Scheduler::init() { // Initialize the scheduler

// 	// asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

// 	asm volatile(
// 		"cpsid i \n"      // Disable interrupts (set PRIMASK)
// 		"ldr r4, =%0 \n"   // Get address of the current TCB
// 		"ldr r4, [r4] \n" // Get the stack pointer from the TCB
// 		"msr psp, r4 \n"  // Set the Program Stack Pointer (PSP) to the TCB stack pointer
// 		"dsb \n"          // Data Synchronization Barrier
// 		"isb \n"          // Instruction Synchronization Barrier
// 		" \n" 
// 		:: "r"(this->activeThread)
// 		: "r4", "memory"
// 	);


// }


void Scheduler::block() { // Block the current thread

	// Disable interrupts to prevent systick from triggering PendSV which might try to read CurrentTCB->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	CurrentTCB->state = blocked; // Set the current thread's state to blocked
	CurrentTCB->updateState = nullptr;
	PendSV_Trigger(); // Trigger PendSV to switch to another thread

	asm volatile("dsb"); // Data Synchronization Barrier
	asm volatile("isb"); // Instruction Synchronization Barrier
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}

void Scheduler::block(TCB* thread) { // Block the current thread

	// Disable interrupts to prevent systick from triggering PendSV which might try to read CurrentTCB->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	thread->state = blocked; // Set the current thread's state to blocked
	thread->updateState = nullptr; // Set the current thread's updateState to nullptr

	asm volatile("dsb"); // Data Synchronization Barrier
	asm volatile("isb"); // Instruction Synchronization Barrier
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}

void Scheduler::unblock(TCB* thread) { // Unblock a thread
	// Disable interrupts to prevent systick from triggering PendSV which might try to read thread->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	thread->state = paused; // Set the thread's state to paused
	thread->updateState = nullptr; // Set the thread's updateState to nullptr
	PendSV_Trigger(); // Trigger PendSV to switch to make sure the unblocked thread preempts the current thread if it has a higher priority

	asm volatile("dsb"); // Data Synchronization Barrier
	asm volatile("isb"); // Instruction Synchronization Barrier
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}
__attribute__((optimize("Og")))
void Scheduler::sleep(uint32_t millis) { // Unblock a thread
	// Disable interrupts to prevent systick from triggering PendSV
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	// sleepQueue.push_back(new sleep_t(millis, CurrentTCB));
	// CurrentTCB->updateState = new sleep_t(millis);
	CurrentTCB->state = sleeping;
	CurrentTCB->_sleep.delay = millis;
	CurrentTCB->_sleep.start = TickCount;

	asm volatile("dsb"); // Data Synchronization Barrier
	asm volatile("isb"); // Instruction Synchronization Barrier
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}

void Sched_onReturn() {
	Sched->onReturn();
}

// Returns the highest priority thread that is not blocked
TCB* Scheduler::selectNextThread() { // No threads can be active when this is called!

	asm volatile("dsb":::"memory"); // Data Synchronization Barrier
	asm volatile("isb":::"memory"); // Instruction Synchronization Barrier

	// If this context switch was not because the current thread just entered a blocked state
	if (CurrentTCB->state == active) CurrentTCB->state = paused; // Set the current thread's state to paused

	// Set the first thread ("_MAIN") as the currently selected thread
	TCB* selected = threads[0];

	// Loop through all the threads starting at the second thread (The first thread is always "_MAIN" so we start with is selected)
	// If the thread is a higher priority than the currently selected thread, set it as the selected thread
	for (unsigned int i = 1; i < threads.size(); i++) {
		if (threads[i]->state >= Kernel::waiting) { // Check if the thread is in a blocked state (waiting, sleeping, blocked)
			continue; // Move on to the next thread
		} else if (threads[i]->priority < selected->priority) { // Check if this thread has a higher priority than the currently selected thread
			selected = threads[i]; // Set this thread as the new selected thread
		} else if (threads[i]->priority == selected->priority && threads[i]->state == paused && selected->state == queued) { // If they have the same priority, check for urgency and state
			selected = threads[i]; // Set this thread as the new selected thread
		}
	}

	// Return the selected thread with the highest priority of all threads
	CurrentTCB = selected;
	// CurrentTCB = threads[1];

	CurrentTCB->state = active; // make the new current thread active

	return (TCB*)CurrentTCB;
}

void Scheduler::updateThreads() {
	for (unsigned int i = 0; i < Sched->threads.size(); i++) { // For each thread

		// Check if the thread is in a blocked state that should be updated (waiting, sleeping, etc)
		// if (threads[i]->state >= waiting && threads[i]->state != blocked) {
			// if (threads[i]->updateState != nullptr) { // if the pointer is not null

				if (this->threads[i]->state == sleeping) {
					// sleep_t *sleep = (sleep_t*)threads[i]->updateState; // Cast the pointer to a sleep_t pointer
					// int start = sleep->start;
					// int now = TickCount;
					// int delay = sleep->delay;
					// if (now - start >= delay) {
					// 	threads[2]->state = paused;
					// }
					// if (TickCount - sleep->start >= sleep->delay) {
					// if (TickCount - sleep->start >= 3000) {
					// if (sleep->check() == true) {
					// 	// threads[2]->state = paused;
					// 	threads[i]->state = paused; // Set the current thread's state to paused
					// 	threads[i]->updateState = nullptr;
					// 	delete sleep; // Delete the sleep object
					// }
					if (threads[i]->_sleep.check() == true) {
						threads[i]->state = paused; // Set the current thread's state to paused
						threads[i]->_sleep.delay = 0;
						threads[i]->_sleep.start = 0;
					}
					// threads[2]->state = paused;
				} else if (threads[i]->state == waiting) {
					/** TODO: Code */
				} else { /** TODO: Throw an error (waiting or sleeping thread has null updateState pointer) */ }

			// } else { /** TODO: Throw an error (waiting or sleeping thread has null updateState pointer) */ }
		// }

	} /** END: for each thread in threads */
}

extern "C" uint32_t SwitchContext() { // Rename??
	/** TODO:
	 * - Check for stack overflow
	 * - Check for suspended threads
	 * 	- vDelays
	 * 	- waiting for resources
	 * 	- etc
	 * - Check tasks that are waiting
	 * 	- delay
	 * 	- interupt
	 * 	- event
	 * - check events???
	 * - Find the highest priority thread and set CurrentTCB to that thread
	 * 
	 */

	// asm volatile("dsb":::"memory"); // Data Synchronization Barrier
	// asm volatile("isb":::"memory"); // Instruction Synchronization Barrier

	// Check if any threads need to exit from sleep or waiting
	Sched->updateThreads();

	// if (Sched->threads[2]->state == blocked) {
	// 	static int start = 0;
	// 	if (start == 0) {
	// 		start = TickCount;
	// 	} else if (TickCount - start >= 3000) {
	// 		Sched->threads[2]->state = paused; // Set the current thread's state to paused
	// 		delete Sched->threads[2]->sleep; // Delete the sleep queue entry
	// 		Sched->threads[2]->sleep = nullptr;
	// 		start = 0; // Set to zero so that the next time the thread is blocked it will set the start value to the current tick count
	// 	}
	// }

	asm volatile("dsb":::"memory"); // Data Synchronization Barrier
	asm volatile("isb":::"memory"); // Instruction Synchronization Barrier
	
	uint32_t result = (uint32_t)Sched->selectNextThread(); // Select the next thread to run
	return result;
}



}; /** END: namespace Kernel */


