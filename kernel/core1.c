// #include "cm0plus.h"
#include "rp2040.h"
#include <inttypes.h>

#ifndef count_of
	#define count_of(a) (sizeof(a)/sizeof((a)[0]))
#endif

extern void* vector_table_core1[];
extern void* _stack_core1;

static inline bool multicore_fifo_wready(void) {
	return !!(SIO->FIFO_ST & SIO_FIFO_ST_RDY_BITS);
}

static inline void multicore_fifo_push_blocking_inline(uint32_t data) {
	// We wait for the fifo to have some space
	while (!multicore_fifo_wready());

	SIO->FIFO_ST = data;

	// Fire off an event to the other core
	__sev();
}

static inline bool multicore_fifo_rvalid(void) {
	return !!(SIO->FIFO_ST & SIO_FIFO_ST_VLD_BITS);
}

static inline uint32_t multicore_fifo_pop_blocking_inline(void) {
	// If nothing there yet, we wait for an event first,
	// to try and avoid too much busy waiting
	while (!multicore_fifo_rvalid())
		__wfe();

	return SIO->FIFO_RD;
}

static inline void multicore_fifo_drain(void) {
	while (multicore_fifo_rvalid())
		(void) SIO->FIFO_RD;
}

void core1_startup();

// Core 0 enables core 1 and give it a vector table offset, stack pointer, and entry location
void init_core1() {
	// values to be sent in order over the FIFO from core 0 to core 1
	//
	// vector_table is value for VTOR register
	// sp is initial stack pointer (SP)
	// entry is the initial program counter (PC) (don't forget to set the thumb bit!)
	const uint32_t cmd_sequence[] = {0, 0, 1, (uintptr_t) &vector_table_core1, (uintptr_t) _stack_core1, (uintptr_t) &core1_startup};
	uint32_t seq = 0;
	do {
		uint32_t cmd = cmd_sequence[seq];
		// always drain the READ FIFO (from core 1) before sending a 0
		if (!cmd) {
			// discard data from read FIFO until empty
			multicore_fifo_drain();
			// execute a SEV as core 1 may be waiting for FIFO space
			__sev();
		}
		// write 32 bit value to write FIFO
		multicore_fifo_push_blocking_inline(cmd);
		// read 32 bit value from read FIFO once available
		uint32_t response = multicore_fifo_pop_blocking_inline();
		// move to next state on correct response (echo-d value) otherwise start over
		seq = cmd == response ? seq + 1 : 0;
	} while (seq < count_of(cmd_sequence));
}

// entry location of core 1
void core1_startup() {
	while(1); // Wait to be given thread by core0
}

void SysTick_Handler_core1() {
	return;
}
