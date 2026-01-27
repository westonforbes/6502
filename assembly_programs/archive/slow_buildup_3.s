; 6502 LCD Program - Corrected for Stepping
PORTA = $6001
DDRA  = $6003
PORTB = $6000
DDRB  = $6002

E  = %10000000
RW = %01000000
RS = %00100000

    .org $8000

reset:
    ldx #$FF
    txs

    ; Set VIA Directions
    lda #$FF
    sta DDRB                ; Port B = Output
    lda #%11100000
    sta DDRA                ; Port A (top 3) = Output

    ; Initialization Sequence
    lda #%00111000          ; Function Set (1st)
    jsr lcd_instruction
    lda #%00111000          ; Function Set (2nd)
    jsr lcd_instruction
    lda #%00111000          ; Function Set (3rd)
    jsr lcd_instruction
    
    lda #%00001110          ; Display On
    jsr lcd_instruction
    lda #%00000110          ; Entry Mode
    jsr lcd_instruction
    lda #%00000001          ; Clear Display
    jsr lcd_instruction

    ; Print "DARRYLL SUCKS"
    lda #'D'
    jsr print_char
    lda #'A'
    jsr print_char
    lda #'R'
    jsr print_char
    lda #'R'
    jsr print_char
    lda #'Y'
    jsr print_char
    lda #'L'
    jsr print_char
    lda #'L'
    jsr print_char
    lda #' '
    jsr print_char
    lda #'S'
    jsr print_char
    lda #'U'
    jsr print_char
    lda #'C'
    jsr print_char
    lda #'K'
    jsr print_char
    lda #'S'
    jsr print_char

stop:
    jmp stop                ; CRITICAL: Keeps CPU from falling into subroutines

print_char:
    sta PORTB
    lda #RS
    sta PORTA
    ora #E                  ; Pulse E high
    sta PORTA
    lda #RS                 ; Pulse E low
    sta PORTA
    rts

lcd_instruction:
    sta PORTB
    lda #0
    sta PORTA
    lda #E                  ; Pulse E high
    sta PORTA
    lda #0                  ; Pulse E low
    sta PORTA
    rts

    .fill $FFFC - *, $EA
    .org $FFFC
    .word reset
    .word $0000