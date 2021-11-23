/*
 * lab2_p3.s
 *
 * Author: Alper Kaan İlik
 *
 */


.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOC_BASE,       (0x50000800)          // GPIOC base address
.equ GPIOC_MODER,      (GPIOC_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOC_ODR,        (GPIOC_BASE + (0x14)) // GPIOC ODR register offset

.equ GPIOB_BASE,			(0x50000400)		//GPIOB base address
.equ GPIOB_MODER,		(GPIOB_BASE + (0x00))//GPIOB MODER register offset
.equ GPIOB_ODR,			(GPIOB_BASE + (0x14))//GPIOB ODR register offset

/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	 ldr	r1, [pc, #100]
     ldr	r2, [pc, #104]
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
    ldr	r6, [pc, #84]
    lsls	r7, r6, #3
    ldr	r1, [r1, #0]
    ldr	r4, [pc, #80]

scissors:
	ldr	r2, [pc, #84]
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


	/* for(;;); */
	b .

	/* this should never get executed */
	nop

