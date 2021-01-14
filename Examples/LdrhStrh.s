.global main
.text
main:
    ldr r0, =limite
    ldrh r1, [r0]
    ldrh r2, [r0,#2]
	mov r8,r0
    mov r2,#0
	ldrh r2, [r8,#2]!
    ldrh r3, [r0],#4
	mov r4,#4
    ldr r0, =limite
	ldrh r2, [r0,r4]
    mov r8,r0
    mov r2,#0
    ldrh r2, [r8,r4]!
    mov r3,#0
    ldrh r3, [r0],r4

    mov r2,#255
    add r2,r2,#87
    strh r1, [r0]
    strh r2, [r0,#4]
	strh r2, [r8,#4]!
    strh r3, [r0],#2
	mov r4,#4
	strh r2, [r0,r4]
    strh r2, [r8,r4]!
    strh r3, [r0],r4


    swi 0x123456
.data
limite:
    .word 0x12015678
	.word 0x11223344
