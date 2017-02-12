#include "dali.h"

#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include "platform.h"
#include "uart.h"

volatile uint16_t frame;
volatile uint8_t count;
volatile uint8_t first_half;
volatile uint8_t thebit;
volatile enum {IDLE, TX_START, TX_DATA, TX_STOP, WAITING} dali_state;

void dali_send(uint16_t f){
	if (dali_state != IDLE) return;
	dali_state = TX_START;

	// Disable edge interrupt
	IEC0bits.INT0IE = 0;
	
	frame = f;
	count = 0;
	first_half = 1;

	// Set correct bit time and enable timer
	TMR1 = 0;
	PR1 = TIMER1_1_BIT;	// (1/1200*2) * 40M / 4 / TCKPS
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;

	dali_timer_tick();
}

void dali_goto_rx(void){
	count = 0;
	thebit = 1;

	// Enable INT0 on falling edge
	INTCONbits.INT0EP = 0;
	IFS0bits.INT0IF = 0;
	IEC0bits.INT0IE = 1;
}

void dali_edge_tick(uint16_t t){
	STAT_LED ^= 1;
	// Timer cleared in advance
	
	// First edge; enable timeout timer
	if (!count){
		PR1 = TIMER1_2P5_BIT;
		IFS0bits.T1IF = 0;
		IEC0bits.T1IE = 1;
		count = 1;
	}
	
	// Smaller than 1,5 bit period -> state change and same bit again
	else if (t<TIMER1_1P5_BIT) count++; 
	
	// No state change and different bit
	else{						
		count += 2;			
		thebit ^= 1;
	}
	
	// Shift in new data when in middle of bit (even periods)
	if (!(count&1)) frame = (frame<<1) | thebit;
}

void dali_timer_tick(void){
	switch(dali_state){
		case TX_START:
			if (first_half){
				DALI_TX = 0;
				first_half = 0;
			}
			else{
				DALI_TX = 1;
				first_half = 1;
				dali_state++;
			}
			break;
		case TX_DATA:
			if (first_half){
				if (frame & 0x8000) DALI_TX = 0;
				else DALI_TX = 1;
				first_half = 0;
			}
			else{
				DALI_TX ^= 1;
				first_half = 1;
				frame <<= 1;
				count++;
				if (count == 16) dali_state++;
			}
			break;
		case TX_STOP:
			DALI_TX = 1;
			count++;
			if (count == 20){
				dali_state = WAITING;
				dali_goto_rx();
				PR1 = TIMER1_22TE;
			}
			break;
			
		case WAITING:
			dali_state = IDLE;
			IEC0bits.T1IE = 0;
			if (!count) uart_puts(0, "OK\r\n");
		case IDLE:
			// Reception timed out; print short or long frame
			switch (count&0xfe){
				case 18:
					uart_put_hex(0, frame, 2);
					uart_puts(0, "\r\n");
					break;
				case 34:
					uart_put_hex(0, frame, 4);
					uart_puts(0, "\r\n");
					break;
			}
			dali_goto_rx();
			break;
	}
}

void dali_init(void){
	// Interrupt pin INT0 for RX
	INTCONbits.INT0EP = 0; // Interrupt on falling edge
    IPC0bits.INT0IP = 1;    // Interrupt priority
    IPC0bits.INT0IS = 1;    // Interrupt sub priority

	// Init timer 0
	T1CON = 0;
	TMR1 = 0;
	IPC1bits.T1IP = 1;
	IPC1bits.T1IS = 1;
	T1CONbits.TCKPS = 0x01; // Prescaler 8
	T1CONbits.ON = 1;
	
	dali_state = IDLE;
	dali_goto_rx();
}
