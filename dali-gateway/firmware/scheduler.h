#ifndef SCHEDULER_H
#define SCHEDULER_H


#define TASK_LED 0
#define TASK_TEST 1
#define TASK_LOGIC 2
#define SCH_TASKS_SIZE 3

#include <stdint.h>

void sch_init(void);
uint32_t sch_uptime_s(void);
uint32_t sch_uptime_ms(void);
uint8_t sch_task_init(uint8_t index, uint32_t interval_ms, void (*task)(void));
uint8_t sch_task_enable(uint8_t index);
uint8_t sch_task_disable(uint8_t index);
void sch_tick(void);
void sch_task_postpone(uint8_t index, uint32_t delay_ms);

#endif
