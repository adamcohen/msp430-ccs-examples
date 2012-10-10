; leds.asm
            .cdecls C, LIST, "msp430g2553.h"

            .text                           ; program start
            .retain
            .retainrefs
            .global _main                   ; define entry point

RESET       mov.w   #0280h,SP               ; initialize stack pointer

            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            mov.b   #00000001b,&P1OUT
            mov.b   #00000001b,&P1DIR
inf_loop
            jmp     inf_loop
                                            ; Interrupt Vectors
;------------------------------------------------------------------------------
;   Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
