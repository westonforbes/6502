E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.
SET_DRAM_ADDRESS = %10000000
CLEAR_DISPLAY   = %00000001
LINE_1 = %00000000
LINE_2 = %01000000

initialize_via:
    ; Set data direction of ports.
    ldy #$04                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    lda #%11111111           ; Set all pins on port B to output.
    sta DDRB
    lda #%11100000           ; Set top 3 pins on port A to output.
    sta DDRA
    ldy #$05                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    rts                      ; Return from subroutine.

initialize_lcd:
    ; Initialize the LCD display.

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
    rts                      ; Return from subroutine.

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


lcd_wait:
    ; This subroutine will run directly into the lcd_busy subroutine.
    ; We're pushing the accumulator value to the stack to preserve it,
    ; then we set port B to input mode to read the busy flag.
    ; We loop until the busy flag clears, then restore port B to output mode
    ; and pull the accumulator value from the stack before returning.

    pha            ; Push accumulator to stack to preserve its value.
    lda #%00000000 ; Wipe the A register.
    sta DDRB       ; Set port B to input mode.

lcd_busy:
    ; Loop until the busy flag clears.

    lda #RW        ; Set RW high for read.
    sta PORTA      ; Write to control port.
    lda #(RW | E)  ; Set E high to latch data.
    sta PORTA      ; Write to control port.
    lda PORTB      ; Read data from port B.
    and #%10000000 ; Mask all bits except the busy flag (bit 7).
    bne lcd_busy   ; If busy flag is set, continue waiting.

    lda #RW        ; Set RW high for read.
    sta PORTA      ; Write to control port.
    lda #%11111111 ; Port B is output
    sta DDRB       ; Set port B to output mode.
    pla            ; Pull accumulator from stack to restore its value.
    rts            ; Return from subroutine.

lcd_instruction:
    ; Send an instruction to the LCD.
    
    jsr lcd_wait   ; Wait until LCD is not busy.
    sta PORTB      ; Send instruction to data port.
    lda #0         ; Clear RS/RW/E bits.
    sta PORTA      ; Write to control port.
    lda #E         ; Set E bit to send instruction.
    sta PORTA      ; Write to control port.
    lda #0         ; Clear RS/RW/E bits.
    sta PORTA      ; Write to control port.
    rts            ; Return from subroutine.

print_character:
    ; Send a character to the LCD.

    jsr lcd_wait   ; Wait until LCD is not busy.
    sta PORTB      ; Send character to data port.
    lda #RS        ; Set RS. Clear RW/E bits.
    sta PORTA      ; Write to control port.
    lda #(RS | E)  ; Set E bit to send instruction.
    sta PORTA      ; Write to control port.
    lda #RS        ; Clear E bits.
    sta PORTA      ; Write to control port.
    rts            ; Return from subroutine.

print:
    ; Print the message string to the LCD.

    lda message,x       ; Load character from message string. (Take the byte at address "message" plus the value in the x register.)
    beq loop            ; If null terminator, jump to loop.
    jsr print_character ; Print the character.

    cpx #16             ; x = 16?
    beq goto_line_2     ; If true, go to this label, else continue to next line.
    inx                 ; Increment index.
    jmp print           ; Repeat.

goto_line_2:
    lda #(SET_DRAM_ADDRESS | LINE_2 | 0)    ; Set DDRAM address to start of line 2.
    jsr lcd_instruction                     ; Send instruction to LCD.
    lda #2                                  ; Update current line pointer to 2.
    sta CURRENT_DISPLAY_LINE
    jmp print