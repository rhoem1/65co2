.segment "CODE"
.ifndef APPLE1
ISCNTC:
        lda     $C000
        cmp     #$83
        beq     L0ECC
        rts
L0ECC:
        jsr     RDKEY
        cmp     #$03
.else
ISCNTC:
        lda KBcr     
        bpl not_cntc 
        jsr MONRDKEY
        and #$7F
        cmp #3
        beq is_cntc
not_cntc:
        rts

is_cntc:
.endif
;!!! runs into "STOP"
