#include <stdint.h>
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include "platform.h"
#include "uart.h"
#include "dali.h"

// WRAPPING? - runs at clk/2
void delay_cs(uint32_t d){
    uint32_t countstart = _CP0_GET_COUNT();
    while ((uint32_t)(_CP0_GET_COUNT() - countstart) < (uint32_t)d);
}

void delay_ms(uint32_t d){
    delay_cs(d*(SYSCLK/2000));
}

void __ISR(_EXTERNAL_0_VECTOR, ipl1auto) int0_interrupt(void){
	uint16_t t = TMR1;	
	TMR1 = 0;
	dali_edge_tick(t);
	
	// Interrupt on opposite edge next time and reset flag
	INTCONbits.INT0EP ^= 1; 
	IFS0bits.INT0IF = 0;
}

void __ISR( _TIMER_1_VECTOR, ipl1auto) timer_1_interrupt(void){
	IFS0CLR = IFS0_T1_BIT;
	dali_timer_tick();
}

void uart_parse(uint8_t c){
	#define CMD_SIZE 32
	static uint8_t cmd[CMD_SIZE];
	static uint8_t cmd_len = 0;

	if ((c=='\r')||(c=='\n')){
		if (cmd_len==4){
			uint16_t data;
			uint8_t j;
			for (j=0; j<cmd_len; j++){
				data <<= 4;
				if ((cmd[j]>='0')&&(cmd[j]<='9')) data |= (cmd[j] - '0');
				else if ((cmd[j]>='A')&&(cmd[j]<='F')) data |= (cmd[j] - 55);
			}
			dali_send(data);
		}
		cmd_len = 0;
	}
	else if (cmd_len<CMD_SIZE) cmd[cmd_len++] = c;
}

int main(int argc, char** argv){
	// Initialize system
	platform_init();
	uart_init(0, 38400);
	dali_init();
	INTEnableSystemMultiVectoredInt();

	while(1){
		if (uart_inwaiting(0)) uart_parse(uart_get(0));
		delay_ms(1);
		ClearWDT();
	}

	return (EXIT_SUCCESS);
}
