#ifndef __KERNEL_SCHEDULER_H
#define __KERNEL_SCHEDULER_H

/**
 * @file Scheduler.h
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 * TODO: Move definitions to cpp file??
 */

/** TODO:
 * Should I make it round robin??
 * "Round-robin" means tasks that share a priority take turns entering the Running state
 * 
 * I don't think I will make it round robin
 */

#ifndef ENABLE_THREAD_URGENCY
	#define ENABLE_THREAD_URGENCY 0
#endif

#ifndef DEFAULT_THREAD_PRIORITY
	#define DEFAULT_THREAD_PRIORITY normal // Default is used when a priority value is not given on task creation
#endif
#ifndef MAX_THREAD_NAME_LEN
	#define MAX_THREAD_NAME_LEN 8 // 
#endif


#include "kernel.h"

#include <vector>
#include <algorithm> // std::find
#include <stdint.h>
#include <string>
// #include <cstring>

extern void PendSV_Trigger();

extern volatile uint32_t Ticks; // Tick counter
extern volatile uint32_t Millis;


class Scheduler;

namespace Kernel {

void Sched_onReturn();

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

// typedef bool (*Condition_t)(void);
// typedef std::function<bool(void)> Condition_t; // Rename Condition_t to Cond_t????

union StateMgr {
	struct {
		uint32_t start; // Start time
		uint32_t delay; // Delay in milliseconds
		bool operator==(bool) { return (millis() - start) >= delay; }
		operator bool() { return (millis() - start) >= delay; }
	} sleep;
	struct {
		void* event;
		bool operator==(bool) { return true; }
		operator bool() { return true; }
	} event;
	struct {
		void* resource;
		bool operator==(bool) { return true; }
		operator bool() { return true; }
	} resource;
}; /** END: union StateMgr */


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
struct TCB_Stack { // Rename????
	// const uint32_t size; // Size of stack in bytes
	uint8_t dummy[256 - 48]; // Dummy space for stack; hard set 256 bytes for now

	// Initial stack contents
	uint32_t r7, r6, r5, r4; // Pushed and popped by PendSV_Handler() to save and restore context
	uint32_t r0, r1, r2, r3; // Return value register, scratch register, or argument register
	uint32_t r12;  // Intra-Procedure-call scratch register
	uint32_t lr;   // Link register
	uint32_t pc;   // Program counter
	uint32_t xPSR; // Program status register

	// TCB_Stack(uint32_t funcAddr) {
	TCB_Stack() {
		xPSR = 0x01000000; // Set xPSR to 0x01000000 (the default value)
		lr = (uint32_t)&Sched_onReturn;
		// pc = funcAddr; // Set PC to the thread function address
	}
};

/** NOTE: struct objects must be created during runtime to properly initialize */
typedef struct TCB { // Thread control block
	volatile uint32_t sp; // Stack pointer (Does it need to be volatile???)
	int (*func)(void); // Pointer to thread code

	// A higher priority thread will pause a lower priority thread and run first
	// Lower priority value means a higher priority.
	ThreadPriority priority;

	// template<int size> struct pri_test {
	// 	int8_t pri : size, urg : 8-size;
	// };

	ThreadState state; // active, paused, blocked, queued

	/** TODO: replace?? improve?? */
	StateMgr stateMgr; // Pointer to object used by blocked states to check if the state should be unblocked

	std::string name; // User defined name for the thread. Used to get a handle to the thread.
	// char name[MAX_THREAD_NAME_LEN]; // User defined name for the thread. Used to get a handle to the thread.

	TCB_Stack stack; // Stack with initialization values for the thread

	TCB(std::string _name, TPri_t _priority, TState_t _state, int (*_func)(void), uint32_t _stackSize) {
		name = _name; // Set name
		func = _func; // Set func
		priority = _priority; // Set priority
		state = _state; // Set state manager

		sp = (uint32_t)(&(stack.r7));
		stack.pc = (uint32_t)func;
	}
} TCB_t;

// extern Scheduler* Sched;
// typedef struct {
// 	Scheduler* operator->() {
// 		/** TODO: Return the scheduler for the current core */
// 		return Sched;
// 	}
// } Core_t;
// extern Core_t Core;

extern "C" volatile TCB* CurrentTCB; // Pointer to the current TCB

/**
 * @class Scheduler
 * @short Class that creates, deletes, and manages threads and tasks
 * @brief
 * @warning Not Thread Safe! Should only be used by the kernel through PendSV_Handler and main()
 */
class Scheduler {
public:
	/** TODO:
	 * Place all global variables like CurrentTCB in a structure or array.
	 * Place the address of the array in a dedicated register (r9)
	 * Create 
	 */
	// static TCB* CurrentTCB; // Pointer to the current TCB
	bool enabled = false; // if the scheduler is running
	std::vector<TCB*> threads; // Holds pointers to all threads

