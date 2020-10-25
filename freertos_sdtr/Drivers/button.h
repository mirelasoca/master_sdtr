/*
 * button.h
 *
 * Created: 10/22/2020 10:52:41 AM
 *  Author: Mirela
 */ 
#include <avr/io.h>
//#include "projdefs.h"

#ifndef BUTTON_H_
#define BUTTON_H_
#define DDR_SWITCH_IP DDRB
#define BIT_SWITCH_IP 0
#define PORT_SWITCH_IP PORTB
#define PIN_SWITCH_IP PINB

void vButtonInit(void);
char xButtonGetStatus(void);

#endif /* BUTTON_H_ */