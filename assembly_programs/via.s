PORTB = $6000   ; Data port for LCD.
PORTA = $6001   ; Upper 3 bits are control port for LCD, lower 5 bits are inputs.
DDRB  = $6002   ; Data direction register for port B.
DDRA  = $6003   ; Data direction register for port A.

; Set data direction of ports.
initialize_via:
    
    ; Set all pins on port B to output.
    lda #%11111111
    sta DDRB

    ; Set top 3 pins on port A to output and the bottom 5 pins to input.
    lda #%11100000
    sta DDRA

    ; Return to caller.
    rts