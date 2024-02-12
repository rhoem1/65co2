; configuration
CONFIG_2A := 1

CONFIG_IO_MSB := 0 ; all I/O has bit #7 set
CONFIG_PRINT_CR := 0 ; print CR when line end reached
CONFIG_SCRTCH_ORDER := 1

; zero page
ZP_START1 = $00
ZP_START2 = $7A
ZP_START3 = $0A
ZP_START4 = $16

;extra ZP variables
USR				:= $000A

; constants
SPACE_FOR_GOSUB := $3E
STACK_TOP		:= $FA
WIDTH			:= 40
WIDTH2			:= 30

RAMSTART2		:= $0400

; bios locations
; monitor functions
LOAD	:= LOADBAS
SAVE	:= SAVEBAS
MONRDKEY        := CHRIN
MONCOUT         := CHROUT
