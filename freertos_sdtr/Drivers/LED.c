/*
 * LED.c
 *
 * Created: 10/14/2020 9:57:57 PM
 *  Author: Mirela
 */ 
#include <avr/io.h>
#include "LED.h"
void vLEDInit(void)
{
	// Set LED_O as output pin
	DDR_LED_O |= (1<<BIT_LED_O);
}
void vLEDToggle(void)
{
	//Toggle LED
	PORT_LED_O ^= (1<<BIT_LED_O);
}