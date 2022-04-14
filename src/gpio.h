#pragma once
#include "rp2040.h"

#define GPIO_FUNC_XIP			0
#define GPIO_FUNC_SPI			1
#define GPIO_FUNC_UART			2
#define GPIO_FUNC_I2C			3
#define GPIO_FUNC_PWM			4
#define GPIO_FUNC_SIO			5
#define GPIO_FUNC_PIO0			6
#define GPIO_FUNC_PIO1			7
#define GPIO_FUNC_CLK			8
#define GPIO_FUNC_USB			9

#define GPIO_OUT 1
#define GPIO_IN  0
#define OUTPUT   1
#define INPUT    0

#define HIGH 1
#define LOW  0

void gpio_init(uint32_t gpio, uint32_t func);
void gpio_dir(uint32_t gpio, uint32_t out);
void gpio_pullup(uint32_t gpio, uint32_t enable);
void gpio_pulldown(uint32_t gpio, uint32_t enable);
void gpio_set(uint32_t gpio, uint32_t value);
bool gpio_get(uint32_t gpio);
bool gpio_out_get(uint32_t gpio);
