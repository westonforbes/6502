; # - Immediate modifiers.
; $ - Hexadecimal notation.
; % - Binary notation.

PORTA = $6001
DDRA  = $6003
PORTB = $6000
DDRB  = $6002

E  = %10000000
RW = %01000000
RS = %00100000

    .org $8000              ; Set origin to address 0x8000.

reset:

    ; Set up stack pointer.
    ldx #%11111111;$FF      ; Initialize stack pointer (standard 6502 practice).
    txs                     ; Transfer X register to stack pointer.

    ; Set the VIA port B data direction.
    lda #%11111111;$FF      ; Load into register A the value to set all bits of port B as output.
    sta DDRB                ; Set all bits of port B as output.

    ; Set the VIA port A data direction.
    lda #%11100000;$E0      ; Load into register A the value to set upper 3 bits of port A as output, lower 5 bits as input.
    sta DDRA                ; Set upper 3 bits of port A as output, lower 5 bits as input.

    ; Initialize the LCD.
    lda #%00111000;$38      ; Set 8-bit mode; 2-line display; 5x8 font.
    jsr lcd_instruction
    lda #%00111000;$38      ; Set 8-bit mode; 2-line display; 5x8 font.
    jsr lcd_instruction
    lda #%00001110;$0E      ; Display on; cursor on; blink off.
    jsr lcd_instruction
    lda #%00000110;$06      ; Increment and shift cursor; don't shift display.
    jsr lcd_instruction
    lda #%00000001;$01      ; Clear display.
    jsr lcd_instruction

    ; Clear the X register for use as an index.
    ldx #%00000000          ; Initialize X register to 0.



lcd_instruction:
    sta PORTB
    lda #%00000000          ; Clear RS/RW/E bits.
    sta PORTA
    lda #E                  ; Set E bit to send instruction.
    sta PORTA
    lda #%00000000          ; Clear RS/RW/E bits.
    sta PORTA
    rts

footer:

    ; This calculates the distance from the current address to $FFFC
    ; and fills it with $EA (NOP instruction)
    .fill $FFFC - *, $EA

    .org $FFFC          ; Set origin to reset vector location.
    .word reset         ; Set reset vector to reset of program at 0x8000.
    .word $0000         ; Fill the remaining space after reset vector with 0x0000.