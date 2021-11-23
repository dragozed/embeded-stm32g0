/*
 * Author: Alper Kaan İlik
 *
 * Board Support Package
 * bsp.c
 */


#include "bsp.h"



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

	//Setup PA2 as AF (Alternate function)
	GPIOA->MODER &= ~(3U << 2*2);
	GPIOA->MODER |= (2U << 2*2); // set MODER '10'
	//Choose AF1 from mux
	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1 << 4*2);


	//Setup PA3 as AF
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

/*PWM Functions*/
void PWM_deneme(void){//initialize for PA8 TIM1_CH1
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR2 |= (1U << 11); //Enable TIM1 clock

	TIM1->CR1 = 0;			//configuration set 0
	TIM1->CCMR1 = 0;		//capture-compare set 0

	TIM1->CR1 |= (1U << 4);	//downcounter

	TIM1->CNT = 0;
	TIM1->PSC = 1;
	TIM1->ARR = 200;		//160khz

	TIM1->CCMR1 &= ~(0x7U << 4);
	TIM1->CCMR1 |= (0x6U << 4);	//OC1M PWM mode 1

	TIM1->CR1 |= (1U << 7);			//autoreload enable
	TIM1->EGR |= (1U << 0);			//update generation

	TIM1->CCMR1 |= (1U << 3);		//output compare 1 preload enable
	TIM1->CCER |= (1U << 0);		//capture compare 1 output enable

	GPIOA->MODER &= ~(3U << 2*8);	//clear bits
	GPIOA->MODER |= (2U << 2*8);	//set alternate function mode
	GPIOA->OSPEEDR |= (3U << 2*8);	//very high speed

	GPIOA->AFR[1] &= ~(0xFU << 4*0);	//AFSEL8
	GPIOA->AFR[1] |= (2U << 4*0);		//Alternate function 2

	TIM1->DIER |= (1U << 0);			//update interrupt enable
	TIM1->CR1 |= (1U << 0);				//TIM1 counter enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2); 	//Timer interrupt priority set
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); 		//Timer interrupt enabled
}

