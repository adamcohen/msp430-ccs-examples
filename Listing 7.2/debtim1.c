// debtim1.c - press button B1 to light LED1 with debounce
// Samples input at 5 ms intervals set by Timer_A , 32KHz ACLK
// Shift reg for debounce , different thresholds for press and release
// LED1 active high on P1.0
// TI Launchpad rev 1.5
// Button B1 active low on P1.3
// Adam Cohen , 15-10-2012; Code Composer Studio 5.2.1.00018


// ----------------------------------------------------------------------
#include <msp430g2553.h>
#include <stdint.h>                 // Standard integer types

union {                             // Debounced state of P2IN
  unsigned char DebP1IN;            // Complete byte
  struct {
    unsigned char DebP1IN_0 : 1;
    unsigned char DebP1IN_1 : 1;
    unsigned char DebP1IN_2 : 1;
    unsigned char DebP1IN_3 : 1;
    unsigned char DebP1IN_4 : 1;
    unsigned char DebP1IN_5 : 1;
    unsigned char DebP1IN_6 : 1;
    unsigned char DebP1IN_7 : 1;
  } DebP1IN_bit; // Individual bits
} DebP1IN;

#define RAWB1 BIT3                   //B1 on P1.3      P2IN_bit.P2IN_1
#define DEBB1 DebP1IN.DebP1IN_bit.DebP1IN_0
#define LED1  BIT0                   //LED1 on P1.0

void main (void)
{
  WDTCTL = WDTPW | WDTHOLD;         // Stop watchdog timer
  P1OUT &= ~LED1;                   // Preload LED1 off (active high)
  P1REN |= RAWB1;                   // Enable pull up/down resistor on Button 1 at P1.3
  P1OUT |= RAWB1;                   // Set pull up resistor on Button 1
  P1DIR |= LED1|LED2;                    // Set pin with LED1 to output
  DebP1IN.DebP1IN = 0xFF;           // Initial debounced state of port
  TACCR0 = 160;                     // 160 counts at 32KHz = 5ms
  TACCTL0 = CCIE;                   // Enable interrupts on Compare 0
  TACTL = MC_1|TASSEL_1|TACLR;      // Set up and start Timer A
  // "Up to CCR0" mode , no clock division , clock from ACLK , clear timer

  for (;;) {                        // Loop forever
    __low_power_mode_3 ();          // Enter LPM3 , only ACLK active
    // Return to main function when a debounced transition has occurred

    // Update LED1 from debounced button
    P1OUT &= 0xFE;                  //clear DEBB1 bit
    P1OUT |= DEBB1;                 //set DEBB1 bit
  }
}
// ----------------------------------------------------------------------
// Interrupt service routine for Timer A chan 0; no need to acknowledge
// Device returns to LPM3 automatically after ISR unless input changes
// PRESS_THRESHOLD = 0x3F = 0b00111111 , needs 2 successive 0s (enough ?)
// RELEASE_THRESHOLD = 0xFC = 0b11111100 , 6 successive 1s (too many?)
// ----------------------------------------------------------------------
#define PRESS_THRESHOLD 0x3F
#define RELEASE_THRESHOLD 0xFC
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR (void)
{
  static uint8_t P13ShiftReg = 0xFF;        // Shift reg for history of P1.3
  P13ShiftReg >>= 1;                        // Update history in shift register

  if (P1IN & RAWB1) {                       // Insert latest input from B1
    P13ShiftReg |= BIT7;                    // Set msb if input high
  }

  if (DEBB1 == 0) {
    // Current debounced value low , looking for input to go high (release)
    if (P13ShiftReg >= RELEASE_THRESHOLD) { // button released
      DEBB1 = 1;                            // New debounced state high (released)
      __low_power_mode_off_on_exit();       // Wake main routine
    }
  } else {
    // Current debounced value high , looking for input to go low (press)
    if (P13ShiftReg <= PRESS_THRESHOLD) {   // button pressed
      DEBB1 = 0;                            // New debounced state low (pressed)
      __low_power_mode_off_on_exit();       // Wake main routine
    }
  }
}
