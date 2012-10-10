;substk0.asm

            .cdecls C, LIST, "msp430g2553.h"  ;header file for this device

            .text                             ; program start
            .retain
            .retainrefs


LED1        .equ    BIT0                      ; LED connected to P1.0
; Iterations of delay loop for about 0.1s (3 cycles/iteration)
DELAYLOOPS  .equ    27000

            .global _main                     ; define entry point

RESET:
            mov.w   #0400h,SP                 ; initialize stack pointer
main:
            mov.w   #WDTPW|WDTHOLD,&WDTCTL

            bic.b   #LED1, &P1OUT             ; Preload LED1 off (active high)
            bis.b   #LED1, &P1DIR            ; Set pin with LED1 to output
InfLoop:                                      ; loop forever
            mov.w   #5,R12                    ; parameter for delay, units of 0.1s                                    ; [clock cycles in brackets]
            call    #DelayTenths              ; call subroutine (don't forget #)
            xor.b   #LED1, &P1OUT             ; toggle LED1
            jmp     InfLoop                   ; back around infinite loop
;-----------------------------------------------------------------------
; Subroutine to give delay of R12*0.1s
; Parameter is passed in R12 and destroyed
; R4 is used for loop counter but is not saved and restored
; Works correctly if R12 = 0: the test is executed first as in while (){}
;-----------------------------------------------------------------------
DelayTenths:
            push.w  R4                        ; Stack R4: will be overwritten
            jmp     LoopTest                  ; Start with test in case R12 = 0
OuterLoop:
            mov.w   #DELAYLOOPS,R4            ; Initialize loop counter
DelayLoop:                                    ; Clock cycles in brackets
            dec.w   R4                        ; Decrement loop counter  [1]
            jnz     DelayLoop                 ; Repeat loop if not zero [2]
            dec.w   R12                       ; Decrement number of 0.1s delays
LoopTest:
            cmp.w   #0,R12                    ; Finished number of 0.1s delays?
            jnz     OuterLoop                 ; No: go around delay loop again
            pop.w   R4                        ; Yes: restore R4 before returning
            ret                               ; Yes: return to caller

;------------------------------------------------------------------------------
            .sect   ".reset"                  ; MSP430 RESET Vector
            .short  RESET

            .end

