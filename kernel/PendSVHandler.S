/**
 * @file PendSVHandler.S
 * @author Michael MacDonald <michael2macdonald@gmail.com>
 * @short PendSV_Handler saves the state of the current threas and loads the thread with the highest priority
 * 
 */

.cpu cortex-m0plus
.syntax unified
.thumb

.section .text
.align 2


.extern SwitchContext // Define SwitchContext as an external function
.extern CurrentTCB    // Define CurrentTCB as an external variable


/**
 * @brief Called when the PendSV interrupt is triggered
 * @param none
 * 
 * On entry the SP will be set to MSP and the PSP will be pointing to the active thread's stack
 * Store the thread state to stack
 */
/** TODO: Check if the highest priority thread is the one already running and don't switch context if it is */
/** TODO: Add 32 bit instruction support */
.global PendSV_Handler
.type	PendSV_Handler,%function
.thumb_func
PendSV_Handler:
	/** NOTE: NVIC automatically r0-r3, r12, LR, PC, and xPSR */

	@ mrs r0, psr             // Move the Program status register to r0
	@ push {r0}               // Push the value of the PSR onto the stack

	@ mrs r0, primask         // Store the state of the PRIMASK in r0
	cpsid i                 // Disable interrupts (set PRIMASK)

	// Get address of the current TCB
	ldr r0, =CurrentTCB     // Load the address of 'CurrentTCB' in register r0
	ldr r1, [r0]            // Load the address of the current TCB into register r1

	// Save Context (Save r4-r7 to thread stack)
	mrs r2, psp             // Move the Program Stack Pointer (PSP) to r2
	subs r2, #4             // Subtract 4 from the PSP
	str r4, [r2]            // Store r4 to the stack
	subs r2, #4             // Subtract 4 from the PSP
	str r5, [r2]            // Store r5 to the stack
	subs r2, #4             // Subtract 4 from the PSP
	str r6, [r2]            // Store r6 to the stack
	subs r2, #4             // Subtract 4 from the PSP
	str r7, [r2]            // Store r7 to the stack
	@ stmdb r2!, {r4-r7}      // Store registers r4-r7 on the thread stack (Decrement address Before each store and store the final address back into R0)
	
	str r2, [r1]            // Store the PSP to the thread stack pointer of the current TCB

	// Save registers to the Main Stack, disable interrupts, branch to SwitchContext, disable interrupts, and then load registers back from stack
	push {r0, lr}           // Push r0 and lr to the Main Stack (r0 holds the address of CurrentTCB)
	bl SwitchContext        // Branch out to SwitchContext (Branch with Link)
	@ mov r0, #0              // Set R0 to value '0'
	@ msr basepri, r0         // Disable interrupts (who knows what happened to basepri when we branched)
	pop {r0, r3}            // Pop r0 and lr from the Main Stack

	// Restore Context (Load register values from thread stack)
	ldr r1, [r0]            // Load the address of the current TCB into register r1
	ldr r2, [r1]            // Load the value of the first member of the current TCB (the thread stack pointer) into register r2
	
	ldr r7, [r2]            // Load r7 from the stack
	adds r2, #4             // Add 4 to the PSP
	ldr r6, [r2]            // Load r6 from the stack
	adds r2, #4             // Add 4 to the PSP
	ldr r5, [r2]            // Load r5 from the stack
	adds r2, #4             // Add 4 to the PSP
	ldr r4, [r2]            // Load r4 from the stack
	adds r2, #4             // Add 4 to the PSP
	
	@ ldmia r2!, {r4-r7}      // Load registers r4-r7 from the thread stack (Increment address After each transfer and store final address in r2)
	msr psp, r2             // Set the Program Stack Pointer (PSP) to the thread stack pointer
	isb                     // Instruction Synchronization Barrier (Clear pipeline after context change)
	@ b stop

	@ movs r0, #2
	@ msr control, r0 // set CONTROL.SPSEL to 1 (psp)
	@ dsb
	@ isb

	@ ldr r1, =0xE000ED04
	@ movs r0, #1
	@ lsls r0, r0, #27
	@ str r0, [r1]            // Clear the PendSV interrupt flag (ICSR.PENDSVCLR = 1)
	@ movs r0, #1
	@ lsls r0, r0, #25
	@ str r0, [r1]            // Clear the systick interrupt flag (ICSR.PENDSTCLR = 1)

	ldr r0, =0xFFFFFFFD     // EXC_RETURN: Return to thread mode with PSP
	@ ldr r0, =0xFFFFFFF9     // EXC_RETURN: Return to thread mode with MSP
	@ ldr r0, =0xFFFFFFF1     // EXC_RETURN: Return to handler mode with MSP
	@ mov lr, r0

	@ mrs r0, primask         // Restore the state of the PRIMASK in r0
	cpsie i                 // Enable interrupts (clear PRIMASK)

	@ pop {r0}                // Push the value of the PSR onto the stack
	@ msr psr, r0             // Move the Program status register to r0

	bx r0                   // Return from the handler (Uses the values in stack to load regesters and gets everything ready to run) (Branch and exchange instruction set??)

.align 4 // ???? Is this needed???


stop:
	b stop
	nop

@ CurrentTCBAddr: .word CurrentTCB // Get address of 'CurrentTCB'
// 'CurrentTCBAddr' holds the address of the 'CurrentTCB' variable
// 'CurrentTCB' holds the address of the current TCB



// Switch to the psp stack.
// movs r0, #2
// msr CONTROL, r0

// SaveContext:
// RestoreContext:
// IncrementTick:





/** STACK:
 * 
 * Exception frame saved by the NVIC hardware onto stack:
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
 * 
 * Registers saved by the software (PendSV_Handler):
 * +------+
 * |  R7  |
 * |  R6  |
 * |  R5  |
 * |  R4  |
 * |  R11 |
 * |  R10 |
 * |  R9  |
 * |  R8  | <- Saved SP (orig. SP + 64 bytes)
 * +------+
 * 
 **/

/**
 * Save registers R4-R11 (32 bytes) onto current PSP (process stack
 * pointer) and make the PSP point to the last stacked register (R8):
 * - The MRS/MSR instruction is for loading/saving a special registers.
 * - The STMIA inscruction can only save low registers (R0-R7), it is
 *   therefore necesary to copy registers R8-R11 into R4-R7 and call
 *   STMIA twice.
 */
	@ mrs	r0, psp
	@ subs	r0, #16
	@ stmia	r0!,{r4-r7}
	@ mov	r4, r8
	@ mov	r5, r9
	@ mov	r6, r10
	@ mov	r7, r11
	@ subs	r0, #32
	@ stmia	r0!,{r4-r7}
	@ subs	r0, #16

	@ /* Save current task's SP: */
	@ ldr	r2, =os_curr_task
	@ ldr	r1, [r2]
	@ str	r0, [r1]

	@ /* Load next task's SP: */
	@ ldr	r2, =os_next_task
	@ ldr	r1, [r2]
	@ ldr	r0, [r1]
	