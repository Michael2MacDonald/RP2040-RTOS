
.syntax unified
.cpu cortex-m0plus
.thumb

#include "vector_table.h"

.section .reset, "ax"
.align 2

/** _reset:
 * 1. clears .bss and initialises .data
 * 2. jumps to begining of .start section (where _start function is located)
 * 3. in case of return _exit (which should eventually hang the processor via _exit)
 */
.global _reset
.type _reset,%function
.thumb_func
_reset:
	// Only core 0 should run the C runtime startup code; core 1 is normally
	// sleeping in the bootrom at this point but check to be sure
	@ ldr r0, =(SIO_BASE + SIO_CPUID_OFFSET)
	@ ldr r0, [r0]
	@ cmp r0, #0
	@ bne hold_non_core0_in_bootrom

	/**
	 * Each set of three entries in data_cpy_table represents a block of data to be copied
	 * from flash to RAM. The first entry is the destination address, the second is the source
	 * address, and the third is the address of the end of the source block.
	 * 
	 * We assume there is at least one entry (a set of 3 addresses) in the table.
	 */
	adr r4, data_cpy_table

1:
	ldmia r4!, {r1-r3}
	cmp r1, #0
	beq 2f
	bl data_cpy
	b 1b
2:
	// Zero out the BSS (This is done by gcc using __bss_start__ and __bss_end__ in the linker script)
	ldr r1, =_sbss
	ldr r2, =_ebss
	movs r0, #0
	b bss_fill_test
bss_fill_loop:
	stm r1!, {r0}
bss_fill_test:
	cmp r1, r2
	bne bss_fill_loop

platform_entry: // symbol for stack traces
	// Use 32-bit jumps, in case these symbols are moved out of branch range (e.g. if main is in SRAM and crt0 in flash)
	blx _startup // Branch to _startup function
	


data_cpy_loop:
	ldm r1!, {r0}
	stm r2!, {r0}
data_cpy:
	cmp r2, r3
	blo data_cpy_loop
	bx lr

.align 2

data_cpy_table:
.word _etext
.word _sdata
.word _edata
.word 0 // null terminator

.align 2


.section .startup, "ax"
.type _startup,%function
.weak _startup
.thumb_func
_startup:
1: // infinite loop
	b 1b




@ @ .global reset_handler
@ @ .section .text
@ @ .type	reset_handler,%function
@ @ reset_handler:
@ @ 	ldr r0, =0x20001000
@ @ 	mov sp, r0

@ @ 	bl init ; change to main?
@ @ 	b .

@ @ .align
@ @ .ltorg
@ @ .balign 0x100



@ @ .cpu cortex-m0
@ @ .thumb

@ .text
@ .align
@ .global Reset_Handler
@ .type Reset_Handler, %function
@ Reset_Handler:
@ 	b main

@ .section .isr_vectors
@ .align 2
@ .long __stack_top
@ .long Reset_Handler
@ .long NMI_Handler
@ .long HardFault_Handler
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long SVCall_Handler
@ .long 0
@ .long 0
@ .long PendSV_Handler
@ .long SysTick_Handler
@ .long 0

@ /* IRQ 0 */
@ .long SPI0_IRQ
@ .long SPI1_IRQ
@ .long 0
@ .long UART0_IRQ
@ .long UART1_IRQ
@ .long UART2_IRQ
@ .long 0
@ .long 0
@ /* IRQ 8 */
@ .long I2C0_IRQ
@ .long SCT_IRQ
@ .long MRT_IRQ
@ .long CMP_IRQ
@ .long WDT_IRQ
@ .long BOD_IRQ
@ .long 0
@ .long WKT_IRQ
@ /* IRQ 16 */
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ .long 0
@ /* IRQ 24 */
@ .long PININT0_IRQ
@ .long PININT1_IRQ
@ .long PININT2_IRQ
@ .long PININT3_IRQ
@ .long PININT4_IRQ
@ .long PININT5_IRQ
@ .long PININT6_IRQ
@ .long PININT7_IRQ
