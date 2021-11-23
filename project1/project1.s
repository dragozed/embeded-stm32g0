/*
 *
 * project1.s
 *
 * Author: Alper Kaan İlik
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
.equ GPIOC_IDR,			(GPIOC_BASE + (0x10)) // GPIOC IDR register offset

.equ GPIOB_BASE,			(0x50000400)		//GPIOB base address
.equ GPIOB_MODER,		(GPIOB_BASE + (0x00))//GPIOB MODER register offset
.equ GPIOB_ODR,			(GPIOB_BASE + (0x14))//GPIOB ODR register offset
.equ GPIOB_IDR,			(GPIOB_BASE + (0x10)) // GPIOB IDR register offset

.equ GPIOA_BASE,		(0x50000000)		  // GPIOA base address
.equ GPIOA_MODER,		(GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,         (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOA_IDR,			(GPIOA_BASE + (0x10)) // GPIOA IDR register offset

.equ RAM,				(0x20000000)		//RAM base address
.equ DISP1_RAM,			(RAM + (0xc))		//DISPLAY 1 RAM offset
.equ DISP2_RAM,			(RAM + (0x8))		//DISPLAY 2 RAM offset
.equ DISP3_RAM,			(RAM + (0x4))		//DISPLAY 3 RAM offset
.equ DISP4_RAM,			(RAM + (0x0))		//DISPLAY 4 RAM offset

/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	movs r0,#0
	movs r0,#0
	movs r1,r4
	lsrs r0, r0,#32
	lsls r7,r2,#1
	lsrs r0,r0,#32
	lsls r7,r2,#1
	lsrs r0,r0,#32
	movs r0,r0
	movs r0,#0
	movs r0,r0
	movs r0,#0
	movs r0,r0
	movs r0,#0
	movs r0,r0
	movs r0,#0 //baska bir olay icin degistirildi diger dosyalardan geri kopyala


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
	/* enable GPIOA and GPIOB clock, bit2 on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, 0x3  //for A and B '11'
	orrs r5, r5, r4
	str r5, [r6]

	/* setup PB0 to PB7 '01'for output, PB8 '00' for input for bits 0to15 in MODER */
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]
	ldr r4, =0xfffff //cleaning the related bits
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x05555 //seting the related bits
	orrs r5, r5, r4
	str r5, [r6]

	//setup PA0-PA1-PA4-PA5 '01'
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	ldr r4, =0xf0f //cleaning related bits
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x505 //setting the related bits
	orrs r5, r5, r4
	str r5, [r6]

	movs r0, #0x0
	ldr r1, =0x0 //needed for end 1second delay
	ldr r3, =0x460 //needed for delay_disp function, can not change
//--------------START OF IDLE ------------------------
Idle://sets idle numbers (ID:4037)
	/* turn on display 1(PA0)to make an 4 in ODR */
	ldr r6, =DISP1_RAM
	//b set_to_4
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x66 // number 4
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	/* turn on display 2(PA1) to make an 0 in ODR */
	ldr r6, =DISP2_RAM
	//b set_to_0
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x3f // number 0
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	/* turn on display 3(PA4) to make an 3	 in ODR */
	ldr r6, =DISP3_RAM
	//b set_to_3
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x4f // number 3
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	/* turn on display 4(PA5) to make an 7	 in ODR */
	ldr r6, =DISP4_RAM
	//b set_to_7
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x7 // number 7
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	movs r0, #0x0
	movs r7, #0x12
	b flash_displays
//--------------END OF IDLE------------------------

delay:
	movs r1, #0x0
	ldr r5, =#0xffff
	ldr r7, =#0x00000000
	ldr r7, =#0xfff
	ldr r6, =DISP2_RAM //set display2 to 0
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x3f // number 0
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	ldr r6, =DISP3_RAM //set display3 to 0
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x3f // number 0
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	ldr r6, =DISP4_RAM //set display4 to 0
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x3f // number 0
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	b flash_displays

delay2:
	adds r1, r1, #0x1
	cmp r1, r5
	blt flash_displays
	b Idle

