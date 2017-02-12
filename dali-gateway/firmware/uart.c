#include "uart.h"

#include "platform.h"
#include <sys/attribs.h> // IPLn syntax

#include "_delay.h"

#define UART_SIZE 1024
#define UART_NR 4

// UART ring buffers and pointers
volatile uint8_t uart_rxb[UART_NR][UART_SIZE];
volatile uint8_t uart_txb[UART_NR][UART_SIZE];
volatile uint16_t uart_txip[UART_NR];
volatile uint16_t uart_txop[UART_NR];
volatile uint16_t uart_rxip[UART_NR];
volatile uint16_t uart_rxop[UART_NR];

// Adresses for UxMODE-registers
uint32_t* UxMODE[UART_NR] = {(uint32_t*)&U6MODE, (uint32_t*)&U3MODE, (uint32_t*)&U5MODE, (uint32_t*)&U2MODE};
uint32_t* UxSTA[UART_NR] = {(uint32_t*)&U6STA, (uint32_t*)&U3STA, (uint32_t*)&U5STA, (uint32_t*)&U2STA};
uint32_t* UxTXREG[UART_NR] = {(uint32_t*)&U6TXREG, (uint32_t*)&U3TXREG, (uint32_t*)&U5TXREG, (uint32_t*)&U2TXREG};
uint32_t* UxRXREG[UART_NR] = {(uint32_t*)&U6RXREG, (uint32_t*)&U3RXREG, (uint32_t*)&U5RXREG, (uint32_t*)&U2RXREG};
uint32_t* UxBRG[UART_NR] = {(uint32_t*)&U6BRG, (uint32_t*)&U3BRG, (uint32_t*)&U5BRG, (uint32_t*)&U2BRG};

volatile uint32_t* UART_IEC[UART_NR] = {(uint32_t*)&IEC2, (uint32_t*)&IEC1, (uint32_t*)&IEC2, (uint32_t*)&IEC1};
const uint32_t UART_TXIE_bm[UART_NR] = {_IEC2_U6TXIE_MASK, _IEC1_U3TXIE_MASK, _IEC2_U5TXIE_MASK, _IEC1_U2TXIE_MASK};
const uint32_t UART_RXIE_bm[UART_NR] = {_IEC2_U6RXIE_MASK, _IEC1_U3RXIE_MASK, _IEC2_U5RXIE_MASK, _IEC1_U2RXIE_MASK};

volatile uint32_t* UART_IFS[UART_NR] = {(uint32_t*)&IFS2, (uint32_t*)&IFS1, (uint32_t*)&IFS2, (uint32_t*)&IFS1};
const uint32_t UART_TXIF_bm[UART_NR] = {_IFS2_U6TXIF_MASK, _IFS1_U3TXIF_MASK, _IFS2_U5TXIF_MASK, _IFS1_U2TXIF_MASK};
const uint32_t UART_RXIF_bm[UART_NR] = {_IFS2_U6RXIF_MASK, _IFS1_U3RXIF_MASK, _IFS2_U5RXIF_MASK, _IFS1_U2RXIF_MASK};

volatile uint32_t* UART_IPC[UART_NR] = {(uint32_t*)&IPC12, (uint32_t*)&IPC7, (uint32_t*)&IPC12, (uint32_t*)&IPC8};
const uint32_t UART_IS_bp[UART_NR] = {_IPC12_U6IS_POSITION, _IPC7_U3IS_POSITION, _IPC12_U5IS_POSITION, _IPC8_U2IS_POSITION};
const uint32_t UART_IP_bp[UART_NR] = {_IPC12_U6IP_POSITION, _IPC7_U3IP_POSITION, _IPC12_U5IP_POSITION, _IPC8_U2IP_POSITION};

#define UART_INT(NR, TXIE, TXIF, TXBF, TXREG, IFSCLR, TXIF_MASK, RXIF_MASK, RXIF, RXDA, RXREG) \
	if (TXIF){ \
		while(!TXBF){ \
			if(uart_txop[NR] == uart_txip[NR]){	\
				TXIE = 0; \
				break; \
			} \
			TXREG = uart_txb[NR][uart_txop[NR]]; \
			uart_txop[NR] = (uart_txop[NR]+1) & (UART_SIZE-1); \
		} \
		IFSCLR = TXIF_MASK; \
	} \
	if (RXIF){ \
		uint16_t next; \
		uint8_t count = 0; \
		while (RXDA){ \
			count++; \
			next = (uart_rxip[NR]+1) & (UART_SIZE-1); \
			if (next != uart_rxop[NR]){ \
				uart_rxb[NR][uart_rxip[NR]] = RXREG; \
				uart_rxip[NR] = next; \
			} \
			else{ \
				(void)RXREG; \
			} \
		} \
		IFSCLR = RXIF_MASK; \
	}

void __ISR(_UART_6_VECTOR, UART_IPL) _UART6Interrupt(void){
	UART_INT(0,	IEC2bits.U6TXIE, IFS2bits.U6TXIF,
		U6STAbits.UTXBF, U6TXREG,
		IFS2CLR, _IFS2_U6TXIF_MASK,	_IFS2_U6RXIF_MASK,
		IFS2bits.U6RXIF, U6STAbits.URXDA, U6RXREG)
}

void __ISR(_UART_3_VECTOR, UART_IPL) _UART3Interrupt(void){
	UART_INT(1,	IEC1bits.U3TXIE, IFS1bits.U3TXIF,
		U3STAbits.UTXBF, U3TXREG,
		IFS1CLR, _IFS1_U3TXIF_MASK, _IFS1_U3RXIF_MASK,
		IFS1bits.U3RXIF, U3STAbits.URXDA, U3RXREG)
}

