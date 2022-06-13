#ifndef __UART_H
#define __UART_H

// #ifdef __cplusplus
// extern "C" {
// #endif

void uart_init(uint32_t id, uint64_t baud);
uint32_t uart_read(uint32_t id, char *buff, uint32_t len);
void uart_putc(uint32_t id, char c);
uint32_t uart_write(uint32_t id, char *buff, uint32_t len);
void uart_print(char *s);
void uart_hex(uint32_t x);

// #ifdef __cplusplus
// }; /** END: __cplusplus */
// #endif /** END: __cplusplus */

#endif /** END: __UART_H */