;file workingTest.as

	.entry LABELD
LABELA:	.string "working"
	cmp #5,LABELC
	lea LABELC,r2
	mov r1,LABELB
LABELE: add r2,LABELD
;test;;;;
LABELD:	.data 5,-1,3
	dec LABELB
	inc r1
	add r1,r0
LABELB: prn #1
	.extern LABELC
	sub r0,r2
	bne LABELB
	cmp LABELB,#1
	bne %LABELB
	jmp %LABELA
	.entry LABELE
	stop
