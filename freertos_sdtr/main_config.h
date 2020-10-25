/*
 * main_config.h
 *
 * Created: 01.01.2009
 * Author : Bogdan Tarnauca
 */

#ifndef _MAIN_CONFIG_H_
	#define  _MAIN_CONFIG_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#ifndef F_CPU
    //#define F_CPU 7372800UL
    //#define F_CPU 11059200UL
    #define F_CPU 16000000UL
#endif

#define __ATMEGA128
	
#endif /* _MAIN_CONFIG_H_ */