button_control://checks if button pressed
	ldr r6, =GPIOB_IDR //PB8 button control
	ldr r5, [r6]
	lsrs r5, r5, #8
	movs r6, #0x1
	ands r5, r5, r6
	cmp r5, #0x1
	beq m1
	bx lr

m1://if button is pressed
	cmp r0, #0x0
	beq move1
	bne m0

m0://set r0=0
	movs r0, #0x0
	bx lr

move1://set r0=1
	movs r0, #0x1
	ldr r4, =#0xff //this is about delays
	bx lr

flash_displays:
	bl display4
	bl delay_disp
	bl display3
	bl delay_disp
	bl display2
	bl delay_disp
	bl display1
	bl delay_disp

	ldr r5, =#0xfff //check if need to go in last delay
	cmp r7, r5
	beq delay2
	bl button_control

	ldr r7, =0x000000
	cmp r0, #0x1 //if r0=1 then button switched to countdown mode
	beq delay_flash
	bne flash_displays

delay_flash://delay before countdown
	adds r7, r7, #0x1
	cmp r7, r4
	beq countdown
	bne delay_flash

extract_random_number://not used because could not implement RNG
	ldr r5, =0x20000200
	ldr r5, [r5]
	lsrs r4, r5, #9
	ldr r6, =DISP1_RAM //ram that holds display1
	cmp r4, #0

	b countdown


display1://flash display1 (Used in flash_displays)
	movs r5, 0x00000000 //reset contents of displaynumber
	movs r4, 0x1
	orrs r5, r5, r4
	str r5, [r2] //stored number of display (1st, 2nd...)
	ldr r6, =DISP1_RAM //ram
	ldr r5, [r6]
	ldr r6, =GPIOB_ODR
	str r5, [r6]

	movs r6, #0x0
	bx lr

display2://flash display2 (Used in flash_displays)
	movs r5, 0x00000000 //reset contents of displaynumber
	movs r4, 0x2
	orrs r5, r5, r4
	str r5, [r2] //stored number of display (1st, 2nd...)
	ldr r6, =DISP2_RAM //ram
	ldr r5, [r6]
	ldr r6, =GPIOB_ODR
	str r5, [r6]

	movs r6, #0x0
	bx lr

display3://flash display3 (Used in flash_displays)
	movs r5, 0x00000000 //reset contents of displaynumber
	movs r4, 0x10
	orrs r5, r5, r4
	str r5, [r2] //stored number of display (1st, 2nd...)
	ldr r6, =DISP3_RAM //ram
	ldr r5, [r6]
	ldr r6, =GPIOB_ODR
	str r5, [r6]

	movs r6, #0x0
	bx lr

display4://flash display4 (Used in flash_displays)
	ldr r2, =GPIOA_ODR
	movs r5, 0x00000000 //reset contents of displaynumber
	movs r4, 0x20
	orrs r5, r5, r4
	str r5, [r2] //stored number of display (1st, 2nd...)
	ldr r6, =DISP4_RAM //ram
	ldr r5, [r6]
	ldr r6, =GPIOB_ODR
	str r5, [r6]

	movs r6, #0x0
	bx lr

//--------------START OF COUNTDOWN-----------------

countdown://counts last digit down(display4)
	ldr r6, =DISP4_RAM
	ldr r5, [r6]
	ldr r4, =0xffffff90
	cmp r5, r4 //number 9
	beq set_to_8
	ldr r4, =0xffffff80
	cmp r5, r4 //number 8
	beq set_to_7
	ldr r4, =0xfffffff8
	cmp r5, r4 //number 7
	beq set_to_6
	ldr r4, =0xffffff82
	cmp r5, r4 //number 6
	beq set_to_5
	ldr r4, =0xffffff92
	cmp r5, r4 //number 5
	beq set_to_4
	ldr r4, =0xffffff99
	cmp r5, r4 //number 4
	beq set_to_3
	ldr r4, =0xffffffb0
	cmp r5, r4 //number 3
	beq set_to_2
	ldr r4, =0xffffffa4
	cmp r5, r4 //number 2
	beq set_to_1
	ldr r4, =0xfffffff9
	cmp r5, r4 //number 1
	beq set_to_0

	b lower_display3

