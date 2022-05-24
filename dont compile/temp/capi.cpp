/**
 * @file capi.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short Exposes the C++ kernel to C
 * @todo rename to cwrapper??? (cwrapper.cpp, cwrapper.h)
 * 
 */

#include "capi.h"
#include "Scheduler.h"

// const static int set_svc_handlers[] = {
// 	// (uint32_t)(&Default_Handler),
// 	(uint32_t)(nullptr),
// 	(uint32_t)(nullptr),
// 	// Public 
// 	(uint32_t)(nullptr), // Start Scheduler
// 	(uint32_t)(&purgeThreads),     // 'purgeThreads()' Remove expired threads
// 	(uint32_t)(&updateThreads),    // 'updateThreads()' Update blocked threads if their condition/delay/wait is over
// 	(uint32_t)(&selectNextThread), // 'selectNextThread()' Returns the highest priority thread that is not blocked
// 	(uint32_t)(nullptr), // 'create()' Create a new thread
// 	(uint32_t)(nullptr),
// 	(uint32_t)(nullptr),
// 	(uint32_t)(nullptr),
// 	(uint32_t)(nullptr),
// 	(uint32_t)(nullptr),
// };
// std::copy(set_svc_handlers, set_svc_handlers+(sizeof(set_svc_handlers)/sizeof(set_svc_handlers[0])), SVC_Handler_Table);

// SVC_Handler_Table[0] = (uint32_t)(nullptr); // Start Scheduler
// SVC_Handler_Table[1] = (uint32_t)(Kernel::Scheduler::purgeThreads);     // 'purgeThreads()' Remove expired threads
// SVC_Handler_Table[2] = (uint32_t)(Kernel::Scheduler::updateThreads);    // 'updateThreads()' Update blocked threads if their condition/delay/wait is over
// SVC_Handler_Table[3] = (uint32_t)(Kernel::Scheduler::selectNextThread); // 'selectNextThread()' Returns the highest priority thread that is not blocked





extern "C" {
	struct C {
	
	public:
		typedef Kernel::TCB TCB;

	private:
		typedef void (*func_void_t)();
		typedef void* (*func_void_ptr_t)();
		typedef TCB* (*func_tcb_ptr_t)();

	public:

		struct Scheduler {
			// const static void* purgeThreads;
			// const static void* updateThreads;
			// const static void* selectNextThread;
			const static func_void_t purgeThreads;
			const static func_void_t updateThreads;
			const static func_void_ptr_t selectNextThread;

			// static TCB* CurrentTCB;
		} const Sched;

		// const void* Scheduler_capi::purgeThreads = Kernel::Scheduler::purgeThreads;
		// const void* Scheduler_capi::updateThreads = Kernel::Scheduler::updateThreads;
		// const void* Scheduler_capi::selectNextThread = Kernel::Scheduler::selectNextThread;
		const func_void_t purgeThreads = Kernel::Scheduler::purgeThreads;
		const func_void_t updateThreads = Kernel::Scheduler::updateThreads;
		const func_void_ptr_t selectNextThread = (func_void_ptr_t)Kernel::Scheduler::selectNextThread;
		
		// TCB* CurrentTCB = Kernel::Scheduler::CurrentTCB;

	}; /** END: struct C */
} /** END: extern "C" */

