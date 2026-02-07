E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.
SET_DRAM_ADDRESS = %10000000
CLEAR_DISPLAY   = %00000001
LINE_0 = %00000000
LINE_1 = %01000000


; Initialize the LCD display.
initialize_lcd:

    ; Function Set - 8-bit, 2-line, 5x8 font
    lda #%00111000
    jsr lcd_instruction

    ; Display ON/OFF Control
    ; Display on, cursor off, blinking off
    lda #%00001100
    jsr lcd_instruction

    ; Entry Mode Set
    ; Increment address AND shift display left on write
    lda #%00000111
    jsr lcd_instruction

    ; --- Set Initial Position for Calculator Mode ---
    ; Set DDRAM address to the 16th character (index 15 = $0F)
    ; This ensures the first character typed appears at the far right.
    lda #(SET_DRAM_ADDRESS | LINE_0 | 15)
    jsr lcd_instruction

    ; Initialize our tracker
    lda #0
    sta CHAR_ROW
    lda #15
    sta CHAR_COL

    rts

clear_lcd:

    ; Clear Display - See page 24 & 26 of Hitachi HD44780 datasheet.
    lda #CLEAR_DISPLAY
    jsr lcd_instruction

    ; Set DDRAM Address.
    lda #(SET_DRAM_ADDRESS | LINE_0 | 0)
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