//--------------END OF COUNTDOWN-----------------


//-----------START OF LOWER DISPLAY3------------


lower_display3://lowers display3 if countdown hits0
	ldr r6, =DISP4_RAM //set display4 to 9
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6f // number 9
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	ldr r6, =DISP3_RAM //ram that holds display3
	ldr r5, [r6]
	ldr r4, =0xffffff90
	cmp r5, r4 //number 9
	beq set_to_8
	ldr r4, =0xffffff80
	cmp r5, r4 //number 8
	beq set_to_7
	ldr r4, =0xfffffff8
	cmp r5, r4 //number 7
	beq set_to_6
	ldr r4, =0xffffff82
	cmp r5, r4 //number 6
	beq set_to_5
	ldr r4, =0xffffff92
	cmp r5, r4 //number 5
	beq set_to_4
	ldr r4, =0xffffff99
	cmp r5, r4 //number 4
	beq set_to_3
	ldr r4, =0xffffffb0
	cmp r5, r4 //number 3
	beq set_to_2
	ldr r4, =0xffffffa4
	cmp r5, r4 //number 2
	beq set_to_1
	ldr r4, =0xfffffff9
	cmp r5, r4 //number 1
	beq set_to_0

	b lower_display2

//-----------END OF LOWER DISPLAY3-------------

//SET FUNCTIONS ARE USED TO SET GIVEN DISPLAY
//-----------START OF SET FUNCTIONS-------------
set_to_3:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x4f // number 3
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_4:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x66 // number 4
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_5:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6d // number 5
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_6:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x7d // number 6
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_7:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x7 // number 7
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_8:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x7f // number 8
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_9:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6f // number 9
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_0:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x3f // number 0
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_1:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6 // number 1
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays

set_to_2:
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x5b // number 2
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)
	b flash_displays
//-----------END OF SET FUNCTIONS-------------

//-----------START OF LOWER DISPLAY2----------
lower_display2://lowers display2 if display3 hits 0
	ldr r6, =DISP3_RAM //set display3 to 9
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6f // number 9
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	ldr r6, =DISP2_RAM //ram that holds display2
	ldr r5, [r6]
	ldr r4, =0xffffff90
	cmp r5, r4 //number 9
	beq set_to_8
	ldr r4, =0xffffff80
	cmp r5, r4 //number 8
	beq set_to_7
	ldr r4, =0xfffffff8
	cmp r5, r4 //number 7
	beq set_to_6
	ldr r4, =0xffffff82
	cmp r5, r4 //number 6
	beq set_to_5
	ldr r4, =0xffffff92
	cmp r5, r4 //number 5
	beq set_to_4
	ldr r4, =0xffffff99
	cmp r5, r4 //number 4
	beq set_to_3
	ldr r4, =0xffffffb0
	cmp r5, r4 //number 3
	beq set_to_2
	ldr r4, =0xffffffa4
	cmp r5, r4 //number 2
	beq set_to_1
	ldr r4, =0xfffffff9
	cmp r5, r4 //number 1
	beq set_to_0

	b lower_display1

//-----------END OF LOWER DISPLAY2-------------

//-----------START OF LOWER DISPLAY1----------


lower_display1://lowers display1 if display2 hits 0
	ldr r6, =DISP2_RAM //set display2 to 9
	movs r5, 0x00000000 //reset contents of display
	movs r4, 0x6f // number 9
	mvns r4, r4  //Not because they need to be zero
	orrs r5, r5, r4
	str r5, [r6] //stored type of display  (1,2,3,4,5,6,7,8,9)

	ldr r6, =DISP1_RAM //ram that holds display1
	ldr r5, [r6]
	ldr r4, =0xffffff90
	cmp r5, r4 //number 9
	beq set_to_8
	ldr r4, =0xffffff80
	cmp r5, r4 //number 8
	beq set_to_7
	ldr r4, =0xfffffff8
	cmp r5, r4 //number 7
	beq set_to_6
	ldr r4, =0xffffff82
	cmp r5, r4 //number 6
	beq set_to_5
	ldr r4, =0xffffff92
	cmp r5, r4 //number 5
	beq set_to_4
	ldr r4, =0xffffff99
	cmp r5, r4 //number 4
	beq set_to_3
	ldr r4, =0xffffffb0
	cmp r5, r4 //number 3
	beq set_to_2
	ldr r4, =0xffffffa4
	cmp r5, r4 //number 2
	beq set_to_1
	ldr r4, =0xfffffff9
	cmp r5, r4 //number 1
	beq set_to_0

	b delay
