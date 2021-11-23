/*
 * Board Support Package
 * bsp.c
 * 
 * Author: Alper Kaan İlik
 */


#include "bsp.h"

/*Set output functions*/
BSP_set_output_A(int k){
	GPIOA->MODER &= ~(3U << 2*k);
	GPIOA->MODER |= (1U << 2*k);
}
BSP_set_output_B(int k){
	GPIOB->MODER &= ~(3U << 2*k);
	GPIOB->MODER |= (1U << 2*k);
}
BSP_set_output_C(int k){
	GPIOC->MODER &= ~(3U << 2*k);
	GPIOC->MODER |= (1U << 2*k);
}
/*Set input functions*/
BSP_set_input_A(int k){
	GPIOA->MODER &= ~(3U << 2*k);
	GPIOA->PUPDR |= (2U << 2*k); //Pull-down mode
}
BSP_set_input_B(int k){
	GPIOB->MODER &= ~(3U << 2*k);
	GPIOB->PUPDR |= (2U << 2*k); //Pull-down mode
}
