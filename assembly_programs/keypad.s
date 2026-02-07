key_map: .asciiz "123A456B789C*0#D"
KEY_AVAILABLE_MASK = %00010000 ; Bit 4 of port A indicates if a key is available.

check_if_key_available:
    lda PORTA               ; Read port A into register A to take a snapshot of current state.
    pha                     ; Push the snapshot to the stack to preserve it for later masking.
    and #KEY_AVAILABLE_MASK ; AND with the mask to isolate the key available bit.
    beq no_key_pressed      ; If false, jump to no_key_pressed.

    ; Key is available, now check if it is a new press.
    lda KEY_PRESSED_FLAG    ; Load the current value of KEY_PRESSED_FLAG to check if we've already processed this key press.
    bne end_check_if_key_available ; If KEY_PRESSED_FLAG is already set, we've already processed this key press, so we can skip to the end.

    ; New key press detected.
    lda #1                  ; Load 1 into register A.
    sta KEY_PRESSED_FLAG    ; Store 1 in KEY_PRESSED_FLAG to indicate a key is currently pressed.
    pla                     ; Pull the original PORTA snapshot back from the stack.
    pha                     ; Push it back immediately so the stack cleanup at the end remains consistent.
    and #%00001111          ; Mask out the upper 4 bits to get the key value from the snapshot.
    tax                     ; Transfer A register to x register to use as an index for the key map.
    lda key_map,x           ; Load the corresponding character from the key map into the A register.
    sta CHAR_VAL            ; Store the last key pressed in memory.
    jsr print_character     ; Print the key value to the LCD.
    jmp end_check_if_key_available ; Jump to the end to clean up the stack and return.

no_key_pressed:
    lda #0                  ; Clear the A register.
    sta KEY_PRESSED_FLAG    ; Store 0 in KEY_PRESSED_FLAG to indicate no key is currently pressed.

end_check_if_key_available:
    pla                     ; Pull the snapshot from the stack to keep the stack pointer balanced.
    rts                     ; Return to caller.