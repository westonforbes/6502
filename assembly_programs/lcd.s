E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.
SET_DRAM_ADDRESS = %10000000
CLEAR_DISPLAY   = %00000001
LINE_0 = %00000000
LINE_1 = %01000000


; Initialize the LCD display.
initialize_lcd:

    ; Function Set - See page 24 & 27 of Hitachi HD44780 datasheet.
    ; BIT 0 - Don't care.
    ; BIT 1 - Don't care.
    ; BIT 2 - F - Set to 1 for 5x10 dot character font, 0 for 5x8 dot character font.
    ; BIT 3 - N - Set to 1 for 2-line display, 0 for 1-line display.
    ; BIT 4 - DL - Set to 1 for 8-bit interface, 0 for 4-bit interface.
    ; BIT 5 - Function Set Flag - Must be 1 for this instruction.
    lda #%00111000
    jsr lcd_instruction

    ; Display ON/OFF Control - See page 24 & 26 of Hitachi HD44780 datasheet.
    ; BIT 0 - B - Blink cursor (1) or steady (0).
    ; BIT 1 - C - Display cursor (1) or hide cursor (0).
    ; BIT 2 - D - Display on (1) or off (0)
    ; BIT 3 - Display On/Off Control Flag - Must be 1 for this instruction.
    lda #%00001100
    jsr lcd_instruction

    ; Entry Mode Set - See page 24 & 26 of Hitachi HD44780 datasheet.
    ; BIT 0 - S - Shift display when writing (1 = shift, 0 = no shift)
    ; BIT 1 - I/D - Increment (1) or decrement (0) cursor position when writing.
    ; BIT 2 - Entry Mode Set Flag - Must be 1 for this instruction.
    lda #%00000111
    jsr lcd_instruction

    ; Return to caller.
    rts

clear_lcd:

    ; Clear Display - See page 24 & 26 of Hitachi HD44780 datasheet.
    lda #CLEAR_DISPLAY
    jsr lcd_instruction

    ; Set cursor to the 16th character of line 2.
    lda #(SET_DRAM_ADDRESS | LINE_1 | 16)
    jsr lcd_instruction

    ; Return to caller.
    rts

lcd_wait:
    ; This subroutine will run directly into the lcd_busy subroutine.
    ; We're pushing the accumulator value to the stack to preserve it,
    ; then we set port B to input mode to read the busy flag.
    ; We loop until the busy flag clears, then restore port B to output mode
    ; and pull the accumulator value from the stack before returning.

    ; Push whatever is in the A register to the stack to preserve it for later.
    pha

    ; Wipe the A register and set port B to input mode.
    lda #%00000000
    sta DDRB

    ; No RTS here, we fall through to the next label.

lcd_busy:
    ; Loop until the busy flag clears.

    ; Set the VIA to read mode by setting RW high on the control port.
    lda #RW
    sta PORTA

    ; Set E high to latch data.
    lda #(RW | E)
    sta PORTA

    ; Read in port B to the A register and AND with mask to isolate busy flag.
    lda PORTB
    and #%10000000

    ; If the busy flag is set, jump back to lcd_busy to keep waiting.
    bne lcd_busy

    ; If we get here, the busy flag is clear.

    ; Set E low and read/write to high (read mode) to finish the read cycle.
    lda #RW
    sta PORTA

    ; Set port B back to output mode.
    lda #%11111111
    sta DDRB

    ; Pull the preserved A register value from the stack.
    pla

    ; Return to caller.
    rts

; Send an instruction to the LCD.
lcd_instruction:
    
    ; Wait until LCD is not busy.
    jsr lcd_wait

    ; Send instruction to data port.
    sta PORTB

    ; Clear RS/RW/E bits.
    lda #0
    sta PORTA

    ; Set E bit to send instruction.
    lda #E
    sta PORTA

    ; Clear RS/RW/E bits.
    lda #0
    sta PORTA

    ; Return to caller.
    rts

; Send a character to the LCD.
print_character:

    ; Preserve A register.
    pha

    ; Wait until LCD is not busy.
    jsr lcd_wait

    ; Load character value from memory into A register.
    lda CHAR_VAL

    ; Send character to data port.
    sta PORTB

    ; Set RS while preserving lower 5 bits on PORTA.
    lda PORTA
    and #%00011111
    ora #RS
    sta PORTA

    ; Set E bit to send instruction (preserve lower 5 bits).
    lda PORTA
    and #%00011111
    ora #(RS | E)
    sta PORTA

    ; Clear E bit (keep RS, preserve lower 5 bits).
    lda PORTA
    and #%00011111
    ora #RS
    sta PORTA

    ; Pull preserved A register value from stack.
    pla

    ; Return to caller.
    rts
