/*
 * main.c
 */
#include <msp430g2553.h>
#include <stdint.h>

#define LED1  BIT0
#define B1    BIT3

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;
  P1OUT &= ~LED1;
  P1DIR |= LED1;
  P1REN |= B1;
  P1OUT |= B1;

  uint8_t button_down = 0;

  for(;;) {
    if ((P1IN & B1) == 0) {     // is button down? (active low)
      button_down = 1;
      P1OUT ^= LED1;            // toggle LED
    }
  }

//  for(;;) {
//      while((P1IN & B1) != 0){}     // loop while button is up (active low)
//      P1OUT ^= LED1;                // button has been pressed, toggle led
//      while((P1IN & B1) == 0){}     // loop while button is down
//    }
}
