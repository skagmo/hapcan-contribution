#include "uart.h"

#include "platform.h"
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>

#define UART_SIZE 1024
#define UART_NR 2

// UART ring buffers and pointers
volatile uint8_t uart_rxb[UART_NR][UART_SIZE];
volatile uint8_t uart_txb[UART_NR][UART_SIZE];
volatile uint8_t uart_txip[UART_NR];
volatile uint8_t uart_txop[UART_NR];
volatile uint8_t uart_rxip[UART_NR];
volatile uint8_t uart_rxop[UART_NR];

// Adresses for UxMODE-registers
uint32_t* UxMODE[2] = {(uint32_t*)&U3MODE, (uint32_t*)&U5MODE};
uint32_t* UxSTA[4] = {(uint32_t*)&U3STA, (uint32_t*)&U5STA};
uint32_t* UxTXREG[4] = {(uint32_t*)&U3TXREG, (uint32_t*)&U5TXREG};
uint32_t* UxRXREG[4] = {(uint32_t*)&U3RXREG, (uint32_t*)&U5RXREG};
uint32_t* UxBRG[4] = {(uint32_t*)&U3BRG, (uint32_t*)&U5BRG};

// U0-> IEC0, U123 -> IEC1, U456 -> IEC2
volatile uint32_t* UART_IEC[2] = {(uint32_t*)&IEC1, (uint32_t*)&IEC2};
const uint32_t UART_TXIE_bm[2] = {_IEC1_U3TXIE_MASK, _IEC2_U5TXIE_MASK};
const uint32_t UART_RXIE_bm[2] = {_IEC1_U3RXIE_MASK, _IEC2_U5RXIE_MASK};
volatile uint32_t* UART_IFS[2] = {(uint32_t*)&IFS1, (uint32_t*)&IFS2};
const uint32_t UART_TXIF_bm[2] = {_IFS1_U3TXIF_MASK, _IFS2_U5TXIF_MASK};
const uint32_t UART_RXIF_bm[2] = {_IFS1_U3RXIF_MASK, _IFS2_U5RXIF_MASK};
volatile uint32_t* UART_IPC[2] = {(uint32_t*)&IPC7, (uint32_t*)&IPC12};

void __ISR(_UART_3_VECTOR, ipl1auto) _UART3Interrupt(void){
    if (IFS1bits.U3TXIF && IEC1bits.U3TXIE){
        while(U3STAbits.UTXBF == 0){
            if(uart_txop[0] == uart_txip[0]){	// Moved first!
                IEC1bits.U3TXIE = 0;
                break;
            }
            U3TXREG = uart_txb[0][uart_txop[0]];
            uart_txop[0] = (uart_txop[0]+1) & (UART_SIZE-1);
        }
        IFS1bits.U3TXIF = 0;
    }
    if (IFS1bits.U3RXIF && IEC1bits.U3RXIE){
        while(U3STAbits.URXDA){
            uart_rxb[0][uart_rxip[0]] = U3RXREG;
            uart_rxip[0] = (uart_rxip[0]+1) & (UART_SIZE-1);
        }
        IFS1bits.U3RXIF = 0;
    }
}

void __ISR(_UART_5_VECTOR, ipl1auto) _UART5Interrupt(void){
    if (IFS2bits.U5TXIF && IEC2bits.U5TXIE){
        while(U5STAbits.UTXBF == 0){
            if(uart_txop[1] == uart_txip[1]){	// Moved first!
                IEC2bits.U5TXIE = 0;
                break;
            }
            U5TXREG = uart_txb[1][uart_txop[1]];
            uart_txop[1] = (uart_txop[1]+1) & (UART_SIZE-1);
        }
        IFS2bits.U5TXIF = 0;
    }
    if (IFS2bits.U5RXIF && IEC2bits.U5RXIE){
        while(U5STAbits.URXDA){
            uart_rxb[1][uart_rxip[1]] = U5RXREG;
            uart_rxip[1] = (uart_rxip[1]+1) & (UART_SIZE-1);
        }
        IFS2bits.U5RXIF = 0;
    }
}

