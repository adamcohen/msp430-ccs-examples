// butled4.c - press button B1 to light LED1
// Responds to interrupts on input pin , LPM4 between interrupts
// Launchpad rev 1.5. MSP430G2553, LED1 active high on P1.0. B1 active low on P1.3
#include <msp430g2553.h>

#define LED1 BIT0
#define B1 BIT3

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  P1OUT &= ~LED1;             // Preload LED1 off (active high!)
  P1DIR |= LED1;              // Set pin with LED1 to output
  P1REN |= B1;                // Enable pull up/down resistor for B1 (P1.3)
  P1OUT |= B1;                // set as pull up resistor for B1 (P1.3)
  P1IE |= B1;                 // Enable interrupts on edge
  P1IES |= B1;                // Sensitive to negative edge (H->L)

  do {
    P1IFG = 0;                // Clear any pending interrupts ...
  } while (P1IFG != 0);       // ... until none remain

  for (;;) {                  // Loop forever (should not need)
    __low_power_mode_4 ();    // LPM4 with int'pts , all clocks off
    // could also switch to LPM4 using the following:
    //__bis_SR_register(GIE|LPM4_bits);
  }
}
// ----------------------------------------------------------------------

// Interrupt service routine for port 1 inputs
// Only one bit is active so no need to check which
// Toggle LED , toggle edge sensitivity , clear any pending interrupts
// Device returns to low power mode automatically after ISR
// ----------------------------------------------------------------------
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
  P1OUT ^= LED1;          // Toggle LED
  P1IES ^= B1;            // Toggle edge sensitivity

  do {
    P1IFG = 0;            // Clear any pending interrupts ...
  } while (P1IFG != 0);   // ... until none remain
}


