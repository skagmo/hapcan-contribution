#include <p32xxxx.h>
#include <stdint.h>
#include <stdio.h> // sprintf
#define _SUPPRESS_PLIB_WARNING
#include <peripheral/wdt.h>
#include <peripheral/power.h> // PowerSaveSleep
#include "_delay.h"
#include "platform.h"
#include "uart.h"
#include "helpers.h"		// adc_task, system_monitor
#include "scheduler.h"
#include "config.h"
#include "can.h"
#include "dali.h"
#include "logic.h"

int main(void){
	// Initialize system
	platform_init(); // Turns off all ports
	//adc_init();
	uart_init(1, 38400);
	
	// Scheduler	
	sch_task_init(TASK_LED, 30, task_led);
	
	sch_task_init(TASK_LOGIC, 100, logic_tick);
	sch_task_enable(TASK_LOGIC);
	
	startup_message();

	can_init();
	dali_init();
	logic_init();
	
	// Main loop
	while(1){
		sch_tick();
		can_try_rx();
		
		//ClearWDT();
	}
	return (EXIT_SUCCESS);
}
