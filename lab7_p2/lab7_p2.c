/*
 * lab7_p2.c
 * Author: Alper Kaan İlik
 */


#include "bsp.h"
#include <math.h>

/*display values*/
int32_t a, b, c, d;

void set_val1(void);

int main(void) {

    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);

    /*Set SSD pins*/
    /*Setup PA0 PA1 PA4 PA5 PA6 PA7 PA11 PA12 output*/
    BSP_set_output_A(0);
    BSP_set_output_A(1);
    BSP_set_output_A(4);
    BSP_set_output_A(5);
    BSP_set_output_A(6);
    BSP_set_output_A(7);
    BSP_set_output_A(11);
    BSP_set_output_A(12);
    /*Setup PB1 PB3 PB6 PB7 output */
    BSP_set_output_B(1);
    BSP_set_output_B(3);
    BSP_set_output_B(6);
    BSP_set_output_B(7);



    /*ADC init*/
    init_ADC();

    while(1) {

    }

    return 0;
}


void flash_displays(void){
	display1(a);
	display2(b);
	display3(c);
	display4(d);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	flash_displays();

	TIM1->SR &= ~(1U << 0);//Clear update status register
}

void ADC_COMP_IRQHandler(void){
	flash_displays();
}
