/*
 * Author: Alper Kaan İlik
 *
 * Board Support Package
 * bsp.c for lab7_p2.c
 */


#include "bsp.h"

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

/*Set output functions*/
void BSP_set_output_A(int k){
	GPIOA->MODER &= ~(3U << 2*k);
	GPIOA->MODER |= (1U << 2*k);
}
void BSP_set_output_B(int k){
	GPIOB->MODER &= ~(3U << 2*k);
	GPIOB->MODER |= (1U << 2*k);
}
void BSP_set_output_C(int k){
	GPIOC->MODER &= ~(3U << 2*k);
	GPIOC->MODER |= (1U << 2*k);
}
/*Set input functions*/
void BSP_set_input_A(int k){
	GPIOA->MODER &= ~(3U << 2*k);
	GPIOA->PUPDR |= (2U << 2*k); //Pull-down mode
}
void BSP_set_input_B(int k){
	GPIOB->MODER &= ~(3U << 2*k);
	GPIOB->PUPDR |= (2U << 2*k); //Pull-down mode
}

/*UART functions*/
//USART2 Receive handler
void USART2_IRQHandler(void){
	uint8_t data = USART2->RDR;
	//RXNE is automatically cleared when read
}
//USART2 print functions
void printChar(uint8_t c){
	USART2->TDR = c;
	while(!(USART2->ISR & (1 << 6)));
}

void _print(int fd, char *buf, int len){
	(void)fd;
	for(int i=0; i<len; i++){
		printChar(buf[i]);
	}
}

void print(char *buf){
	int len = 0;
	while (buf[len++] != '\0');
	_print(0, buf, len);
}

/*
 * PA2 PA3 are connected to the Virtual COM port
 * USART2 module
 */
void BSP_UART_init(uint32_t baud){
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 17);

	//Setup PA2 as AF1 (Alternate function 1)
	GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2); // set MODER '10'
	//Choose AF1 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1 << 4*2);


	//Setup PA3 as AF1
	GPIOA->MODER &= ~(3U << 2*3);
	GPIOA->MODER |= (2U << 2*3);
	//Choose AF1 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1 << 4*3);

	//Setup USART2
	//Reset USART2 cr1
	USART2->CR1 = 0;
	USART2->CR1 |= (1 << 3); //TE
	USART2->CR1 |= (1 << 2); //RE
	USART2->CR1 |= (1 << 5); //RXNEIE

	USART2->BRR =  (uint16_t)(SystemCoreClock / baud);

	USART2->CR1 |= (1 << 0); //UE

	NVIC_SetPriority(USART2_IRQn , 1);
	NVIC_EnableIRQ(USART2_IRQn);
}

/*Interrupts for keypad inputs*/
void BSP_set_keypad_interrupts(void){
	EXTI->EXTICR[2] |= (0U << 8*1); //PA9
	EXTI->EXTICR[0] |= (1U << 8*0);	//PB0
	EXTI->EXTICR[0] |= (1U << 8*2);	//PB2
	EXTI->EXTICR[2] |= (1U << 8*0); //PB8
	/*Rising edge triggered*/
	EXTI->RTSR1 |= (1U << 9);	//9th pin
	EXTI->RTSR1 |= (1U << 0);	//0th pin
	EXTI->RTSR1 |= (1U << 2);	//2nd pin
	EXTI->RTSR1 |= (1U << 8);	//8th pin
	/*Mask*/
	EXTI->IMR1 |= (1U << 9);	//9th pin
	EXTI->IMR1 |= (1U << 0);	//0th pin
	EXTI->IMR1 |= (1U << 2);	//2nd pin
	EXTI->IMR1 |= (1U << 8);	//8th pin
	/*NVIC*/
	NVIC_SetPriority(EXTI0_1_IRQn, 3);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn, 3);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 3);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}
/*Set keypad row outputs low function*/
void BSP_clear_keypad_rows(void){
	GPIOA->ODR &= ~(1U << 8);
	GPIOB->ODR &= ~(1U << 9);
	GPIOB->ODR &= ~(1U << 5);
	GPIOB->ODR &= ~(1U << 4);
}
/*Set keypad row outputs high function*/
void BSP_set_keypad_rows(void){
	GPIOA->ODR |= (1U << 8);
	GPIOB->ODR |= (1U << 9);
	GPIOB->ODR |= (1U << 5);
	GPIOB->ODR |= (1U << 4);
}

