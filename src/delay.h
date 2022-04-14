#pragma once
#include "rp2040.h"

#define F_CPU (120*1000000)

// #define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
// #define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
// #define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (F_CPU / 1000000L) )

/**
 * System Tick Counter
 * Increments every systick interrupt
 */
volatile uint32_t TickCount;


void delay(unsigned long msec);
void delay_nop(uint32_t t);
void sleep(unsigned long msec);
unsigned long millis();

void reset_ticks(unsigned int ticks);
