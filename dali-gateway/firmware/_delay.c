#include "_delay.h"

#include <xc.h>
#include "platform.h"	// sysclk

void delay_cs(uint32_t d){
    uint32_t countstart = _CP0_GET_COUNT();
    while ((uint32_t)(_CP0_GET_COUNT() - countstart) < (uint32_t)d);
}

void delay_ms(uint32_t d){
    delay_cs(d*(SYSCLK/2000));
}

void delay_us(uint32_t d){
    delay_cs(d*(SYSCLK/2000000));
}
