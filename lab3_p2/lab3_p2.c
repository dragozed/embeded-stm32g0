/*
 * lab3_p2.c
 *
 * Author: Alper Kaan İlik
 *
 */

#include "stm32g0xx.h"


#define LEDDELAY    1600000

int main(void);
void delay(volatile uint32_t);
void HardFault_Handler(void);
void EXTI2_3_IRQHandler();
void EXTI4_15_IRQHandler();

int main(void) {

    /* Enable GPIOB and GPIOA clock */
    RCC->IOPENR |= (3U << 0);

    /* Setup PB3 as input */
    GPIOB->MODER &= ~(3U << 2*3); //GPIOB_MODER 2*3 rd bits '00'

    /* Setup PA5 as output */
    GPIOA->MODER &= ~(3U << 2*5); //set 2*5 th bits '00'(clear)
    GPIOA->MODER |= (1U << 2*5); //set 2*5 th bits '01'

    /* Setup PB3 as interrupt */
    EXTI->RTSR1 |= (1U << 3);
    EXTI->EXTICR[0] |= (1U << 8*3);
    EXTI->IMR1 |= (1U << 3);
    NVIC_SetPriority(EXTI2_3_IRQn,0);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    while(1) {


    }

    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void EXTI2_3_IRQHandler(){
	EXTI->RPR1 |= (1U << 3);

    GPIOA->ODR ^= (1U << 5);//Toggle LED on PA5
}


