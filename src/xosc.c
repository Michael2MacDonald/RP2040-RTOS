#include "xosc.h"

struct xosc_hw {
	uint32_t ctrl;
	uint32_t status;
	uint32_t dormant;
	uint32_t startup;
	uint32_t count;
};

#define xosc 	((volatile struct xosc_hw*)XOSC_BASE)

#define CTRL_ENABLE           (0xFAB << 12) // 0xfab → ENABLE
#define CTRL_DISABLE          (0xD1E << 12) // 0xd1e → DISABLE
#define CTRL_FREQ_1_15_MHZ    0xAA0

#define STATUS_STABLE         (1 << 31)
#define STATUS_BADWRITE       (1 << 24)
#define STATUS_ENABLED        (1 << 12)
#define STATUS_FREQ_1_15_MHZ  (0 << 0)

#define DORMANT_WAKE          0x77616B65
#define DORMANT_DORMANT       0x636F6D61


void xosc_init() {
	xosc->ctrl = CTRL_FREQ_1_15_MHZ; // Set Crystal Oscillator frequency range (1-15MHZ is the only available option)
	xosc->startup = (((12 * 1000000)/1000)+128)/256; // wait 1 ms (specifies how many clock cycles must be seen from the crystal before it can be used)
	xosc->ctrl |= CTRL_ENABLE; // Switch from Ring Oscillator (ROSC) to Crystal Oscillator (XOSC)
	while (!(xosc->status & STATUS_STABLE));
}

void xosc_disable() {
	uint32_t tmp = xosc->ctrl;
	tmp &= (0xFFF << 12);
	tmp |= CTRL_DISABLE;
	xosc->ctrl = tmp;
	while (xosc->status & STATUS_STABLE);
}

void xosc_dormant() {
	xosc->dormant = DORMANT_DORMANT;
	while (!(xosc->status & STATUS_STABLE));
}