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
        bpl NOT_CNTC 
        jsr MONRDKEY
        and #$7F
        cmp #3
        beq IS_CNTC
NOT_CNTC:
        rts

IS_CNTC:
.endif
;!!! runs into "STOP"