	Scheduler() {}
	~Scheduler() {}

	// void init(); // Initialize the scheduler

	int create(std::string name, int stackSize, int (*_func)(void), TPri_t priority = DEFAULT_THREAD_PRIORITY) {
		if (thread(name.c_str()) == nullptr) { // Check that name does not already exist
			threads.push_back(new TCB(name, priority, queued, *_func, stackSize));
			return 0;
		} else {
			return -1;
		}
	}

	// void purgeThreads(); // Remove expired threads
	void updateThreads(); /** TODO: Remove?? Replace?? */ // Updates blocked threads if there condition/delay/wait is over
	TCB* setActiveThread(); // Returns the highest priority thread that is not blocked
	
	/** TODO: Remove?? Replace?? */
	void onReturn() { // Called when active thread returns
		threads.erase( std::find(threads.begin(),threads.end(),CurrentTCB) ); // Finds and removes the activeThread from the threads vector
		// CurrentTCB = (TCB*)nullptr; // Clear activeThread pointer
		CurrentTCB = main(); // Clear activeThread pointer (set to _MAIN thread)
		/** TODO: 
		 * When a thread is removed from running (ends??), check that the stack pointer is within the valid range
		 * (on context switch, check for stack overflow)
		 */
		PendSV_Trigger(); // Call PendSV to run the next thread
	}

	// Find the thread with the given name and return its pointer
	TCB* thread(const char name[MAX_THREAD_NAME_LEN]) {
		for (uint16_t i = 0; i < threads.size(); i++)
			if (threads[i]->name == name) return threads[i];
		return nullptr;
	}
	// Return a pointer to the currently running thread
	TCB* self() { return (TCB*)CurrentTCB; }
	// Return a pointer to the main thread
	TCB* main() { return (TCB*)threads[0]; }


	void block();                // Set the current thread's state to blocked and call PendSV
	void block(TCB* thread);     // Set the given thread's state to blocked
	void unblock(TCB* thread);   // Set the given thread's state to paused
	void sleep(uint32_t millis); // Sleep the current thread for _ms milliseconds
	// void wait(Resource_t);       // Wait for a resource to be available (Rename Resource_t to Res_t or Rsrc_t)
	// Wait for a condition to be true (The Contition type is a lambda function that returns the value of the user condition)
	// void wait(Condition_t);
	
	/** halt()
	 * @short Immediately invoke a context switch.
	 * This can be used to force the status of blocked/sleeping/waiting threads to be updated.
	 * If a thread is unblocked and has a higher priority than the current thread, it will be
	 * run. Otherwise the current thread will be resumed.
	 */
	void halt() { PendSV_Trigger(); } // Needed???? (rename yield()???)

}; /** END: Class Scheduler */

}; /** END: namespace Kernel */


#endif /** END: __KERNEL_SCHEDULER_H */




// StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack, TaskFunction_t pxCode, void * pvParameters ) {
//     /* Simulate the stack frame as it would be created by a context switch
//      * interrupt. */

//     /* Offset added to account for the way the MCU uses the stack on entry/exit
//      * of interrupts, and to ensure alignment. */
//     pxTopOfStack--;

//     *pxTopOfStack = portINITIAL_XPSR;                                    /* xPSR */
//     pxTopOfStack--;
//     *pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK; /* PC */
//     pxTopOfStack--;
//     *pxTopOfStack = ( StackType_t ) portTASK_RETURN_ADDRESS;             /* LR */

//     /* Save code space by skipping register initialisation. */
//     pxTopOfStack -= 5;                            /* R12, R3, R2 and R1. */
//     *pxTopOfStack = ( StackType_t ) pvParameters; /* R0 */

//     /* A save method is being used that requires each task to maintain its
//      * own exec return value. */
//     pxTopOfStack--;
//     *pxTopOfStack = portINITIAL_EXC_RETURN;

//     pxTopOfStack -= 8; /* R11, R10, R9, R8, R7, R6, R5 and R4. */

//     return pxTopOfStack;
// }