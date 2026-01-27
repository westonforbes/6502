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

reset:
    ldy #$01                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.



initialize_stack:
    ; Initialize stack pointer at top of stack memory.
    ldy #$02                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    ldx #$ff                 ; Load ones into x register.
    txs                      ; Transfer x to stack pointer.
    ldx #$00                 ; Clear x register.
    ldy #$03                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.

initialize_via:
    ; Set data direction of ports.
    ldy #$04                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    lda #%11111111           ; Set all pins on port B to output.
    sta DDRB
    lda #%11100000           ; Set top 3 pins on port A to output.
    sta DDRA
    ldy #$05                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.

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

    ; Clear Display - See page 24 & 26 of Hitachi HD44780 datasheet.
    lda #%00000001
    jsr lcd_instruction

print:
    ; Print the message string to the LCD.

    lda message,x       ; Load character from message string.
    beq loop            ; If null terminator, jump to loop.
    jsr print_character ; Print the character.
    inx                 ; Increment index.
    jmp print           ; Repeat.

loop:
  jmp loop

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

; Data section.
message: .asciiz "6502 Computer"

end:
    ; End of program, fill remaining space with NOPs.

    ldy #$ff                 ; Using the Y register for breadcrumb debugging, ldy immediate is 0xa0.
    
    ; Handle the remaining space in memory and set reset vector.
    .fill $fffc - *, $EA     ; Calculate the distance from the current address to $fffc; Fill remaining space with NOPs.
    .org $fffc               ; Set origin to reset vector location. This means the assembler is placing the following data at memory location $fffc.
    .word $8000              ; Set the reset vector loacation to start of ROM, 0x8000.
    .word $0000              ; Fill the remaining space after reset vector with 0x0000.