void __ISR(_UART_5_VECTOR, UART_IPL) _UART5Interrupt(void){
	UART_INT(2, IEC2bits.U5TXIE, IFS2bits.U5TXIF,
		U5STAbits.UTXBF, U5TXREG,
		IFS2CLR, _IFS2_U5TXIF_MASK,	_IFS2_U5RXIF_MASK,
		IFS2bits.U5RXIF, U5STAbits.URXDA, U5RXREG)
}

void __ISR(_UART_2_VECTOR, UART_IPL) _UART2Interrupt(void){
	UART_INT(3,	IEC1bits.U2TXIE, IFS1bits.U2TXIF,
		U2STAbits.UTXBF, U2TXREG,
		IFS1CLR, _IFS1_U2TXIF_MASK, _IFS1_U2RXIF_MASK,
		IFS1bits.U2RXIF, U2STAbits.URXDA, U2RXREG)
}

/* General functions */

void uart_put(uint8_t nr, uint8_t c){
	if (nr >= UART_NR) return;
	
	uint16_t next = (uart_txip[nr]+1) & (UART_SIZE-1);
	if( uart_txop[nr] == next ) return; // Return if ring buffer is full
    //*UART_IEC[nr] &= ~UART_TXIE_bm[nr];	// Disable UART TX interrupts
	uart_txb[nr][uart_txip[nr]] = c;
	uart_txip[nr] = next;
	*UART_IEC[nr] |= UART_TXIE_bm[nr];	// Enable UART TX interrupts
}

void uart_putf(uint8_t nr, uint8_t* d, uint16_t d_len){
    *UART_IEC[nr] &= ~UART_TXIE_bm[nr];	// Disable UART TX interrupts
	uint16_t j;
	for (j=0; j<d_len; j++){
		if ( uart_txop[nr] == ((uart_txip[nr]+1)&(UART_SIZE-1)) ) break; // Stop on full buffer
		uart_txb[nr][uart_txip[nr]] = d[j];
		uart_txip[nr] = (uart_txip[nr]+1) & (UART_SIZE-1);
	}
	*UART_IEC[nr] |= UART_TXIE_bm[nr];	// Enable UART TX interrupts
}

uint16_t uart_inwaiting(uint8_t nr){
	if (*UxSTA[nr] & _U1STA_OERR_MASK){
		*UxMODE[nr] &= ~_U1MODE_UARTEN_MASK;
        *UxMODE[nr] |= _U1MODE_UARTEN_MASK;
		return 0;
	}
    return ( (uart_rxip[nr] - uart_rxop[nr]) & (UART_SIZE-1) );
}

uint8_t uart_get(uint8_t nr){
	uint8_t c;
	//*UART_IEC[nr] &= ~UART_RXIE_bm[nr];	// Disable UART RX interrupts - probably not necessary!
	c = uart_rxb[nr][uart_rxop[nr]];
	uart_rxop[nr] = (uart_rxop[nr]+1) & (UART_SIZE-1);
	//*UART_IEC[nr] |= UART_RXIE_bm[nr];	// Enable UART RX interrupts
	return c;
}

void uart_deinit(uint8_t nr){
    *UxMODE[nr] = 0;	// Disable UART and more
    *UxSTA[nr] = 0;		// Disable RX, TX and more
}

void uart_init(uint8_t nr, uint32_t baud){
	// TXISEL sto på at least one empty space, satte til "when buffer empty" (hva med all transmitted?)
	// TODO: Delay required if initializing after another UART
	// Wake up from sleep disabled
	delay_ms(1);			 // TODO: Might cause problems?						
    *UxMODE[nr] = 0;							// Disable UART and more
    *UxSTA[nr] = 0;								// Disable RX, TX and more
    *UxBRG[nr] = (uint32_t)((PBCLK/baud)/4)-1;
    *UxMODE[nr] |= _U1MODE_BRGH_MASK;
	*UART_IFS[nr] &= ~(UART_TXIF_bm[nr] | UART_RXIF_bm[nr]);		// Clear UART interrupt flags
	*UART_IPC[nr] |= (1 << UART_IS_bp[nr]) | (UART_PRI << UART_IP_bp[nr]);	// Interrupt priority / subpriority 1
	*UART_IEC[nr] |= UART_TXIE_bm[nr] | UART_RXIE_bm[nr];			// Enable UART interrupts
	// UTXISEL = 0b10, Interrupt is generated and asserted while the transmit buffer is empty
    *UxSTA[nr] |= (0b10 << _U1STA_UTXISEL_POSITION) | _U1STA_UTXEN_MASK | _U1STA_URXEN_MASK;	// Enable UART RX and TX
	*UxMODE[nr] |= _U1MODE_UARTEN_MASK;								// Enable UART
    uart_txip[nr] = uart_txop[nr] = uart_rxip[nr] = uart_rxop[nr] = 0;
}

void uart_puts(uint8_t nr, const char* s){
	while(*s) uart_put(nr, *s++);
}

void uart_put_hex(uint8_t nr, uint32_t n, uint8_t n_len){
	uint8_t j;
	uint32_t temp;
	uint8_t bitshift = (n_len-1)*4;
	uint32_t mask = 0xF << bitshift;
	for (j=0; j<n_len; j++){
		temp = (n & mask) >> bitshift;
		n <<= 4;
		if(temp < 10) uart_put(nr, (temp + '0'));
		else uart_put(nr, (temp + 55));
	}
}

#include <stdio.h>

#define MAX_SIZE 255
void uart_printf(uint8_t nr, const char* format, ...){
	va_list args;
	va_start(args, format);
	
	char out[MAX_SIZE];
	uint16_t out_len;
	out_len = vsnprintf(out, MAX_SIZE, format, args);
	uart_putf(nr, (uint8_t*)out, out_len);

	va_end(args);
}
