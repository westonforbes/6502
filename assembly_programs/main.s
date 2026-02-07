    ; Start of program memory.
    .org $8000

; Program entry point.
reset:

    ; Jump to initialization routine.
    ; We need to jump here to avoid executing the includes directly.
    jmp initialize

    ; Includes.
    include "variables.s"
    include "via.s"
    include "keypad.s"
    include "lcd.s"

; Start of actual initialization code.
initialize:

    ; Initialize stack pointer at top of stack memory.
    ; Load ones into x register.
    ldx #$00ff
    
    ; Transfer x to stack pointer.
    txs

    ; Clear x register.
    ldx #0

    ; Clear the y register.
    ldy #0

    ; Call initialization routines for VIA, and LCD.
    jsr initialize_via
    jsr initialize_lcd
    jsr clear_lcd

; Infinite loop to end program.
loop:
  jsr check_if_key_available
  jmp loop

; Handle trailing data and reset vector.
end:
    ; Fill remaining space with NOPs.
    .fill $fffc - *, $EA

    ; Set origin to reset vector location. This means the assembler is placing the following data at memory location $fffc.
    .org $fffc
    
    ; Set the reset vector loacation to start of ROM, 0x8000.              
    .word reset

    ; Fill the remaining space after reset vector with 0x0000.
    .word $0000