/* General functions */
uint8_t uart_inwaiting(uint8_t nr){
	if (*UxSTA[nr] & _U1STA_OERR_MASK){
		*UxMODE[nr] &= ~_U1MODE_UARTEN_MASK;
        *UxMODE[nr] |= _U1MODE_UARTEN_MASK;
		return 0;
	}
    return ( (uart_rxip[nr] - uart_rxop[nr]) & (UART_SIZE-1) );
}

void uart_put(uint8_t nr, uint8_t c){
	while( uart_txop[nr] == ((uart_txip[nr]+1)&(UART_SIZE-1)) ); // Wait if ring buffer is full
    *UART_IEC[nr] &= ~UART_TXIE_bm[nr];	// Disable UART TX interrupts
	uart_txb[nr][uart_txip[nr]] = c;
	uart_txip[nr] = (uart_txip[nr]+1) & (UART_SIZE-1);
	*UART_IEC[nr] |= UART_TXIE_bm[nr];	// Enable UART TX interrupts
}

uint8_t uart_get(uint8_t nr){
	uint8_t c;
	*UART_IEC[nr] &= ~UART_RXIE_bm[nr];	// Disable UART RX interrupts - probably not necessary!
	c = uart_rxb[nr][uart_rxop[nr]];
	uart_rxop[nr] = (uart_rxop[nr]+1) & (UART_SIZE-1);
	*UART_IEC[nr] |= UART_RXIE_bm[nr];	// Enable UART RX interrupts
	return c;
}

void uart_init(uint8_t nr, uint32_t baud){
	delay_ms(1);	// TODO: Delay required if initializing after another UART
    *UxMODE[nr] &= ~_U1MODE_UARTEN_MASK;     // Disable UART
    *UxSTA[nr] &= ~_U1STA_UTXEN_MASK;        // Disable UART TX
    *UxBRG[nr] = (uint32_t)((PBCLK/baud)/4)-1;
    *UxMODE[nr] |= _U1MODE_BRGH_MASK;
    *UxSTA[nr] |= 0b10 << 14; // Trikset! Sto på at least one empty space, satte til "when buffer empty" (hva med all transmitted?)
	*UART_IFS[nr] &= ~UART_TXIF_bm[nr];     // Clear UART TX interrupt flag
    *UART_IFS[nr] &= ~UART_RXIF_bm[nr];     // Clear UART RX interrupt flag
	*UART_IEC[nr] |= UART_TXIE_bm[nr];      // Enable UART TX interrupts
    *UART_IEC[nr] |= UART_RXIE_bm[nr];      // Enable UART RX interrupts
    *UART_IPC[nr] |= (1 << 26) | (1 << 24); // Interrupt priority / subpriority 1 ----
    *UxSTA[nr] |= _U1STA_UTXEN_MASK;        // Enable UART TX
    *UxSTA[nr] |= _U1STA_URXEN_MASK;        // Enable UART RX ----
	*UxMODE[nr] |= _U1MODE_UARTEN_MASK;     // Enable UART
    uart_txip[nr] = uart_txop[nr] = uart_rxip[nr] = uart_rxop[nr] = 0;
}

void uart_puts(uint8_t nr, const uint8_t* s){
	while(*s) uart_put(nr, *s++);
}

//void uartPutf(uint8_t nr, const uint8_t* s, uint8_t s_len){
//	uint8_t j;
//	for (j=0; j<s_len; j++) uartPut(nr, *s++);
//}

void uart_put_int(uint8_t nr, int32_t n){
	uint8_t str[12]; // Sign, 10 digits, termination
	uint8_t len = 0;
	uint8_t j;

	if (n<0){
		uart_put(nr, '-');
		n *= -1;
	}
	do{
		str[len++] = (n % 10) + '0';
		n /= 10;
	}while(n);

	for (j=0; j<len; j++) uart_put(nr, str[len-1-j]);
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
