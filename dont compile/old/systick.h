#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "kernel.h"

void systick_init();
void systick_deinit();
void systick_set(uint32_t value);
uint32_t systick_get();

#endif /** END: __SYSTICK_H */