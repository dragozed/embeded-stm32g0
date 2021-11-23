/*
 * 
 * BSP
 * main.c
 * 
 * Author: Alper Kaan İlik
 */

#include "bsp.h"


int main(void);

int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    BSP_set_output_C(6);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);


    while(1) {

    }

    return 0;
}
