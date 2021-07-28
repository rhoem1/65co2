
	
HOSTRNG = $CFFF
; 
CHARROM = $D400
SCREEN = $D800
SCREEN_COLOR = $DC00

; apple 1 i/o
KBD = $D010
KBDCR = $D011
DSP = $D012
DSPCR = $D013

; screen control
SCREEN_CONTROL = $D000
IDVID = #$80 ; interrupt on vidblank
CHRAM = #$20 ; character rom active
INVRT = #$10 ; invert output
CACTV = #$08 ; Cursor Active
BLINKMASK = #$07 ; Blinking rate

; should be non-zero during last "row" of characters (25, off screen)
VIDBLANK = $D001 

; should be non-zero during the "overscan" area of each row, the last "4" characters
ROWBLANK = $D002 

; line currently scanning
SCANROW = $D003 
; write: set Cursor Y

; character column currently scanned
SCANCOL = $D004 
; write: set Cursor X

CURSORCOLOR = $D005
CURSORCHARACTER = $D006

; write the row you want
; two reads will return the low/high bytes for the offset
SCREENROWOFFSET = $D00C

CYCLECOUNTER = $D00D
; through 0xD00F

QUIT_EMULATION = $D01F


PHASOR_CHIP_0 = $D020 ; 0010
PHASOR_CHIP_1 = $D040 ; 0100
PHASOR_CHIP_B = $D060 ; 0110
PHASOR_CONTROL = $D080 ; 1000
PHASOR_CONTROL_LENGTH = $10

;ADDRESSES FOR FIRST 6522
PHASOR_ORB = $D020 ;PORT B
PHASOR_ORA = $D021 ;PORT A
PHASOR_DDRB = $D022 ;DATA DIRECTION REGISTER (A)
PHASOR_DDRA = $D023 ;DATA DIRECTION REGISTER (B)
;ADDRESSES FOR SECOND 6522
PHASOR_ORB2 = $D040 ;PORT B
PHASOR_ORA2 = $D041 ;PORT A
PHASOR_DDRB2 = $D042 ;DATA DIRECTION REGISTER (B)
PHASOR_DDRA2 = $D043 ;DATA DIRECTION REGISTER (A)



CHARACTER_ROM = $D400
CHARACTER_ROM_LENGTH = $400

SCREEN_ADDRESS = $D800
SCREEN_LENGTH = $400

SCREEN_COLOR_ADDRESS = $DC00
SCREEN_COLOR_LENGTH = $400

; color pallet lives in the extra bytes in the screen color page
; the low 8 colors in the 78-7F range and the upper 8 in the F8-FF range
; red on page DC, green on DD and blue on DE
COLOR_PALLET_RED_LOW = $DC78
COLOR_PALLET_RED_HIGH = $DCF8
COLOR_PALLET_GREEN_LOW = $DD78
COLOR_PALLET_GREEN_HIGH = $DDF8
COLOR_PALLET_BLUE_LOW = $DE78
COLOR_PALLET_BLUE_HIGH = $DEF8
COLOR_PALLET_LENGTH = $08





; 0 page locations used by things, like wozmon
XAML  =     $24  ;Last "opened" location Low
XAMH  =     $25  ;Last "opened" location High
STL   =     $26  ;Store address Low
STH   =     $27  ;Store address High
L     =     $28  ;Hex value parsing Low
H     =     $29  ;Hex value parsing High
MODE  =     $2B  ;$00=XAM, $7F=STOR, $AE=BLOCK XAM

PRBYTE = $FFDC
ECHO = $FFEF

WOZMON = $FF00
INTBASIC = $E000
KRUSADER = $F000

NMI_IRQ = $FFFE
IRQ = $FFFA