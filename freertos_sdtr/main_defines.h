/*
 * main_defines.h
 *
 * Created: 20.07.2019 21:21:20
 * Author : Bogdan Tarnauca
 */

#ifndef _MAIN_DEFINES_H_
    #define _MAIN_DEFINES_H_

#include <stdint.h>
#include <stdbool.h>

#include "avr_serial_com_hal.h"


#define RELOAD_LED_TICK		500
#define RELOAD_10MS_TICK	10
#define RELOAD_50MS_TICK	50
#define RELOAD_100MS_TICK	100
#define RELOAD_250MS_TICK	250
#define ELAPSED_COUNTER		0


#define  LED0_PORT PORTC
#define  LED0_PIN PINC0	

#define MAX_RX_MESSAGE_LENGTH 64
#define MAX_TX_MESSAGE_LENGTH MAX_RX_MESSAGE_LENGTH + 2

#define SW_TIMER_RELOAD 500

#define RX_TERM 0x0D



uint16_t softTimerCnt0;

uint8_t btnLeftState, btnRightState; 
uint16_t btnLeftClicks, btnRightClicks;
bool btnLeftPressedEvt, btnRightPressedEvt;

uint8_t ledPosition;

SerialCom_t serialCom0;
SerialComInitStr_t serialComInitStr;
EXT_USART_HANDLE_T usart0Handle;


char rxMessage[MAX_RX_MESSAGE_LENGTH];
char txMessage[MAX_TX_MESSAGE_LENGTH];
char newLine[3];

uint16_t counterLED;
uint16_t counter10ms;
uint16_t counter50ms;
uint16_t counter100ms;
uint16_t counter250ms;

bool execute_LED;

bool execute_1ms;
bool execute_10ms;
bool execute_50ms;
bool execute_100ms;
bool execute_250ms;



#endif /* _MAIN_DEFINES_H_ */