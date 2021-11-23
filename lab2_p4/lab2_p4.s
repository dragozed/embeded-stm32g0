/*
 * lab2_p4.s
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
.equ GPIOB_IDR,			(GPIOB_BASE + (0x10)) // GPIOB IDR register offset

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
	/* enable GPIOB clock, bit2 on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, 0x2
	orrs r5, r5, r4
	str r5, [r6]

	/* setup PB0 to PB7 for led 01 for bits 0 to 15 in MODER */
	//and setup PB8 for button input 00 in 16-17 bits
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0xfffff
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x05555
	orrs r5, r5, r4
	str r5, [r6]

	movs r2, 0x1 //register that controls switch operation
	movs r4, 0xe //starting position of the leds
	ldr r3, =0x161A80 //needed for delay_100ms function
button_ctrl:
	ldr r6, =GPIOB_IDR
	ldr r5, [r6]
	lsrs r5, r5, #8
	movs r7, #0x1
	ands r5, r5, r7
	cmp r5, #0x1
	beq switcher
	bne selecter


switcher: //switches left shift or right shift
	cmp r2, #0x1
	beq switch1
	bne switch2

switch1:
	movs r2, #0x0
	b selecter

switch2:
	movs r2, #0x1
	b selecter

selecter:
	movs r1, #0x0 //for counter in delay_100ms func.
	bl delay_100ms
	cmp r2, #0x1
	beq led_loop_left
	bne led_loop_right

led_loop_left: //loop that shifts the leds to the left
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	cmp r5, #0xe0
	beq led_loop_left1
	cmp r5, #0xc1
	beq led_loop_left2
	cmp r5, #0x83
	beq led_loop_left3
	lsls r4, r4, #1
	str r4, [r6]



	b button_ctrl

led_loop_left1: //loop that resets the leds to first three led
	ldr r6, =GPIOB_ODR
	ldr r4, =0xc1
	str r4, [r6]

	b button_ctrl

led_loop_left2:
	ldr r6, =GPIOB_ODR
	ldr r4, =0x83
	str r4, [r6]

	b button_ctrl

led_loop_left3:
	ldr r6, =GPIOB_ODR
	ldr r4, =0x7
	str r4, [r6]

	b button_ctrl

led_loop_right: //loop that shifts the leds to the right
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	cmp r5, #0x7
	beq led_loop_right1
	cmp r5, #0x83
	beq led_loop_right2
	cmp r5, #0xc1
	beq led_loop_right3
	lsrs r4, r4, #1
	str r4, [r6]

	b button_ctrl

led_loop_right1: //loop that resets the leds to first three led
	ldr r6, =GPIOB_ODR
	ldr r4, =0x83
	str r4, [r6]

	b button_ctrl

led_loop_right2:
	ldr r6, =GPIOB_ODR
	ldr r4, =0xc1
	str r4, [r6]

	b button_ctrl

led_loop_right3:
	ldr r6, =GPIOB_ODR
	ldr r4, =0xe0
	str r4, [r6]

	b button_ctrl

delay_100ms: //delay 100ms loop
	adds r1, r1, #0x1
	cmp r1,r3
	blt delay_100ms
	bx lr


	/* for(;;); */
	b .

	/* this should never get executed */
	nop