/*Timer1 Setup Lab7 icin interrupt silindi*/
void init_timer1(void){
	/*Enable TIM1 clock*/
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; //Configuration set 0
	TIM1->CR1 |= (1 << 7); //ARPE
	TIM1->CNT = 0; //Counter set 0

	TIM1->PSC = 999;
	TIM1->ARR = 2;//0.125milisecond interrupt PSC*ARR


	TIM1->DIER |= (1 << 0); //DIER set 0
	TIM1->CR1 |= (1 << 0); //TIM1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 5); //Timer interrupt priority set
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //Timer interrupt enabled
}

/*SSD Functions*/

void display1(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 1);	//PB1
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 1);
}

void display2(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 3);	//PB3
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 3);
}

void display3(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 6);	//PB6
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 6);
}

void display4(uint32_t k){
	cleardisplay();
	GPIOB->ODR |= (1U << 7);	//PB7
	setdisplay(k);
	GPIOB->ODR &= ~(1U << 7);
}

void cleardisplay(void){
	GPIOA->ODR |= (1U << 0);//PA0	A
	GPIOA->ODR |= (1U << 1);//PA1	B
	GPIOA->ODR |= (1U << 4);//PA4	C
	GPIOA->ODR |= (1U << 5);//PA5	D
	GPIOA->ODR |= (1U << 6);//PA6	E
	GPIOA->ODR |= (1U << 7);//PA7	F
	GPIOA->ODR |= (1U << 11);//PA11	G
	GPIOA->ODR |= (1U << 12);//PA12	.
}

void setdisplay(uint32_t l){
	switch(l)
		{
		case 18://d display
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 17://n display
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 16://I display
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 15://O display
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 14://L display
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 13://F display
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;
		case 12://u and v display
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			break;
		case 11://negative sign display
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;
		case 10://blank display
			//no instruction needed because cleared already
			break;
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
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 3:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 4:
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 5:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			break;

		case 6:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 11);//PA11	G
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
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 9:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 7);//PA7	F
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		}
}

/*IWDG set function*/
void BSP_set_IWDG(void){
	IWDG->KR = (0x0000CCCC);//Enable IWDG
	/*Configuring IWDG prescaler*/
	IWDG->KR = (0x00005555);
	IWDG->PR = (0x7);
	/*Reload register*/
	IWDG->RLR = (1250);//Set for 10 seconds.
	/*feeding the dog*/
	IWDG->KR = (0x0000AAAA);
}

/*ADC init*/
void init_ADC(void){//PB2 as ADC

	RCC->IOPENR |= (1U << 1);
	GPIOB->MODER &= ~(3U << 2*2); 	//Set B2 analog

	RCC->APBENR2 |= (1U << 20);		 //ADC enable

	ADC1->CR = 0; 					//reset ADC control register
	ADC1->CR |= (1U << 28); 		//enable voltage regulator
	delay(500);						//delay

	ADC1->CR |= (1U << 31);			//enable calibration register
	while(ADC1->CR & (1U << 31));	//wait until calibration is done

	ADC1->IER |= (1U << 3);			//end of conversion sequence interupt enable
	ADC1->CFGR1 |= (2U << 3);		//resolution set 8 bit
	ADC1->CFGR1 |= (0U << 13);		//single mode
	/*-------------------------------------------------------------------------*/
	ADC1->CFGR1 &= ~(7U << 6);		//clear bits to set EXTSEL: TRG0
	ADC1->CFGR1 |= (1U << 10);		//set EXTEN rising edge
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; 					//Configuration set 0
	TIM1->CR1 |= (1 << 7); 			//ARPE
	TIM1->CNT = 0; 					//Counter set 0

	TIM1->PSC = 999;
	TIM1->ARR = 2;					//0.125milisecond interrupt PSC*ARR


	TIM1->DIER |= (1 << 0); 		//DIER set 0
	TIM1->CR2 |= (2U << 20);		//set TIM1 MMS2 update mode.
	TIM1->CR1 |= (1 << 0); 			//TIM1 enable
	/*---------------------------------------------------------------------------*/

	ADC1->SMPR |= (0x000U << 0);	//1.5 clock cycle sample time
	ADC1->CFGR1 |= (10U << 26);		// bits26:30 set to 10 for channel 10
	ADC1->CHSELR |= (1U << 10);		//channel 10 for conversion (PB2)
	ADC1->ISR &= ~(1U << 0);		//clear ADC ready flag
	ADC1->CR |= (1U << 0);			//enable ADC
	while(!(ADC1->ISR & (1U << 0)));//wait until ADC ready flag set 1

	NVIC_SetPriority(ADC1_IRQn, 2);
	NVIC_EnableIRQ(ADC1_IRQn);

	ADC1->CR |= (1U << 2); 			//start conversion


}


