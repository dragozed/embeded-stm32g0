/*
 * Author: Alper Kaan İlik
 *
 * Board Support Package
 * bsp.h
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

/*BSP for STM32G031*/

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

#endif
