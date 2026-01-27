; VIA register definitions.
PORTB = $6000   ; Data port for LCD.
PORTA = $6001   ; Upper 3 bits are control port for LCD, lower 5 bits are inputs.
DDRB  = $6002   ; Data direction register for port B.
DDRA  = $6003   ; Data direction register for port A.

; Control pins for the LCD.
E  = %10000000  ; Enable pin, high to latch data.
RW = %01000000  ; Read/write pin, high for read, low for write.
RS = %00100000  ; Register select pin, high for data, low for instruction.

  .org $8000    ; Start of program memory.
  ldy #$01      ; Using the Y register for breadcrumb debugging.
; Define the message to be displayed.
message: .asciiz "Hello, world!"

stack_initialize:
  ; Initialize stack pointer at top of stack memory.
  ldx #$ff              ; Load ones into x register.
  txs                   ; Transfer x to stack pointer.
  ldx #$00              ; Clear x register.

via_initialize:
  ; Set data direction of ports.
  lda #%11111111        ; Set all pins on port B to output.
  sta DDRB
  lda #%11100000        ; Set top 3 pins on port A to output.
  sta DDRA

lcd_initialize:
  lda #%00111000        ; Set 8-bit mode, 2-line display, 5x8 text cells.
  jsr lcd_instruction
  lda #%00001110        ; Set display on, cursor on, blink off.
  jsr lcd_instruction
  lda #%00000110        ; Increment and shift cursor, don't shift display.
  jsr lcd_instruction
  lda #$00000001        ; Clear display.
  jsr lcd_instruction
  nop
  nop
  nop

print:
  lda message,x
  beq loop
  jsr print_char
  inx
  jmp print

loop:
  jmp loop

lcd_wait:
  pha
  lda #%00000000  ; Port B is input
  sta DDRB
lcdbusy:
  lda #RW
  sta PORTA
  lda #(RW | E)
  sta PORTA
  lda PORTB
  and #%10000000
  bne lcdbusy

  lda #RW
  sta PORTA
  lda #%11111111  ; Port B is output
  sta DDRB
  pla
  rts

lcd_instruction:
  jsr lcd_wait
  sta PORTB
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  lda #E         ; Set E bit to send instruction
  sta PORTA
  lda #0         ; Clear RS/RW/E bits
  sta PORTA
  rts

print_char:
  jsr lcd_wait
  sta PORTB
  lda #RS         ; Set RS; Clear RW/E bits
  sta PORTA
  lda #(RS | E)   ; Set E bit to send instruction
  sta PORTA
  lda #RS         ; Clear E bits
  sta PORTA
  rts

  .org $fffc
  .word stack_initialize
  .word $0000
