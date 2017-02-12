#ifndef DALI_H
#define DALI_H

#include <stdint.h>

#define DALI_BROADCAST 0b1111111000000000
#define DALI_GROUP     0b1000000000000000
#define DALI_SINGLE    0b0000000000000000
#define DALI_SPECIAL   0b1010000000000000

#define DALI_NOT_DIRECT_ARC    0b0000000100000000

#define DALI_ADDRESS_bp 9

#define DALI_CMD_OFF		0b000
#define DALI_CMD_UP			0b001
#define DALI_CMD_DOWN		0b010
#define DALI_CMD_STEP_UP	0b011
#define DALI_CMD_STEP_DOWN	0b100
#define DALI_CMD_RECALL_MAX 0b101

// Upper and lower limit for dali dimming, as well as step size
#define DIM_STEP 4
#define DIM_LOWER 0x95
#define DIM_UPPER 0xfe

void dali_tx_append(uint16_t data);
void dali_init(void);

#endif