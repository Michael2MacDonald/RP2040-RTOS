.cpu cortex-m0plus
.syntax unified
.thumb

.extern enabled // is the scheduler enabled?

.extern _MAIN // main thread TCB pointer
.extern _FAST // main thread TCB pointer
.extern CurrentTCB // main thread TCB pointer
.extern blink_led
.extern main

.section .text
.align 2

@ .global PendSV_Handler
@ .type	PendSV_Handler,%function
@ .thumb_func
@ PendSV_Handler:
.global Sched_init
.type	Sched_init,%function
.thumb_func
Sched_init:
	/** NOTE: NVIC automatically r0-r3, r12, LR, PC, and xPSR */

	@ push {lr}
	@ bl blink_led
	@ pop {pc}

	cpsid i                 // Disable interrupts (set PRIMASK)

	@ b stop

	// Get address of the current TCB
	ldr r0, =CurrentTCB // Load the address of CurrentTCB pointer in register r3
	ldr r0, [r0]        // Load the address of the current TCB into register r3
	@ b stop

	// Restore Context (Load register values from thread stack)
	ldr r0, [r0] // Get the stack pointer from the TCB
	msr psp, r0  // Set the Program Stack Pointer (PSP) to the TCB stack pointer
	isb          // Instruction Synchronization Barrier (Clear pipeline after context change)

	@ movs r0, #2
	@ msr control, r0 // set CONTROL.SPSEL to 1 (psp)
	@ movs r0, #1
	@ msr control, r0 // set CONTROL.nPRIV to 1 (unprivileged mode)
	@ mrs r0, msp
	@ msr psp, r0

	ldr r0, =enabled // address of enabled pointer
	ldr r0, [r0] // address of enabled
	movs r1, #1
	str r1, [r0] // set enabled to 1

	@ push {lr}
	@ bl blink_led
	@ pop {pc}

	@ b stop

	@ ldr r0, =0xFFFFFFFD     // EXC_RETURN: Return to thread mode with PSP
	@ ldr r0, =0xFFFFFFF9     // EXC_RETURN: Return to thread mode with MSP
	@ ldr r0, =0xFFFFFFF1     // EXC_RETURN: Return to handler mode with MSP
	@ ldr r0, =main // Return to main thread
	dsb
	isb

	cpsie i      // Enable interrupts (clear PRIMASK)
	@ b stop

	@ bx r0                   // Return from the handler (Uses the values in stack to load regesters and gets everything ready to run) (Branch and exchange instruction set??)
	bx lr                   // Return from the handler (Uses the values in stack to load regesters and gets everything ready to run) (Branch and exchange instruction set??)


stop:
	b stop
	nop

@ blink_led_addr: .word blink_led


.section .data
.align 8
MAIN_TCB:
	.word 0x00000000 // Return value register, scratch register, or argument register
	.word 0x00000000 // Return value register, scratch register, or argument register
	.word 0x00000000 // Return value register, scratch register, or argument register
	.word 0x00000000 // Return value register, scratch register, or argument register
	.word 0x00000000 // Intra-Procedure-call scratch register
	@ .word =stop      // Return address
	@ .word =main      // Program counter
	.word 0x1000077c // Return address
	.word 0x1000245c // Program counter
	.word 0x01000000 // Program status register
@ END_MAIN_TCB:

	@ .word 0x01000000 // Program status register
	@ .word =main      // Program counter
	@ .word =stop      // Return address
	@ .word 0x00000000 // Intra-Procedure-call scratch register
	@ .word 0x00000000 // Return value register, scratch register, or argument register
	@ .word 0x00000000 // Return value register, scratch register, or argument register
	@ .word 0x00000000 // Return value register, scratch register, or argument register
	@ .word 0x00000000 // Return value register, scratch register, or argument register

	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef
	.word 0xdeadbeef








