
	processor 6502

	include "apple1.inc"
  
  org $2000


loop
  lda HOSTRNG
  bmi slash
  lda #47
  jmp print
slash
  lda #92
print
  jsr ECHO
  jmp loop
