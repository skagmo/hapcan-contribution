#ifndef UART_H
#define UART_H

#include <stdint.h>

/* General functions */
uint8_t uart_inwaiting(uint8_t nr);
void uart_put(uint8_t nr, uint8_t c);
uint8_t uart_get(uint8_t nr);
void uart_init(uint8_t nr, uint32_t baud);
void uart_put_hex(uint8_t nr, uint32_t n, uint8_t n_len);
void uart_puts(uint8_t nr, const uint8_t* s);
void uart_put_int(uint8_t nr, int32_t n);

#endif
