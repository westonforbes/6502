;======================================================================================================================
;        ░██       ░██ ░██████████ ░██████  ░████████   ░████    ░██████  
;        ░██       ░██ ░██        ░██   ░██ ░██        ░██ ░██  ░██   ░██ 
;        ░██  ░██  ░██ ░██        ░██       ░███████  ░██ ░████       ░██ 
;        ░██ ░████ ░██ ░█████████ ░███████        ░██ ░██░██░██   ░█████  
;        ░██░██ ░██░██ ░██        ░██   ░██ ░██   ░██ ░████ ░██  ░██      
;        ░████   ░████ ░██        ░██   ░██ ░██   ░██  ░██ ░██  ░██       
;        ░███     ░███ ░██         ░██████   ░██████    ░████   ░████████ 
;======================================================================================================================
; Terrace ASCII Art - https://patorjk.com/software/taag/                                                          
;======================================================================================================================
; BUILD INSTRUCTIONS:
; ./vasm6502_oldstyle -Fbin -dotdir assembly_programs/main.s -o assembly_programs/rom.bin
; minipro -p "AT28C256" -w assembly_programs/rom.bin
;
;
;                                                                
;======================================================================================================================                                                              
; Memory map.
;======================================================================================================================
; 0x0100 - 0x01FF: Stack.
; 0x6000 - 0x6003: LCD interface.
; 0x6004 - 0x7FFF: Unused, VIA is selected in this range.
; 0x8000 - 0xFFFB: Program memory.
;
;
;
;
; 
;======================================================================================================================
; Prefix notations.
;======================================================================================================================
; $ = Hexadecimal prefix.
; % = Binary prefix.
; No prefix = Decimal number.
;
;
;
;
; 

;======================================================================================================================
; Initialization.
;======================================================================================================================
    .org $8000                  ; Start of program memory.
    jmp initialize              ; Jump to initialization routine.
    include "variables.s"       ; Include variable definitions.
    include "via_registers.s"   ; Include VIA register definitions.
    include "lcd.s"             ; Include LCD constant definitions.

initialize:
    ; Initialize stack pointer at top of stack memory.
    ldy #$02                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    ldx #$ff                 ; Load ones into x register.
    txs                      ; Transfer x to stack pointer.
    ldx #$00                 ; Clear x register.

    jsr initialize_via
    jsr initialize_lcd
    jsr clear_lcd
    jsr print

loop:
  jmp loop

end:
    ; End of program, fill remaining space with NOPs.

    ldy #$ff                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    
    ; Handle the remaining space in memory and set reset vector.
    .fill $fffc - *, $EA     ; Calculate the distance from the current address to $fffc; Fill remaining space with NOPs.
    .org $fffc               ; Set origin to reset vector location. This means the assembler is placing the following data at memory location $fffc.
    .word initialize         ; Set the reset vector loacation to start of ROM, 0x8000.
    .word $0000              ; Fill the remaining space after reset vector with 0x0000.
