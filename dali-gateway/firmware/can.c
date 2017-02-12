#include "can.h"

#include <p32xxxx.h>
#include <stdint.h>
#include <string.h> // memcpy
#include "platform.h"
#include "uart.h" // Only for LED
#include "logic.h"

typedef unsigned long _paddr_t; /* a physical address */
typedef unsigned long _vaddr_t; /* a virtual address */
#define KVA_TO_PA(v) 	((_paddr_t)(v) & 0x1fffffff)
#define PA_TO_KVA0(pa)	((void *) ((pa) | 0x80000000))
#define PA_TO_KVA1(pa)	((void *) ((pa) | 0xa0000000))

// Allocate (64 x 4) x 4 bytes = 64 messages
unsigned int CanFifoMessageBuffers[256];

#define REQOP_NORMAL 0b000
#define REQOP_LISTEN_ALL 0b111
#define REQOP_CONFIG_MODE 0b100

void can_init(void){
	C1CONbits.ON = 1;
	
	// Switch to configuration mode
	C1CONbits.REQOP = REQOP_CONFIG_MODE;
	while(C1CONbits.OPMOD != REQOP_CONFIG_MODE);

	// Initialize C1FIFOBA register with physical address
	C1FIFOBA = KVA_TO_PA(CanFifoMessageBuffers);
	
	// Configure FIFO0, transmit, 32 messages
	C1FIFOCON0bits.FSIZE = 31;
	C1FIFOCON0SET = 0x80; // Set TXEN

	// Configure FIFO2, RX, 32 messages
	C1FIFOCON1bits.FSIZE = 31;
	C1FIFOCON1CLR = 0x80; // Clear TXEN

	// Bit Time = Sync seg. + Prop. seg. + Phase seg. 1 + Phase seg. 2
	// Phase segment 2 = 30% * 10 = 3
	// Phase segment 1 = 10tq - (1tq + 3tq + 3tq)	
	C1CFGbits.SEG2PHTS = 1;
	C1CFGbits.SEG2PH = 2; // Seq 2 = 3 tq
	C1CFGbits.SEG1PH = 2; // Seq 1 = 3 tq
	C1CFGbits.PRSEG = 2; // Prop. = 3 tq
	C1CFGbits.SAM = 1; // Sample bit three times, WILL NOT WORK FOR BRP < 2
	C1CFGbits.SJW = 2; // Sync jump width 3 tq
	C1CFGbits.BRP = 3; // (10000000÷(2×10×125000))−1 = 3
	
	// Set up filter and mask, SID/EID and mask defaults to 0
	C1FLTCON0bits.FSEL0 = 1;	// For filter 0, put messages in fifo 1
	C1FLTCON0bits.MSEL0 = 1;	// Use mask 1
	C1RXF0bits.EXID = 1;		// Enable EID
	C1RXM1bits.MIDE = 1;		// Mask match only on both SID/EID
	C1FLTCON0bits.FLTEN0 = 1;	// Enable filter 0

	// Go from configuration 
	C1CONbits.REQOP = REQOP_NORMAL;
	while(C1CONbits.OPMOD != REQOP_NORMAL);
}

CANTxMessageBuffer* can_tx_prepare_buffer(void){
	CANTxMessageBuffer* msg = (CANTxMessageBuffer *)(PA_TO_KVA1(C1FIFOUA0));
	memset(msg, 0, sizeof(CANTxMessageBuffer));
	return msg;
}
void can_tx_finish_buffer(void){
	// At this point the messages are loaded in FIFO0
	C1FIFOCON0SET = 0x2008; // Set UINC and TXREQ
}

void can_try_rx(void){
	CANRxMessageBuffer * buffer;
	while(C1FIFOINT1bits.RXNEMPTYIF){
		// Get the address of the message buffer to read from the C1FIFOUA1
		// register. Convert this physical address to virtual address.
		buffer = (CANRxMessageBuffer *)(PA_TO_KVA1(C1FIFOUA1));
		
		logic_handle_can_msg(buffer);

		// Set the UINC bit when a message has been read
		C1FIFOCON1bits.UINC = 1;
	}
}
