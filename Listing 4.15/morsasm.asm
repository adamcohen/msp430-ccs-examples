; morsasm2.s43 - sends dots and dashes
; Software delay subroutine with down loop for 0.1s
; Bug: gives very brief flash between letters
; Message stored as string in ROM with loop in main routine
; Registers in main routine: R5 used as loop counter,
;   R12 to pass delay to subroutine
;-----------------------------------------------------------------------
            .cdecls C, LIST, "msp430g2553.h"  ;header file for this device

            .text                             ; program start
            .retain
            .retainrefs


LED1        .equ    BIT0                      ; LED connected to P1.0
; Iterations of delay loop for about 0.1s (3 cycles/iteration)
DELAYLOOPS  .equ    27000

; Durations of symbols for morse code in units of 0.1s
; LETTER gives gap between letters; ENDTX terminates message
DOT         .EQU    2
DASH        .EQU    6
SPACE       .EQU    2
LETTER      .EQU    0
ENDTX       .EQU    0xFF

;-----------------------------------------------------------------------

RESET:
            mov.w   #0400h,SP                 ; initialize stack pointer
main:                                         ; Equivalent to start of main() in C
            mov.w   #WDTPW|WDTHOLD,&WDTCTL    ; Stop watchdog timer
            bic.b   #LED1, &P1OUT             ; Preload LED1 off (active high)
            bis.b   #LED1, &P1DIR             ; Set pin with LED1 to output
            clr.w   R5                        ; Initialize counter to step through message
            jmp     MessageTest               ; Jump to test so it is evaluated first
MessageLoop:
            bic.b   #LED1,&P1OUT              ; LED1 on
            mov.b   Message(R5),R12           ; Load duration of delay as parameter
            call    #DelayTenths              ; Call subroutine: don't forget the #!
            bis.b   #LED1,&P1OUT              ; LED1 off
            mov.w   #SPACE,R12                ; Load duration of delay (space)
            call    #DelayTenths              ; Call subroutine
            inc.w   R5                        ; Next symbol to send
MessageTest:
            cmp.b   #ENDTX,Message(R5)        ; Reached end of message?
            jne     MessageLoop               ; No: continue around loop
InfLoop:            ; Yes: loop forever
            jmp     InfLoop                     ; around infinite, empty loop

;-----------------------------------------------------------------------
; Subroutine to give delay of R12*0.1s
; Parameter is passed in R12 and destroyed
; R4 is used for loop counter but is not saved and restored
; Works correctly if R12 = 0: the test is executed first as in while(){}
;-----------------------------------------------------------------------
DelayTenths:
            jmp   LoopTest                    ; Start with test in case R12 = 0
OuterLoop:
            mov.w #DELAYLOOPS,R4              ; Initialize loop counter
DelayLoop:                                    ; [clock cycles in brackets]
            dec.w R4                          ; Decrement loop counter  [1]
            jnz   DelayLoop                   ; Repeat loop if not zero [2]
            dec.w R12                         ; Decrement number of 0.1s delays
LoopTest:
            cmp.w #0,R12                      ; Have we finished number of 0.1s delays?
            jnz   OuterLoop                   ; No: go around delay loop again
            ret                               ; Yes: return to caller

;-----------------------------------------------------------------------
; Segment for constant data in ROM
Message:                                      ; Message to send (dots and dashes)
            .byte DOT,DOT,DOT,LETTER
            .byte DASH,DASH,DASH,LETTER
            .byte DOT,DOT,DOT,ENDTX
;------------------------------------------------------------------------------
            .sect   ".reset"                  ; MSP430 RESET Vector
            .short  RESET

            .end
