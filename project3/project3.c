/*
 * project3.c
 
 * Author: Alper Kaan Ilik
 */


#include "bsp.h"
#include <math.h>

/*display values*/
int32_t a, b, c, d;
/*Synchronous conversion enable*/
int32_t syncEn = 0;


int main(void) {

	a=1;b=5;c=3; d=7;//set display values

    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);

    /*Set Keypad Pins*/
	BSP_set_input_B(0);
	BSP_set_output_A(8);
	BSP_set_output_A(9); //B9 DU A9 OLDU
	BSP_set_output_B(5);
	BSP_set_output_B(4);

    /*Set external interrupts for keypad*/
    BSP_set_keypad_interrupts();
    BSP_set_keypad_rows();//Set keypad row outputs high

    /*Set SSD pins*/
    /*Setup PA0 PA1 PA4 PA5 PA6 PA7 PA11 PA12 output*/
    BSP_set_output_A(0);
    BSP_set_output_A(1);
    BSP_set_output_A(4);
    BSP_set_output_A(5);
    BSP_set_output_A(6);
    BSP_set_output_A(10);	//PA7 -> PA10
    BSP_set_output_A(11);
    BSP_set_output_A(12);
    /*Setup PB1 PB3 PB6 PB7 output */
    BSP_set_output_B(1);
    BSP_set_output_B(3);
    BSP_set_output_B(6);
    BSP_set_output_B(7);

    /*init TIM1 for displays*/
    init_timer1();

    /*ADC init*/
    init_ADC();//Uses PB2 pin

    /*PWM init*/
    BSP_PWM_init();//Uses PA7 pin

    /*I2C init for EEPROM Uses PB8-PB9*/
    init_I2C();

    write_I2C(0x50,0x0000,0xFF);//device address for EEPROM is 0x50
    write_I2C(0x50,0x0001,0x20);
    uint8_t data = read_I2C(0x50,0x0010);
    uint8_t data2 = read_I2C(0x50,0x0000);
    //I am sure that write and read on EEPROM works thanks to 4 lines of code above
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

/*External interrupt handler*/
void EXTI0_1_IRQHandler(void){//Interrupt from PB0(C3)
	BSP_clear_keypad_rows();

	GPIOA->ODR ^= (1U << 8);	//PA8(R4) # recordstate rcd
	if((GPIOB->IDR >> 0) & 1){
		a = 10;
		b = 19;
		c = 20;
		d = 18;
		syncEn = 0;
	}
	GPIOA->ODR ^= (1U << 8);

	GPIOA->ODR ^= (1U << 9);	//PA9(R3) 9 playbackstate PLb
	if((GPIOB->IDR >> 0) & 1){
		a = 10;
		b = 22;
		c = 14;
		d = 21;
		syncEn = 0;
	}
	GPIOA->ODR ^= (1U << 9);

	GPIOB->ODR ^= (1U << 5);	//PB5(R2) 6 synchronous state Sncr
	if((GPIOB->IDR >> 0) & 1){
		a = 5;
		b = 17;
		c = 20;
		d = 19;
		syncEn = 1;
	}
	GPIOB->ODR ^= (1U << 5);

	GPIOB->ODR ^= (1U << 4);	//PB4(R1) 3
	if((GPIOB->IDR >> 0) & 1){
		a = 11;
		b = 11;
		c = 11;
		d = 11;
		syncEn = 0;
	}
	GPIOB->ODR ^= (1U << 4);

	BSP_set_keypad_rows();
	EXTI->RPR1 |= (1U << 0);//Pending register is cleared
}

uint32_t n = 0;
/*Interrupt handler for speaker PWM*/
void TIM3_IRQHandler(void){

	TIM3->CCR2 = n;
	TIM3->SR &= ~(1U << 0);//Clear update status register
}

/*Interrupt handler for microphone ADC*/
void ADC_COMP_IRQHandler (void){
	uint32_t val;
	val = ADC1->DR;
	if(syncEn == 1){
		n = (val*2500) / 255;
	}
	else {
		n = 0;
	}
	//write_I2C(0x50,0x0010,n);
	ADC1->ISR |= (1U << 3); //Clear end of sequence EOS
}
