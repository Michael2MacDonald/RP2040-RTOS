
#include "core.h"
#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "clock.h"


void gpio_setup() { // rename?? io_setup?? pads_setup??
	reset_release_wait(RESET_IO_BANK0); // Release reset for IO
	reset_release_wait(RESET_PADS_BANK0); // Release reset for Pads
}

/** TODO: Move to UART file and add setup functions */
void uart_setup() {
	reset_release_wait(RESET_UART0); // Release reset for UART0
	uart_init(0, 115200); // Baud rate = 115200
	// uart_init(0, 9600); // Baud rate = 9600
	gpio_init(0, GPIO_FUNC_UART); // RX
	gpio_init(1, GPIO_FUNC_UART); // TX
	gpio_dir(0, 1); // Set as output
}

void usb_setup() {
	reset_release_wait(RESET_PLL_USB); // Release reset for PLL_USB
	reset_release_wait(RESET_USBCTRL); // Release reset for USBCTRL
}

// extern "C" uint8_t GetCore();

void core_startup() {
	/** TODO: Implement GetCore() function */
	// if (GetCore() != 0) { // If this is the default core
	// }

	clock_setup(); // Setup clock, PLL, etc.
	gpio_setup();  // Setup GPIOs
	uart_setup();  // Setup UART
}


// TODO: Setup CAN bus clock output (CLOCK_GPOUT1)
// div = (uint32_t) (((uint64_t) src_freq << 8) / freq);
// clock->div = div;


//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
// static void cdc_task(void) {
//   uint8_t itf;

// 	for (itf = 0; itf < CFG_TUD_CDC; itf++) {
// 		// connected() check for DTR bit
// 		// Most but not all terminal client set this when making connection
// 		// if ( tud_cdc_n_connected(itf) )
// 		{
// 			if ( tud_cdc_n_available(itf) ) {
// 				uint8_t buf[64];

// 				uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

// 				// echo back to both serial ports
// 				echo_serial_port(0, buf, count);
// 				echo_serial_port(1, buf, count);
// 			}
// 		}
// 	}
// }