#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "xosc.h"
#include "systick.h"
#include "delay.h"


extern int main();

void clock_setup();
void gpio_setup();
void uart_setup();
void systick_setup();

__attribute__((noreturn, section(".startup"), optimize("no-tree-loop-distribute-patterns")))
void _startup(void) {
	clock_setup(); // Setup clock, PLL, etc.
	gpio_setup(); // Setup GPIOs
	uart_setup(); // Setup UART
	systick_setup(); // Setup systick

	main(); // Call main()
	while (1) asm("WFI"); // DON'T RETURN!!!!
}


void clock_setup() {
	xosc_init(); // Switch to external oscillator (xosc) and configure it

	// ==== Setup SYS PLL for (12 MHz * 40 / 4 / 1) = 120 MHz ====
	reset_release_wait(RESET_PLL_SYS); // Release reset for PLL_SYS
	// Load VCO-related dividers before starting VCO
	PLL_SYS->CS = (1 << PLL_SYS_CS_REFDIV_Pos); // Divide the PLL input reference clock
	PLL_SYS->FBDIV_INT = 40;
	// PLL_SYS->FBDIV_INT = 4;
	// Set up post dividers
	PLL_SYS->PRIM = (4 << PLL_SYS_PRIM_POSTDIV1_Pos) | (1 << PLL_SYS_PRIM_POSTDIV2_Pos);
	// Turn on PLL, VCO, And Post Dividers
	PLL_SYS->PWR &= ~(PLL_SYS_PWR_PD_Msk | PLL_SYS_PWR_VCOPD_Msk | PLL_SYS_PWR_POSTDIVPD_Msk); // Main power, VCO power, and Post Dividers power
	while ( 0 == ((PLL_SYS->CS & PLL_SYS_CS_LOCK_Msk) >> PLL_SYS_CS_LOCK_Pos) ); // Wait for PLL to lock

	// ==== Setup USB PLL for (12 MHz * 36 / 3 / 3) = 48 MHz ====
	reset_release_wait(RESET_PLL_USB); // Release reset for PLL_USB
	// Load VCO-related dividers before starting VCO
	PLL_USB->CS = (1 << PLL_SYS_CS_REFDIV_Pos);
	PLL_USB->FBDIV_INT = 36;
	// Set up post dividers
	PLL_USB->PRIM = (3 << PLL_SYS_PRIM_POSTDIV1_Pos) | (3 << PLL_SYS_PRIM_POSTDIV2_Pos);
	// Turn on PLL, VCO, And Post Dividers
	PLL_SYS->PWR &= ~(PLL_SYS_PWR_PD_Msk | PLL_SYS_PWR_VCOPD_Msk | PLL_SYS_PWR_POSTDIVPD_Msk); // Main power, VCO power, and Post Dividers power
	while ( 0 == ((PLL_SYS->CS & PLL_SYS_CS_LOCK_Msk) >> PLL_SYS_CS_LOCK_Pos) ); // Wait for PLL to lock

	// ==== Switch clocks to their final sources ====
	// Set source to 'xosc_clksrc' for reference clock
	CLOCKS->CLK_REF_CTRL = (CLOCKS_CLK_REF_CTRL_SRC_xosc_clksrc << CLOCKS_CLK_REF_CTRL_SRC_Pos);
	// Set auxiliary source to 'clksrc_pll_sys' for system clock
	CLOCKS->CLK_SYS_CTRL = (CLOCKS_CLK_SYS_CTRL_AUXSRC_clksrc_pll_sys << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos);
	// Set source to 'clksrc_clk_sys_aux' for system clock
	CLOCKS->CLK_SYS_CTRL = (CLOCKS_CLK_SYS_CTRL_SRC_clksrc_clk_sys_aux << CLOCKS_CLK_SYS_CTRL_SRC_Pos);
	// Enable clock for peripherals and set source to 'clk_sys'
	// CLOCKS->CLK_PERI_CTRL = (CLOCKS_CLK_PERI_CTRL_ENABLE_Msk | (CLOCKS_CLK_PERI_CTRL_AUXSRC_clk_sys << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos));
	CLOCKS->CLK_PERI_CTRL = (CLOCKS_CLK_PERI_CTRL_ENABLE_Msk | (1 << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos));
	// Enable clock for USB and set source to 'clksrc_pll_usb'
	CLOCKS->CLK_USB_CTRL = CLOCKS_CLK_USB_CTRL_ENABLE_Msk | (CLOCKS_CLK_USB_CTRL_AUXSRC_clksrc_pll_usb << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos);
	// Enable clock for ADC and set source to 'clksrc_pll_usb'
	CLOCKS->CLK_ADC_CTRL = CLOCKS_CLK_ADC_CTRL_ENABLE_Msk | (CLOCKS_CLK_ADC_CTRL_AUXSRC_clksrc_pll_usb << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos);
	// Set RTC clock divider
	CLOCKS->CLK_RTC_DIV = (256 << CLOCKS_CLK_RTC_DIV_INT_Pos); // 12MHz / 256 = 46875 Hz
	// Enable clock for RTC and set source to 'xosc_clksrc'
	CLOCKS->CLK_RTC_CTRL = CLOCKS_CLK_RTC_CTRL_ENABLE_Msk | (CLOCKS_CLK_RTC_CTRL_AUXSRC_xosc_clksrc << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos);
}

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


void systick_setup() {

	systick_init();

	/** Set the number of clock cycles between each systick interrupt
	 * 120MHz = 120000000 = (120*1000000)
	 * The largest possible value you can set is (2^24)-1 (16777215) which at 120MHz is about 0.1398 seconds or 139.8ms ( 16777215/(120*1000000) = ~0.1398 )
	 * We have the reset value set at 120000 which is 1ms at 120MHz ( 120000/(120*1000000) = ~0.001 )
	 * So the systick interrupt is set to trigger every 1ms
	 */
	systick_set(120000);

}