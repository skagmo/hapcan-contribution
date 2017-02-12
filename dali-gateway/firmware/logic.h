#ifndef LOGIC_H
#define	LOGIC_H

#include <stdint.h>

#include "can.h"

void logic_init(void);
void logic_handle_can_msg(CANRxMessageBuffer* msg);
void logic_tick(void);

#endif
