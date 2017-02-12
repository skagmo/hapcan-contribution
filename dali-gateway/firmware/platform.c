#include "platform.h"

#include <xc.h>

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>

#pragma config FSOSCEN = OFF        // Disable secondary oscillator
#pragma config FWDTEN = OFF         // Enable watchdog timer
#pragma config WDTPS = PS4096		// PS is equal to timeout in ms
#pragma config ICESEL = ICS_PGx1    // select pins to transfer program data on (ICSP pins)
#pragma config PWP = OFF            // Program Flash Write Protect (Disable)
#pragma config BWP = OFF             // Boot Flash Write Protect bit (Protection Enabled)
#pragma config CP = OFF              // Code Protect (Protection Enabled)
#pragma config POSCMOD=XT			// XT mode because of 8 MHz!
//#pragma config FNOSC=PRI
#pragma config FNOSC=PRIPLL			// Use Primary Oscillator with PLL (XT, HS, or EC)
#pragma config FPLLIDIV=DIV_2		// Divide 16MHz to between 4-5MHz before PLL (now 4MHz)
#pragma config FPLLMUL=MUL_20		// Multiply with PLL (now 80 MHz)
#pragma config FPLLODIV=DIV_8		// Divide After PLL (now 10 MHz)

void platform_init(){
    SYSTEMConfigPerformance(SYSCLK);
	INTEnableSystemMultiVectoredInt(); // Enable interrupts
	
	AD1PCFG = 0xffff; // Set all pins as digital
	DDPCONbits.JTAGEN = 0;
	//EnableWDT(); // Enable WDT (with 1:1024 postscaler from BL)

    // Set output pins
	TRISBbits.TRISB6 = 0; // Status LED
	TRISDbits.TRISD1 = 0; // DALI TX
	TRISEbits.TRISE0 = 0; // CAN standby

	CAN_STANDBY = 0;
	DALI_TX = 0;
	STATUS_LED = 0;
	
	// U3
	// AN15 - vsense
	// INT0 DALI RX
}
