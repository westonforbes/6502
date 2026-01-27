    .org $8000               ; Start of program memory.
    ldy #$01                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    
initialize_stack:
    ; Initialize stack pointer at top of stack memory.
    ldx #$ff                 ; Load ones into x register.
    txs                      ; Transfer x to stack pointer.
    ldx #$00                 ; Clear x register.
    ldy #$02                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.

end:
    ; Handle the remaining space in memory and set reset vector.
    .fill $fffc - *, $EA     ; Calculate the distance from the current address to $fffc; Fill remaining space with NOPs.
    .org $fffc               ; Set origin to reset vector location. This means the assembler is placing the following data at memory location $fffc.
    .word $8000              ; Set the reset vector loacation to start of ROM, 0x8000.
    .word $0000              ; Fill the remaining space after reset vector with 0x0000.
