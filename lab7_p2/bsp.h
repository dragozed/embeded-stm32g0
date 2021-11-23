/*
 * Author: Alper Kaan İlik
 *
 * Board Support Package
 * bsp.h for lab7_p2.c
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

/*BSP for STM32G031*/

void delay(volatile uint32_t s);

/*Set output functions*/
void BSP_set_output_A(int k);
void BSP_set_output_B(int k);
void BSP_set_output_C(int k);

/*Set input functions*/
void BSP_set_input_A(int k);
void BSP_set_input_B(int k);

/*UART functions*/
void USART2_IRQHandler(void);
void printChar(uint8_t);
void _print(int, char *, int);
void print(char *);
void BSP_UART_init(uint32_t);

/*Keypad functions*/
void BSP_set_keypad_interrupts(void);
void BSP_clear_keypad_rows(void);
void BSP_set_keypad_rows(void);

/*Timer1 init*/
void init_timer1(void);

/*SSD functions*/
void display1(uint32_t k);
void display2(uint32_t k);
void display3(uint32_t k);
void display4(uint32_t k);
void cleardisplay(void);
void setdisplay(uint32_t l);

/*IWDG set function*/
void BSP_set_IWDG(void);

/*ADC init*/
void init_ADC(void);

#endif
