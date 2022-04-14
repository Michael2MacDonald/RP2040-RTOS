#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "systick.h"

// #define uint32_t unsigned long

#define LED 13 // Onboard LED
#define UART0 0 // UART0

// Everything works currently!

/** PICK:
 * You can run two different programs:
 * 1. The first program shows the use of both delays functions to blink an LED.
 * 2. The second program runs a simple "blink without delay" program in the SysTick
 *    interrupt function while using a delay to print "Hello World" every 5 seconds
 *    in the main loop
 * 
 * NOTE: Option two requires you to uncomment two sections of code!
 */

void init() {
	// Setup LED GPIO
	gpio_init(LED, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(LED, GPIO_OUT); // Set pin as output
	// gpio_set(LED, HIGH); // Set pin high on startup
}

void loop() {
// ========== UNCOMMENT FOR OPTION ONE ==========
	// blink_with_two_delays();
// ==============================================

// ========== UNCOMMENT FOR OPTION TWO ==========
	// Print "Hello World!" to uart on every loop
	uart_write(UART0, "Hello World\r\n", 14);
	delay(5000); // Delay 5 seconds
// ==============================================

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
	static uint32_t count;
	if (count++ >= 1000) { // Every 1s
		gpio_set(LED, !gpio_out_get(LED)); // toggle LED
		count = 0; // reset counter
	}
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


int main() { // Main
	/** TODO: Setup RTOS */
	init();
	while(1){
		loop();
	}
	return 0;
}