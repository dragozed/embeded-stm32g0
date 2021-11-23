/*
 * lab4_p1.c
 *
 * Author: Alper Kaan İlik
 *
 */

#include "stm32g0xx.h"

#define DELAY    64000 //1 ms

int main(void);
void delay_ms(volatile uint32_t);

int main(void) {


    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

    /* Setup PB4 as output */
    GPIOB->MODER &= ~(3U << 2*4);
    GPIOB->MODER |= (1U << 2*4);

    /* Turn on LED */
    GPIOB->ODR |= (1U << 4);

    /*Configure SysTick that gives interrupt after 64000 clock cycles = 1ms*/
    SysTick_Config(DELAY);
    while(1) {

    }

    return 0;
}

void SysTick_Handler(){
	SysTick_Config(DELAY);
	delay_ms(6400000);//1sec
	GPIOB->ODR ^= (1U << 4);//Toggle LED on PB4 to measure accuraccy of delay
}

void delay_ms(volatile uint32_t s) {
    for(; s>0; s--);
}
