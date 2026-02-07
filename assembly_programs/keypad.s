KEY_AVAILABLE_MASK = %00010000 ; Bit 4 of port A indicates if a key is available.

check_if_key_available:
    ; Check if a key is available by reading the appropriate bit from the VIA's port A.
    ; If a key is available, the bit will be 0; if no key is available, the bit will be 1.
    lda PORTA
    and #KEY_AVAILABLE_MASK
    bne key_available
    rts

key_available:
    ; If a key is available, read the key value from port A and store it in the A register.
    lda PORTA
    and #%00001111 ; Mask out the upper 4 bits to get the key value.
    tay ; Transfer accumulator to Y register to use as an index for the key map.
    lda key_map,y; Load the corresponding character from the key map into the A register.
    jmp key_map_done

key_map: .asciiz "123A456B789C*0#D"

key_map_done:
    sta CHAR_VAL                ; Store the last key pressed in memory.
    ;jsr clear_lcd
    jsr print_character                 ; Print the key value to the LCD.
    rts

