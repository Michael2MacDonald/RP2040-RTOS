#pragma once
#include "rp2040.h"

void systick_init();
void systick_deinit();
void systick_set(uint32_t value);
uint32_t systick_get();