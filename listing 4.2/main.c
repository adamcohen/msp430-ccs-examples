/*
 * main.c
 */
#include <msp430g2553.h>

void main(void) {
	WDTCTL = WDTPW|WDTHOLD;
	P1DIR |= BIT0;
	P1OUT |= BIT0;

	for(;;) {

	}

}
