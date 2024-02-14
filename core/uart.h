#ifndef __UART_H
#define __UART_H


void uart_init(uint32_t id, uint64_t baud);
uint32_t uart_read(uint32_t id, char *buff, uint32_t len);
void uart_putc(uint32_t id, char c);
uint32_t uart_write(uint32_t id, const char *buff, uint32_t len);
void uart_print(const char *s);
void uart_hex(uint32_t x);


#endif /** END: __UART_H */