/*
 * lab4_p2.c
 *
 * Author: Alper Kaan İlik
 *
 */


#include "stm32g0xx.h"


int main(void);
void init_timer1();
void EXTI2_3_IRQHandler();
uint32_t n = 1;

int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /*Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /*Setup PB3 as input */
    GPIOB->MODER &= ~(3U << 2*3);
    GPIOB->PUPDR |= (2U << 2*3); //Pull-down mode

    /* Setup PB3 as interrupt */
	EXTI->EXTICR[0] |= (1U << 8*3);
    EXTI->RTSR1 |= (1U << 3); //rising edge
	EXTI->IMR1 |= (1U << 3);
	NVIC_SetPriority(EXTI2_3_IRQn, 5);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    init_timer1();

    while(1) {

    }

    return 0;
}
void init_timer1(){/*TIM1 Configuration*/
	/*Enable TIM1 clock*/
	RCC->APBENR2 |= (1U << 11);

	TIM1->CR1 = 0; //Configuration set 0
	TIM1->CR1 |= (1 << 7); //ARPE
	TIM1->CNT = 0; //Counter set 0

	TIM1->PSC = (999*n);
	TIM1->ARR = 16000;//1second interrupt PSC*ARR


	TIM1->DIER |= (1 << 0); //DIER set 0
	TIM1->CR1 |= (1 << 0); //TIM1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 10); //Timer interrupt priority set
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //Timer interrupt enabled
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	/* Toggle LED */
	GPIOC->ODR ^= (1U << 6);
	TIM1->SR &= ~(1U << 0);//Clear update status register

}

void EXTI2_3_IRQHandler(){
	if(n<11){
		n++;
	}
	else{
		n=1;
	}
    init_timer1();
	EXTI->RPR1 |= (1U << 3);//Pending register is cleared by setting 1
}

