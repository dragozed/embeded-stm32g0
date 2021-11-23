/*
 * lab5_p1.c
 *
 * Author: Alper Kaan Ilik
 *
 */

#include "bsp.h"

char a[] = "Hello from Kaan Ilik";

int main(void) {

	BSP_UART_init(9600);//UART initialize function

	print("Hello from Kaan Ilik\n");

    while(1){

    }
   return 0;
}

