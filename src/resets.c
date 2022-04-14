#include "resets.h"

void reset_set(uint32_t hw) {
	RESETS_CLR->reset |= hw;
}

void reset_release(uint32_t hw) {
	RESETS_CLR->reset &= (~hw);
}

void reset_release_wait(uint32_t hw) {
	RESETS_CLR->reset &= (~hw);
	while (!(RESETS_CLR->reset_done & hw));
}