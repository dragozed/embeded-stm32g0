/*
 * Board Support Package
 * bsp.c for project3.c
 * 
 * Author: Alper Kaan İlik
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
	EXTI->EXTICR[0] |= (1U << 8*0);	//PB0
	/*Rising edge triggered*/
	EXTI->RTSR1 |= (1U << 0);	//0th pin
	/*Mask*/
	EXTI->IMR1 |= (1U << 0);	//0th pin
	/*NVIC*/
	NVIC_SetPriority(EXTI0_1_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}
/*Set keypad row outputs low function*/
void BSP_clear_keypad_rows(void){
	GPIOA->ODR &= ~(1U << 8);
	GPIOA->ODR &= ~(1U << 9);
	GPIOB->ODR &= ~(1U << 5);
	GPIOB->ODR &= ~(1U << 4);
}
/*Set keypad row outputs high function*/
void BSP_set_keypad_rows(void){
	GPIOA->ODR |= (1U << 8);
	GPIOA->ODR |= (1U << 9);
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
	GPIOA->ODR |= (1U << 10);//PA10	F
	GPIOA->ODR |= (1U << 11);//PA11	G
	GPIOA->ODR |= (1U << 12);//PA12	.
}

void setdisplay(uint32_t l){
	switch(l)
		{
		case 22://P display
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 21://b display
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			break;

		case 20://c display
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			break;

		case 19://r display
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			break;

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
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			break;

		case 15://O display
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			break;

		case 14://L display
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			break;

		case 13://F display
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;
		case 12://u or v display
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
			GPIOA->ODR &= ~(1U << 10);//PA10	F
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
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			break;

		case 5:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			break;

		case 6:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 6);//PA6	E
			GPIOA->ODR &= ~(1U << 10);//PA10	F
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
			GPIOA->ODR &= ~(1U << 10);//PA10	F
			GPIOA->ODR &= ~(1U << 11);//PA11 G
			break;

		case 9:
			GPIOA->ODR &= ~(1U << 0);//PA0	A
			GPIOA->ODR &= ~(1U << 1);//PA1	B
			GPIOA->ODR &= ~(1U << 4);//PA4	C
			GPIOA->ODR &= ~(1U << 5);//PA5	D
			GPIOA->ODR &= ~(1U << 10);//PA10	F
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
	GPIOB->MODER |= (3U << 2*2);

	RCC->APBENR2 |= (1U << 20);		 //ADC enable

	ADC1->CR = 0; 					//reset ADC control register
	ADC1->CR |= (1U << 28); 		//enable voltage regulator
	delay(500);						//delay

	ADC1->CR |= (1U << 31);			//enable calibration register
	while(ADC1->CR & (1U << 31));	//wait until calibration is done

	ADC1->IER |= (1U << 3);			//end of conversion sequence interrupt enable
	ADC1->CFGR1 |= (2U << 3);		//resolution set 8 bit
	ADC1->CFGR1 |= (0U << 13);		//single mode
	/*-------------------------------------------------------------------------*/
	ADC1->CFGR1 &= ~(7U << 6);		//clear bits to set EXTSEL: TRG0
	ADC1->CFGR1 |= (1U << 10);		//set EXTEN rising edge
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; 					//Configuration set 0
	TIM1->CR1 |= (1 << 7); 			//ARPE
	TIM1->CNT = 0; 					//Counter set 0

	TIM1->PSC = 1;
	TIM1->ARR = 2500;				//5000 -> 6,4khz 2500 -> 12,8khz


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

void BSP_PWM_init(void){//initialize for PA7 TIM3_CH2
	RCC->IOPENR |= (1U << 0);
	RCC->APBENR1 |= (1U << 1); //Enable TIM3 clock

	TIM3->CR1 = 0;			//configuration set 0
	TIM3->CCMR1 = 0;		//capture-compare set 0
	TIM3->CR1 |= (1U << 4);	//downcounter

	TIM3->CNT = 0;
	TIM3->PSC = 1;
	TIM3->ARR = 2500;		//5000 -> 6,4khz 2500 -> 12,8khz
	TIM3->CCR2 = 0;			//duty cycle

	TIM3->CCMR1 &= ~(0x7U << 12);
	TIM3->CCMR1 |= (0x6U << 12);	//OC2M PWM mode 1

	TIM3->CR1 |= (1U << 7);			//autoreload enable
	TIM3->EGR |= (1U << 0);			//update generation

	TIM3->CCMR1 |= (1U << 11);		//output compare 2 preload enable
	TIM3->CCER |= (1U << 4);		//capture compare 2 output enable CC2E

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

/*I2C init for EEPROM*/
void init_I2C(void){//for PB8 as AF6 SCL, PB9 as AF6 SDA
	RCC->IOPENR |= (1U << 1);//enable GPIOB

	GPIOB->MODER &= ~(3U << 2*8);
	GPIOB->MODER |= (2 << 2*8);//PB8 as AF
	GPIOB->OTYPER |= (1U << 8);//set opendrain

	GPIOB->AFR[1] &= ~(0xFU << 4*0);
	GPIOB->AFR[1] |= (6 << 4*0);//PB8 set as AF6

	GPIOB->MODER &= ~(3U << 2*9);
	GPIOB->MODER |= (2 << 2*9);//PB9 as AF
	GPIOB->OTYPER |= (1U << 9);//set opendrain

	GPIOB->AFR[1] &= ~(0xFU << 4*1);
	GPIOB->AFR[1] |= (6 << 4*1);//PB9 set as AF6

	RCC->APBENR1 |= (1U << 21);//I2C1 Enable

	I2C1->CR1 = 0;
	I2C1->CR1 |= (1U << 7);//Error interrupt ERRI

	I2C1->TIMINGR |= (3 << 28);//PRESC
	I2C1->TIMINGR |= (0x13 << 0);//SCLL
	I2C1->TIMINGR |= (0xF << 8);//SCLH
	I2C1->TIMINGR |= (0x2 << 16);//SDADEL
	I2C1->TIMINGR |= (0x4 << 20);//SCLDEL

	I2C1->CR1 = (1U << 0); //PE

	NVIC_SetPriority(I2C1_IRQn, 1);
	NVIC_EnableIRQ(I2C1_IRQn);
}

/*I2C read for EEPROM*/
uint8_t read_I2C(uint8_t devAddr, uint16_t regAddr){

	//WRITE OPERATION (send address and register to read)
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);//slave address
	I2C1->CR2 |= (2U << 16);	//number of bytes
	I2C1->CR2 |= (1U << 13);	//Generate start

	while(!(I2C1->ISR & (1 << 1))); // TXIS
	I2C1->TXDR = (uint32_t)(regAddr >> 8);//regAddr highbits

	while(!(I2C1->ISR & (1 << 1))); // TXIS
	I2C1->TXDR = (uint32_t)(regAddr & 0xFF);//regAddr lowbits

	while(!(I2C1->ISR & (1 << 6))); // TC

	//READ OPERATION (read data)
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);
	I2C1->CR2 |= (1U << 10);	//READ mode
	I2C1->CR2 |= (1U << 16);	//number of bytes
	I2C1->CR2 |= (1U << 15);	//NACK
	I2C1->CR2 |= (1U << 25);	//AUTOEND

	I2C1->CR2 |= (1U << 13);	//Generate start
	while(!(I2C1->ISR & (1 << 2)));	//wait until RXNE = 1

	uint8_t data = (uint8_t)I2C1->RXDR;
	return data;

}

/*I2C write for EEPROM*/
void write_I2C(uint8_t devAddr, uint16_t regAddr, uint8_t data){

	//WRITE OPERATION (send address and register to read)
	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)devAddr << 1);//slave address
	I2C1->CR2 |= (3U << 16);	//number of bytes (1for data, 1 for addresslowbits, 1for address highbits)
	I2C1->CR2 |= (1U << 25);	//AUTOEND
	I2C1->CR2 |= (1U << 13);	//Generate start

	while(!(I2C1->ISR & (1 << 1))); // TXIS
	I2C1->TXDR = (uint32_t)(regAddr >> 8);//regAddr highbits

	while(!(I2C1->ISR & (1 << 1))); // TXIS
	I2C1->TXDR = (uint32_t)(regAddr & 0xFF);//regAddr lowbits

	while(!(I2C1->ISR & (1 << 1))); // TXIS
	I2C1->TXDR = (uint32_t)data;

    delay(6000);

}



void I2C1_IRQHandler(void){
	//Only enters when error
}
