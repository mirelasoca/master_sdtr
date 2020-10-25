/*
 * main_func.h
 *
 * Created: 20.07.2019 21:24:10
 * Author : Bogdan Tarnauca
 */

#ifndef _MAIN_FUNC_H_
	#define _MAIN_FUNC_H_
	
//Apps must include an "main_config.h" header where libraries related params, overrides, settings should be defined.
//This header file is application specific. Must be provided even it is empty.
#include "main_config.h"
#include "main_defines.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>
#include <stdio.h>

/*!  @fn      pinToggle(volatile uint8_t *port, uint8_t pin)
 *   @brief   Toggles the value of the specified pin.
 *   @param   port the port whose pin value is to be toggled.
 *   @param   pin the pin which will be toggled.
 *   @return  none.
 */
void pinToggle(volatile uint8_t *port, uint8_t pin);

/*!  @fn      pinSet(volatile uint8_t *port, uint8_t pin)
 *   @brief   Sets to 1 the value of the specified pin.
 *   @param   port the port whose pin value is to be set.
 *   @param   pin the pin which will be set.
 *   @return  none.
 */
void pinSet(volatile uint8_t *port, uint8_t pin);

/*!  @fn      pinReset(volatile uint8_t *port, uint8_t pin)
 *   @brief   Sets to 0 the value of the specified pin.
 *   @param   port the port whose pin value is to be reset.
 *   @param   pin the pin which will be reset.
 *   @return  none.
 */
void pinReset(volatile uint8_t *port, uint8_t pin);

void tick1ms();
void tick10ms();
void tick50ms();
void tick100ms();
void tick250ms();

void inLoop1msTask();
void inLoop10msTask();
void inLoop50msTask();
void inLoop100msTask();
void inLoop250msTask();

void tickLED();

void processConsoleComm(SerialCom_t *serialCom);

#endif /* _MAIN_FUNC_H_ */