// #include "Scheduler.h"

// #define CORE_COUNT 1

// namespace Kernel {


// class Core_t {

// 	Scheduler* sched;

// 	void init() {

// 	}

// 	void sleep(uint32_t ms) { // Put the CPU to sleep for ms milliseconds
// 		/** TODO: implement this */
// 	}

// 	/** TODO: Implement other levels of sleep */

// 	void stop() { // Stop the CPU core
// 		/** TODO: implement this */
// 	}

// };


// class CoreManager {
// private:
// 	Core_t cores[CORE_COUNT];

// public:

// 	void init() {
// 		// for (int i = 0; i < CORE_COUNT; i++) {
// 		// 	cores[i].init();
// 		// }
// 	}

// 	Core_t* operator->() {
// 		/** TODO: Return the current core that this is running on */
// 		return &cores[0];
// 	}

// 	Core_t* operator[](uint8_t index) {
// 		if (0 < index < CORE_COUNT) {
// 			return &cores[index];
// 		} else {
// 			return &cores[0];
// 		}
// 	}

// private:
// 	CoreManager();
// 	~CoreManager();

// } Core;

// } /** END: namespace Kernel */