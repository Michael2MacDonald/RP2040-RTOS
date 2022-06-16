#include <kernel.h>

#include <Scheduler.h>

#include <gpio.h>
#include <uart.h>

// #include <print/Print.h>

#include <cstring>

#define LED 13        // Onboard LED
#define STROBE 13     // (Onboard LED) Vessel Strobe LED
#define NAVLIGHTS 14  // Vessel side lights, masthead light, and stern light
#define UART0 0 // UART0


// extern void gpio_set(uint32_t, uint32_t);
// gpio_set(13, 0);


namespace Kernel {
	extern Scheduler* Sched;
}

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

extern char * dtoa(char *s, double n);
extern "C" int heartbeat() {
	while(1) {
		uart_print("I am alive!\n\r");
		// printf("Vessel Status: Operational\n");
		Kernel::Sched->sleep(5000);
	}
}

extern "C" int strobe_light() {
	while(1) {
		gpio_set(STROBE, HIGH);
		// Kernel::Sched->sleep(250);
		delay(250);
		gpio_set(STROBE, LOW);
		Kernel::Sched->sleep(2000);
	}
}

double VesselHeading;
extern double GetHeading();
// simulate continuously calculating the heading (this is a non time-sensitive task)
extern "C" int fetch_heading_thread() {
	while(1) {
		VesselHeading = GetHeading();
		// char msg[100];
		// dtoa(msg, VesselHeading);
		// uart_print("Vessel Heading: ");
		// uart_print(msg);
		// uart_print("\n");
		// printf("Heading: %f\n", VesselHeading);
		Kernel::Sched->sleep(5000);
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
}

// Main loop runs at the lowest priority when no other threads are running
extern "C" void loop() {
	// printf("Hello World\n");
	uart_print("Hello World\n\r");
	delay(1000);
	// blink_led();
} /** END: loop() */

// Rapid blink to indicate a hardfault has occurred (For debugging purposes)
extern "C" void HardFault_Handler() {
	// Setup LED GPIO pins
	gpio_init(13, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(13, GPIO_OUT); // Set pin as output
	while(1){ // Blink LED forever
		gpio_set(13, HIGH);
		delay_nop(2000000);
		gpio_set(13, LOW);
		delay_nop(2000000);
	}
}
