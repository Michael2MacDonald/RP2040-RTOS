/**
 * @file Scheduler.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 */

#include "kernel.h"
#include "Scheduler.h"
#include "cortex.h" // PendSV_Trigger()

#include "list.h"

volatile TCB_t* CurrentTCB; // Pointer to the current TCB


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

/** TODO:
 * Place all global variables like CurrentTCB in a structure or array.
 * Place the address of the array in a dedicated register (r9)
 * Create 
 */
// static TCB* CurrentTCB; // Pointer to the current TCB
bool sched_enabled = false; // if the scheduler is running
list_t* threads;


void return_handler() { // Rename???
	konReturn();
}

TCB_t* create_TCB(const char* _name, TPri_t _priority, TState_t _state, int (*_func)(void), uint32_t _stackSize) {
	TCB_t* tcb = (TCB_t*)malloc(sizeof(TCB_t));
	if (tcb == NULL) { /** TODO: Add error handling in case memory is not allocated */ }

	tcb->name = _name; // Set name
	tcb->func = _func; // Set func
	tcb->priority = _priority; // Set priority
	tcb->state = _state; // Set state manager

	tcb->stateMgr = (TStateMgr_t*)malloc(sizeof(TStateMgr_t));
	if (tcb->stateMgr == NULL) { /** TODO: Add error handling in case memory is not allocated */ }

	if (_stackSize < 48) _stackSize = 48; // Ensure that the stack is at least 48 bytes to accommodate the compiler saving registers on function entry (I need to find a way to tell gcc not to save registers on function entry because the context switch does it for us)
	tcb->stack = malloc(_stackSize + sizeof(stack_t)); // Allocate stack size plus space for the stack frame
	if (tcb->stack == NULL) { /** TODO: Add error handling in case memory is not allocated */ }
	stack_t* frame = (stack_t*)((uint32_t)tcb->stack + _stackSize); // Get the stack frame from the top of the stack

	tcb->sp = (uint32_t)frame;         // Set SP to the start of the stack frame
	frame->pc = (uint32_t)tcb->func;            // Set PC to the function address
	frame->xPSR = 0x01000000;              // Set xPSR to 0x01000000 (the default value)
	frame->lr = (uint32_t)&return_handler; // Set LR to a return handler function

	return tcb;
}

void konReturn() { // (Rename???) Called when active thread returns
	// Make sure main thread is not deleted - Return to main thread if it exits
	if (CurrentTCB == thread_main()) {
		PendSV_Trigger(); // Call PendSV to run the next thread
		// TODO: Shit, I forgot if I should make an infinite loop and wait for PendSV_Trigger() or return this function...
		return;
	}

	list_t* list_item = NULL;
	for (unsigned int i = list_size(threads); i>0; i--) {
		list_t* tmp = list_get_index(threads,i);
		if ((TCB_t*)tmp->data == CurrentTCB) {
			list_item = tmp;
		}
	}
	if (list_item == NULL) { while(1); /* ERROR: Could not find thread. This should never occur */ }
	//free((void*)((TCB_t*)list_item->data)->name); // Free TCB name string (name str is constant at compile time currently but that might change later)
	free((void*)((TCB_t*)list_item->data)->stack); // Free thread stack
	free((TCB_t*)list_item->data); // Free TCB
	list_delete(list_item); // No need to save output as first list item (main thread) should never be removed
	// CurrentTCB = (TCB*)nullptr; // Clear activeThread pointer
	CurrentTCB = thread_main(); // Clear activeThread pointer (set to _MAIN thread)
	/** TODO: 
	 * When a thread is removed from running (ends??), check that the stack pointer is within the valid range
	 * (on context switch, check for stack overflow)
	 */
	PendSV_Trigger(); // Call PendSV to run the next thread
}

// Return a pointer to the currently running thread
TCB_t* thread_self() { return (TCB_t*)CurrentTCB; }
// Return a pointer to the main thread
TCB_t* thread_main() { return (TCB_t*)threads->data; }

TCB_t* find_thread(const char *name) {
	if (strlen(name) > MAX_THREAD_NAME_LEN) return NULL;
	for (uint16_t i = 0; i < list_size(threads); i++) {
		TCB_t* thread = (TCB_t*)list_get_index(threads,i)->data;
		if (strcmp(thread->name, name) == 0) return thread;
	}
	return NULL;
}

int thread_create(const char* name, int stackSize, int (*_func)(void), TPri_t priority) {
	if (strlen(name)>MAX_THREAD_NAME_LEN) { return -1; }
	if (threads == NULL) {
		threads = list_new(create_TCB(name, priority, queued, *_func, stackSize));
		if (threads == NULL) return -1;
		return 0;
	}
	if (find_thread(name) == NULL) { // Check that name does not already exist
		TCB_t* tmp = create_TCB(name, priority, queued, *_func, stackSize);
		if (tmp == NULL) return -1;
		 else {
			if (list_add(threads, tmp) == NULL) return -1;
		}
		return 0;
	} else {
		return -1;
	}
}


