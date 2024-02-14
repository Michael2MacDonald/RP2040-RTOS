#ifndef __KERNEL_SCHEDULER_H
#define __KERNEL_SCHEDULER_H

/**
 * @file Scheduler.h
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 */

/** TODO:
 * Should I make it round robin??
 * "Round-robin" means tasks that share a priority take turns entering the Running state
 * 
 * I don't think I will make it round robin
 */

#ifndef DEFAULT_THREAD_PRIORITY
	#define DEFAULT_THREAD_PRIORITY normal /** Default is used when a priority value is not given on task creation */
#endif
#ifndef MAX_THREAD_NAME_LEN
	#define MAX_THREAD_NAME_LEN 12 /**  */
#endif


#include "kernel.h"
#include "list.h"

#include <stdlib.h>
#include <string.h>

extern void PendSV_Trigger();

extern volatile uint32_t Ticks; // Tick counter
extern volatile uint32_t Millis;


typedef enum ThreadPriority { // Priority levels for tasks
	uninterruptible = -1,
	critical,
	high,
	moderate,
	normal,
	low,
	none
} TPri_t;

typedef enum ThreadState {
	// States that can be run
	active,   // Thread is currently running (Rename to Running?)
	paused,   // Rename Preempted, Stopped???
	queued,   // Has not started yet (rename to ready, available???)
	// States prevent the thread from running (Blocked)
	waiting,  // Blocked on a resource (Mutex, Semaphore, etc) and will unblock when the resource is available
	sleeping, // Blocked for a specific period of time and will unblock when the timer expires.
	blocked   // Blocked by a user program or thread and can only be unblocked manually by a user program or thread
} TState_t;


typedef struct {
	union {
		uint32_t sleep_start;
		void* event_ptr;
		void* resource_ptr;
	};
	uint32_t sleep_delay;
} TStateMgr_t;


/** STACK: Task Initialization
 * Each task is defined by its handler function and stack. The initialization phase of task’s
 * stack must ensure that the first 64 bytes (16 words) form a valid exception frame. It is
 * necessary to store the default value of at least these three registers:
 * - xPSR to 0x01000000 (the default value)
 * - PC to the handler function
 * - LR to a function to be called when the handler function finishes (otherwise the CPU would jump to invalid location, causing HardFault or undefined behavior)
 * - The actual function os_task_init stores values for registers r0-r12 as well for debugging purposes.
 */
/** STACK: ARM expects this frame format when returning from an interrupt:
 * +------+
 * |      | <- SP before interrupt (orig. SP)
 * | xPSR |
 * |  PC  |
 * |  LR  |
 * |  R12 |
 * |  R3  |
 * |  R2  |
 * |  R1  |
 * |  R0  | <- SP after entering interrupt (orig. SP + 32 bytes)
 * +------+
 */
typedef struct TCB_Stack_Frame { // Rename????
	// Initial stack contents
	uint32_t r7, r6, r5, r4; // Pushed and popped by PendSV_Handler() to save and restore context
	uint32_t r0, r1, r2, r3; // Return value register, scratch register, or argument register
	uint32_t r12;  // Intra-Procedure-call scratch register
	uint32_t lr;   // Link register
	uint32_t pc;   // Program counter
	uint32_t xPSR; // Program status register
} stack_t; // Rename????

/** NOTE: struct objects must be created during runtime to properly initialize */
typedef struct TCB { // Thread control block
	volatile uint32_t sp; // Stack pointer (Does it need to be volatile???)
	int (*func)(void); // Pointer to thread code

	// A higher priority thread will pause a lower priority thread and run first
	// Lower priority value means a higher priority.
	TPri_t priority;

	TState_t state; // active, paused, blocked, queued

	/** TODO: replace?? improve?? */
	TStateMgr_t* stateMgr; // Pointer to object used by blocked states to check if the state should be unblocked

	const char* name; // User defined name for the thread. Used to get a handle to the thread.

	void* stack; // Stack with initialization values for the thread
} TCB_t;

TCB_t* create_TCB(const char* _name, TPri_t _priority, TState_t _state, int (*_func)(void), uint32_t _stackSize);

extern volatile TCB_t* CurrentTCB; // Pointer to the current TCB


int thread_create(const char* name, int stackSize, int (*_func)(void), TPri_t priority);

// void purgeThreads(); // Remove expired threads
void kupdateThreads(); /** TODO: Remove?? Replace?? */ // Updates blocked threads if there condition/delay/wait is over
TCB_t* ksetActiveThread(); // Returns the highest priority thread that is not blocked

/** TODO: Remove?? Replace?? */
void konReturn();


TCB_t* find_thread(const char *name);
// Return a pointer to the currently running thread
TCB_t* thread_self();
// Return a pointer to the main thread
TCB_t* thread_main();


void thread_block_self();                // Set the current thread's state to blocked and call PendSV
void thread_block(TCB_t* thread);     // Set the given thread's state to blocked
void thread_unblock(TCB_t* thread);   // Set the given thread's state to paused
void thread_sleep(uint32_t millis); // Sleep the current thread for _ms milliseconds
// void wait(Resource_t);       // Wait for a resource to be available (Rename Resource_t to Res_t or Rsrc_t)
// Wait for a condition to be true (The Contition type is a lambda function that returns the value of the user condition)
// void wait(Condition_t);

/** halt()
 * @short Immediately invoke a context switch.
 * This can be used to force the status of blocked/sleeping/waiting threads to be updated.
 * If a thread is unblocked and has a higher priority than the current thread, it will be
 * run. Otherwise the current thread will be resumed.
 */
void thread_halt(); // Needed???? rename yield()???


#endif /** END: __KERNEL_SCHEDULER_H */