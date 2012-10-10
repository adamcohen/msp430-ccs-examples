// timrled2.c - toggles LEDs with period of about 1.3s
// Poll free-running timer A with period of about 0.65s
// Timer clock is SMCLK divided by 8, continuous mode
//----------------------------------------------------------------------
#include <msp430g2553.h>		// Specific device

// Pins for LEDs
#define	LED1	BIT0
#define	LED2	BIT6

void main (void)
{
	WDTCTL = WDTPW|WDTHOLD;				      // Stop watchdog timer
	P1OUT = LED1;						            // Preload LED1 on, LED2 off
	P1DIR = LED1|LED2;					        // Set pins for LED1,2 to output
	TACCR0 = 49999;                     // Upper limit of count for TAR
	TACTL = MC_1|ID_3|TASSEL_2|TACLR;	  // Set up and start Timer A
	// Up to "CCR0" mode, divide clock by 8, clock from SMCLK, clear timer

	for (;;) {							            // Loop forever
		while ((TACTL & TAIFG) == 0) {	  // Wait for overflow
		}								                  // doing nothing
		TACTL &= ~TAIFG;			            // Clear overflow flag
		P1OUT ^= LED1|LED2;				        // Toggle LEDs
	}									                  // Back around infinite loop
}
