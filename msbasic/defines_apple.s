; configuration
CONFIG_2 := 1
CONFIG_APPLE := 1
;CONFIG_SMALL := 1


;APPLE_BAD_BYTE := 1
CONFIG_IO_MSB := 1 ; all I/O has bit #7 set
;CONFIG_PRINT_CR := 1 ; print CR when line end reached
CONFIG_SAFE_NAMENOTFOUND := 1
CONFIG_SCRTCH_ORDER := 3
CONFIG_NO_CR := 1

CONFIG_PEEK_SAVE_LINNUM := 1

BYTES_PER_ELEMENT := 6 ; XXX override

; zero page
ZP_START1 = $00
ZP_START2 = $4F
ZP_START3 = $0D
ZP_START4 = $55

;extra ZP variables
USR				:= $000A

; inputbuffer
INPUTBUFFER     := $0200

; constants
STACK_TOP		:= $F8
SPACE_FOR_GOSUB := $36
CRLF_1 := CR
CRLF_2 := $80
WIDTH			:= 0
WIDTH2			:= 0

; memory layout  $2600 - $CFFF  is for your programs and heap
RAMSTART2	:= $2600
APPLE1 := 1

.ifdef APPLE1
; monitor functions
;MONRDKEY        := $FD0C
MONCOUT         := $FFEF


KBin := $D010
KBcr := $D011
DSPout  := $D012
DSPcr := $D013

.else
MONRDKEY := $FD0C
MONCOUT  := $FDED
LF689			:= $F689
LF800			:= $F800
LF819			:= $F819
LF828			:= $F828
LF864			:= $F864
TEX				:= $FB2F
LFB40			:= $FB40
LFD0C			:= $FD0C
LFD6A			:= $FD6A
LFECD			:= $FECD
LFEFD			:= $FEFD

.endif
