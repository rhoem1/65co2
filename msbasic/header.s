		.segment "HEADER"
.ifdef KBD
        jmp     LE68C
        .byte   $00,$13,$56
.endif
.ifdef APPLE
        jmp COLD_START
WARM_START:
		jmp RESTART
.endif
