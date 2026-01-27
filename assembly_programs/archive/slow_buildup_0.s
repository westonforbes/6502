; # - Immediate modifiers.
; $ - Hexadecimal notation.
; % - Binary notation.

PORTA = $6001
DDRA  = $6003
PORTB = $6000
DDRB  = $6002

    .org $8000          ; Set origin to address 0x8000.

reset:

    ; Set up stack pointer.
    ldx #$ff                ; Initialize stack pointer (standard 6502 practice).
    txs                     ; Transfer X register to stack pointer.

    ; Set the VIA port B data direction.
    lda #%11111111          ; Load into register A the value to set all bits of port B as output.
    sta DDRB                ; Set all bits of port B as output.

    ; Set the VIA port A data direction.
    lda #%11100000          ; Load into register A the value to set upper 3 bits of port A as output, lower 5 bits as input.
    sta DDRA                ; Set upper 3 bits of port A as output, lower 5 bits as input.



footer:

    ; This calculates the distance from the current address to $FFFC
    ; and fills it with $EA (NOP instruction)
    .fill $FFFC - *, $EA

    .org $FFFC          ; Set origin to reset vector location.
    .word reset         ; Set reset vector to reset of program at 0x8000.
    .word $0000         ; Fill the remaining space after reset vector with 0x0000.