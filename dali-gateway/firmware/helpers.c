#include "helpers.h"

#include "_string.h"
#include "platform.h"	// Pin names
#include "scheduler.h"	// System timer
#include "uart.h"
#include "_delay.h"

#include <stdio.h>
#include <sys/attribs.h> // IPLn syntax

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h> // ClearWDT

#include "config.h"

void adc_init(void){
    AD1CON1bits.ON = 0;			// Disable ADC
	
	AD1PCFGbits.PCFG9 = 0; // Set pin as analog
	AD1CSSLbits.CSSL9 = 1; // Select for scanning
	
    AD1CON1bits.SSRC = 0b111;	// Internal counter ends sampling and starts conversion (auto-convert)
    AD1CON2bits.VCFG = 0;		// Set voltage reference to pins AVSS/AVDD
	AD1CON2bits.CSCNA = 1;	 	// Scanning
	AD1CON3bits.SAMC = 15;		// Acquisition time = 15*TAD
	AD1CON3bits.ADCS = 1;		// TAD = 4*TPB
	AD1CON1bits.SSRC = 0b111;	// Auto convert after sampling
	AD1CON2bits.SMPI = 15;		// Interrupt every n+1 sample - IMPORTANT!
	AD1CON1bits.CLRASAM = 1;	// Stop conversions when the first ADC interrupt is generated
    
	// Enable, start sampling
	AD1CON1bits.ON = 1;
	AD1CON1bits.ASAM = 1; // Sampling begins immediately after last conversion completes; SAMP bit is automatically set
}

void startup_message(void){
	const uint16_t size = 200;
	uint8_t s[size];
	uint16_t s_len = 0;
	if (RCONbits.POR) str_cpycl(s, &s_len, size, "POR, ");
	if (RCONbits.BOR) str_cpycl(s, &s_len, size, "BOR, ");
	if (RCONbits.CMR) str_cpycl(s, &s_len, size, "CMR, ");
	if (RCONbits.EXTR) str_cpycl(s, &s_len, size, "EXTR, ");
	if (RCONbits.IDLE) str_cpycl(s, &s_len, size, "IDLE, ");
	if (RCONbits.SLEEP) str_cpycl(s, &s_len, size, "SLEEP, ");
	if (RCONbits.SWR) str_cpycl(s, &s_len, size, "SWR, ");
	if (RCONbits.VREGS) str_cpycl(s, &s_len, size, "VREGS, ");
	if (RCONbits.WDTO) str_cpycl(s, &s_len, size, "WDTO, ");
	s[s_len-2] = 0;
	
	// Reset POR and BOR flags for next reset
	RCONbits.POR = RCONbits.BOR = 0;
		
	uart_printf(1, "Owl ready. Reset cause \"%s\"\r\n", s);
}

void task_led(void){
	STATUS_LED = 0;
	sch_task_disable(TASK_LED);
}

void blink_led(void){
	STATUS_LED = 1;
	sch_task_enable(TASK_LED);
}
