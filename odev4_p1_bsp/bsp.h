/*
 * 
 * Board Support Package
 * bsp.h
 * 
 * Author: Alper Kaan İlik
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

/*BSP for STM32G031*/

/*Set output functions*/
BSP_set_output_A(int k);
BSP_set_output_B(int k);
BSP_set_output_C(int k);

/*Set input functions*/
BSP_set_input_A(int k);
BSP_set_input_B(int k);



#endif
