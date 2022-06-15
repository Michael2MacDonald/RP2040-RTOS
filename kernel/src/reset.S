@ #include "vector_table.h"

.syntax unified
.cpu cortex-m0plus
.thumb

.extern _startup // Runs core startup, kernel startup, and jumps to main()


/** Reset_Handler:
 * 1. Clears .bss and initialises .data
 * 2. Jumps to _startup function
 * 3. In case of return, hang the processor
 */
.section .reset, "ax"
.align 2
.global Reset_Handler
.type Reset_Handler,%function
.thumb_func
Reset_Handler:
	// Only core 0 should run the C runtime startup code; core 1 is normally
	// sleeping in the bootrom at this point but check to be sure
	@ ldr r0, =(SIO_BASE + SIO_CPUID_OFFSET)
	@ ldr r0, [r0]
	@ cmp r0, #0
	@ bne hold_non_core0_in_bootrom

	@ dsb
	@ isb
	@ // Set PRI_14 (PendSV) to 0b11 [=0xC0] (lowest priority) [23:22]
	@ LDR     r1,=0xE000ED20  // System Handler Priority Register 3 Addr
	@ MOVS    r2,#0x03        // 0x03 = 0b00000011
	@ LSLS    r2,r2,#22       // Shift value to bits [23:22] (0b11 << 22)
	@ STR     r2,[r1]         // Write the value of R2 to SHPR3
	@ dsb
	@ isb

	bl initialize_data // Initialize .data section
	bl zero_bss        // Zero .bss section
	bl fill_trap       // Fill .trap with 0xDEADBEEF

	@ movs r0, #2
	@ msr control, r0 // set CONTROL.SPSEL to 1 (psp)
	@ mrs r0, msp
	@ msr psp, r0
	dsb
	isb

platform_entry: // symbol for stack traces
	// Use 32-bit jumps, in case these symbols are moved out of branch range (e.g. if main is in SRAM and crt0 in flash)
	blx _startup // Branch to _startup function

hang: // In case of application return, hang the processor
	wfi // Sleep to save power
	@ bkpt #255 // Breakpoint for debugging
	b hang // Infinite loop
	nop




/** 
 * Copy data and text from flash to RAM.
 */
initialize_data:
	push {lr} // Save the lr to return to the reset function
	adr r4, data_cpy_table // Load the data_cpy_table address into r4
1:
	ldmia r4!, {r1-r3} // Load the destination, start, and end addresses of the block of data to be copied
	cmp r1, #0         // Check if the first address in the block is 0
	beq 2f             // If it is then we are done copying data
	bl data_cpy        // Otherwise call the data_cpy function
	b 1b               // Loop: Check if there is another block of data to copy
2: // We are done copying data
	pop {pc} // Return to the reset function
	nop

data_cpy_loop:
	ldm r1!, {r0}
	stm r2!, {r0}
data_cpy:
	cmp r2, r3
	blo data_cpy_loop
	bx lr

/**
 * Each set of three words in data_cpy_table represents a block of data to be copied
 * from Flash to RAM. The first word is the destination address, the second is the source
 * address, and the third is the address of the end of the source block.
 * 
 * data_cpy_table holds the destination start address and
 * the source start and end addresses for every block of
 * data to be copied from Flash to RAM.
 */
.align 2
data_cpy_table:
	// Copy .text.fastcode section (FASTRUN code)
	.word _efastcode
	.word _sfastcode
	.word _efastcode
	// Copy .data section (initialized data)
	.word _etext
	.word _sdata
	.word _edata
	// Null terminator
	.word 0
.align 2




zero_bss: // Zero out the BSS
	ldr r1, =_sbss
	ldr r2, =_ebss
	@ ldr r2, =_stack
	movs r0, #0x00
	b bss_fill_test
bss_fill_loop:
	stm r1!, {r0}
bss_fill_test:
	cmp r1, r2
	bne bss_fill_loop
	bx lr // Return to the reset function




fill_trap: // Fill the stack/heap trap with 0xDEADBEEF for checking for overflows
	ldr r1, =_strap
	ldr r2, =_etrap
	ldr r0, =0xDEADBEEF
	b trap_fill_test
trap_fill_loop:
	stm r1!, {r0}
trap_fill_test:
	cmp r1, r2
	bne trap_fill_loop
	bx lr // Return to the reset function








	/** Set The System Handler Priority Registers: SHPR1 (Reserved), SHPR2, and SHPR3
	 * R1 is the SHP register base address
	 * R2 is the current value of the SHP register (used to modify a single priority without changing the other priorities)
	 * 
	 */

	@ MRS     r0,PRIMASK      // Store the state of the PRIMASK in r0
	@ CPSID   i               // Disable interrupts (set PRIMASK)

	@ LDR     r1,=0xE000ED18  // System Handler Priority Register Base Addr

	@ @ LDR     r2,[r1,#8]      // Load the value of SHPR3 (System Handlers 12-15)
	@ @ MOVS    r3,#0xFF
	@ @ LSLS    r3,r3,#16
	@ @ ORRS    r2,r3           // set PRI_14 (PendSV) to 0xFF                */
	@ @ STR     r2,[r1,#8]      // write the System 12-15 Priority Register   */

	@ // Load the value of SHPR2 (System Handlers 8-11)
	@ LDR     r2,[r1,#4]

	@ // Set PRI_11 (SVCall) to 0b11 [=0xC0] (lowest priority) [31:30]
	@ MOVS    r3,#0x03        // 0x03 = 0b00000011
	@ LSLS    r3,r3,#30       // Shift value to bits [31:30] (0b11 << 24) = (0xC0000000) = (0b11000000,00000000,00000000,00000000)
	@ ORRS    r2,r3           // Set Bits
	@ @ BICS    r2,r3           // Clear Bits

	@ // Write the value of R2 to SHPR2
	@ STR     r2,[r1,#4]

	@ // Re-enable Interrupts:
	@ // Use MSR to restore PRIMASK to its original value, that we previously
	@ // saved into R0, before we disabled inturupts. This is perfered to
	@ // re-enabling the interupts with CPSIE because the interrupts have been
	@ // disabled before we disabled them (again) so we don't want to enable them
	@ MSR     PRIMASK,r0      // Restore the original PRIMASK


