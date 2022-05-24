#include <kernel.h>
#include "rp2040.h"
#include "clock.h"

#include "resets.h"
// #define _GLIBCXX_USE_CXX11_ABI 1
#include <math.h> // For floor()


#define CTRL_ENABLE_Val           (0xFAB << XOSC_CTRL_ENABLE_Pos) // 0xfab → ENABLE
#define CTRL_DISABLE_Val          (0xD1E << XOSC_CTRL_ENABLE_Pos) // 0xd1e → DISABLE

#define DORMANT_WAKE          0x77616B65
#define DORMANT_DORMANT       0x636F6D61

void xosc_init(uint32_t tStable) { // Initialize the XOSC
	xosc->startup = floor( ((F_XOSC/tStable)/256.0f) + 0.5 ); // Wait 1 ms (specifies how many clock cycles must be seen from the crystal oscillator before it can be used) [ round((F_XOSC/tStable)/256) = value ]
	xosc->ctrl = (xosc->ctrl&~(0xFFF << XOSC_CTRL_ENABLE_Pos)) | CTRL_ENABLE_Val; // Switch from Ring Oscillator (ROSC) to Crystal Oscillator (XOSC)
	while (!(xosc->status & XOSC_STATUS_STABLE_Msk)); // Wait for crystal oscillator to stabilize
}

// void xosc_disable() {
// 	xosc->ctrl = (xosc->ctrl&~(0xFFF << XOSC_CTRL_ENABLE_Pos)) | CTRL_DISABLE_Val;
// 	while (xosc->status & XOSC_STATUS_STABLE_Msk);
// }

// void xosc_dormant() {
// 	xosc->dormant = DORMANT_DORMANT;
// 	while (!(xosc->status & XOSC_STATUS_STABLE_Msk)); // Wait for crystal oscillator to stabilize
// }

void sys_pll_init(uint32_t fbDiv, uint32_t postDiv1, uint32_t postDiv2) {
	reset_release_wait(RESET_PLL_SYS); // Release reset for PLL_SYS
	// Load VCO-related dividers before starting VCO
	PLL_SYS->CS = (1 << PLL_SYS_CS_REFDIV_Pos); // Divide the PLL input reference clock
	PLL_SYS->FBDIV_INT = fbDiv; // ignore: 40
	// PLL_SYS->FBDIV_INT = 4;
	// Set up post dividers
	PLL_SYS->PRIM = (postDiv1 << PLL_SYS_PRIM_POSTDIV1_Pos) | (postDiv2 << PLL_SYS_PRIM_POSTDIV2_Pos); // ignore: 4, 1
	// Turn on PLL, VCO, And Post Dividers
	PLL_SYS->PWR &= ~(PLL_SYS_PWR_PD_Msk | PLL_SYS_PWR_VCOPD_Msk | PLL_SYS_PWR_POSTDIVPD_Msk); // Main power, VCO power, and Post Dividers power
	while ( 0 == ((PLL_SYS->CS & PLL_SYS_CS_LOCK_Msk) >> PLL_SYS_CS_LOCK_Pos) ); // Wait for PLL to lock
}

void usb_pll_init(uint32_t fbDiv, uint32_t postDiv1, uint32_t postDiv2) {
	reset_release_wait(RESET_PLL_USB); // Release reset for PLL_USB
	// Load VCO-related dividers before starting VCO
	PLL_USB->CS = (1 << PLL_SYS_CS_REFDIV_Pos);
	PLL_USB->FBDIV_INT = fbDiv; // ignore: 36
	// Set up post dividers
	PLL_USB->PRIM = (postDiv1 << PLL_SYS_PRIM_POSTDIV1_Pos) | (postDiv2 << PLL_SYS_PRIM_POSTDIV2_Pos); // ignore: 3, 3
	// Turn on PLL, VCO, And Post Dividers
	PLL_SYS->PWR &= ~(PLL_SYS_PWR_PD_Msk | PLL_SYS_PWR_VCOPD_Msk | PLL_SYS_PWR_POSTDIVPD_Msk); // Main power, VCO power, and Post Dividers power
	while ( 0 == ((PLL_SYS->CS & PLL_SYS_CS_LOCK_Msk) >> PLL_SYS_CS_LOCK_Pos) ); // Wait for PLL to lock
}

void clock_setup() {
	// 1000 = Wait 1 ms for crystal oscillator to stabilize
	xosc_init(1000); // Switch to external oscillator (xosc) and configure it

	// F_CPU_CURRENT = (12*1000000); // External oscillator at 12 MHz ????

	// ==== Setup SYS PLL for (12 MHz * 40 / 4 / 1) = 120 MHz ====
	sys_pll_init(40, 4, 1); // Set up PLL_SYS for 120 MHz
	// ==== Setup USB PLL for (12 MHz * 36 / 3 / 3) = 48 MHz ====
	usb_pll_init(36, 3, 3); // Set up PLL_USB for 48 MHz

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

	F_CPU_CURRENT = (120*1000000); // Set the current CPU clock frequency to 120 MHz for reference
}