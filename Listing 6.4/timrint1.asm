; timrint1.asm - toggles LEDs with period of about 1s
; TACCR0 interrupts from timer A with period of about 0.5s
; Timer clock is SMCLK divided by 8, up mode , period 50000
; Launchpad rev 1.5 , LED1,2 active high on P1.0,6
;-----------------------------------------------------------------------
            .cdecls C, LIST, "msp430g2553.h"  ;header file for this device

            .text                             ; program start
            .retain
            .retainrefs
;-----------------------------------------------------------------------
; Pins for LED on port 1
LED1        .equ    BIT0                      ; LED connected to P1.0
LED2        .equ    BIT6                      ; LED connected to P1.0
;-----------------------------------------------------------------------

RESET:
            mov.w   #0400h,SP                 ; initialize stack pointer
main:                                         ; Equivalent to start of main() in C
            mov.w   #WDTPW|WDTHOLD,&WDTCTL    ; Stop watchdog timer
            bis.b   #LED1, &P1OUT             ; Preload LED1 (active high)
            bic.b   #LED2, &P1OUT             ; Preload LED2 off
            bis.b   #LED1|LED2, &P1DIR        ; Set pin with LED1 to output

            mov.w   #49999,&TACCR0            ; Period for up mode
            mov.w   #CCIE,&TACCTL0            ; Enable interrupts on Compare 0

            ; Set up Timer A. Up mode, divide clock by 8, clock from SMCLK, clear TAR
            mov.w   #MC_1|ID_3|TASSEL_2|TACLR,&TACTL
            bis.w   #GIE,SR                   ; Enable interrupts (just TACCR0)
            jmp     $                         ; Loop forever; interrupts do all
;-----------------------------------------------------------------------
; Interrupt service routine for TACCR0 , called when TAR = TACCR0
; No need to acknowledge interrupt explicitly - done automatically
TA0_ISR:                                      ; ISR for TACCR0 CCIFG
            xor.b   #LED1|LED2 ,&P1OUT        ; Toggle LEDs
            reti                              ; That's all: return from interrupt
;------------------------------------------------------------------------------
            .sect ".int09"                    ; Timer_A0 Vector
            .short TA0_ISR

            .sect   ".reset"                  ; MSP430 RESET Vector
            .short  RESET

            .end

            ; PORT1_VECTOR            ".int02"                    /* 0xFFE4 Port 1 */
            ; PORT2_VECTOR            ".int03"                    /* 0xFFE6 Port 2 */
            ; ADC10_VECTOR            ".int05"                    /* 0xFFEA ADC10 */
            ; USCIAB0TX_VECTOR        ".int06"                    /* 0xFFEC USCI A0/B0
            ; USCIAB0RX_VECTOR        ".int07"                    /* 0xFFEE USCI A0/B0 Receive */
            ; TIMER0_A1_VECTOR        ".int08"                    /* 0xFFF0 Timer0)A CC1, TA0 */
            ; TIMER0_A0_VECTOR        ".int09"                    /* 0xFFF2 Timer0_A CC0 */
            ; WDT_VECTOR              ".int10"                    /* 0xFFF4 Watchdog Timer */
            ; COMPARATORA_VECTOR      ".int11"                    /* 0xFFF6 Comparator A */
            ; TIMER1_A1_VECTOR        ".int12"                    /* 0xFFF8 Timer1_A CC1-4, TA1 */
            ; TIMER1_A0_VECTOR        ".int13"                    /* 0xFFFA Timer1_A CC0 */
            ; NMI_VECTOR              ".int14"                    /* 0xFFFC Non-maskable */
            ; RESET_VECTOR            ".reset"                    /* 0xFFFE Reset [Highest Priority] */
            ;
