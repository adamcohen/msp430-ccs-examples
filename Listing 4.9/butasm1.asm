;butasm1.asm

            .cdecls C, LIST, "msp430g2553.h"

            .text                           ; program start
            .retain
            .retainrefs


LED1        .equ    BIT0
B1          .equ    BIT3

            .global _main                   ; define entry point

RESET:      mov.w   #0280h,SP               ; initialize stack pointer

            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            bic.b   #LED1,  &P1OUT          ; Preload LED1 off (active high)
            bis.b   #LED1,  &P1DIR          ; Set pin with LED1 to output
            bis.b   #B1, &P1REN             ; enable pull up resistor on B1
            bis.b   #B1, &P1OUT             ; set pull up resistor on B1
InfLoop:                                    ; loop forever
ButtonUpLoop:                               ; loop while button up
            bit.b   #B1,&P1IN               ; Test bit B1 of P1IN
            jnz     ButtonUpLoop            ; Jump if not zero, button up
; Actions to be taken when button is pressed
            bis.b   #LED1,&P1OUT            ; Turn LED1 on (active high!)
ButtonDownLoop:                             ; Loop while button down
            bit.b   #B1,&P1IN               ; Test bit B1 of P1IN to see if it's down
            jz      ButtonDownLoop
; Actions to be taken when button is released
            bic.b   #LED1,&P1OUT            ; Turn LED1 off (active high!)
            jmp     InfLoop                 ; Back around infinite loop

                                            ; Interrupt Vectors
;------------------------------------------------------------------------------
;   Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end

