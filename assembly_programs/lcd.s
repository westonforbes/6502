E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.
SET_DRAM_ADDRESS = %10000000
CLEAR_DISPLAY   = %00000001
LINE_1 = %00000000
LINE_2 = %01000000

; Set data direction of ports.
initialize_via:
    
    ; Set all pins on port B to output.
    lda #%11111111
    sta DDRB

    ; Set top 3 pins on port A to output.
    lda #%11100000
    sta DDRA

    ; Return to caller.
    rts

; Initialize the LCD display.
initialize_lcd:

    ; Function Set - See pages 24, 25 & 27 of Hitachi HD44780 datasheet.
    ; Set 8 bit mode.
    ; Set 2 line display.
    ; Set 5x8 font.
    lda #%00111000
    jsr lcd_instruction

    ; Display ON/OFF Control - See page 24 & 26 of Hitachi HD44780 datasheet.
    ; Turn display on.
    ; Turn cursor on.
    ; Turn blinking off.
    lda #%00001110
    jsr lcd_instruction

    ; Entry Mode Set - See page 24, 25 & 26 of Hitachi HD44780 datasheet.
    ; Set cursor to increment.
    ; Do not shift display.
    lda #%00000110
    jsr lcd_instruction

    ; Return to caller.
    rts

clear_lcd:

    ; Clear Display - See page 24 & 26 of Hitachi HD44780 datasheet.
    lda #CLEAR_DISPLAY
    jsr lcd_instruction

    ; Set DDRAM Address.
    lda #(SET_DRAM_ADDRESS | LINE_1 | 0)
    jsr lcd_instruction

    ; Keep track of which line we're on.
    lda #1
    sta CURRENT_DISPLAY_LINE

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

    ; Wait until LCD is not busy.
    jsr lcd_wait

    ; Send character to data port.
    sta PORTB

    ; Set RS. Clear RW/E bits.
    lda #RS
    sta PORTA

    ; Set E bit to send instruction.
    lda #(RS | E)
    sta PORTA

    ; Clear E bits.
    lda #RS
    sta PORTA

    ; Return to caller.
    rts

; Print the message string to the LCD.
print:

    ; Load character from message string. (Take the byte at address "message" plus the value in the x register.)
    lda message,x

    ; If null terminator, jump to loop.
    beq loop

    ; Print the character.
    jsr print_character

    ; x = 16?
    cpx #15

    ; If true, go to this label, else continue to next line.
    beq goto_line_2

    ; Increment index.
    inx

    ; Repeat.
    jmp print

goto_line_2:

    ; Increment index.
    inx

    ; Set DDRAM address to start of line 2.
    lda #(SET_DRAM_ADDRESS | LINE_2 | 0)
    jsr lcd_instruction

    ; Update current line pointer to 2.
    lda #2
    sta CURRENT_DISPLAY_LINE

    ; Continue printing.
    jmp print