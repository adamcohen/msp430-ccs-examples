;substk1.asm

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
            bis.b   #LED1, &P1DIR             ; Set pin with LED1 to output
InfLoop:                                      ; loop forever
            mov.w   #5,R12                    ; parameter for delay, units of 0.1s                                    ; [clock cycles in brackets]
            call    #DelayTenths              ; call subroutine (don't forget #)
            xor.b   #LED1, &P1OUT             ; toggle LED1
            jmp     InfLoop                   ; back around infinite loop
; Subroutine to give delay of R12 *0.1s
; Parameter is passed in R12 and destroyed
; Space for two loop counters is created on stack , after which
; 0(SP) is innermost (little) loop , 2(SP) is big loop counter
;-----------------------------------------------------------------------
; Iterations of delay loop for about 0.1s (6 cycles/iteration ):
BIGLOOPS     .equ   130
LITTLELOOPS  .equ   100
;-----------------------------------------------------------------------
DelayTenths:
            sub.w #4,SP                       ; Allocate 2 words (4 bytes) on stack
            jmp LoopTest                      ; Start with test in case R12 = 0
OuterLoop:
            mov.w #BIGLOOPS ,2(SP)            ; Initialize big loop counter
BigLoop:
            mov.w #LITTLELOOPS ,0(SP)         ; Initialize little loop counter
LittleLoop:                                   ; [clock cycles in brackets]
            dec.w 0(SP)                       ; Decrement little loop counter [4]
            jnz LittleLoop                    ; Repeat loop if not zero [2]
            dec.w 2(SP)                       ; Decrement big loop counter [4]
            jnz BigLoop                       ; Repeat loop if not zero [2]
            dec.w R12                         ; Decrement number of 0.1s delays
LoopTest:
            cmp.w #0,R12                      ; Finished number of 0.1s delays?
            jnz OuterLoop                     ; No: go around delay loop again
            add.w #4,SP                       ; Yes: finished , release space on stack
            ret                               ; Return to caller

;------------------------------------------------------------------------------
            .sect   ".reset"                  ; MSP430 RESET Vector
            .short  RESET

            .end

