/*	
*   Author: Alper Kaan İlik
*	Hw3 problem3
*
*	I didnt get the necessary code to solve the problem
*	wanted to add how far i could come.
*	found most of the functions but, internal values are
*	problematic.
*/

.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

/* make linker see this */
.global Reset_Handler

/* vector table, +1 thumb mode */
.section .text
v:
movs	r0, #0
asrs	r0, r0, #32
movs	r1, r4
lsrs	r0, r0, #32
movs	r3, r5
lsrs	r0, r0, #32
movs	r3, r5
lsrs	r0, r0, #32
movs	r0, r0
asrs	r0, r0, #32
movs	r0, r0
asrs	r0, r0, #32
movs	r0, r0
asrs	r0, r0, #32
movs	r0, r0
asrs	r0, r0, #32



/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr	r0, =0x10002000
	mov sp, r0

	/* initialize data and bss
	 * not necessary for rom only code
	 * */

	/* call main */
	bl main
	/* trap if returned */
	b .

/* default handler */
.section .text
Default_Handler:
	 b Default_Handler


/* main function */
.section .text
main:
	 ldr	r1, =0x08000080
     ldr	r2, =0x10000200
     movs	r3, #0

rock:
	bl	paper
	str	r0, [r2, #0]
    adds	r1, #4
     adds	r2, #4
     adds	r3, #1
     cmp	r3, #4
     bne	rock
     b	eof

paper:
	push	{r1, r2, r3}
    ldr	r6, =0x14159265
    lsls	r7, r6, #3
    ldr	r1, [r1, #0]
    ldr	r4, =0x00000080

scissors:
	ldr	r2, =0x08000078
    ldr	r5, [r2, #0]
    lsls	r0, r1, #4
    adds	r0, r0, r5
    push	{r0}
    ldr	r5, [r2, #4]
    lsrs	r0, r1, #5
    adds	r0, r0, r5
    adds	r2, r1, r7
    eors	r0, r2
   pop	{r2}
    eors	r0, r2
    subs	r1, r1, r0
    subs	r7, r7, r6
     lsrs	r4, r4, #1
    bne	scissors
    movs	r0, r1
    pop	{r1, r2, r3}
    bx	lr
eof:
	b	eof
    nop

spock:
	ldrh	r4, [r3, r6]
    asrs	r2, r1, #14
    ldr	    r5, [sp, #916]
    strh	r1, [r6, #28]

lizard:

    lsrs	r5, r2, #17
	add	r2, pc, #228
	subs	r1, #252
	asrs	r7, r7, #4
    str r0, [r0, #0x14]
	ldr	r4, [r6, #64]
	bcc	0x07ffffb6
	movs	r0, #0
    asrs	r0, r0, #32
    lsls	r0, r0, #2
    lsrs	r0, r0, #32
    lsls	r0, r0, #8
    asrs	r0, r0, #32
    str	r2, [sp, #404]
    asrs	r5, r2, #16
    lsls	r0, r0, #2
    movs	r0, r0
    lsls	r0, r7, #1
    lsrs	r0, r0, #32
