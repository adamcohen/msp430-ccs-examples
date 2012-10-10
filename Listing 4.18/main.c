// random1.c - pseudorandom sequence on LEDs
// Poll timer A in Up mode with period of about 0.5s
// Timer clock is SMCLK divided by 8, up mode, period 50000
//----------------------------------------------------------------------
#include <msp430g2553.h>    // Specific device
#include <stdint.h>           // For uint16_t

// Pins for LEDs
#define LED1  BIT0
#define LED2  BIT6

// Parameters for shift register; length <= 15 (4 is good for testing)
#define REGLENGTH 4
#define LASTMASK  ((uint16_t) (BIT0 << REGLENGTH))
#define NEXTMASK  ((uint16_t) (BIT0 << (REGLENGTH-1)))

void main (void)
{
  uint16_t pattern;                 // next pattern to be displayed

  WDTCTL = WDTPW|WDTHOLD;           // Stop watchdog timer
  P1OUT &= ~(LED1|LED2);            // Preload LEDs off
  P1DIR = LED1|LED2;                // Set pins with LEDs to output
  TACCR0 = 49999;                   // Upper limit of count for TAR
  TACTL = MC_1|ID_3|TASSEL_2|TACLR; // Set up and start Timer A
  // "Up to CCR0" mode, divide clock by 8, clock from SMCLK, clear timer

  pattern = 1;                      // Initialize pattern (nonzero)
  for (;;) {                        // Loop forever
    while ((TACTL & TAIFG) == 0) {  // Wait for timer to overflow
    }                 //   doing nothing

    TACTL &= ~TAIFG;                // Clear overflow flag
    P1OUT = pattern;                // Update pattern (lower byte)
    pattern <<= 1;                  // Shift for next pattern

    // Mask two most significant bits, simulate XOR using switch, feed back
    switch (pattern & (LASTMASK|NEXTMASK)) {
    case LASTMASK:
    case NEXTMASK:
      pattern |= BIT0;              // XOR gives 1
      break;
    default:
      pattern &= ~BIT0;             // XOR gives 0
      break;
    }
  }                                 // Back around infinite loop
}
