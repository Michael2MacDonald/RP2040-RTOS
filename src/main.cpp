#include <kernel.h>
#include <cm0plus.h>
#include <include/ArmUtil.h>

#include <Scheduler.h>

#include <gpio.h>
#include <uart.h>

// #define uint32_t unsigned long

#define LED 13 // Onboard LED
#define UART0 0 // UART0

extern uint32_t SVC_Handler_Table[256];

/** PICK:
 * You can run two different programs:
 * 1. The first program shows the use of both delays functions to blink an LED.
 * 2. The second program runs a simple "blink without delay" program in the SysTick
 *    interrupt function while using a delay to print "Hello World" every 5 seconds
 *    in the main loop
 * 
 * NOTE: Option two requires you to uncomment two sections of code!
 */

extern "C" void blink_led() {
	gpio_set(13, HIGH);
	// delay_nop(40000000);
	delay(1000);
	gpio_set(13, LOW);
	// delay_nop(40000000);
	delay(1000);
}

extern "C" int blink_led_fast() {
	gpio_set(13, HIGH);
	delay_nop(10000000);
	// delay(1000);
	gpio_set(13, LOW);
	delay_nop(10000000);
	return 0;
}

extern "C" int blink_led_fast_loop() {
	while(1) {
		gpio_set(13, HIGH);
		delay_nop(10000000);
		// delay(1000);
		gpio_set(13, LOW);
		delay_nop(10000000);
	}
}

namespace Kernel {
	extern Scheduler* Sched;
}
volatile bool switch_signal;
extern "C" Kernel::TCB* CurrentTCB;

extern "C" __attribute__((optimize("Og")))//__attribute__((optimize("no-gcse")))
int blink_led_fast_loop_switch() {
	while(1) {
		gpio_set(13, HIGH);
		// delay_nop(10000000);
		delay(500);
		// delay(250);
		gpio_set(13, LOW);
		// delay_nop(10000000);
		delay(500);
		// delay(250);
	}
}
extern "C" __attribute__((optimize("Og")))//__attribute__((optimize("no-move-loop-invariants", "no-gcse")))
int blink_led_slow_loop_switch() {
	while(1) {
		// Kernel::Sched->sleep(3000);
		gpio_set(13, HIGH);
		// delay_nop(80000000);
		delay(3000);
		gpio_set(13, LOW);
		// delay_nop(20000000);
		// delay(3000);
		Kernel::Sched->sleep(3000);
	}
}

void init() {
	// __NVIC_SetPriority(PendSV_IRQn, 3);

	// Setup LED GPIO
	gpio_init(LED, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(LED, GPIO_OUT); // Set pin as output
	// gpio_set(LED, HIGH); // Set pin high on startup

	SVC_Handler_Table[0] = (uint32_t)(&blink_led);
	// SVC_Handler_0 = (uint32_t)(&blink_led);

	SVC_Handler_Table[1] = (uint32_t)(&blink_led_fast);
	// SVC_Handler_1 = (uint32_t)(&blink_led_fast);

	// SVC_Handler_8 = (uint32_t)(&blink_led_fast);
	// SVC_Handler_Table[8] = (uint32_t)(&blink_led);


	nvic_setpriority(PendSV_IRQn, 3);
	
}

void blink_with_two_delays();
uint32_t test[4] = {1,2,3,4};
void loop() {
// ========== UNCOMMENT FOR OPTION ONE ==========
	blink_with_two_delays();
// ==============================================

// ========== UNCOMMENT FOR OPTION TWO ==========
	// Print "Hello World!" to uart on every loop
	// uart_write(UART0, "Hello World\r\n", 14);
	// delay(5000); // Delay 5 seconds
// ==============================================
	// test[0] = 1;
	// test[1] = 2;
	// test[2] = 3;
	// test[3] = 4;
	// uart_write(UART0, (char*)test, 4); // prints the value of test[0]
	// uart_write(UART0, (char*)&test, 4); // prints the value of test[0]

	// uart_write(UART0, (char*)&SVC_Handler_Table, 4); // prints the value of test[0]

	// uint32_t *test_ptr; // Points to test
	// test_ptr = (uint32_t*)&test; // Point it to test
	// *test_ptr = (uint32_t)&test; // Sets print_me value to the address of test
	// char *print_addr; // Points to test
	// print_addr = (char*)&test; // Point it to test
	// uart_write(UART0, print_addr, 4); // prints the address of test

	// uart_write(UART0, (char*)(&SVC_Handler_Table[0]), 4);
	// uart_write(UART0, (char*)((uint32_t)&SVC_Handler_Table[1]), 4);
	
	// SCB->ICSR |= (1 << 28); // Set PendSV to pending

	// asm volatile ("mov r12, pc \n svc #0 \n");
	// asm volatile ("mov r12, pc \n svc #1 \n");


	// SVC(0,(uint32_t*)&test);
	// SVC(0);
	// SVC(1);
	// SVC(8,0);

	// delay(1000); // Delay 2 seconds
	// delay_nop(40000000);

	// blink_led();
	

/** IGNORE: ======== This is just random stuff ======== */
	// NOP consumes two clock cycles. A for loop takes about 24 instruction cycles I believe.
	// So (5000000*(24+2))/120000000 = 1.0833 seconds
	// and (2500000*(24+2))/120000000 = 0.5416 seconds
	// and (4615385*(24+2))/120000000 = 1.000000083 seconds
	// delay_nop(5000000);
	// uart_write(UART0, (char *)&TickCount, 5);
	// uart_write(UART0, "\r\n", 3);

} /** END: loop() */

void systick_hook() {
// ========== UNCOMMENT FOR OPTION TWO ==========
	// Blink LED every 1 second using the SysTick Interrupt
	// static uint32_t count;
	// if (count++ >= 1000) { // Every 1s
	// 	gpio_set(LED, !gpio_out_get(LED)); // toggle LED
	// 	count = 0; // reset counter
	// }
// ==============================================
}

void blink_with_two_delays() {
	/** Use delay() and delay_nop() to flash an LED
	 * 	delay() uses a counter incremented by systick to determine how long to delay
	 * 	delay_nop() uses a while loop to run a specified number of nops
	 */
	gpio_set(LED, LOW); // flash off
	delay(1000); // Delay for 1000 msec (1 second)
	gpio_set(LED, HIGH); // flash on
	delay_nop(40000000); // Delay for 1 second
	/** NOP takes 2 clock cycles and the while loop takes about 3 cycles (I think? Check blink.lst to be sure)
	 * 120MHz = 120000000
	 * 120000000/3 = 40000000 loops per second
	 * Now check our work:
	 * (40000000*3)/120000000 = 1 second
	 */
}


extern "C" //__attribute__((used, section(".mytext")))
int main() { // Main
	/** TODO: Setup RTOS */
	// init();
	while(1){
		// loop();
		blink_led();
	}
	return 0;
}