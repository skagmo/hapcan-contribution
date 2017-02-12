#ifndef PLATFORM_H
#define PLATFORM_H

#include <p32xxxx.h>
#include <stdint.h>

#define SYSCLK 10000000L
#define PBCLK  10000000L

#define DALI_TX LATCbits.LATC14
#define STAT_LED LATBbits.LATB6

#define TIMER1_1_BIT 521
#define TIMER1_1P5_BIT 781
#define TIMER1_2P5_BIT 1302
#define TIMER1_22TE 11458

void platform_init();

#endif
