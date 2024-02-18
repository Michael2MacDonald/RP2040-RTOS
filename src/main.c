#include <kernel.h>
#include <Scheduler.h>

#include <gpio.h> // For GPIO
#include <uart.h> // For UART/Serial

#define STROBE 13     // (Onboard LED) Vessel Strobe LED
#define NAVLIGHTS 14  // Vessel side lights, masthead light, and stern light
#define UART0 0       // UART0

// Integer to string conversion from 'src/itoa.c'
extern char * itoa(int, char*, int);


// void blink_led() {
// 	gpio_set(13, HIGH); delay(1000);
// 	gpio_set(13, LOW); delay(1000);
// }
// int blink_led_loop() {
// 	while(1) {
// 		gpio_set(13, !gpio_out_get(13));
// 		delay(1000);
// 	}
// }


// Send a heartbeat message every 5 seconds so that mission control knows we're still alive
int heartbeat() {
	while(1) {
		uart_print("==================================================\n\r");
		uart_print("Heartbeat: All systems functional!\n\r");
		thread_sleep(5000);
	}
}

// Short flash followed by long pause to warn passersby of the presence of the vessel
int strobe_light() {
	while(1) {
		gpio_set(STROBE, HIGH);
		thread_sleep(250); // Stay on for 250ms
		gpio_set(STROBE, LOW);
		thread_sleep(2000); // Stay off for 2 seconds
	}
}

// Calculate the heading of the vessel for navigation purposes
// We are using fake data here but the calculations still take a while to complete
// Before I created this RTOS, this calculation caused timing issues with fetching the GPS serial data
double VesselHeading; // Global variable to store the heading so that it can be grabbed by the navigation system
extern double GetHeading();
int fetch_heading_thread() {
	while(1) {
		VesselHeading = GetHeading(); // Run time consuming calculations on sensor data to get heading

		int i = (int)(VesselHeading + 0.5); // Cast to int; Add 0.5 to round instead of truncate
		char buf[5]; itoa(i, buf, 10); // Convert the heading to a string in decimal format

		uart_print("==================================================\n\r");
		uart_print("Vessel Heading: ");
		uart_print(buf);
		uart_print("\n\r");

		thread_sleep(2000); // Get data every 2 seconds
	}
}


// Initialization function
void init() {
	// Create thread to send heartbeat messages to mission control every 5 seconds
	thread_create("heart", 640, &heartbeat, critical);
	// Create thread to drive the strobe light (the onboard led)
	thread_create("strobe", 0, &strobe_light, high); // stack size is 0 so it will default to 48 bytes
	// Create thread to continuously calculate heading from compass data
	//thread_create("heading", 256, &fetch_heading_thread, normal);

	// Setup strobe light
	gpio_init(STROBE, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(STROBE, GPIO_OUTPUT); // Set pin as output
	gpio_set(STROBE, HIGH); // Set pin high on startup
	// Setup navigation lights
	gpio_init(NAVLIGHTS, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(NAVLIGHTS, GPIO_OUTPUT); // Set pin as output
	gpio_set(NAVLIGHTS, HIGH); // turn on sidelights
}

// Main loop runs at the lowest priority; i.e. when no other threads are running
void loop() {
	// Print a sensor report every 10 seconds
	uart_print("==================================================\n\r");
	uart_print("Sensor Report:\n\r");
	uart_print("	Temp: 30 C\n\r");
	uart_print("	Humidity: 46%\n\r");
	thread_sleep(10000);
}




// Rapid blink to indicate a hardfault has occurred (For debugging purposes)
void HardFault_Handler() {
	// Setup LED GPIO pins
	gpio_init(13, GPIO_FUNC_SIO); // Set pin function to SIO
	gpio_dir(13, GPIO_OUTPUT); // Set pin as output
	while(1){ // Blink LED forever
		gpio_set(13, HIGH);
		delay_nop(1000000);
		gpio_set(13, LOW);
		delay_nop(1000000);
	}
}