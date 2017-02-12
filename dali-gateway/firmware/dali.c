#include "dali.h"

#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include "platform.h"
#include "uart.h"
#include "helpers.h"

volatile uint16_t frame;
volatile uint8_t count;
volatile uint8_t first_half;
volatile uint8_t thebit;
volatile enum {IDLE, TX_START, TX_DATA, TX_STOP, WAITING} dali_state;

// Calculated for 40M tckps=8, change TCKPS to 2 for 10 M
#define TIMER1_1_BIT 521 // (1/1200*2) * PBCLK / 4 / TCKPS
#define TIMER1_1P5_BIT 781
#define TIMER1_2P5_BIT 1302
#define TIMER1_22TE 11458

#define DALI_HIGH 1
#define DALI_LOW 0

#define DALI_TX_QUEUE_SIZE 32
uint16_t dali_tx_queue[DALI_TX_QUEUE_SIZE];
uint16_t dali_tx_ip, dali_tx_op;

void dali_timer_tick(void);
void dali_tx_start(void);
void dali_goto_rx(void);

// Append to queue and attempt to start TX
void dali_tx_append(uint16_t data){
	dali_tx_queue[dali_tx_ip] = data;
	dali_tx_ip = (dali_tx_ip+1)&(DALI_TX_QUEUE_SIZE-1);
	
	if (dali_state == IDLE) dali_tx_start();
}

void dali_tx_start(void){
	dali_state = TX_START;

	// Disable edge interrupt used for RX
	IEC0bits.INT0IE = 0;
	
	frame = dali_tx_queue[dali_tx_op];
	dali_tx_op = (dali_tx_op+1)&(DALI_TX_QUEUE_SIZE-1);
	count = 0;
	first_half = 1;

	// Set correct bit time and enable timer
	TMR1 = 0;
	PR1 = TIMER1_1_BIT;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	
	blink_led();

	dali_timer_tick();
}

// Edge interrupt for received data
void __ISR(_EXTERNAL_0_VECTOR, ipl1auto) int0_interrupt(void){
	uint16_t t = TMR1;	
	TMR1 = 0;

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
	
	// Interrupt on opposite edge next time and reset flag
	INTCONbits.INT0EP ^= 1; 
	IFS0bits.INT0IF = 0;
}

// Use T1 interrupt for sent data
void __ISR( _TIMER_1_VECTOR, ipl1auto) timer_1_interrupt(void){
	IFS0CLR = IFS0_T1_BIT;
	dali_timer_tick();
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
	dali_tx_ip = dali_tx_op = 0;
	
	dali_goto_rx();
}

void dali_goto_rx(void){
	count = 0;
	thebit = 1;

	// Enable INT0 on falling edge
	INTCONbits.INT0EP = 0;
	IFS0bits.INT0IF = 0;
	IEC0bits.INT0IE = 1;
}

void dali_timer_tick(void){
	switch(dali_state){
		case TX_START:
			if (first_half){
				DALI_TX = DALI_HIGH;
				first_half = 0;
			}
			else{
				DALI_TX = DALI_LOW;
				first_half = 1;
				dali_state++;
			}
			break;
			
		case TX_DATA:
			if (first_half){
				if (frame & 0x8000) DALI_TX = DALI_HIGH;
				else DALI_TX = DALI_LOW;
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
			DALI_TX = DALI_LOW;
			count++;
			if (count == 20){
				dali_state = WAITING;
				dali_goto_rx();
				PR1 = TIMER1_22TE;
			}
			break;
			
		case WAITING:
			IEC0bits.T1IE = 0;
			
			if (!count){
				// No response from DALI command
				// uart_puts(0, "OK\r\n");
			}
			switch (count&0xfe){
				case 18:
					//uart_put_hex(0, frame, 2);
					break;
				case 34:
					//uart_put_hex(0, frame, 4);
					break;
			}
			
			// If waiting packets, continue TX, else go to RX
			if (dali_tx_ip != dali_tx_op) dali_tx_start();
			else{
				dali_state = IDLE;
				dali_goto_rx();
			}
			break;
			
		case IDLE:
			break;
	}
}
