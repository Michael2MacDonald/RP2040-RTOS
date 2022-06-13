#include <kernel.h>
#include <cm0plus.h>
#include <include/ArmUtil.h>

#include <Scheduler.h>

#include <gpio.h>
#include <uart.h>

#include <cstring>

// #include <cstdio>
// #include <cstdlib>
// #include <stdio.h>
// #include <cstdio>
// #include <iostream>

// #define uint32_t unsigned long

#define LED 13        // Onboard LED
#define STROBE 13     // (Onboard LED) Vessel Strobe LED
#define NAVLIGHTS 14  // Vessel side lights, masthead light, and stern light
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
	gpio_set(LED, HIGH);
	delay(1000);
	gpio_set(LED, LOW);
	delay(1000);
}
extern "C" int blink_led_loop() {
	while(1) {
		gpio_set(LED, !gpio_out_get(LED));
		delay(1000);
	}
}

namespace Kernel {
	extern Scheduler* Sched;
}


int int2char(int64_t num, char* out){
	if (sizeof(out) < 21) { return 1; } // Not enough space for 64-bit int plus null terminator
	for (uint8_t i = 0; i < sizeof(out); i++) out[i] = '\0'; // Clear buffer

	// bool neg = (num<0);
	// if (neg) out[0] = '-'; // Add negative sign

	for (int i=0; i<20; i++) {
		// out[i+neg]=(char)((num%10)|48); // (i+neg offset index to allow for negative sign) (ASCII '0' = 48)
		// out[i]=(char)((num%10)+'0'); // (i+neg offset index to allow for negative sign) (ASCII '0' = 48)
		out[i]='0'; // (i+neg offset index to allow for negative sign) (ASCII '0' = 48)
		num /= 10;
		if (num <= 0) break; // Break if no more digits
	}

	return 0; // success
}

extern char * dtoa(char *s, double n);
float VesselHeading;
extern "C" int heartbeat() {
	while(1) {
		char message[58] = "Vessel Status: Operational\n\rHeading: ";
		// char message[100] = "I am alive!\nCurrent heading: ";
		// strcat((char*)&message, dtoa(message, VesselHeading));
		// strcat((char*)&message, "\n");
		// message[38] = (char)(53); // '5'
		uart_write(UART0, (char*)&message, sizeof(message));

		// char headingStr[20];
		// int2char(VesselHeading, (char*)&headingStr);
		// int2char(5, (char*)&headingStr);
		// uart_write(UART0, (char*)&headingStr, 1);
		// for (int i=0; i < sizeof(message); i++) message[i] = '\0'; // Clear value
		// message[0] = (char)(53); // '5'
		// uart_write(UART0, (char*)&message, sizeof(message));
		Kernel::Sched->sleep(5000);
	}
}


extern "C" int strobe_light() {
	while(1) {
		gpio_set(STROBE, HIGH);
		Kernel::Sched->sleep(250);
		gpio_set(STROBE, LOW);
		Kernel::Sched->sleep(2000);
	}
	// while(1) {
	// 	gpio_set(LED, !gpio_out_get(LED));
	// 	Kernel::Sched->sleep(3000);
	// }
}

extern double GetHeading();
// simulate continuously calculating the heading (this is a non time-sensitive task)
extern "C" int fetch_heading_thread() {
	while(1) {
		VesselHeading = GetHeading();
		// printf("Heading: %f\n", heading);
	}
}

extern "C" void init() {
	// Create thread to send heartbeat messages to mission control every 5 seconds
	Kernel::Sched->create("heart", 512, &heartbeat, Kernel::critical);
	// Create thread to drive the strobe light (the onboard led)
	Kernel::Sched->create("strobe", 0, &strobe_light, Kernel::high); // stack size is 0 so it will default to 48 bytes
	// Create thread to continuously calculate heading from compass data
	Kernel::Sched->create("heading", 256, &fetch_heading_thread, Kernel::normal);

	// Setup strobe light
	gpio_init(STROBE, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(STROBE, GPIO_OUT); // Set pin as output
	gpio_set(STROBE, HIGH); // Set pin high on startup
	// Setup navigation lights
	gpio_init(NAVLIGHTS, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(NAVLIGHTS, GPIO_OUT); // Set pin as output
	gpio_set(NAVLIGHTS, HIGH); // turn on sidelights

	// SVC_Handler_Table[0] = (uint32_t)(&blink_led);
	// SVC_Handler_0 = (uint32_t)(&blink_led);

	// SVC_Handler_Table[1] = (uint32_t)(&blink_led_fast);
	// SVC_Handler_1 = (uint32_t)(&blink_led_fast);

	// SVC_Handler_8 = (uint32_t)(&blink_led_fast);
	// SVC_Handler_Table[8] = (uint32_t)(&blink_led);
}

uint32_t test[4] = {1,2,3,4};
extern "C" void loop() {
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

	// asm volatile ("mov r12, pc \n svc #0 \n");
	// asm volatile ("mov r12, pc \n svc #1 \n");


	// SVC(0,(uint32_t*)&test);
	// SVC(0);
	// SVC(1);
	// SVC(8,0);

	// blink_led();
	// blink_led_fast();
	delay(1000);
	asm volatile("nop"); // forever loop

/** IGNORE: ======== This is just random stuff ======== */
	// NOP consumes two clock cycles. A for loop takes about 24 instruction cycles I believe.
	// So (5000000*(24+2))/120000000 = 1.0833 seconds
	// and (2500000*(24+2))/120000000 = 0.5416 seconds
	// and (4615385*(24+2))/120000000 = 1.000000083 seconds
	// delay_nop(5000000);
	// uart_write(UART0, (char *)&Ticks, 5);
	// uart_write(UART0, "\r\n", 3);

} /** END: loop() */
