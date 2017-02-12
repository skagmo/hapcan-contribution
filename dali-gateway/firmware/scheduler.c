#include "scheduler.h"

#include <stdint.h>
#include "platform.h"

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>

#define RAW_TIMER			_CP0_GET_COUNT()
#define RAW_TIMER_MS_SCALE	(SYSCLK/2000)
#define RAW_TIMER_S_SCALE	(SYSCLK/2)

typedef struct{
	uint8_t enable;
	uint32_t interval_ms;	// Running interval, can maximum be half of the variable size.
	uint32_t next_ms;		// Next time to run. "interval" is added each run.
	uint32_t last_run_ms;	// Timestamp of actual last run.
	void (*task)(void);	// Function pointer - will be called with delta ms since last round as argument
}sch_task_t;

sch_task_t sch_tasks[SCH_TASKS_SIZE];

uint32_t raw_timer, prev_raw_ms, prev_raw_s;
uint32_t uptime_ms, uptime_s;

void sch_init(void){
	raw_timer = 0;
	prev_raw_ms = 0;
	prev_raw_s = 0;
	uptime_ms = 0;
	uptime_s = 0;
}

uint32_t sch_uptime_s(void){
	return uptime_s;
}

uint32_t sch_uptime_ms(void){
	return uptime_ms;
}

void sch_task_postpone(uint8_t index, uint32_t delay_ms){
	if (index < SCH_TASKS_SIZE){
		sch_tasks[index].next_ms += delay_ms;
	}
}

// Add new task
uint8_t sch_task_init(uint8_t index, uint32_t interval_ms, void (*task)(void)){
	if (index < SCH_TASKS_SIZE){
		sch_tasks[index].interval_ms = interval_ms;
		sch_tasks[index].task = task;
		return 1;
	}
	return 0;
}

uint8_t sch_task_enable(uint8_t index){
	if (index < SCH_TASKS_SIZE){
		sch_tasks[index].enable = 1;
		sch_tasks[index].next_ms = uptime_ms + sch_tasks[index].interval_ms;
		return 1;
	}
	return 0;
}

// Set task inactive / free one slot
uint8_t sch_task_disable(uint8_t index){
	if (index < SCH_TASKS_SIZE){
		sch_tasks[index].enable = 0;
		return 1;
	}
	return 0;
}

void sch_tick(void){
	// Check if we are in a new millisecond
	raw_timer = RAW_TIMER;
	uint32_t delta_ms = (raw_timer - prev_raw_ms)/RAW_TIMER_MS_SCALE;
	if (delta_ms){
		uptime_ms += delta_ms;
		prev_raw_ms += (delta_ms*RAW_TIMER_MS_SCALE);
		
		// Check if we are in a new second
		uint32_t delta_s = (raw_timer - prev_raw_s)/RAW_TIMER_S_SCALE;
		if (delta_s){
			uptime_s += delta_s;
			prev_raw_s += (delta_s*RAW_TIMER_S_SCALE);
		}
		
		// Find the due task(s) including those after the upcoming, run it, and find next interval
		uint8_t j;
		for (j=0; j<SCH_TASKS_SIZE; j++){
			// Check if its enabled and due, run it, find next interval
			if ( sch_tasks[j].enable && ((uptime_ms-sch_tasks[j].next_ms) < 0x7FFFFFFF) ){
				sch_tasks[j].task();
				sch_tasks[j].next_ms += sch_tasks[j].interval_ms;
			}
		}
	}
}
