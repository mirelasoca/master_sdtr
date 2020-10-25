/*
 * LED.h
 *
 * Created: 10/14/2020 9:58:17 PM
 *  Author: Mirela
 */ 


#ifndef LED_H_
#define LED_H_
#include <avr/io.h>
#define DDR_LED_O DDRB
#define BIT_LED_O 7
#define PORT_LED_O PORTB

void vLEDInit(void);
void vLEDToggle(void);




#endif /* LED_H_ */