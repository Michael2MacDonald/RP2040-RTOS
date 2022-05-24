
#include "kernel.h"
#include <vector>

class Mutex {
private:
	bool locked = false;

public:

	int lock() {
		while (locked) {
			/* thread sleep or wait */
			
		}
		/** TODO: Disable inturupts */
		locked = true;
		/** TODO: Enable inturupts */
		return 0;
	}

	int trylock() {
		
		if (locked) {
			return 1; // Oops! It was locked so we returned 1
		} else {
			/** TODO: Disable inturupts */
			locked = true;
			/** TODO: Enable inturupts */
			return 0; // Success! It was not locked so we locked it
		}
	}

	int unlock() {
		/** TODO: Disable inturupts */
		locked = false;
		/** TODO: Enable inturupts */
	}

};


