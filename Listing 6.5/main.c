#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6

void main(void) {
  WDTCTL = WDTPW|WDTHOLD;       // Stop watchdog timer
  P1OUT = LED1;                 // Preload LED1 on , LED2 off
  P1DIR = LED1|LED2;            // Set pins with LED1 ,2 to output
  TACCR0 = 49999;               // Upper limit of count for TAR
  TACCTL0 = CCIE;               // Enable interrupts on Compare 0

  TACTL = MC_1|ID_3|TASSEL_2|TACLR; // Set up and start Timer A
  // "Up to CCR0" mode , divide clock by 8, clock from SMCLK , clear timer

  //_BIS_SR(GIE);               // enable interrupts or the one below


  for (;;) { // Loop forever doing nothing
    __bis_SR_register(GIE|LPM0_bits);       // enable interrupts
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void) {
  P1OUT ^= LED1|LED2;
}

