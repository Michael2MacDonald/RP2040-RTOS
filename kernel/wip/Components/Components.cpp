/**
 * @file Components.cpp
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short 
 * 
 */

#include "Components.h"
#include "kernel.h"


void Component::createThread() { // Create a thread for the component

	// Kernel::Sched->create("", priority, this->main, this->arg, this->flags);
}


/** IMPLEMENT: List 'StaticComponent' type pointers to all components that implement the 'StaticComponent' class */
// StaticComponent* static_components[] = {
	/** EXAMPLE: How you would list a component that implements the StaticComponent class */
	// (StaticComponent*)(&MyComp),
// };


/** EXAMPLE: Here is an example of a component class and its implementation. */
/**
class MyComponent : public StaticComponent {
	public:
		myComponent() {
			// Initialize component
		}
		void init() {
			// Initialize component
		}
		void main() {
			// Do something
		}
};
*/
