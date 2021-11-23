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
