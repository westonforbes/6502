; # - Immediate modifiers.
; $ - Hexadecimal notation.
; % - Binary notation.

PORTA = $6001
DDRA  = $6003
PORTB = $6000
DDRB  = $6002

    .org $8000          ; Set origin to address 0x8000.

reset:
    ; Set the VIA port B data direction to output (all bits).
    lda #%11111111      ; Load register A with all ones.
    sta DDRB           ; Set all bits of port B as output.

loop:
    ; Load alternating bit pattern to port B.
    lda #%01010101      ; Load register A.
    sta PORTB          ; Write value of register A to VIA port B.
    nop                 ; No operation (delay).
    nop                 ; No operation (delay).
    nop                 ; No operation (delay).

    ; Load alternating bit pattern 0xAA to port B.
    lda #%10101010      ; Load register A.
    sta PORTB          ; Write value of register A to VIA port B.

    jmp loop            ; Repeat the loop indefinitely.

    .org $FFFC          ; Set origin to reset vector location.
    .word reset         ; Set reset vector to reset of program at 0x8000.
    .word $0000         ; Fill the remaining space after reset vector with 0x0000.