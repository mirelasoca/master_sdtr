/*
 * serial_com_cofig.h
 *
 *  Created on: 21.06.2019
 *	Author: Bogdan Tarnauca
 */

#ifndef INC_SERIAL_COM_COFIG_H_
#define INC_SERIAL_COM_COFIG_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct AVRUsartHandler{
    volatile uint8_t usartIndex;
    volatile uint8_t *driverDirectionControlPort;
    volatile uint8_t driverDirectionControlMask;
    volatile bool invertedDriverDirectionControl;        
}AVRUsartHandler_t;

#define EXT_USART_HANDLE_T AVRUsartHandler_t 
#define HAL_USART_HANDLE_T void * 

#endif /* INC_SERIAL_COM_COFIG_H_ */
