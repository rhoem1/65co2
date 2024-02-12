.setcpu "65C02"
.debuginfo

KBD             =     $D010          ;PIA.A keyboard input
KBDCR           =     $D011          ;PIA.A keyboard control register
DSP             =     $D012          ;PIA.B display output register
DSPCR           =     $D013          ;PIA.B display control register

LOADBAS:
  rts

SAVEBAS:
  rts

CHRIN:
  lda     KBDCR          ;Wait for key press
  beq     NOCHRIN       ;No key yet!
  lda     KBD            ;Load character. B7 should be '1'
  jsr     CHROUT
  sec
  rts
NOCHRIN:
  clc
  rts

CHROUT:
  bit DSP
  bmi CHROUT
  sta DSP
  rts
