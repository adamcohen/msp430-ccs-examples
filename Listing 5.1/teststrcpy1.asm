;teststrcpy1.asm

            .cdecls C, LIST, "msp430g2553.h"  ;header file for this device

            .text                             ; program start
            .retain
            .retainrefs

;------------------------------------------------------------------------------
DestStr     .usect ".bss",16,2               ; Allocate 16 bytes (8 words) in RAM for
                                             ; destination string

;------------------------------------------------------------------------------

RESET:
            mov.w    #0400h,SP                 ; initialize stack pointer

main:
            mov.w    #WDTPW|WDTHOLD,&WDTCTL    ; stop watchdog timer

            mov.w    #0x0200,R4               ; Pointer to start of RAM
            mov.w    #0xA5A5,R5               ; Pattern for fill
FillRAM:
            mov.w    R5 ,0(R4)                ; Write to RAM , clumsy destination
            incd.w   R4                       ; Step pointer to next word
            cmp.w    #0x0400,R4               ; Set flags for (R4 - end of RAM)

            ; Not SFE(DATA16_N), which would include only the declared variables
            jlo       FillRAM                 ; Repeat loop while R4 < end of RAM

            mov.w    #SourceStr,R14           ; Load address of source
            mov.w    #DestStr ,R12            ; Load address of destination
            call     #MyStrCpy                ; Copy string (don't forget #!)
            jmp      $                        ; Infinite , empty loop
;-----------------------------------------------------------------------
; Copy source string starting in R14 to destination starting in R12
; Both registers overwritten; no local registers used
; No checks for overlap , space in destination , unterminated source ...
MyStrCpy:
            jmp      CopyTest
CopyLoop:
            inc.w    R12                       ; [1 word , 1 cycle] inc dst address
CopyTest:
            tst.b    0(R14)                    ; [2 words , 4 cycles] test source
            mov.b    @R14+,0(R12)              ; [2 words , 5 cycles] copy src -> dst
            jnz      CopyLoop                  ; [1 word , 2 cycles] continue if not \0
            ret                                ; Yes: return to caller

;-----------------------------------------------------------------------
; Segment for constant data in ROM
SourceStr:                                     ; string constant, stored between 0xC000 and 0xFFFF
            .string  "hello , world\n"         ; "" causes a '\0' to be appended
;------------------------------------------------------------------------------
            .sect    ".reset"                  ; MSP430 RESET Vector
            .short   RESET

            .end

