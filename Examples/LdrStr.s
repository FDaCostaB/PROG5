.global main
.text
main:
    ldr r0, =limite
    ldrb r1, [r0]
    ldrb r2, [r0,#3]
	mov r8,r0
    mov r2,#0
	ldrb r2, [r8,#3]!
    ldrb r3, [r0],#3
	sub  r4,r1,#16
	ldrb r2, [r0,r4]
    mov r8,r0
    mov r2,#0
    ldrb r2, [r8,r4]!
    mov r3,#0
    ldrb r3, [r0],r4
	mov r5,#1
    ldr r0, =limite
    ldr r6, [r0, r5, LSL #2]
    ldr r6, [r0, r5, LSL #2]!

    strb r1, [r0]
    strb r2, [r0,#3]
	strb r2, [r8,#3]!
    strb r3, [r0],#3
	sub  r4,r1,#16
	strb r2, [r0,r4]
    strb r2, [r8,r4]!
    strb r3, [r0],r4
    ldr r0, =limite
    str r6, [r0, r5, LSL #2]
    str r6, [r0, r5, LSL #2]!

    swi 0x123456
.data
limite:
    .word 0x12015678
	.word 0x11223344
