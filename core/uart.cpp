// #include <kernel.h>
#include "rp2040.h"
#include "uart.h"

struct uart_hw {
	uint32_t dr;
	uint32_t rsr;
	uint32_t unused1[4];
	uint32_t fr;
	uint32_t unuded2;
	uint32_t ilpr;
	uint32_t ibrd;
	uint32_t fbrd;
	uint32_t lcr_h;
	uint32_t cr;
	uint32_t ifls;
	uint32_t imsc;
	uint32_t ris;
	uint32_t mis;
	uint32_t icr;
};

#define uart0 ((volatile struct uart_hw*)UART0_BASE)
#define uart1 ((volatile struct uart_hw*)UART1_BASE)

#define FR_TXFE		(1 << 7)
#define FR_RXFF		(1 << 6)
#define FR_TXFF		(1 << 5)
#define FR_RXFE		(1 << 4)
#define FR_BUSY		(1 << 3)

#define LCR_WLEN_5	(0 << 5)
#define LCR_WLEN_6	(1 << 5)
#define LCR_WLEN_7	(2 << 5)
#define LCR_WLEN_8	(3 << 5)
#define LCR_FEN		(1 << 4)

#define CR_RXE		(1 << 9)
#define CR_TXE		(1 << 8)
#define CR_UARTEN	(1 << 0)


void uart_init(uint32_t id, uint64_t baud) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	baud = (120*1000000 * (uint64_t)4) / baud;
	uint32_t IBRD = baud / 64;
	uint32_t FBRD = baud % 64;

	uart->cr = 0; // disable uart
	// uart->ibrd = 6; // 115200
	// uart->fbrd = 33;
	uart->ibrd = IBRD; // 115200
	uart->fbrd = FBRD;
	uart->lcr_h = LCR_WLEN_8 | LCR_FEN; // Word length of 8 && Enable FIFO
	uart->cr = CR_TXE | CR_RXE | CR_UARTEN; // enable uart
}

uint32_t uart_read(uint32_t id, char *buff, uint32_t len) {
	uint32_t i;
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	for (i = 0; i < len; ++i) {
		while (uart->fr & FR_RXFE);
		buff[i] = uart->dr & 0xFF;
	}
	return i;
}

void uart_putc(uint32_t id, char c) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	while (uart->fr & FR_TXFF);
	uart->dr = c;
}

uint32_t uart_write(uint32_t id, char *buff, uint32_t len) {
	uint32_t i;
	// volatile struct uart_hw *uart;

	// if (id == 0)
	// 	uart = uart0;
	// else
	// 	uart = uart1;

	for (i = 0; i < len; ++i) {
		// while (uart->fr & FR_TXFF);
		// uart->dr = buff[i];
		uart_putc(id, buff[i]);
	}
	return i;
}

static void hex2str(char *str, int n) {
	int i, hb;

	for (i = 0; i < 8; ++i) {
		hb = n >> (7 - i) * 4;
		hb &= 0x0F;

		if (hb > 9)
			str[i] = (char)(hb + 'A' - 10);
		else
			str[i] = (char)(hb + '0');
	}
	str[8] = 0;
}

void uart_print(char *s) {
	while (*s) {
		uart_write(0, s, 1);
		++s;
	}
}

void uart_hex(uint32_t x) {
	char str[8 + 1];
	hex2str(str, x);
	uart_print(str);
}