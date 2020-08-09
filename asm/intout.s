ECHO = $FFEF

value = $0200 ; 2 bytes (short)
mod10 = $0202 ; 2 bytes (short)
message = $0204 ; 

    processor 6502

    org $300

    lda #$8D
    jsr ECHO
    jsr ECHO

    ; load value then call shortout
    lda number
    sta value
    lda number + 1
    sta value + 1
    jsr shortout
    lda #$8D
    jsr ECHO
    jsr ECHO
    jmp $FF00

shortout:
    lda #0
    sta message

shortout_0mod10:
    ; init modulus
    lda #0
    sta mod10
    sta mod10 + 1
    clc

    ldx #16
shortout_loop:
    rol value
    rol value + 1
    rol mod10
    rol mod10 + 1

    sec
    lda mod10
    sbc #10
    tay
    lda mod10 + 1
    sbc #0
    bcc shortout_next ; branch if dividen < divisor
    sty mod10
    sta mod10 + 1

shortout_next:
    dex
    bne shortout_loop

    rol value
    rol value + 1

    ; we have a char to print in lsb of mod10
    lda mod10
    clc
    adc #'0
    jsr push_char

    lda value
    ora value + 1
    bne shortout_0mod10

    ;; fall through to print_message

print_message:
    ldx #0
print_message_loop:
    lda message,x
    beq print_message_done
    ora #$80
    jsr ECHO
    inx
    jmp print_message_loop

print_message_done:
    rts

push_char:
    pha ; push char to stack
    ldy #0

push_char_loop:
    lda message,y
    tax
    pla
    sta message,y
    iny
    txa
    pha
    bne push_char_loop

    pla
    sta message,y 
    
    rts

number DC.w 54321
