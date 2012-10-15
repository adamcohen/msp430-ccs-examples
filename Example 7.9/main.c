// example 7.9
// Responds to interrupts on input pin, LPM0 between interrupts
// Outputs count of how many times a button press has been detected via printf through UART
// Launchpad rev 1.5. MSP430G2553, LED1 active high on P1.0. B1 active low on P1.3
#include <msp430g2553.h>

#define LED1 BIT0
#define LED2 BIT6
#define B1 BIT3

#include "stdarg.h"
#include <stdint.h>

// Define flags used by the interrupt routines
#define TX BIT0

// Flag register
volatile unsigned char FLAGS = 0;

void sendByte(unsigned char);
void printf(char *, ...);

void initUART(void);

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

  initUART();

  P1OUT = 0;                  // Preload LEDs off (active high!)
  P1DIR |= LED1|LED2;         // Set pin with LED1 to output
  P1REN |= B1;                // Enable pull up/down resistor for B1 (P1.3)
  P1OUT |= B1;                // set as pull up resistor for B1 (P1.3)
  P1IE |= B1;                 // Enable interrupts on edge
  P1IES |= B1;                // Sensitive to negative edge (H->L)

  do {
    P1IFG = 0;                // Clear any pending interrupts ...
  } while (P1IFG != 0);       // ... until none remain

  for (;;) {                  // Loop forever (should not need)
    __low_power_mode_0 ();    // LPM0 with int'pts
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
  // Store how many times a button press has been detected
  static uint8_t button_count = 0;

  P1OUT ^= LED1;              // Toggle LED
  button_count++;

  // output button count through UART
  printf("Count: %i\r\n", button_count);

  do {
    P1IFG = 0;                // Clear any pending interrupts ...
  } while (P1IFG != 0);       // ... until none remain
}


/**
* Initializes the UART for 9600 baud with a RX interrupt
**/
void initUART(void) {
  P1SEL |= BIT1 | BIT2 ;      // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 | BIT2 ;     // P1.1 = RXD, P1.2=TXD

  UCA0CTL1 |= UCSSEL_1;       // CLK = ACLK
  UCA0BR0 = 0x03;             // 32kHz/9600 = 3.41
  UCA0BR1 = 0x00;
  UCA0MCTL = UCBRS1 + UCBRS0; // Modulation UCBRSx = 3
  UCA0CTL1 &= ~UCSWRST;       // **Initialize USCI state machine**
}

/**
* puts() is used by printf() to display or send a string.. This function
* determines where printf prints to. For this case it sends a string
* out over UART, another option could be to display the string on an
* LCD display.
**/
void puts(char *s) {
  char c;

  // Loops through each character in string 's'
  while (c = *s++) {
    sendByte(c);
  }
}
/**
* puts() is used by printf() to display or send a character. This function
* determines where printf prints to. For this case it sends a character
* out over UART.
**/
void putc(unsigned b) {
  sendByte(b);
}

/**
* Sends a single byte out through UART
**/
void sendByte(unsigned char byte )
{
  while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
  UCA0TXBUF = byte; // TX -> RXed character
}



