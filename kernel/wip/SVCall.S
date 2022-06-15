.cpu cortex-m0plus
.syntax unified
.thumb

.extern blink_led
.extern SVCall_Handler_Main
.extern SVCall_Handler_Helper
.extern HardFault_Handler

.section .text

.global SVC
.type SVC,%function
.thumb_func
SVC:
// Done in SVC_Handler
@ #if LOW_REGS_ONLY
@ 	// Hack for cortex-m0plus and cortex-m0 (Only allows the use of r4-r7)
@ 	push {r4-r7, lr} // Push registers to stack
@ #else
@ 	// Push r4 though r8, r10, r11, and lr to the Stack (While these may not all be used, it is still a good idea to push them all incase a SVC funtion uses them)
@ 	push {r4-r8,r10,r11,lr} // 'stmdb sp!, <reg>' is equivalent to 'push <reg>'
@ #endif
	push {r4, lr} // Push registers to stack

	// Modify the SVC instruction to use the user defined SVC number
	@ ldr r4,=svc_instr // Load the address of the SVC instruction into r4
	@ strb r0,[r4, #3] // Store the SVC number into the SVC instruction (Store byte)
	dsb
	isb

	ldr r4, =svc_instr // r4 = address of svc_instr
	blx r4 // Call the SVC instruction

	// Revert the SVC instruction back to a predictable state
	@ ldr r4,=svc_instr // Load the address of the SVC instruction into r3
	@ movs r5,#0 // Load 0 (The new SVC number) into r0
	@ strb r5,[r4, #3] // Store the SVC number (byte) into the SVC instruction
	dsb
	isb

	pop {r4, pc} // Pop registers from stack (Immediately return by popping lr to pc)
@ #if LOW_REGS_ONLY
@ 	// Hack for cortex-m0plus and cortex-m0 (Only allows the use of r4-r7)
@ 	pop {r4-r7, pc} // Pop registers from stack (Immediately return by popping lr to pc)
@ #else
@ 	pop {r4-r8,r10,r11,pc} // Pop r4 though r8, r10, r11, and lr from the Stack
@ #endif

/** END: SVC */


.global SVCall_Handler
.type SVCall_Handler,%function
.thumb_func
SVCall_Handler:
#if LOW_REGS_ONLY
	// Hack for cortex-m0plus and cortex-m0 (Only allows the use of r4-r7)
	push {r4-r7, lr} // Push registers to stack
	mrs r4, psr      // Get program status register
	push {r4}        // Store psr onto stack. Only required for nested SVCs.
#else
	// Push r4 though r8, r10, r11, and lr to the Stack (While these may not all be used, it is still a good idea to push them all incase a SVC funtion uses them)
	push {r4-r8,r10,r11,lr} // 'stmdb sp!, <reg>' is equivalent to 'push <reg>'
#endif

	@ mrs r0, msp // Set msp to r0
	@ @ adds r0, #48 // Add 48 to msp to get the return address of the handler
	@ adds r0, #4 // Add 48 to msp to get the return address of the handler
	@ ldr r0, [r0, #0] // Load the return address to r0
	@ subs r0, #2
	@ ldr r0, [r0, #0] // Load the value of the svc instruction to r0
	@ bx lr


// r12 should hold the value of pc directly before the SVC instruction (basically the lr or return address)
#if LOW_REGS_ONLY
	// Get SVC number
	mov r4, r12   // Move r12 to a low register
	subs r4, #2   // r4 -= 2 (Get the address of SVC instruction)
	ldrb r4, [r4] // r4 = get SVC number
	mov r12, r4   // Move the SVC number back to r12
	@ movs r4, r0

	// Use the SVC number to generate an offset for SVC_Handler_Table (svc_num * 4) to get the address of the SVC handler function from SVC_Handler_Table
	// The entry at SVC_Handler_Table+offset is the address of the SVC handler function for the SVC number
	ldr r5, =SVC_Handler_Table // r5 = address of SVC_Handler_Table
	// Multiply SVC number by the size of each SVC_Handler_Table entry (4 bytes) to get offset
	lsls r4, r4, #2  // r4 = r4 * 4 (Logical Shift Left: x<<2 is the same as x*4)
	ldr r5, [r5, r4] // Get SVC handler address at the address [SVC_Handler_Table + offset]
#else
	// Get SVC number
	sub r12, #2     // r12 -= 2 (Get the address of SVC instruction)
	ldrb r12, [r12] // r12 = get SVC number

	// Use the SVC number to generate an offset for SVC_Handler_Table (svc_num * 4) to get the address of the SVC handler function from SVC_Handler_Table
	// The entry at SVC_Handler_Table+offset is the address of the SVC handler function for the SVC number
	ldr r5, =SVC_Handler_Table // r5 = address of SVC_Handler_Table
	// Multiply SVC number by the size of each SVC_Handler_Table entry (4 bytes) to get offset
	lsls r12, r12, #2 // r12 = r12 * 4 (Logical Shift Left: x<<2 is the same as x*4)
	ldr r5, [r5, r12] // Get SVC handler address at the address [SVC_Handler_Table + offset]
#endif

	@ push {r7}
	@ mov r7, r0
	@ bl print
	@ pop {r7}

	blx r5 // branch to SVC handler function
	@ blx blink_led // Blink

	pop {r4} // Pop psr from stack
	msr APSR_nzcvq, r4 // Restore psr. Only required for nested SVCs.
#if LOW_REGS_ONLY
	// Hack for cortex-m0plus and cortex-m0 (Only allows the use of r4-r7)
	pop {r4-r7, pc} // Pop registers from stack (pop lr to pc to immediately return)
#else
	pop {r4-r8,r10,r11,pc} // Pop r4 though r8, r10, r11, and lr from the Stack
#endif

/** END: SVCall_Handler */

@ .extern uart_write
@ print:
@ 	push {r0-r3, lr} // Push registers to stack
@ 	push {r7}
@ 	movs r0, #0
@ 	mov r1, sp
@ 	movs r2, #4
@ 	blx uart_write
@ 	pop {r7}
@ 	pop {r0-r3, pc} // Pop registers from stack

.global SVC_Handler_Default
.thumb_func
SVC_Handler_Default:
	@ push {lr}
	@ bl blink_led_addr
	@ pop {pc}
	bx lr
@ .align 2
@ blink_led_addr: .word blink_led


.macro svc_handler_stubX number
	.global SVC_Handler_\number
	SVC_Handler_\number: .word SVC_Handler_Default
.endm

// Array of SVC handler addresses
.section .data // Put array in .data.svc_table section so handler addresses can be set at runtime
.global SVC_Handler_Table
SVC_Handler_Table:   // Array of SVC handler addresses
@ SVC_Handlers_Kernel: // Start of handlers reserved for the kernel
.altmacro
.set i,0
.rept 256
	@ .if %i == 32 // Reserve the first 32 SVCs for the kernel
	@ 	SVC_Handlers:
	@ 	SVC_Handlers_User:
	@ .endif
	svc_handler_stubX %i
	.set i, i+1
	// Should create a table of SVC handler addresses that look like this: "SVC_Handler_[num]: .word [default handler addr]"
.endr


// Get SVC instruction address
@ .align 4
@ .global svc_instr_addr
@ @ svc_instr_addr: .word svc_instr
@ svc_instr_addr: .word 0x20000000
@ .align 4

@ .section .livetext, "x" // Put it in data so that it can be edited (ax means alicatable and executable) 
.section .text

// SVC instruction can be edited, allowing any svc value to be used
.global svc_instr // Make it global so it can be edited and accessed by anyone
.type svc_instr,%function
.thumb_func
svc_instr:
	// .short 0xDF00 // SVC instruction as machine code
	push {lr} // Push lr to stack
	mov r12, pc
	svc #0
	pop {pc} // Pop lr from stack into pc to return
	@ bx lr


// bx lr = 0x1E = 0b00011110
// blx lr = 0x3E = 0b00111110

// bx pc = 0x4778 = 0b010001110 1111 000 (same as bx r15)
// bx lr = 0x4770 = 0b010001110 1110 000 (same as bx r14)
// bx sp = 0x4768 = 0b010001110 1101 000 (same as bx r13)
// bx ip = 0x4760 = 0b010001110 1100 000 (same as bx r12)
// bx fp = 0x4758 = 0b010001110 1011 000 (same as bx r11)
// ...
// bx r2 = 0x4710 = 0b010001110 0010 000 (same as bx r14)
// bx r1 = 0x4708 = 0b010001110 0001 000 (same as bx r14)
// bx r0 = 0x4700 = 0b010001110 0000 000 (same as bx r14)




/** BL: Branch w/ Link (long branch with link format)
 * 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 * 1  1  1  1  H  [       Offset       ]
 * 
 * Made of 2 instructions:
 * (H = 0) In the first instruction the Offset field contains
 *         the upper 11 bits of the target address
 * (H = 1) In the second instruction the Offset field contains
 *         the lower half (11 bits) of the target address. The
 *         address of the instruction following the BL is
 *         placed in LR and bit 0 of LR is set.
 * 
 * 
 */

/** BX: Branch and Exchange (Hi register operations/branch exchange)
 * 15 14 13 12 11 10 9  8 7  6  5  4  3 2  1  0
 * 0  1  0  0  0  1  [Op] H1 H2 [Rs/Hs] [Rd/Hd]
 * 
 * Op H1 H2 [THUMB]  Action
 * 11 0  0  [BX Rs]  BX Rs Perform branch (plus optional state change) to address in a register in the range 0-7.
 * 11 0  1  [BX Hs]  BX Hs Perform branch (plus optional state change) to address in a register in the range 8-15.
 * 
 * Bit 0 of the address determines the processor state on entry to the routine: 0 = ARM, 1 = THUMB.
 * 
 */



@ @ stmfd   sp!, {r0-r3, r12, lr}  ; Store registers (fd = Full descending)
@ push {r0-r3, lr}  ; Push registers to stack
@ mov r1, sp                     ; Set pointer to parameters
@ mrs r0, spsr                   ; Get spsr
@ push {r0, r3}            ; Store spsr onto stack. Only required for nested SVCs.

@ ldrh r0, [lr,#-2]              ; Load halfword
@ bic r0, r0, #0xFF00            ; Extract comment field (Bit clear: r0 &= ~0xFF00)


@ ; r0 now contains SVC number
@ ; r1 now contains pointer to stacked registers

@ bl C_SVC_Handler          ; Call main part of handler
@ pop {r0}          ; Pop spsr from stack
@ msr SPSR_cxsf, r0          ; Restore spsr
@ pop {r0-r3, pc}         ; Restore registers and return




@ 	adds	r0, r0, #0
@ 	adds	r1, r1, #0
@ 	ldr	r2, [pc, #20] // (100002b8 <svc_instr_addr+0x6>)
@ 	strb	r0, [r2, #0]
@ 	mrs	r2, MSP

@ svc_instr:
@ 	.short	0xdf00

@ 	ldr	r2, [pc, #12] // (100002b8 <svc_instr_addr+0x6>)
@ 	movs	r0, #0
@ 	strb	r0, [r2, #0]
	
@ 	bx lr
