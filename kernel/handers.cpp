#include "kernel.h"
#include <gpio.h>
#include <uart.h>

#include <Scheduler.h>


extern "C" void blink_led();

extern "C" __attribute__((used, weak))
void Default_Handler() { // Gets overwritten by strong definition in assembly file
	return;
}

extern "C" __attribute__((used, weak, alias("Default_Handler")))
void NMI_Handler();

// __attribute__((used, weak, alias("Default_Handler")))
extern "C" __attribute__((used, weak))
void HardFault_Handler() {
	// Setup LED GPIO pins
	gpio_init(13, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(13, GPIO_OUT); // Set pin as output
	while(1){
		gpio_set(13, HIGH);
		delay_nop(2000000);
		gpio_set(13, LOW);
		delay_nop(2000000);
	}
}

// extern "C" __attribute__((used, weak))
// void PendSV_Handler() {
// 	asm volatile ("mov r12, pc \n svc #0 \n");
// 	// gpio_set(13, HIGH);
// 	// delay(1000);
// 	// gpio_set(13, LOW);
// 	// delay(1000);
// }


// extern "C" uint32_t Ticks;
extern volatile uint32_t Ticks, Uptime, Millis, Micros;

extern "C" __attribute__((weak))
void systick_hook() {
	return;
}

extern "C" __attribute__((used))
void SysTick_Handler() { // Runs every 1ms as defined in startup.c

	Ticks++; // Increment Ticks for delays and other functions
	/** TODO: Increment Millis and Micros based off of ticks and the Systick interval */
	Uptime++; // Increment Uptime for delays and other functions
	Millis++; // Increment Millis for delays and other functions
	Micros++; // Increment Micros for delays and other functions

	if (Kernel::Sched->enabled == true && ((SCB->ICSR & SCB_ICSR_PENDSVSET_Msk) >> SCB_ICSR_PENDSVSET_Pos) != 1) { // only run if scheduler is enabled and no PendSV interrupt is pending
	// if (Kernel::Sched->enabled == true) { // only run if scheduler is enabled and no PendSV interrupt is pending
		// Check for the highest priority thread and switch context
		PendSV_Trigger(); // Call PendSV to switch context
	}

	systick_hook(); // Call the user defined systick hook

}








extern "C" __attribute__((used, weak))
// void SVCall_Handler(uint32_t *stackPtr, uint8_t svc_num, void *args) {
void SVCall_Handler(uint8_t svc_num, uint32_t *args, uint32_t *stackPtr) {
	/**
	 * Note: r12 may be used as a scratch register and can not be relied on by the user program
	 * or the SVC function
	 * 
	 * - Get the link register
	 * - Get the 'hard-coded' SVC number from the SVC instruction located at [lr-2]
	 * - Use the top 3 bits of the 'hard-coded' SVC number to determine how to handle
	 *   arguments and what regesters must be preserved for the function calling SVC
	 *    - [7:6] 0b00 = no args, passes (r0) with (sp) value, perserves r0-r3
	 *    - [7:6] 0b00 = no args, passes (r0) with (sp) value, returns (r0), perserves r1-r3
	 *    - [7:6] 0b01 = 1 arg, passes & returns (r0), perserves r1-r3
	 *    - [7:6] 0b10 = 2 args, passes & returns (r0-r1), perserves r2-r3
	 *    - [7:6] 0b11 = 4 arguments, passes (r0-r3), returns (r0-r1), no registers preserved
	 *    - (r4-r7) are always preserved
	 * - Use the bottom 6 bits of the 'hard-coded' SVC number to determine which SVC
	 *   function to call. This allows for 64 SVC functions to be defined.
	 * 
	 * - Fetch the SP if nessary
	 * - Push the required registers onto the stack to preserve them
	 * 
	 */

	// asm(
	// 	"ldr R4,[lr,#-4] \n"        // Calculate address of SVC instruction and load it into R0.
	// 	"bic R4,R4,#0xFF000000 \n"  // Mask off top 8 bits of instruction to give SVC number.
	// );
	// asm(
	// 	".global SVCall_Handler_Main\n"
	// 	"MRS r0, MSP\n" // Set r0 to the main stack pointer
	// 	"B SVCall_Handler_Main\n" // branch to SVCall_Handler_Main without a link
	// );
	// asm("MRS r2, MSP"); // Set r0 to the main stack pointer
	
	/*
	* Stack contains:
	* r0, r1, r2, r3, r12, r14 (link regester), and xPSR
	* I think this is all wrong
	*/
	// Find the SVC number by going to the return address in stack and reading the two bytes before that address (which is the last two bytes of the SVC call instruction)
	// register uint32_t svc_instr_addr asm("r4"); // Holds the address of the SVC instruction
	// register uint32_t svc_instr asm("r5");      // Holds the SVC instruction (raw machine code)
	register uint32_t reg12 asm("r12");      // Holds the SVC instruction (raw machine code)
	// volatile uint32_t svc_instr_addr_var; // Holds the address of the SVC instruction
	// uint32_t svc_instr_var;      // Holds the SVC instruction (raw machine code)
	// uint32_t test_var;      // Holds the SVC instruction (raw machine code)
	asm volatile (
		// "ldr r5, [sp,#4] \n"  // r5 = lr value when pushed to stack
		// "mov %1,r12 \n"  // print address of SVC instruction
		// "sub r5, #4 \n"  // r5 -= 2 (r5 = lr - 2 = address of SVC instruction)
		// "mov r4, r5 \n"  // r4 = address of SVC instruction
		// "mov r4, r12 \n" // r4 = pc value right before SVC instruction ran (aka, addr of svc intr.)
		// "sub r4, #2 \n"  // r4 -= 2 (r4 = lr - 2 = address of SVC instruction)
		// "mov %0,r4 \n"   // Store the value in r4 (address of SVC instr.) into svc_instr_addr for debugging

		// "ldr r0, [r1,#0] \n"  // r0 = value at address in r1 with offset 0
		// "mov %0,r0 \n"
		// "add r0, #2 \n"  // r0 += 2
		// "sub r0, #2 \n"  // r0 -= 2
		"nop"
		// : "=r" (svc_instr_addr_var), "=r" (test_var) // %0 now holds the svc instruction and %1 now holds the address of the svc instruction
		// :: "r4", "r5", "r12" // Tell gcc that we use these registers so that it pushes them to stack and/or does not use them for anything else
	);

	// if(svc_instr_addr == 0x1000025a) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(40000000);
	// 	gpio_set(13, LOW);
	// }
	gpio_set(13, HIGH);
	// delay_nop(40000000);
	delay(1000);
	gpio_set(13, LOW);

	// char *test_var_ptr = (char*)((uint32_t)&test_var); // (print address of SVC instruction) pointer to svc_instr_addr which holds the address of svc instruction
	// uart_write(0, (char*)test_var_ptr, 4); // prints the value of test_var
	// uart_write(0, (char*)test_var, 4); // prints the value of svc_number


	// char *print_addr_ptr = (char*)((uint32_t)&svc_instr_addr_var); // (print address of SVC instruction) pointer to svc_instr_addr which holds the address of svc instruction
	// char *print_val_ptr_ptr = (char*)(svc_instr_addr_var); // (print raw SVC instruction) pointer to the address stored in svc_instr_addr (svc_instr_addr holds the address of svc instruction)
	// char *print_val_ptr = (char*)((uint32_t)&svc_instr_var); // (print raw SVC instruction) pointer to svc_instr which holds the raw SVC instruction
	// uart_write(0, print_addr_ptr, 16); // prints the value of svc_number
	// uart_write(0, print_addr_ptr, 4); // prints lr address
	// uart_write(0, (char*)svc_instr_addr_var, 4); // prints the value of svc_number
	// uart_write(0, (char*)reg12-2, 4); // prints the value of svc_number
	uart_write(0, (char*)reg12-2, 1); // prints the value of svc_number

	// Stack pointer = 0x20041fd4
	// [0x20041fd0] = 0x00000000
	// [0x20041fd4] = 0x10000218
	// [0x20041fd8] = 0x20041f01
	// [0x20041fdc] = 0x18000000
	// [0x20041fe0] = 0x00000000
	// [0x20041fe4] = 0x10000477 (this should be the LR saved to stack from the "SVC" function)
	
	// PC before SVC = 0x10000258
	// [0x10000256] = 0xbdf0dfdb (db df f0 bd)
	// [0x10000258] = 0x0256bdf0 (f0 bd 56 02)

	// [lr in svc mode] = 0xfffffff9
	// [pc in svc mode] = 0x100002b6
	
	// [0x20041fe8] = 0x10000218
	// [0x20041fd4] = 

	// uint32_t svc_number = ((char*)stackPtr[6])[-2];
	// uart_write(0, (char*)((uint32_t)&svc_number), 4); // prints the value of svc_number

	// uint32_t *test_ptr_1 = (uint32_t*)args; // Points to test
	// *test_ptr_1 = stackPtr; // Sets print_me value to the address of test
	// uint32_t test_1 = (uint32_t)stackPtr;
	// char *print_addr_1 = (char*)&test_1; // Points to test

	// volatile uint32_t returnAddr = stackPtr[6]; // Points to test
	// returnAddr -= 2;
	// char *testing = (char*)(&returnAddr);
	// char *testing = (char*)(0x100002a4);
	// uart_write(0, testing, 2); // prints the address of test
	// uart_write(0, (char*)(returnAddr), 2); // prints the address of test

	// gpio_set(13, HIGH);
	// delay_nop(40000000);
	// gpio_set(13, LOW);

	// if (svc_num == 0) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(20000000);
	// 	gpio_set(13, LOW);
	// }
	// else if (svc_num == 1) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(40000000);
	// 	gpio_set(13, LOW);
	// }
	// else if (svc_num == 2) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(80000000);
	// 	gpio_set(13, LOW);
	// }
	// else if (svc_num == 5) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(40000000);
	// 	gpio_set(13, LOW);
	// }
	// else {
	// 	// Unknown SVC number
	// }


	// ==================== WORKS ====================
	// creates pointer using args (points to test[4]) and sets the test[0] value to the address of test
	// creates a char pointer and uses it to print test[0] which should be the address of test
	// ====================
	// uint32_t *test_ptr; // Points to test
	// test_ptr = (uint32_t*)args; // Point it to test
	// *test_ptr = args; // Sets print_me value to the address of test
	// char *print_addr = (char*)args; // Points to test
	// uart_write(0, print_addr, 4); // prints the address of test
	// ===============================================

	// uart_write(0, (char*)args, 4); // prints the value of test[0]

	// uint32_t *CheckAddArray = (uint32_t*)args; // {val_1, val_2, sum}
	// uint32_t res = CheckAddArray[0] + CheckAddArray[1];
	// if (res == CheckAddArray[2]) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(20000000);
	// 	gpio_set(13, LOW);
	// }
	// if (svc_number == 5) {
	// 	gpio_set(13, HIGH);
	// 	delay_nop(20000000);
	// 	gpio_set(13, LOW);
	// }
}