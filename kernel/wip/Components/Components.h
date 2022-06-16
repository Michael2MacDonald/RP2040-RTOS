/**
 * @file Components.h
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 * Components allow for portability and easy creation of classes that require initialization
 * a looping run function. To create a component, simply create a class that inherits one of
 * the component classes (Component or StaticComponent). The component classes will handle
 * running the 'init' functions of every component when the system starts. In addition, it
 * will run handle errors and can even implement the ability to rerun failed initializations.
 * The component classes will also handle the creation of threads for components that require
 * a 'main()' function that loops.
 * 
 * For example, you could create a component for serial communication. Simply create a class
 * that inherits a component class and implement the 'init' and 'run' functions. Initialize
 * the serial connection in 'init'. In the 'main' function, periodically check for new serial
 * data that has been received and save it to a buffer. Then, implement functions for reading,
 * writing, peaking, and flushing the serial buffer for the application to interface with the
 * serial connection. The component class will handle calling the init function and creating a
 * thread for the 'main' function.
 * 
 * The component classes are not fully implemented. They are designed to be used with a RTOS
 * or other scheduler. You must call the 'initialize()' function in the base component class
 * on device startup. This will automatically run the 'init()' function of every component.
 * You may also implement the 'failedInit()' function to handle errors when a component fails
 * to initialize correctly.
 * 
 */

#ifndef __KERNEL_COMPONENTS_H
#define __KERNEL_COMPONENTS_H

#ifndef COMP_THREAD_T
	#define COMP_THREAD_T void*
#endif

// #include <Arduino.h>
// #include <limits.h> // ULONG_MAX
#include <algorithm> // std::find
#include <vector> // std::vector

// extern StaticComponent* static_components[]; // TODO: Rename static_component_list???


typedef enum InitializationError { /** TODO: Rename InitializationState??? */
	failed = -1,
	uninitialized,
	initialized
} InitError_t; /** TODO: Rename InitState??? */


/** 
 * @class Component
 * @brief This class is inherited by other classes to make them into components
 * @note This class is virtual and cannot be instantiated directly.
 * @note You must provide an implementation for 'reportInitErrors()'.
 * @note You must provide an implementation for 'init()' in the derived class.
 * @note You must provide an implementation for 'main()' in the derived class.
 * 
 * @warning Components of this class can not be statically allocated. Use 'StaticComponent' for static or global components.
 * 
 */
class Component {
private:
	InitError_t initState = uninitialized; // Flag to indicate if the component has been successfully initialized
	bool loop = true; // Default to looping the 'main()' function continuously

public:

	COMP_THREAD_T thread; // Pointer to thread object

	Component() { // Constructor
		createThread(); // Create a thread for this component
	}

	// Static functions to be implemented by the user
	static void createThread(); // Create a thread for the component

	int threadFunc() { // Run the main function of the component
		if (init() != 0) { // Initialize the component and check if the initialization was successful
			// If the initialization failed, deinitialize and return
			initState = failed; // Set the initialization state flag to 'failed'
			deinit(); // Deinitialize the component
			return 1;   // Return from the thread
		}
		do { // Run the main function of the component at least once
			main();
		} while(loop); // Loop the main function if the component was configured to loop
		return 0;
	}

	// Virtual functions to be overwritten by each component
	virtual int init() = 0;   // Initialize the component
	virtual int main() = 0;   // Run the component's main loop (Replace with thread????)
	virtual int deinit() = 0; // Uninitialize the component (free resources, etc)

	// static int initialize() { // Initialize all components and handle errors
	// 	int errorCount = 0; // Number of errors
	// 	for(auto component : components) {
	// 		// Only initialize components if they have not already been successfully initialized
	// 		if(!component->initState) {
	// 			if (component->init() == 0) { // Check if the component initialized successfully
	// 				component->initState = initialized; // Set the initialization state flag to 'initialized'
	// 			} else {
	// 				component->initState = failed; // Set the initialization state flag to 'failed'
	// 			}
	// 		}
	// 	}
	// 	reportInitErrors(errorCount);
	// }

};


/** 
 * @class StaticComponent
 * @brief Class to derive new static component classes from
 * @warning Components of the StaticComponent class must be static (created globally and not during run-time)
 */
// class StaticComponent {
// private:
// 	static std::vector<StaticComponent*> components; // Vector of pointers to all components
// 	InitError_t initState = uninitialized; // Flag to indicate if the component has been successfully initialized
// 	const bool LoopMain;

// public:
// 	constexpr StaticComponent(bool loop) : LoopMain(loop) {} // Constructor

// 	// Initialize all components and check for errors
// 	static int initialize() { // Initialize all components and handle errors
// 		int errorCount = 0; // Number of errors

// 		for(auto component : components) {
// 			// Only initialize components if they have not already been successfully initialized
// 			if(!component->initState) {
// 				if (component->init() == 0) { // Check if the component initialized successfully
// 					component->initState = initialized; // Set the initialization state flag to 'initialized'
// 				} else {
// 					component->initState = failed; // Set the initialization state flag to 'failed'
// 				}
// 			}
// 		}

// 		// reportInitErrors(errorCount);
// 	}

// 	void init_failed();

// 	// Virtual functions to be overwritten by each component
// 	virtual int init() = 0;
// 	virtual int deinit() = 0;
// 	virtual int construct() = 0;
// 	virtual int main() = 0; // Replace with thread

// 	void task_function() { // Run the main function of the component
// 		// Initialize the component and check if the initialization was successful
// 		// if (init() != 0) {
// 		// 	// If the initialization failed, log it, deinitialize, and return
// 		// 	init_failed();
// 		// 	deinit();
// 		// 	return;
// 		// }
// 		do {
// 			main();
// 		} while(LoopMain);
// 	}

// 	// bool operator==(Component *instance) { return *this == instance;} // Needed?
// };





#endif /** END: __KERNEL_COMPONENTS_H */