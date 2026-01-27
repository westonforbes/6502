; VIA register definitions.
PORTB = $6000   ; Data port for LCD.
PORTA = $6001   ; Upper 3 bits are control port for LCD, lower 5 bits are inputs.
DDRB  = $6002   ; Data direction register for port B.
DDRA  = $6003   ; Data direction register for port A.

; Control pins for the LCD.
E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.

    .org $8000               ; Start of program memory.
    ldy #$01                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    
initialize_stack:
    ; Initialize stack pointer at top of stack memory.
    ldy #$02                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    ldx #$ff                 ; Load ones into x register.
    txs                      ; Transfer x to stack pointer.
    ldx #$00                 ; Clear x register.
    ldy #$03                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.

via_initialize:
    ; Set data direction of ports.
    ldy #$04                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    lda #%11111111           ; Set all pins on port B to output.
    sta DDRB
    lda #%11100000           ; Set top 3 pins on port A to output.
    sta DDRA
    ldy #$05                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.

end:
    ldy #$ff                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    ; Handle the remaining space in memory and set reset vector.
    .fill $fffc - *, $EA     ; Calculate the distance from the current address to $fffc; Fill remaining space with NOPs.
    .org $fffc               ; Set origin to reset vector location. This means the assembler is placing the following data at memory location $fffc.
    .word $8000              ; Set the reset vector loacation to start of ROM, 0x8000.
    .word $0000              ; Fill the remaining space after reset vector with 0x0000.
