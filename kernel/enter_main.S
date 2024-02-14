.cpu cortex-m0plus
.syntax unified
.thumb

.extern CurrentTCB // Points to the _MAIN thread's TCB on initialization

.section .text
.align 2


/** 
 * @brief Switches to PSP, sets the stack pointer to _MAIN's stack pointer, and jumps to main()
 */
.global enter_main
.type	enter_main,%function
.thumb_func
enter_main:
	// Get address of the current TCB
	ldr r0, =CurrentTCB // Load the address of CurrentTCB pointer in register r3
	ldr r0, [r0]        // Load the address of the current TCB into register r3

	// Set the Program Stack Pointer (PSP)
	ldr r0, [r0] // Get the thread stack pointer from the TCB
	msr psp, r0  // Set PSP to the thread stack pointer

	// Set the Link Register (LR)
	ldr r1, [r0, #36] // Get the thread link register from the thread's stack
	mov lr, r1        // Set LR to the thread link register
	
	// Get the function address to jump to (Should be main() unless overwriten by user)
	ldr r1, [r0, #40] // Get the function address from the thread's stack

	// Set system mode for running threads
	movs r0, #0 // Reset value of r0
	@ adds r0, #1 // Use Unprivileged Mode (Set bit 0 of the CONTROL register [CONTROL.nPRIV])
	adds r0, #2 // Use Program Stack Pointer (PSP) (Set bit 1 of the CONTROL register [CONTROL.SPSEL])
	msr control, r0 // Write to the control register

	bx r1 // Jump to main()