void thread_block_self() { // Block the current thread
	// Disable interrupts to prevent systick from triggering PendSV which might try to read CurrentTCB->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	CurrentTCB->state = blocked; // Set the current thread's state to blocked
	// CurrentTCB->stateMgr = nullptr;
	PendSV_Trigger(); // Trigger PendSV to switch to another thread

	asm volatile("dsb \n isb"); // Data & Instruction Synchronization Barriers
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}
void thread_block(TCB_t* thread) { // Block the current thread
	// Disable interrupts to prevent systick from triggering PendSV which might try to read CurrentTCB->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	thread->state = blocked; // Set the current thread's state to blocked
	// thread->stateMgr = nullptr; // Set the current thread's stateMgr to nullptr

	asm volatile("dsb \n isb"); // Data & Instruction Synchronization Barriers
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}
void thread_unblock(TCB_t* thread) { // Unblock a thread
	// Disable interrupts to prevent systick from triggering PendSV which might try to read thread->state while we are modifying it
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	thread->state = paused; // Set the thread's state to paused
	// thread->stateMgr = nullptr; // Set the thread's stateMgr to nullptr
	PendSV_Trigger(); // Trigger PendSV to switch to make sure the unblocked thread preempts the current thread if it has a higher priority

	asm volatile("dsb \n isb"); // Data & Instruction Synchronization Barriers
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}
void thread_sleep(uint32_t msec) { // Unblock a thread
	// Disable interrupts to prevent systick from triggering PendSV
	asm volatile("cpsid i"); // Disable interrupts (set PRIMASK)

	CurrentTCB->state = sleeping;
	CurrentTCB->stateMgr->sleep_delay = msec;
	CurrentTCB->stateMgr->sleep_start = millis();
	PendSV_Trigger(); // Trigger PendSV to switch to make sure the unblocked thread preempts the current thread if it has a higher priority

	asm volatile("dsb \n isb"); // Data & Instruction Synchronization Barriers
	asm volatile("cpsie i"); // Enable interrupts (clear PRIMASK)
}
void thread_halt() { PendSV_Trigger(); } // Needed???? rename yield()???




// Returns the highest priority thread that is not blocked
TCB_t* ksetActiveThread() { // No threads can be active when this is called!

	// Set the current thread's state to paused if it did not just entered a blocked state
	if (CurrentTCB->state == active) CurrentTCB->state = paused;

	// Set the first thread ("_MAIN") as the currently selected thread
	TCB_t* selected = (TCB_t*)(threads->data);

	// Loop through all the threads starting at the second thread (The first thread is always "_MAIN" so we start with is selected)
	// If the thread is a higher priority than the currently selected thread, set it as the selected thread
	for (unsigned int i = 1; i < list_size(threads); i++) {
		TCB_t* thread = (TCB_t*)list_get_index(threads,i)->data;
		if (thread->state >= waiting) { // Check if the thread is in a blocked state (waiting, sleeping, blocked)
			continue; // Move on to the next thread
		} else if (thread->priority < selected->priority) { // Check if this thread has a higher priority than the currently selected thread
			selected = thread; // Set this thread as the new selected thread
		} else if (thread->priority == selected->priority && thread->state == paused && selected->state == queued) { // If they have the same priority, check for urgency and state
			selected = thread; // Set this thread as the new selected thread
		}
	}

	// Return the highest priority thread that is not blocked
	CurrentTCB = selected; // Set the current thread to the thread selected by the code above
	CurrentTCB->state = active; // Make the new current thread active
	return (TCB_t*)CurrentTCB; // Return the new current thread in case it is needed
}

void kupdateThreads() {
	for (unsigned int i = 0; i < list_size(threads); i++) { // For each thread
		TCB_t* thread = (TCB_t*)list_get_index(threads,i)->data;
		// Check if the thread is in a blocked state that should be updated (waiting, sleeping, etc)
		if (thread->state == sleeping) {
			TStateMgr_t* state = thread->stateMgr;
			if ((millis() - state->sleep_start) >= state->sleep_delay) { // If the thread has finished sleeping
				thread->state = paused; // Set the current thread's state to paused
			}
		} else if (thread->state == waiting) {
			/** TODO: Code */
		} else { /** TODO: Throw an error (waiting or sleeping thread has null stateMgr pointer) */ }

	} /** END: for each thread in threads */
}

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
uint32_t SwitchContext() { // Rename??
	// Check if any threads need to exit from sleeping or waiting
	kupdateThreads();
	// Set the currently selected thread to the highest priority thread that is not blocked
	uint32_t result = (uint32_t)ksetActiveThread(); // Select the next thread to run

	/** TODO: Needed?? */
	asm volatile("dsb":::"memory"); // Data Synchronization Barrier
	asm volatile("isb":::"memory"); // Instruction Synchronization Barrier
	
	return result;
}



