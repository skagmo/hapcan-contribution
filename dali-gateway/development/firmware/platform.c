#include "platform.h"

#include <p32xxxx.h>
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>


// Fuses
#pragma config FSOSCEN = OFF        // Disable secondary oscillator
#pragma config FWDTEN = OFF         // Enable watchdog timer
#pragma config WDTPS = PS1024
#pragma config ICESEL = ICS_PGx1    // select pins to transfer program data on (ICSP pins)

//#define XOSC_EN
#ifdef XOSC_EN
#pragma config POSCMOD=HS			// High speed crystal mode
#pragma config FNOSC=PRIPLL			// Use Primary Oscillator with PLL (XT, HS, or EC)
#pragma config FPLLIDIV=DIV_4		// Divide 16MHz to between 4-5MHz before PLL (now 4MHz)
#pragma config FPLLMUL=MUL_20		// Multiply with PLL (now 80MHz)
#pragma config FPLLODIV=DIV_2		// Divide After PLL (now 40 MHz)
#else
#pragma config FNOSC = FRCPLL		// Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2		// Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20		// PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_8		// Divide After PLL - SYSCLK
#pragma config FPBDIV = DIV_4		// PBCLK - ignored by SYSTEMConfigPerformance
#endif

void platform_init(){
    SYSTEMConfigPerformance(SYSCLK);

    // LED pin
	TRISBbits.TRISB6 = 0;
	
	// DALI TX pin as open drain output
	TRISCbits.TRISC14 = 0;
	ODCCbits.ODCC14 = 1;

    // General
    //EnableWDT();
    DDPCONbits.JTAGEN = 0;
}
