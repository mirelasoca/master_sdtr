/*
 * serial_com.h
 *
 *  Created on: 15.06.2019
 *	Rework of the 2009 avr_usart.h driver converted into a platform independent driver
 *  Author: Bogdan Tarnauca
 */

#ifndef INC_SERIAL_COM_H_
#define INC_SERIAL_COM_H_

#include "serial_com_config.h"


#ifndef HAL_USART_HANDLE_T
	#define EXT_USART_HANDLE_T void *
#endif

#ifndef HAL_USART_HANDLE_T
	#define HAL_USART_HANDLE_T void *
#endif


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//Save the existing packing layout and update only for the sections of interest. Restore with the push
#pragma pack(push)
#pragma pack(1)

typedef struct SerialCom {
    HAL_USART_HANDLE_T *halUsartHandle; //if available this should point to the hal usart handle (tipically the vendor one)
	EXT_USART_HANDLE_T *extUsartHandle; //user define extended usart handle
    //uint8_t usartIndex;
	volatile uint8_t *rxBufferPtr;
	volatile uint16_t rxLength;
	volatile uint16_t rxBufferSize;
	volatile uint8_t rxTerminalChar;
	volatile uint16_t rxCounter;
	volatile uint16_t lastTestedIndex;
	volatile uint8_t *txBufferPtr;
	volatile uint16_t txLength;
	volatile uint16_t txCounter;
	volatile uint8_t rxTestCritertiaCount;
	volatile uint16_t *multiLengthRxPtr;
	volatile uint8_t *multiTerminalCharPtr;
	volatile uint16_t rxTransferTimeoutTicks;
	volatile uint16_t rxTrasnferTimeoutTickCounter;
	volatile uint16_t rxCharTimeoutTicks;
	volatile uint16_t rxCharTimeoutTickCounter;
	volatile bool usingDriverDirectionControl;
	volatile bool terminalCharBasedReception;
	volatile bool rxOngoing;
	volatile bool rxComplete;
	volatile bool txOngoing;
	volatile bool txComplete;
	volatile bool rxBufferLimitError;
	volatile bool hwError;
	volatile bool rxTransferTimeout;
	volatile bool rxTransferTimerCounting;
	volatile bool rxCharTimeout;
	volatile bool rxCharTimerCounting;
	volatile bool unprocessedState;
	volatile bool ongoingParameterSetting;
	volatile bool blockReceptionOnFinish;
   
	void (*switchDriverToReceiveMode)();
	void (*switchDriverToTransmitMode)();
}SerialCom_t;

typedef struct SerialComInitStr {
    EXT_USART_HANDLE_T *extUsartHandle;
    HAL_USART_HANDLE_T *halUsartHandle;    
    //uint8_t usartIndex;
    uint16_t rxTransferTimeoutTicks;
    uint16_t rxCharTimeoutTicks;
    bool usingDriverDirectionControl;
    void (*switchDriverToReceiveMode)(SerialCom_t *serialCom);
    void (*switchDriverToTransmitMode)(SerialCom_t *serialCom);
}SerialComInitStr_t;

#pragma pack(pop)


void abtSerCommInit(SerialCom_t* serialCom, SerialComInitStr_t* serialComInitStr);

extern void abtSerTimeoutTick(SerialCom_t* serialCom);

extern void abtSerTxDataRegisterEmtpyHandler(SerialCom_t* serialCom);

extern void abtSerTxDataSentHandler(SerialCom_t* serialCom);

//to be implemented if HW errors (parity error, frame error, etc) are to be detected and handled.
extern void abtSerHwErrorHandler(SerialCom_t* serialCom);

//this function should be called by the hwErrorHandler to block the ongoing transfer (Rx or Tx) upon the detecion of and HW error
extern void abtSerLockTransferOnHWError(SerialCom_t* serialCom);

extern void abtSerRxDataReceivedHandler(SerialCom_t* serialCom, uint8_t receivedChar);

extern void abtSerSendChar(SerialCom_t* serialCom, uint8_t charToSend);

extern void abtSerClearTxDataRegisterEmptyFlag(SerialCom_t* serialCom);

extern void abtSerClearHWErrorFlags(SerialCom_t* serialCom);

extern void abtSerWatchdogReset();

/******************************************************************************/
void abtSerTransmitData(SerialCom_t* serialCom, uint8_t *txDataPtr,
		uint16_t txDataLength,
		bool waitOngoingTransmission,
		bool blockUntilTransmissionFinished);

/******************************************************************************/
bool abtSerEvaluateReceptionStatus(SerialCom_t* serialCom);

/******************************************************************************/
void fixedLengthReception(SerialCom_t* serialCom, uint8_t *rxDataPtr,
		uint16_t rxDataLength, uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish);

/******************************************************************************/
void abtSerTerminalCharReception(SerialCom_t* serialCom, uint8_t *rxDataPtr,
		uint8_t terminalChar, uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish);

/*****************************************************************************/
void abtSerMultiCriteriaReception(SerialCom_t* serialCom, uint8_t *rxDataPtr,
		uint8_t *multiTerminalCharPtr, uint16_t *multiLengthRxPtr,
		uint8_t rxTestCriteriaCount, uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish);

#endif /* INC_SERIAL_COM_H_ */
