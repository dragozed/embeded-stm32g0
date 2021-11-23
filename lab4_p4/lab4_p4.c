/*
 * lab4_p4.c
 *
 * Author: Alper Kaan İlik
 *
 */

#include "stm32g0xx.h"

#define LEDDELAY    1600000

int main(void);
void delay(volatile uint32_t);


int main(void){
	/*Enable IWDG*/
	IWDG->KR = (0x0000CCCC);

	/*Configuring IWDG prescaler*/
	IWDG->KR = (0x00005555);
	IWDG->PR = (0x3);

	/*Reload register*/
	IWDG->RLR = (1100);//Set for roughly 1.1 seconds.

	/*feeding the dog*/
	IWDG->KR = (0x0000AAAA);

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);



    while(1) {

    	delay(LEDDELAY);

    	/* Toggle LED */
    	GPIOC->ODR ^= (1U << 6);

    	/*feeding the dog*/
    	IWDG->KR = (0x0000AAAA);

       }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);

}
