/*
* avr_usart.h
*
* Created: 01.01.2009
* Author : Bogdan Tarnauca
*/

//MD+ Acest comentariu nu trebuie inlaturat!!!

#ifndef _AVR_SERIAL_COM_HAL_H_
	#define _AVR_SERIAL_COM_HAL_H_    

#include<stdint.h>
//Apps must include an "main_config.h" header where libraries related params, overrides, settings should be defined.
//This header file is application specific. Must be provided even if it is empty.
#include "main_config.h"
//#include "avr_defines.h"
#include "serial_com_config.h"
#include "serial_com.h"

#include<avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void switchPinTransmitMode(SerialCom_t *serialCom);
void switchPinReceiveMode(SerialCom_t *serialCom);

void switchDriverToReceiveMode(SerialCom_t *serialCom);
void switchDriverToTransmitMode(SerialCom_t *serialCom);

/******************************************************************************/
/******************************************************************************/
#if (!defined __ATMEGA128) && (!defined __ATMEGA2560) && (!defined __ATMEGA328) && (!defined __ATMEGA2560)
#error Target device not supported
#endif

#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)
SerialCom_t *serialComPtr0;
// volatile uint8_t *uart0_Port485Dir;
// volatile uint8_t uart0_Port485DirMask;
// volatile bool uart0_bRS485;
// volatile bool uart0_bRS485Inversat;
#endif
	
	
#if (defined __ATMEGA128) || (defined __ATMEGA2560)
SerialCom_t *serialComPtr1;
// volatile uint8_t *uart1_Port485Dir;
// volatile char uart1_Port485DirMask;
// //volatile bool uart1_bRS485;
// volatile bool uart1_bRS485Inversat;
#endif
	
#if (defined __ATMEGA2560)
SerialCom_t *serialComPtr2;
// volatile uint8_t *uart2_Port485Dir;
// volatile char uart2_Port485DirMask;
// //volatile bool uart2_bRS485;
// volatile bool uart2_bRS485Inversat;
#endif

#if (defined __ATMEGA2560)
SerialCom_t *serialComPtr3;
// volatile uint8_t *uart3_Port485Dir;
// volatile char uart3_Port485DirMask;
// //volatile bool uart3_bRS485;
// volatile bool uart3_bRS485Inversat;
#endif

/******************************************************************************/
#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)

//MD+ Acest comentariu nu trebuie inlaturat!!!
void uart0_init(SerialCom_t *serialCom, 
                volatile uint8_t *PORT485_DIR,
                uint8_t Port485DirMask,
                bool bRS485Inversat,
                uint8_t paramUCSR0A,
                uint8_t paramUCSR0C,
                uint8_t paramUBRR0L,
                uint8_t paramUBRR0H,
                uint8_t paramUCSR0B,
                uint16_t  nValoareTimeout,
                uint16_t  nValoareTimeoutChar);

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart0_udre_isr: uart0_UDRE_ISR_VECTOR_NO
void uart0_udre_isr(void);
#else
	#ifdef __ATMEGA328
	ISR(USART_UDRE_vect);
	#else
	ISR(USART0_UDRE_vect);
	#endif
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart0_tx_isr: uart0_TX_ISR_VECTOR_NO
void uart0_tx_isr(void);
#else
	#ifdef __ATMEGA328
	ISR(USART_TX_vect);
	#else
	ISR(USART0_TX_vect);
	#endif
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart0_rx_isr: uart0_RX_ISR_VECTOR_NO
void uart0_rx_isr(void);
#else
	#ifdef __ATMEGA328
	ISR(USART_RX_vect);
	#else
	ISR(USART0_RX_vect);
	#endif
#endif

#endif

/******************************************************************************/
#if (defined __ATMEGA128) || (defined __ATMEGA2560)

//MD+ Acest comentariu nu trebuie inlaturat!!!
void uart1_init(SerialCom_t *serialCom,
                volatile uint8_t *PORT485_DIR,
                uint8_t Port485DirMask,
                bool bRS485Inversat,
                uint8_t paramUCSR0A,
                uint8_t paramUCSR0C,
                uint8_t paramUBRR0L,
                uint8_t paramUBRR0H,
                uint8_t paramUCSR0B,
                uint16_t  nValoareTimeout,
                uint16_t  nValoareTimeoutChar);

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart1_udre_isr: uart1_UDRE_ISR_VECTOR_NO
void uart1_udre_isr(void);
#else
ISR(USART1_UDRE_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart1_tx_isr: uart1_TX_ISR_VECTOR_NO
void uart1_tx_isr(void);
#else
ISR(USART1_TX_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart1_rx_isr: uart1_RX_ISR_VECTOR_NO
void uart1_rx_isr(void);
#else
ISR(USART1_RX_vect);
#endif

#endif
/******************************************************************************/
/******************************************************************************/
#if (defined __ATMEGA2560)

//MD+ Acest comentariu nu trebuie inlaturat!!!
void uart2_init(SerialCom_t *serialCom, 
                volatile uint8_t *PORT485_DIR,
                uint8_t Port485DirMask,
                bool bRS485Inversat,
                uint8_t paramUCSR0A,
                uint8_t paramUCSR0C,
                uint8_t paramUBRR0L,
                uint8_t paramUBRR0H,
                uint8_t paramUCSR0B,
                uint16_t  nValoareTimeout,
                uint16_t  nValoareTimeoutChar);

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart2_udre_isr: uart2_UDRE_ISR_VECTOR_NO
void uart2_udre_isr(void);
#else
ISR(USART2_UDRE_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart2_tx_isr: uart2_TX_ISR_VECTOR_NO
void uart2_tx_isr(void);
#else
ISR(USART2_TX_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart2_rx_isr: uart2_RX_ISR_VECTOR_NO
void uart2_rx_isr(void);
#else
ISR(USART2_RX_vect);
#endif

#endif
/******************************************************************************/
/******************************************************************************/
#if (defined __ATMEGA2560)
//MD+ Acest comentariu nu trebuie inlaturat!!!
void uart3_init(SerialCom_t *serialCom,
                volatile uint8_t *PORT485_DIR,
                uint8_t Port485DirMask,
                bool bRS485Inversat,
                uint8_t paramUCSR0A,
                uint8_t paramUCSR0C,
                uint8_t paramUBRR0L,
                uint8_t paramUBRR0H,
                uint8_t paramUCSR0B,
                uint16_t  nValoareTimeout,
                uint16_t  nValoareTimeoutChar);

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart3_udre_isr: uart3_UDRE_ISR_VECTOR_NO
void uart3_udre_isr(void);
#else
ISR(USART3_UDRE_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart3_tx_isr: uart3_TX_ISR_VECTOR_NO
void uart3_tx_isr(void);
#else
ISR(USART3_TX_vect);
#endif

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart3_rx_isr: uart3_RX_ISR_VECTOR_NO
void uart3_rx_isr(void);
#else
ISR(USART3_RX_vect);
#endif

#endif

#endif /* _AVR_USART_H_ */