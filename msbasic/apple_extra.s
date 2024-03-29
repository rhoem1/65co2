.segment "EXTRA"

.ifdef APPLE1

EXITEMU:
      lda $D01f
      rts

; GETchar from wozmon, mostly
MONRDKEY:
        lda     KBcr     
        bpl     MONRDKEY
        lda     KBin    ; get key
        rts

MONRDLINE:
        stx     $33   ; save X

;X = 0
;WHILE A != CR DO
  ; A = GETCH
  ; COUT
  ; IF A = BS THEN
    ; X--
    ; IF X < 0 THEN
      ; BEEP
      ; X = 0
      ; CONTINUE
  ; IF A != CR THEN
    ; INPUTBUFFER,X = A
    ; X++
    ; IF X > 240 THEN
      ; BEEP
      ; X = 240
      ; CONTINUE
; INPUTBUFFER,X = 0

        ldx     #0
APPLENEXTCHAR:
        jsr     MONRDKEY      ; get a key
        and     #$7F

        cmp     #$5F           ; backspace/underline?
        bne     APPLENOTBACKSPACE

        dex
        bmi     APPLEBACKSPACELIM  ; < 0?
        jsr     MONCOUT       ; show it
        jmp     APPLENEXTCHAR
APPLEBACKSPACELIM:
        ldx     #0
        beq     APPLELINELIM

APPLENOTBACKSPACE:
        cmp     #$3     ; CTRL-C
        bne APPLENOTCTRLC
        jmp     IS_CNTC

APPLENOTCTRLC:
        jsr     MONCOUT       ; show it
        ; store it
        sta     INPUTBUFFER,x 
        inx                 

        cmp     #$0D    ; Was CR?
        beq     APPLEHASCR   

        ; input length limit
        txa
        cmp     #$EF
        bcc     APPLENEXTCHAR

APPLELINELIM:
        lda     #$07     ; output a BELL
        jsr     MONCOUT
        jmp     APPLENEXTCHAR


APPLEHASCR:
        dex
        lda #0
        sta INPUTBUFFER,x

; replace CR with a 0, strip high bit
;        ldx     #0
;L2907:
;        lda     INPUTBUFFER,x
;        and     #$7F
;        cmp     #$0D
;        bne     L2912
;        lda     #$00
;L2912:
;        sta     INPUTBUFFER,x
;        inx
;        bne     L2907

        ; return to sender
        ldx     $33
        rts

INLINX:
        jsr     OUTQUES
        jsr     OUTSP
        jmp     INLIN

USR_FUNC:
        jsr     L29DA
        lda     FAC+3
        sta     FAC+5
        jmp     (FAC+4)
L29DA:
        jmp     (GOAYINT)

.endif




.ifdef APPLE_2
MONRDLINE:
        jsr     LFD6A
        ; turn CR to a #$00
        stx     $33
        ldx     #$00
L2907:
        lda     $0200,x
        and     #$7F
        cmp     #$0D
        bne     L2912
        lda     #$00
L2912:
        sta     $0200,x
        inx
        bne     L2907
        ldx     $33
        ; done
        rts



        .byte   0,0,0

PLT:
        jmp     L29F0
L291E:
        cmp     #$47
        bne     L2925
        jmp     L29E0
L2925:
        cmp     #$43
        bne     L292B
        beq     L2988
L292B:
        cmp     #$50
        beq     L2930
        inx
L2930:
        stx     $33
L2932:
        jsr     FRMEVL
        jsr     ROUND_FAC
        jsr     AYINT
        lda     FAC+4
        ldx     $33
        sta     $0300,x
        dec     $33
        bmi     L294Dx
        lda     #$2C
		jsr     SYNCHR
        bpl     L2932
L294Dx:
        tay
        pla
        cmp     #$43
        bne     L2957
        tya
        jmp     LF864
L2957:
        cmp     #$50
        bne     L2962
        tya
        ldy     $0301
        jmp     LF800
L2962:
        pha
        lda     $0301
        sta     $2C
        sta     $2D
        pla
        cmp     #$48
        bne     L2978
        lda     $0300
        ldy     $0302
        jmp     LF819
L2978:
        cmp     #$56
        beq     L297F
        jmp     SYNERR
L297F:
        ldy     $0300
        lda     $0302
        jmp     LF828
L2988:
        dex
        beq     L2930
		
INLINX:
        jsr     OUTQUES
        jsr     OUTSP
        ldx     #$80
        jmp     INLIN1
        .byte   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte   0,0,0,0,0,0,0,0,0,0


USR_FUNC:
        jsr     L29DA
        lda     FAC+3
        sta     FAC+5
        jmp     (FAC+4)
L29DA:
        jmp     (GOAYINT)

        brk
        brk
        brk
L29E0:
        pla
        jmp     LFB40

        .byte   0,0,0,0,0,0,0,0,0,0,0,0
L29F0:
        pha
        ldx     #$01
        inc     $B9
        bne     L29F9
        inc     $BA
L29F9:
        jmp     L291E
        .byte   $00,$00,$00,$00,$41,$53,$21,$D2
		.byte   $02,$FA,$00 
        lda     $12
        beq     L2A0E
        jmp     (GOGIVEAYF)
L2A0E:
        jsr     LF689
        .byte   $15,$BC,$08,$10,$52,$45,$75,$10
        .byte   $CD,$00,$55,$15,$9E,$08,$10,$4C
        .byte   $45,$75,$10,$D4,$00,$55,$15,$0E
        .byte   $08,$10,$89,$10,$75,$15,$1C,$08
        .byte   $10,$1F,$10,$75,$00 
        jmp     (GOGIVEAYF)
; ----------------------------------------------------------------------------
        .byte   0,0,0,0,0,0


.endif

