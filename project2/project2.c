/*
 *
 * project2.c
 *
 * Author: Alper Kaan İlik
 */


#include "bsp.h"
#include <math.h>

/*display values*/
int32_t a, b, c, d;

int32_t e = 1;//idle bit(don't need32)

/*char that holds calculators instruction*/
char inst = 'N' ;//default inst None
/*intruction values*/
int32_t val1,val2;//signed values
void set_val1(void);

int main(void) {
	/*set display values*/
	a = 1;
	b = 5;
	c = 3;
	d = 7;
    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);

    /*Set Keypad Pins*/
    BSP_set_input_A(9);
    BSP_set_input_B(0);
    BSP_set_input_B(2);
    BSP_set_input_B(8);
    BSP_set_output_A(8);
    BSP_set_output_B(9);
    BSP_set_output_B(5);
    BSP_set_output_B(4);

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

    /*Init timer1*/
    init_timer1();
    /*Set external interrupts for keypad*/
    BSP_set_keypad_interrupts();

    /*Set all Keypad pins high*/
    BSP_set_keypad_rows();

    /*Set IWDG*/
    BSP_set_IWDG();

    while(1) {

    }

    return 0;
}

/*External interrupt handlers*/
void EXTI0_1_IRQHandler(void){//Interrupt from PB0(C3)
	BSP_clear_keypad_rows();
	IWDG->KR = (0x0000AAAA); //feeding the dog

	if(e == 1){
		e = 0; //Idle bit
		a = 10;
		b = 10;
		c = 10;
		d = 10;
	}

	GPIOA->ODR ^= (1U << 8);	//PA8(R4) #
	if((GPIOB->IDR >> 0) & 1){
		if(inst == 'J'){inst = 'O';}//O = EEE
		if(inst == 'E'){inst = 'J';}//J = EE
		if(inst == 'N'){inst = 'E';}
		a = 10;
		b = 10;
		c = 10;
		d = 10;
	}
	GPIOA->ODR ^= (1U << 8);

	//Value check
	int32_t vala, valb, valc, vald, val;
	if(a == 10){vala = 0;}
	if(b == 10){valb = 0;}
	if(c == 10){valc = 0;}
	if(d == 10){vald = 0;}
	if(a != 10){vala = a;}
	if(b != 10){valb = b;}
	if(c != 10){valc = c;}
	if(d != 10){vald = d;}
	val = vald + (10*valc) + (100*valb) + (1000*vala);
	if((val >= 1000) | (val <= -100)){
	}

	else{
		if(b != 10){
			a = b;
		}
		if(c != 10){
			b = c;
		}
		if(d != 10){
			c = d;
		}


		GPIOB->ODR ^= (1U << 9);	//PB9(R3) 9
		if((GPIOB->IDR >> 0) & 1){
			d = 9;
		}
		GPIOB->ODR ^= (1U << 9);

		GPIOB->ODR ^= (1U << 5);	//PB5(R2) 6
		if((GPIOB->IDR >> 0) & 1){
			d = 6;
		}
		GPIOB->ODR ^= (1U << 5);

		GPIOB->ODR ^= (1U << 4);	//PB4(R1) 3
		if((GPIOB->IDR >> 0) & 1){
			d = 3;
		}
		GPIOB->ODR ^= (1U << 4);

	}
	BSP_set_keypad_rows();
	EXTI->RPR1 |= (1U << 0);//Pending register is cleared
}
void EXTI2_3_IRQHandler(void){//Interrupt from PB2(C2)
	BSP_clear_keypad_rows();
	IWDG->KR = (0x0000AAAA); //feeding the dog

	if(e == 1){
		e = 0; //Idle bit
		a = 10;
		b = 10;
		c = 10;
		d = 10;
	}
	//Value check
	int32_t vala, valb, valc, vald, val;
	if(a == 10){vala = 0;}
	if(b == 10){valb = 0;}
	if(c == 10){valc = 0;}
	if(d == 10){vald = 0;}
	if(a != 10){vala = a;}
	if(b != 10){valb = b;}
	if(c != 10){valc = c;}
	if(d != 10){vald = d;}
	val = vald + (10*valc) + (100*valb) + (1000*vala);
	if((val >= 1000) | (val <= -100)){
	}

	else{
		if(b != 10){
			a = b;
		}
		if(c != 10){
			b = c;
		}
		if(d != 10){
			c = d;
		}
		GPIOA->ODR ^= (1U << 8);	//PA8(R4) 0
		if((GPIOB->IDR >> 2) & 1){
			d = 0;
		}
		GPIOA->ODR ^= (1U << 8);

		GPIOB->ODR ^= (1U << 9);	//PB9(R3) 8
		if((GPIOB->IDR >> 2) & 1){
			d = 8;
		}
		GPIOB->ODR ^= (1U << 9);

		GPIOB->ODR ^= (1U << 5);	//PB5(R2) 5
		if((GPIOB->IDR >> 2) & 1){
			d = 5;
		}
		GPIOB->ODR ^= (1U << 5);

		GPIOB->ODR ^= (1U << 4);	//PB4(R1) 2
		if((GPIOB->IDR >> 2) & 1){
			d = 2;
		}
		GPIOB->ODR ^= (1U << 4);

	}
	BSP_set_keypad_rows();
	EXTI->RPR1 |= (1U << 2);//Pending register is cleared
}
void EXTI4_15_IRQHandler(void){//Interrupt from PA9(C4) or PB8(C1)
	IWDG->KR = (0x0000AAAA); //feeding the dog
	if(e == 1){
		e = 0; //Idle bit
		a = 10;
		b = 10;
		c = 10;
		d = 10;
	}

	if((GPIOA->IDR >> 9) & 1){	//Interrupt from PA9(C4)
		BSP_clear_keypad_rows();

		GPIOA->ODR ^= (1U << 8);	//PA8(R4) D
		if((GPIOA->IDR >> 9) & 1){
			if(inst == 'N'){
				inst = 'D';
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if((a != 10) & (a != 11) & (a != 15) & (a != 16)){vala = a;}
				if((b != 10) & (b != 12) & (b != 17)){valb = b;}
				if((c != 10) & (c != 13) & (c != 12)){valc = c;}
				if((d != 10) & (d != 14) & (d != 18)){vald = d;}
				if(a == 11){val1 = -1*(vald + (10*valc) + (100*valb));}//if number is negative
				else{val1 = vald + (10*valc) + (100*valb) + (1000*vala);}
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'E'){
				inst = 'I';//I = ED x^2
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'J'){
				inst = 'P';//P = EED cot
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
		}
		GPIOA->ODR ^= (1U << 8);

		GPIOB->ODR ^= (1U << 9);	//PB9(R3) C
		if((GPIOA->IDR >> 9) & 1){
			if(inst == 'N'){
				inst = 'C';
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if((a != 10) & (a != 11) & (a != 15) & (a != 16)){vala = a;}
				if((b != 10) & (b != 12) & (b != 17)){valb = b;}
				if((c != 10) & (c != 13) & (c != 12)){valc = c;}
				if((d != 10) & (d != 14) & (d != 18)){vald = d;}
				if(a == 11){val1 = -1*(vald + (10*valc) + (100*valb));}//if number is negative
				else{val1 = vald + (10*valc) + (100*valb) + (1000*vala);}
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'E'){
				inst = 'H';//H=EC sqrt
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'J'){
				inst = 'M';//M = EEC tan
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
		}
		GPIOB->ODR ^= (1U << 9);

		GPIOB->ODR ^= (1U << 5);	//PB5(R2) B
		if((GPIOA->IDR >> 9) & 1){
			if(inst == 'N'){
				inst = 'B';
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if((a != 10) & (a != 11) & (a != 15) & (a != 16)){vala = a;}
				if((b != 10) & (b != 12) & (b != 17)){valb = b;}
				if((c != 10) & (c != 13) & (c != 12)){valc = c;}
				if((d != 10) & (d != 14) & (d != 18)){vald = d;}
				if(a == 11){val1 = -1*(vald + (10*valc) + (100*valb));}//if number is negative
				else{val1 = vald + (10*valc) + (100*valb) + (1000*vala);}
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'E'){
				inst = 'G';//G = EB ln
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'J'){
				inst = 'L';//L= EEB cos
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
		}
		GPIOB->ODR ^= (1U << 5);

		GPIOB->ODR ^= (1U << 4);	//PB4(R1) A
		if((GPIOA->IDR >> 9) & 1){
			if(inst == 'N'){
				inst = 'A';
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if((a != 10) & (a != 11) & (a != 15) & (a != 16)){vala = a;}
				if((b != 10) & (b != 12) & (b != 17)){valb = b;}
				if((c != 10) & (c != 13) & (c != 12)){valc = c;}
				if((d != 10) & (d != 14) & (d != 18)){vald = d;}
				if(a == 11){val1 = -1*(vald + (10*valc) + (100*valb));}//if number is negative
				else{val1 = vald + (10*valc) + (100*valb) + (1000*vala);}
				a = 10;
				b = 10;
				c = 10;
				d = 10;
				}
			if(inst == 'E'){
				inst = 'F';//F = EA log
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
			if(inst == 'J'){
				inst = 'K';//K = EEA sin
				a = 10;
				b = 10;
				c = 10;
				d = 10;
			}
		}
		GPIOB->ODR ^= (1U << 4);

		BSP_set_keypad_rows();
	}


	if ((GPIOB->IDR >> 8) & 1){	//Interrupt from PB8(C1)
		BSP_clear_keypad_rows();

		GPIOA->ODR ^= (1U << 8);	//PA8(R4) * equals
		if((GPIOB->IDR >> 8) & 1){
			if(inst == 'A'){//if the instruction was addition 'A'
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = val1 + val2; //Answer
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'B'){//if the instruction was subtraction 'B'
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = val1 - val2; //Answer
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'C'){//if the instruction was multiplication 'C'
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = val1 * val2; //Answer
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'D'){//if the instruction was division 'D'
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = val1 / val2; //Answer
				if(val2 == 0){//invalid division
					a = 16; b = 17; c = 12; d = 18;
				}
				else{
					set_val1();
				}
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'F'){//if the instruction was log 'F'
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = log(val2)/log(10);
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'G'){//if the instruction was ln
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = log(val2);
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'H'){//if the instruction was sqrt
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = sqrt(val2);
				if(val2 < 0){//invalid sqrt
					a = 16; b = 17; c = 12; d = 18;
				}
				else{
					set_val1();
				}
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'I'){//if the instruction was x^2
				int32_t vala, valb, valc, vald;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				val1 = pow(val2,2);
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'K'){//if the instruction was sin
				int32_t vala, valb, valc, vald;
				float_t valf;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				valf = (M_PI/180) * val2;
				valf = sin(valf);
				val1 = valf;
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'L'){//if the instruction was cos
				int32_t vala, valb, valc, vald;
				float_t valf;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				valf = (M_PI/180) * val2;
				valf = cos(valf);
				val1 = valf;
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'M'){//if the instruction was tan
				int32_t vala, valb, valc, vald;
				float_t valf;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				valf = (M_PI/180) * val2;
				valf = tan(valf);
				val1 = valf;
				set_val1();
				inst = 'N';//Instruction set back to None
			}
			if(inst == 'P'){//if the instruction was cot
				int32_t vala, valb, valc, vald;
				float_t valf;
				if(a == 10){vala = 0;}
				if(b == 10){valb = 0;}
				if(c == 10){valc = 0;}
				if(d == 10){vald = 0;}
				if(a != 10){vala = a;}
				if(b != 10){valb = b;}
				if(c != 10){valc = c;}
				if(d != 10){vald = d;}
				val2 = vald + (10*valc) + (100*valb) + (1000*vala);
				valf = (M_PI/180) * val2;
				valf = 1/tan(valf);
				val1 = valf;
				set_val1();
				inst = 'N';//Instruction set back to None
			}


			if(inst == 'O'){//if the instruction was pi
				val1 = M_PI;
				d = val1 % 10;
				if(val1 >= 10){c = (val1 / 10) % 10;}else{c = 10;}
				if(val1 >= 100){b = (val1 / 100) % 10;}else{b = 10;}
				if(val1 >= 1000){a = (val1 / 1000) % 10;}else{a = 10;}
				inst = 'N';//Instruction set back to None
			}
			//if the instruction was 'B' 'C' ...
		}
		GPIOA->ODR ^= (1U << 8);

		//Value check
		int32_t vala, valb, valc, vald, val;
		if(a == 10){vala = 0;}
		if(b == 10){valb = 0;}
		if(c == 10){valc = 0;}
		if(d == 10){vald = 0;}
		if(a != 10){vala = a;}
		if(b != 10){valb = b;}
		if(c != 10){valc = c;}
		if(d != 10){vald = d;}
		val = vald + (10*valc) + (100*valb) + (1000*vala);
		if((val >= 1000) | (val <= -100)){
		}

		else{

			GPIOB->ODR ^= (1U << 9);	//PB9(R3) 7
			if((GPIOB->IDR >> 8) & 1){
				if(b != 10){
					a = b;
				}
				if(c != 10){
					b = c;
				}
				if(d != 10){
					c = d;
				}
				d = 7;
			}
			GPIOB->ODR ^= (1U << 9);

			GPIOB->ODR ^= (1U << 5);	//PB5(R2) 4
			if((GPIOB->IDR >> 8) & 1){
				if(b != 10){
					a = b;
				}
				if(c != 10){
					b = c;
				}
				if(d != 10){
					c = d;
				}
				d = 4;
			}
			GPIOB->ODR ^= (1U << 5);

			GPIOB->ODR ^= (1U << 4);	//PB4(R1) 1
			if((GPIOB->IDR >> 8) & 1){
				if(b != 10){
					a = b;
				}
				if(c != 10){
					b = c;
				}
				if(d != 10){
					c = d;
				}
				d = 1;
			}
			GPIOB->ODR ^= (1U << 4);

		}

	}
	BSP_set_keypad_rows();
	EXTI->RPR1 |= (1U << 9);//Pending register is cleared
	EXTI->RPR1 |= (1U << 8);//Pending register is cleared
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

void set_val1(void){//a routine i used a lot so i decided to make it a function
	if((val1 > 9999) | (val1 < -999)){//overflow
		a = 15;
		b = 12;
		c = 13;
		d = 14;
	}
	if((val1 >= 0) & (val1 <= 9999)){
		d = val1 % 10;
		if(val1 >= 10){c = (val1 / 10) % 10;}else{c = 10;}
		if(val1 >= 100){b = (val1 / 100) % 10;}else{b = 10;}
		if(val1 >= 1000){a = (val1 / 1000) % 10;}else{a = 10;}
	}
	if((val1 < 0) & (val1 >= -999)){
		a = 11;//minus sign
		d= -1*(val1 % 10);
		if(val1 <= -10){c = -1*((val1 / 10) % 10);}else{c = 10;}
		if(val1 <= -10){b = -1*((val1 / 100) % 10);}else{c = 10;}
	}
}
