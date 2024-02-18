#include "rp2040.h"
#include "gpio.h"

// struct sio_hw {
// 	uint32_t cpuid;
// 	uint32_t gpio_in;
// 	uint32_t gpio_hi_in;
// 	uint32_t unused;
// 	uint32_t gpio_out;
// 	uint32_t gpio_out_set;
// 	uint32_t gpio_out_clr;
// 	uint32_t gpio_out_xor;
// 	uint32_t gpio_oe;
// 	uint32_t gpio_oe_set;
// 	uint32_t gpio_oe_clr;
// 	uint32_t gpio_oe_xor;
// };

struct io_bank0_hw {
	struct gpio {
		uint32_t status;
		uint32_t ctrl;
	} gpio[30];
};

struct pads_bank0_hw {
	uint32_t voltage_select;
	uint32_t gpio[30];
	uint32_t swclk;
	uint32_t swd;
};

#define sio    ((volatile struct sio_hw*)SIO_BASE)
#define io     ((volatile struct io_bank0_hw*)IO_BANK0_BASE)
#define pads   ((volatile struct pads_bank0_hw*)PADS_BANK0_BASE)

#define PADS_GPIO_OD      (1 << 7)
#define PADS_GPIO_IE      (1 << 6)
#define PADS_GPIO_PUE     (1 << 3)
#define PADS_GPIO_PDE     (1 << 2)


void gpio_init(uint32_t gpio, uint32_t func) {
	io->gpio[gpio].ctrl = func;
	pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	sio->GPIO_OE_CLR = 1 << gpio;
}

void gpio_dir(uint32_t gpio, uint32_t out) {
	if (out) {
		SIO->GPIO_OE_SET = 1 << gpio;
		pads->gpio[gpio] = 0;
	} else {
		SIO->GPIO_OE_CLR = 1 << gpio;
		pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	}
}

void gpio_set(uint32_t gpio, uint32_t value) {
	if (value)
		SIO->GPIO_OUT_SET = 1 << gpio;
	else
		SIO->GPIO_OUT_CLR = 1 << gpio;
}

bool gpio_get(uint32_t gpio) { // WORKS ONLY IF THE GPIO IS INPUT!!!
	return (SIO->GPIO_IN & (1 << gpio)) >> gpio;
}
// Temporary solution for reading GPIOs while in output mode
bool gpio_out_get(uint32_t gpio) { // WORKS ONLY IF THE GPIO IS OUTPUT (I think)!!!
	return (SIO->GPIO_OUT & (1<<gpio)) >> gpio;
}

void gpio_deinit(uint32_t gpio) {
	pads->gpio[gpio] = PADS_GPIO_IE | PADS_GPIO_OD;
	SIO->GPIO_OE_CLR = 1 << gpio;
}

void gpio_pullup(uint32_t gpio, uint32_t enable) {
	uint32_t reg = pads->gpio[gpio];

	if (enable) {
		reg |= PADS_GPIO_PUE;
	} else {
		reg &= (~PADS_GPIO_PUE);
	}
	pads->gpio[gpio] = reg;
}

void gpio_pulldown(uint32_t gpio, uint32_t enable) {
	uint32_t reg = pads->gpio[gpio];

	if (enable) {
		reg |= PADS_GPIO_PDE;
	} else {
		reg &= (~PADS_GPIO_PDE);
	}
	pads->gpio[gpio] = reg;
}