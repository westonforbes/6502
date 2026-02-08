; Memory for current character position and value.
CHAR_COL = $0200
CHAR_ROW = $0201
CHAR_VAL = $0202

; Memory reflecting what is currently on the upper row of the LCD. LSB is rightmost character on the LCD.
CHAR_STORE_ROW_0 = $0203 ; End of space is $0212.
; Memory reflecting what is currently on the lower row of the LCD. LSB is rightmost character on the LCD.
CHAR_STORE_ROW_1 = $0213 ; End of space is $0222.

KEY_PRESSED_FLAG = $0223