void PWM_deneme2(void){//initialize for PB3 TIM2_CH2
	RCC->IOPENR |= (1U << 1);
	RCC->APBENR1 |= (1U << 0); //Enable TIM2 clock

	TIM2->CR1 = 0;			//configuration set 0
	TIM2->CCMR1 = 0;		//capture-compare set 0

	TIM2->CR1 |= (1U << 4);	//downcounter

	TIM2->CNT = 0;
	TIM2->PSC = 1;
	TIM2->ARR = 200;		//160khz

	TIM2->CCMR1 &= ~(0x7U << 12);
	TIM2->CCMR1 |= (0x6U << 12);	//OC2M PWM mode 1

	TIM2->CR1 |= (1U << 7);			//autoreload enable
	TIM2->EGR |= (1U << 0);			//update generation

	TIM2->CCMR1 |= (1U << 11);		//output compare 1 preload enable
	TIM2->CCER |= (1U << 4);		//capture compare 1 output enable

	GPIOB->MODER &= ~(3U << 2*3);	//clear bits
	GPIOB->MODER |= (2U << 2*3);	//set alternate function mode
	GPIOB->OSPEEDR |= (3U << 2*3);	//very high speed

	GPIOB->AFR[0] &= ~(0xFU << 4*3);	//AFSEL3
	GPIOB->AFR[0] |= (2U << 4*3);		//Alternate function 2

	TIM2->DIER |= (1U << 0);			//update interrupt enable
	TIM2->CR1 |= (1U << 0);				//TIM2 counter enable

	NVIC_SetPriority(TIM2_IRQn, 2); 	//Timer interrupt priority set
	NVIC_EnableIRQ(TIM2_IRQn); 		//Timer interrupt enabled
}
void PWM_deneme3(void){//initialize for PA7 TIM3_CH2
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 1); //Enable TIM3 clock

	TIM3->CR1 = 0;			//configuration set 0
	TIM3->CCMR1 = 0;		//capture-compare set 0

	TIM3->CR1 |= (1U << 4);	//downcounter

	TIM3->CNT = 0;
	TIM3->PSC = 1;
	TIM3->ARR = 200;		//160khz

	TIM3->CCMR1 &= ~(0x7U << 12);
	TIM3->CCMR1 |= (0x6U << 12);	//OC2M PWM mode 1

	TIM3->CR1 |= (1U << 7);			//autoreload enable
	TIM3->EGR |= (1U << 0);			//update generation

	TIM3->CCMR1 |= (1U << 11);		//output compare 1 preload enable
	TIM3->CCER |= (1U << 4);		//capture compare 1 output enable

	GPIOA->MODER &= ~(3U << 2*7);	//clear bits
	GPIOA->MODER |= (2U << 2*7);	//set alternate function mode
	GPIOA->OSPEEDR |= (3U << 2*7);	//very high speed

	GPIOA->AFR[0] &= ~(0xFU << 4*7);	//AFSEL7
	GPIOA->AFR[0] |= (1U << 4*7);		//Alternate function 1

	TIM3->DIER |= (1U << 0);			//update interrupt enable
	TIM3->CR1 |= (1U << 0);				//TIM2 counter enable

	NVIC_SetPriority(TIM3_IRQn, 2); 	//Timer interrupt priority set
	NVIC_EnableIRQ(TIM3_IRQn); 		//Timer interrupt enabled
}
void BSP_PWM_init(void){

	RCC->IOPENR |= (3U << 0);
	//initialize for PA8 TIM1_CH1
	//Set PA8 as alternate func
	GPIOA->MODER &= ~(3U << 2*8);
	GPIOA->MODER |= (2U << 2*8); // set MODER '10' alternate mode
	//Choose AF2 for PA8
	GPIOA->AFR[1] &= ~(0xFU << 4*0);
	GPIOA->AFR[1] |= (2 << 4*0);
	/*Enable TIM1 clock*/
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; //Configuration set 0
	TIM1->CNT = 0; //Counter set 0

	/*Set interrupt period*/
	TIM1->PSC = 1;
	TIM1->ARR = 200;//??milisecond interrupt period(160kHz)

	TIM1->CCMR1 = 0;
	TIM1->CCMR1 |= (6U << 4); //set PWM mode 1

	TIM1->CCER = 0;
	TIM1->CCER |= (1U << 0);
	/*Set duty cycle*/
	TIM1->CCR1 = 0;
	TIM1->CCR1 |= (190);//3000

	TIM1->DIER |= (1 << 0); //DIER set 0
	TIM1->CR1 |= (1 << 0); //TIM1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 4); //Timer interrupt priority set
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //Timer interrupt enabled
}

/*TIM1 Interrupt Handler*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	if (TIM1->CCR1 < 200){
		uint32_t x = (TIM1->CCR1)+(10U);
		TIM1->CCR1 = x;
	}
	else{
		TIM1->CCR1 = 0;
		TIM1->CCR1 |= (0);
	}
	//TIM1->SR &= ~(1U << 0);//Clear update status register
}

/*TIM2 Interrupt Handler*/
void TIM2_IRQHandler(void){
	if (TIM2->ARR < 20000){
		uint32_t x = (TIM2->ARR)+(100U);
		TIM2->ARR = x;
	}
	else{
		TIM2->ARR = 0;
		TIM2->ARR |= (0);
	}
	TIM2->SR &= ~(1U << 0);//Clear update status register
}
void TIM3_IRQHandler(void){
	if (TIM3->ARR < 2000){
		uint32_t x = (TIM3->ARR)+(100U);
		TIM3->ARR = x;
	}
	else{
		TIM3->ARR = 0;
		TIM3->ARR |= (0);
	}
	TIM3->SR &= ~(1U << 0);//Clear update status register
}
