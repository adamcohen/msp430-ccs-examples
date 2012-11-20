// wdtest1.c - trival program to demonstrate watchdog timer
// LED1 active high on P1.0
// LED2 active high on P1.6
// Button B1 active low on P1.3
// TI Launchpad rev 1.5
// Adam Cohen , 20-11-2012; Code Composer Studio 5.2.1.00018
// ----------------------------------------------------------------------
#include <msp430g2553.h> // Specific device
// ----------------------------------------------------------------------
// Pins for LEDs and button
#define LED1  BIT0
#define LED2  BIT6
#define B1    BIT3

// Watchdog config: active , ACLK /32768 -> 1s interval; clear counter
#define WDTCONFIG (WDTCNTCL|WDTSSEL)
// Include settings for _RST/NMI pin here as well
// ----------------------------------------------------------------------
void main(void) {
  WDTCTL = WDTPW | WDTCONFIG;                   // Configure and clear watchdog
  P1DIR |= LED1 | LED2;                         // Set pins with LEDs to output
  P1OUT &= ~(LED1 | LED2);                      // LEDs off (active low)

  P1REN |= B1;
  P1OUT |= B1;

  for (;;) { // Loop forever
    P1OUT &= ~LED2;                             //clear LED2 before setting it
    P1OUT |= LED2 & ((IFG1 & WDTIFG) << 6 );    // LED2 shows state of WDTIFG

    if (P1IN & B1) {                            // Button up
      P1OUT &= ~LED1;                           // LED1 off
    } else {                                    // Button down
      WDTCTL = WDTPW | WDTCONFIG;               // Feed/pet/kick/clear watchdog
      IFG1 &= ~WDTIFG;                          // clear the WDTIFG bit
      P1OUT |= LED1;                            // LED1 on
    }
  }
}
