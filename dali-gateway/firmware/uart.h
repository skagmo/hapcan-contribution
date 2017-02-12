#ifndef UART_H
#define UART_H

#include <stdint.h>

/* General functions */
uint16_t uart_inwaiting(uint8_t nr);
void uart_put(uint8_t nr, uint8_t c);
void uart_putf(uint8_t nr, uint8_t* d, uint16_t d_len);
uint8_t uart_get(uint8_t nr);
void uart_deinit(uint8_t nr);
void uart_init(uint8_t nr, uint32_t baud);
void uart_put_hex(uint8_t nr, uint32_t n, uint8_t n_len);
void uart_puts(uint8_t nr, const char* s);
void uart_printf(uint8_t nr, const char* format, ...);

#endif
