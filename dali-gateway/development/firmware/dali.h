#ifndef DALI_H
#define DALI_H

#include <stdint.h>

void dali_send(uint16_t f);
void dali_edge_tick(uint16_t t);
void dali_timer_tick(void);
void dali_init(void);

#endif