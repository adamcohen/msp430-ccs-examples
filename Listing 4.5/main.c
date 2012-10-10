#include <msp430g2553.h>
/*
 * main.c
 */
#define LED1    BIT0
#define B1      BIT3


void main(void) {
	WDTCTL = WDTPW | WDTHOLD;
	P1OUT = 0;
	P1OUT &= ~LED1; //turn off LED1, set output to low (Bit = 0)

	P1DIR = LED1;   // set P1DIR as output for LED1
	P1REN |= B1;    // enable pull up resistor for B1, by default pulling value to high when button is open.
	                // If P1OUT is set to low (Bit = 0), then P1REN becomes pull down resistor,
	                // pulling the output to low, otherwise if P1OUT is set to high (Bit = 1),
	                // P1REN becomes pull up resistor, pulling the pin up

	P1OUT |= B1;    // set as pull up resistor for B1 (P1.3)

  for(;;){
      if (P1IN & B1){   // is button up? Active low
        P1OUT &= ~LED1;        // turn off LED1
      } else {                 // button is closed
        P1OUT |= LED1;         // turn on LED1
      }
    }

}
