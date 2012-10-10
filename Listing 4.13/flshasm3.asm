;flshasm3.asm

            .cdecls C, LIST, "msp430g2553.h"

            .text                           ; program start
            .retain
            .retainrefs


LED1        .equ    BIT0
LED2        .equ    BIT6
; Iterations of delay loop
DELAYLOOPS  .equ    50000

;------------------------------------------------------------------------------
; can use either of the following to Allocate memory
LoopCtr     .usect ".bss",2,2               ; Allocate 1 word in RAM for loop counter
                                            ; into named uninitialized section

;            .bss LoopCtr,2,2                ; Allocate 1 word in RAM
;------------------------------------------------------------------------------

            .global _main                   ; define entry point

RESET:      mov.w   #0400h,SP               ; initialize stack pointer

            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            mov.b   #LED1, &P1OUT           ; Preload LED1 on, LED2 off (active high)
            bis.b   #LED1|LED2, &P1DIR      ; Set pin with LED1,LED2 to output
InfLoop:                                    ; loop forever
            mov.w   #DELAYLOOPS, &LoopCtr   ; initialize loop counter
DelayLoop:                                  ; [clock cycles in brackets]
            dec     &LoopCtr                ; decrement loop counter     [1]
            jnz     DelayLoop               ; repeat loop if not zero    [2]
            xor     #LED1|LED2, &P1OUT      ; toggle LED1, LED2
            jmp     InfLoop
                                            ; Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET

            .end
