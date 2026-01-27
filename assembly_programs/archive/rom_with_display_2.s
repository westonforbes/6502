; Assembly symbols.
; # - Immediate modifiers.
; $ - Hexadecimal notation.
; % - Binary notation.

; Our VIA occupies the 0x6000 to 0x8000 range in the 6502 address space. We actually only use the four registers below.
; but for wiring simplicity, the VIA technically "owns" all addresses in that range.
PORTA = $6001
DDRA  = $6003
PORTB = $6000
DDRB  = $6002
WRITE_DATA = %11000000  ; E (7) = 1 (enable), RS (6) = 1 (data), RWB (5) = 0 (write).
WRITE_CMD = %10000000  ; E (7) = 1 (enable), RS (6) = 0 (command), RWB (5) = 0 (write).

    ; Our ROM occupies the upper 32KB of the 6502 address space, so we need to start at the halfway point which is 0x8000 in hex.
    .org $8000          ; Set origin to address 0x8000.

reset:

    ldx #$ff                ; Initialize stack pointer (standard 6502 practice)
    txs                     ; Transfer X register to stack pointer.

    ; Set the VIA port B data direction.
    lda #%11111111          ; Load into register A the value to set all bits of port B as output.
    sta DDRB                ; Set all bits of port B as output.

    ; Set the VIA port A data direction.
    lda #%11100000          ; Load into register A the value to set upper 3 bits of port A as output, lower 5 bits as input.
    sta DDRA                ; Set upper 3 bits of port A as output, lower 5 bits as input.
    jsr display_setup       ; Call subroutine to set up the display.
    jsr write_message    ; Call subroutine to write message to display.

loop:
    jmp loop       ; Repeat the loop indefinitely.

write_to_display:

    ; Write to display by toggling the E (Enable) control line. Before calling this subroutine,
    ; run either lda WRITE_DATA or lda WRITE_CMD to set RS and RWB appropriately.

    ; msb to lsb, big endian.
    ; bit 7, PORTA = E (Enable) = 1 to enable, 0 to disable.
    ; bit 6, PORTA = RS (Register Select) = 1 for data, 0 for command.
    ; bit 5, PORTA = RWB (Read/Write(bar)) = 1 for read, 0 for write.
    ; bit 4, PORTA = Not connected to display.
    ; bit 3, PORTA = Not connected to display.
    ; bit 2, PORTA = Not connected to display.
    ; bit 1, PORTA = Not connected to display.
    ; bit 0, PORTA = Not connected to display.

    sta PORTA           ; Write whatever was loaded in register A to VIA port A.
    nop                 ; Small delay for the display to update.
    nop                 ; Small delay for the display to update.
    nop                 ; Small delay for the display to update.
    eor #%10000000      ; Exclusive OR register A with 0x80 to set E = 1 (enable).
    sta PORTA           ; Write to VIA port A.

    rts                 ; Return from subroutine.

display_setup:

    ; Set the RS (Register Select) and RWB (Read/Write(bar)) control lines. See Hitatchi HD44780U datasheet page 23, 24, 25 & 40.
    ; msb to lsb, big endian.
    ; bit 7, PORTA = E (Enable) = 1 to enable, 0 to disable.
    ; bit 6, PORTA = RS (Register Select) = 1 for data, 0 for command.
    ; bit 5, PORTA = RWB (Read/Write(bar)) = 1 for read, 0 for write.
    ; bit 4, PORTA = Not connected to display.
    ; bit 3, PORTA = Not connected to display.
    ; bit 2, PORTA = Not connected to display.
    ; bit 1, PORTA = Not connected to display.
    ; bit 0, PORTA = Not connected to display.
    lda #%01100000          ; Set RS = 1 (data), RWB = 0 (write), E = 0 (disabled).
    sta PORTA               ; Write to VIA port A.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.
    
    ; Function set. See Hitatchi HD44780U datasheet page 24, 27 & 40.
    ; msb to lsb, big endian.
    ; bit 7 = 0.
    ; bit 6 = 0.
    ; bit 5 = command bit.
    ; bit 4 = 8 bit mode = 1 / 4 bit mode = 0.
    ; bit 3 = 2 line display = 1 / 1 line display = 0.
    ; bit 2 = 5x10 dots = 1 / 5x8 dots = 0.
    ; bit 1 = don't care.
    ; bit 0 = don't care.
    lda #%00110000          ; Set 8 bit mode, 2 line display, 5x8 dots.
    sta PORTB               ; Write to VIA port B.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.
    lda #%00110000          ; Set 8 bit mode, 2 line display, 5x8 dots.
    sta PORTB               ; Write to VIA port B.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.
    lda #%00110000          ; Set 8 bit mode, 2 line display, 5x8 dots.
    sta PORTB               ; Write to VIA port B.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.
    
    ; Display on/off control. See Hitatchi HD44780U datasheet page 24, 26 & 40.
    ; msb to lsb, big endian.
    ; bit 7 = 0.
    ; bit 6 = 0.
    ; bit 5 = 0.
    ; bit 4 = 0.
    ; bit 3 = command bit.
    ; bit 2 = display on = 1 / off = 0.
    ; bit 1 = cursor on = 1 / off = 0.
    ; bit 0 = blinking cursor on = 1 / off = 0
    lda #%00001110          ; Display on, cursor on, blinking cursor off.
    sta PORTB               ; Write to VIA port B.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.

    ; Entry mode set. See Hitatchi HD44780U datasheet page 24, 26 & 40.
    ; msb to lsb, big endian.
    ; bit 7 = 0.
    ; bit 6 = 0.
    ; bit 5 = 0.
    ; bit 4 = 0.
    ; bit 3 = 0.
    ; bit 2 = command bit.
    ; bit 1 = increment cursor position = 1 / decrement = 0.
    ; bit 0 = display shift on = 1 / off = 0.
    lda #%00000110          ; Set entry mode: increment cursor, no display shift.
    sta PORTB               ; Write to VIA port B.
    lda #WRITE_CMD
    jsr write_to_display    ; Call subroutine to write command to display.
    ; Display is now ready for data loading.

    rts                     ; Return from subroutine.

write_message:
    ; Write "HELLO" to the display.
    lda #'H'            ; Load character 'H'.
    sta PORTB           ; Write to VIA port B.
    lda #WRITE_DATA
    jsr write_to_display
    lda #'E'            ; Load character 'E'.
    sta PORTB           ; Write to VIA port B.
    lda #WRITE_DATA
    jsr write_to_display
    lda #'L'            ; Load character 'L'.
    sta PORTB           ; Write to VIA port B.
    lda #WRITE_DATA
    jsr write_to_display
    lda #'L'            ; Load character 'L'.
    sta PORTB           ; Write to VIA port B.
    lda #WRITE_DATA
    jsr write_to_display
    lda #'O'            ; Load character 'O'.
    sta PORTB           ; Write to VIA port B.
    lda #WRITE_DATA
    jsr write_to_display
    
    rts                 ; Return from subroutine.


    .org $FFFC     ; Set origin to reset vector location.
    .word reset    ; Set reset vector to start of program at 0x8000.
    .word $0000    ; Fill the remaining space after reset vector with 0x0000.