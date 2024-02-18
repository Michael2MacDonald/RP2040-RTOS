.syntax unified
.cpu cortex-m0plus
.thumb

// Arm Cortex Handlers are defined in Kernel.cpp
.extern Reset_Handler
.extern NMI_Handler
.extern HardFault_Handler
.extern SVCall_Handler
.extern PendSV_Handler
.extern SysTick_Handler_core1

// =============== Vector Table ===============
.section .vectors1, "ax"
.align 2
.global vector_table_core1
vector_table_core1:
	.word _stack_core1
	.word Reset_Handler
	.word NMI_Handler
	.word HardFault_Handler
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word SVCall_Handler
	.word Default_Handler_Core1 // Reserved, should never fire
	.word Default_Handler_Core1 // Reserved, should never fire
	.word PendSV_Handler
	.word SysTick_Handler_core1
	.word isr_irq0
	.word isr_irq1
	.word isr_irq2
	.word isr_irq3
	.word isr_irq4
	.word isr_irq5
	.word isr_irq6
	.word isr_irq7
	.word isr_irq8
	.word isr_irq9
	.word isr_irq10
	.word isr_irq11
	.word isr_irq12
	.word isr_irq13
	.word isr_irq14
	.word isr_irq15
	.word isr_irq16
	.word isr_irq17
	.word isr_irq18
	.word isr_irq19
	.word isr_irq20
	.word isr_irq21
	.word isr_irq22
	.word isr_irq23
	.word isr_irq24
	.word isr_irq25
	.word isr_irq26
	.word isr_irq27
	.word isr_irq28
	.word isr_irq29
	.word isr_irq30
	.word isr_irq31

// =============== Define Interupt Handlers ===============
.macro decl_isr name
	.weak \name
	.type \name,%function
	.thumb_func
	\name:
.endm

decl_isr isr_irq0
decl_isr isr_irq1
decl_isr isr_irq2
decl_isr isr_irq3
decl_isr isr_irq4
decl_isr isr_irq5
decl_isr isr_irq6
decl_isr isr_irq7
decl_isr isr_irq8
decl_isr isr_irq9
decl_isr isr_irq10
decl_isr isr_irq11
decl_isr isr_irq12
decl_isr isr_irq13
decl_isr isr_irq14
decl_isr isr_irq15
decl_isr isr_irq16
decl_isr isr_irq17
decl_isr isr_irq18
decl_isr isr_irq19
decl_isr isr_irq20
decl_isr isr_irq21
decl_isr isr_irq22
decl_isr isr_irq23
decl_isr isr_irq24
decl_isr isr_irq25
decl_isr isr_irq26
decl_isr isr_irq27
decl_isr isr_irq28
decl_isr isr_irq29
decl_isr isr_irq30
decl_isr isr_irq31

// All unhandled USER IRQs fall through to here
.global Default_Handler_Core1
.type Default_Handler_Core1,%function
.thumb_func
Default_Handler_Core1:
	push {lr}
	bl get_current_exception_core1 // Get the current exception number
#if 0
.global unhandled_user_irq_num_in_r0
unhandled_exception_num_in_r0:
	bkpt #0
#else
	pop {r0}
	bx r0
#endif

// Get the current exception number, subtract 16, and store it in r0
.global get_current_exception_core1
.type get_current_exception_core1,%function
.thumb_func
get_current_exception_core1:
	mrs r0, ipsr // Read the current exception number
	uxtb r0, r0  // Zero Extend to 32 bits
	subs r0, #16 // Subtract 16 to get the exception number
	bx lr        // Return