//-----------END OF LOWER DISPLAY1-------------


delay_disp: //delay function for display
	adds r6, r6, #0x1
	cmp r6,r3
	blt delay_disp
	bx lr

/*
//------------START OF RANDOM NUMBER GEN.-----------
random_number:
	ldr r6, =0xFA8B7CE1 //random number generator seed

	ldr r5, =0x20000200
	ldr r5, [r5]
	adds r6, r5

	lsrs r5, r6, #1
	eors r5, r6
	lsrs r4, r6, #2
	eors r5, r4
	lsrs r4, r6, #22
	eors r5, r4
	lsrs r6, r6, #1
	lsls r5, r5, #31
	orrs r6, r5 // r6 is 32bit random number
	lsrs r6, r6, #20 //r6 is 12bit random number
	ldr r5, =0x20000200 //store random number in ram
	str r6, [r5]
//------------END OF RANDOM NUMBER GEN.-----------
*/
/*extract_random_number:
	movs r1, #0xfc //random number return from set_to_ funcs
	ldr r5, =0x20000200
	ldr r5, [r5]
	lsrs r4, r5, #9

	ldr r6, =DISP1_RAM //ram that holds display1
	cmp r4, #0
	beq set_to_0
	cmp r4, #1
	beq set_to_1
	cmp r4, #2
	beq set_to_2
	cmp r4, #3
	beq set_to_3
	cmp r4, #4
	beq set_to_4
	cmp r4, #5
	beq set_to_5
	cmp r4, #6
	beq set_to_6
	cmp r4, #7
	beq set_to_7
	cmp r4, #8
	beq set_to_8
	cmp r4, #9
	beq set_to_9
	b countdown

	ldr r6, =DISP2_RAM //ram that holds display2
	cmp r4, #0
	beq set_to_0
	cmp r4, #1
	beq set_to_1
	cmp r4, #2
	beq set_to_2
	cmp r4, #3
	beq set_to_3
	cmp r4, #4
	beq set_to_4
	cmp r4, #5
	beq set_to_5
	cmp r4, #6
	beq set_to_6
	cmp r4, #7
	beq set_to_7
	cmp r4, #8
	beq set_to_8
	cmp r4, #9
	beq set_to_9
	b countdown

	ldr r6, =DISP3_RAM //ram that holds display3
	cmp r4, #0
	beq set_to_0
	cmp r4, #1
	beq set_to_1
	cmp r4, #2
	beq set_to_2
	cmp r4, #3
	beq set_to_3
	cmp r4, #4
	beq set_to_4
	cmp r4, #5
	beq set_to_5
	cmp r4, #6
	beq set_to_6
	cmp r4, #7
	beq set_to_7
	cmp r4, #8
	beq set_to_8
	cmp r4, #9
	beq set_to_9
	b countdown

	ldr r6, =DISP4_RAM //ram that holds display4
	cmp r4, #0
	beq set_to_0
	cmp r4, #1
	beq set_to_1
	cmp r4, #2
	beq set_to_2
	cmp r4, #3
	beq set_to_3
	cmp r4, #4
	beq set_to_4
	cmp r4, #5
	beq set_to_5
	cmp r4, #6
	beq set_to_6
	cmp r4, #7
	beq set_to_7
	cmp r4, #8
	beq set_to_8
	cmp r4, #9
	beq set_to_9
	b countdown*/

	/* for(;;); */
	b .

	/* this should never get executed */
	nop

