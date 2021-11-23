/*
 * Simple State Machine
 * main.c
 * NOT DONE
 * 
 * Author: Alper Kaan İlik
 * 
 */

#include "ssm.h"


int main(void);


int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    while(1) {
        /* Toggle LED */
        GPIOC->ODR ^= (1U << 6);
    }

    return 0;
}
