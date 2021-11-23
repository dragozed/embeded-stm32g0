/*
 * lab3_p1.c
 *
 * Author: Alper Kaan İlik
 *
 */

#include "stm32g0xx.h"


#define LEDDELAY    600000

int main(void);
void delay(volatile uint32_t);
void HardFault_Handler(void);


int main(void) {

    /* Enable GPIOB and GPIOA clock */
    RCC->IOPENR |= (3U << 0);

    /* Setup PB6 as input */
    GPIOB->MODER &= ~(3U << 2*6); //GPIOB_MODER 2*6 th bits '00'

    /* Setup PA5 as output */
    GPIOA->MODER &= ~(3U << 2*5); //set 2*5 th bits '00'(clear)
    GPIOA->MODER |= (1U << 2*5); //set 2*5 th bits '01'


    //Toggle LED on PA5
    GPIOA->ODR ^= (1U << 5);

    while(1) {
        delay(LEDDELAY);
        //HardFault_Handler();

        if(GPIOB->IDR == (1U << 6)){//check PB6 input
        	HardFault_Handler();
        }
    }

    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void HardFault_Handler(void){
	__asm("ldr r1, =0x20001ff8");
	__asm("movs sp, r1");
	__asm("b Reset_Handler");//call Reset_Handler

}

