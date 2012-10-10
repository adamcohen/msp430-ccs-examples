// flashled.c - toggles LEDs with period of about 1s
// Software delay for() loop
// ----------------------------------------------------------------------
#include <msp430g2553.h> // Specific device
#include <stdint.h>

// Pins for LEDs
#define LED1 BIT0
#define LED2 BIT6

// Iterations of delay loop; reduce for simulation
#define DELAYLOOPS 50000

void main (void)
{
  volatile uint16_t LoopCtr;    // Loop counter: volatile!
  WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer
  P1OUT = ~LED1;                // Preload LED1 on , LED2 off
  P1DIR = LED1|LED2;            // Set pins with LED1 ,2 to output

  for (;;) { // Loop forever
    for (LoopCtr = 0; LoopCtr < DELAYLOOPS; ++ LoopCtr) {
    } // Empty delay loop

    P1OUT ^= LED1|LED2; // Toggle LEDs
  }
}
