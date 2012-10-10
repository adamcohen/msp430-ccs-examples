;flshasm1.asm

            .cdecls C, LIST, "msp430g2553.h"

            .text                           ; program start
            .retain
            .retainrefs


LED1        .equ    BIT0
LED2        .equ    BIT6
DELAYLOOPS  .equ    50000

            .global _main                   ; define entry point

RESET:      mov.w   #0400h,SP               ; initialize stack pointer

            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            mov.b   #LED1,  &P1OUT          ; Preload LED1 on, LED2 off (active high)
            bis.b   #LED1|LED2,  &P1DIR     ; Set pin with LED1,LED2 to output
InfLoop:                                    ; loop forever
            clr.w   R4                      ; initialize loop counter
DelayLoop:                                  ; [clock cycles in brackets]
            inc.w   R4                      ; increment loop counter     [1]
            cmp.w   #DELAYLOOPS,R4          ; Compare with maximum value [2]
            jne     DelayLoop               ; repeat loop if not equal   [2]
            xor     #LED1|LED2, &P1OUT      ; toggle LED1, LED2
            jmp     InfLoop
                                            ; Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end

