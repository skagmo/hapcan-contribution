#ifndef PLATFORM_H
#define PLATFORM_H

#include <p32xxxx.h>
#include <stdint.h>

#define SYSCLK 10000000L
#define PBCLK  10000000L

#define INT0_PRI 6          // Used for GPS PPS
#define INT0_IPL IPL6AUTO
#define UART_PRI 6          // Small buffer, so should have priority
#define UART_IPL IPL6AUTO

#define STATUS_LED LATBbits.LATB6
#define DALI_TX LATDbits.LATD1
#define CAN_STANDBY LATEbits.LATE0

void platform_init();

#endif
