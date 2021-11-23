/*
 * lab4_p3.c
 *
 * Author: Alper Kaan İlik
 *
 *
 */

#include "stm32g0xx.h"

#define LEDDELAY    1600000

int main(void);
void init_timer1();
void flash_displays();
void display1(uint32_t k);
void display2(uint32_t k);
void display3(uint32_t k);
void display4(uint32_t k);
void cleardisplay();
void setdisplay(uint32_t l);
void countup();

/*values for setting displays*/
uint32_t a = 0;
uint32_t b = 0;
uint32_t c = 0;
uint32_t d = 0;
/*variable that toggles countup on/off */
uint32_t e = 0;

int main(void) {

	/* Enable GPIOC clock */
	RCC->IOPENR |= (1U << 2);

	/*Enable GPIOB clock */
	RCC->IOPENR |= (1U << 1);

	/*Enable GPIOA clock */
	RCC->IOPENR |= (1U << 0);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Setup PB3 as input */
    GPIOB->MODER &= ~(3U << 2*3);
    GPIOB->PUPDR |= (2U << 2*3); //Pull-down mode

    /*Setup PA0 PA1 PA4 PA5 PA6 PA7 PA8 PA9 output*/
    GPIOA->MODER &= ~(3U << 2*0);
    GPIOA->MODER |= (1U << 2*0);
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->MODER |= (1U << 2*1);
    GPIOA->MODER &= ~(3U << 2*4);
    GPIOA->MODER |= (1U << 2*4);
    GPIOA->MODER &= ~(3U << 2*5);
    GPIOA->MODER |= (1U << 2*5);
    GPIOA->MODER &= ~(3U << 2*6);
    GPIOA->MODER |= (1U << 2*6);
    GPIOA->MODER &= ~(3U << 2*7);
    GPIOA->MODER |= (1U << 2*7);
    GPIOA->MODER &= ~(3U << 2*8);
    GPIOA->MODER |= (1U << 2*8);
    GPIOA->MODER &= ~(3U << 2*9);
    GPIOA->MODER |= (1U << 2*9);

    /*Setup PB0 PB2 PB4 PB5 output */
    GPIOB->MODER &= ~(3U << 2*0);
    GPIOB->MODER |= (1U << 2*0);
    GPIOB->MODER &= ~(3U << 2*2);
    GPIOB->MODER |= (1U << 2*2);
    GPIOB->MODER &= ~(3U << 2*4);
    GPIOB->MODER |= (1U << 2*4);
    GPIOB->MODER &= ~(3U << 2*5);
    GPIOB->MODER |= (1U << 2*5);

    init_timer1();

    /* Setup PB3 as interrupt */
    EXTI->EXTICR[0] |= (1U << 8*3);
    EXTI->RTSR1 |= (1U << 3); //rising edge
    EXTI->IMR1 |= (1U << 3);
    NVIC_SetPriority(EXTI2_3_IRQn, 5);
    NVIC_EnableIRQ(EXTI2_3_IRQn);



    while(1) {

    }

    return 0;
}


/*TIM1 Configuration*/
void init_timer1(){
	/*Enable TIM1 clock*/
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; //Configuration set 0
	TIM1->CR1 |= (1 << 7); //ARPE
	TIM1->CNT = 0; //Counter set 0

	TIM1->PSC = 999;
	TIM1->ARR = 16;//1milisecond interrupt PSC*ARR


	TIM1->DIER |= (1 << 0); //DIER set 0
	TIM1->CR1 |= (1 << 0); //TIM1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 10); //Timer interrupt priority set
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //Timer interrupt enabled
}

/*TIM1 Interrupt Handler*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	flash_displays();
	if(e == 1){
	countup();
	}
	TIM1->SR &= ~(1U << 0);//Clear update status register
}

/*Button Interrupt Handler*/
void EXTI2_3_IRQHandler(){
	/*set displays*/
	a = 0; b = 0; c = 0; d = 0; e = 1;
	/* Turn off LED */
	GPIOC->ODR &= ~(1U << 6);
	EXTI->RPR1 |= (1U << 3);//Pending register is cleared by setting 1
}

void countup(){
	if(d < 9){
		d = d+1;
	}
	else{
		d = 0;
		if(c < 9){
			c = c+1;
		}
		else{
			c = 0;
			if(b < 9){
				b = b+1;
			}
			else{
				b = 0;
				if(a < 9){
					a = a+1;
				}
				else{
					a = 9;
					b = 9;
					c = 9;
					d = 9;
					e = 0;
					/* Turn on LED */
					GPIOC->ODR |= (1U << 6);
				}
			}
		}
	}
}

/*Mother display funcion*/
void flash_displays(){
	display1(a);
	display2(b);
	display3(c);
	display4(d);
}
void display1(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 0);	//PB0
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 0);
}

void display2(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 2);	//PB2
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 2);
}

void display3(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 4);	//PB4
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 4);
}

void display4(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 5);	//PB5
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 5);
}

/*Clear display*/
void cleardisplay(){
	GPIOA->ODR |= (1U << 0);//PA0	A
	GPIOA->ODR |= (1U << 1);//PA1	B
	GPIOA->ODR |= (1U << 4);//PA4	C
	GPIOA->ODR |= (1U << 5);//PA5	D
	GPIOA->ODR |= (1U << 6);//PA6	E
	GPIOA->ODR |= (1U << 7);//PA7	F
	GPIOA->ODR |= (1U << 8);//PA8	G
	GPIOA->ODR |= (1U << 9);//PA9	.
}

void setdisplay(uint32_t l){
	switch(l)
		{
		case 0:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 1:
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			break;

		case 2:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			break;

		case 3:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			break;

		case 4:
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 5:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 6:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			break;

		case 7:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			break;

		case 8:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 8);//PA8	G
			break;

		case 9:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 8);//PA8	G
		}
}
