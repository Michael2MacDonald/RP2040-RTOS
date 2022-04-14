#pragma once
#include "rp2040.h"

void uart_init(uint32_t id, uint64_t baud);
uint32_t uart_read(uint32_t id, char *buff, uint32_t len);
uint32_t uart_write(uint32_t id, char *buff, uint32_t len);
