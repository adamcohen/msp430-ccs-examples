// flashled.c - toggles LEDs with period of about 1s
// Software delay for() loop
// ----------------------------------------------------------------------
#include <msp430g2553.h> // Specific device
#include <stdint.h>

// Pins for LEDs
#define LED1 BIT0
#define LED2 BIT6

// Iterations of delay loop; reduce for simulation
#define DELAYLOOPS 10000

void delayLoop(uint16_t delay)
{
  volatile uint16_t LoopCtr;    // Loop counter: volatile!
  volatile uint16_t counter = 0;

  for (counter; counter < delay; counter++)
  {
    for (LoopCtr = 0; LoopCtr < DELAYLOOPS; ++ LoopCtr) {
       } // Empty delay loop
  }
}

void main (void)
{
  WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer
  P1OUT = ~LED1;                // Preload LED1 on , LED2 off
  P1DIR = LED1|LED2;            // Set pins with LED1 ,2 to output

  for (;;) { // Loop forever
    delayLoop(5);

    P1OUT ^= LED1|LED2; // Toggle LEDs
  }
}
