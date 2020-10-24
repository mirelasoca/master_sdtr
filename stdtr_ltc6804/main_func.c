/*
 * main_func.c
 *
 * Created: 20.07.2019 21:25:15
 * Author : Bogdan Tarnauca
 */

#include "main_func.h"

uint16_t counterLED = RELOAD_LED_TICK;

uint16_t counter10ms = RELOAD_10MS_TICK;
uint16_t counter50ms = RELOAD_50MS_TICK;
uint16_t counter100ms = RELOAD_100MS_TICK;
uint16_t counter250ms = RELOAD_250MS_TICK;

bool execute_1ms = false;
bool execute_10ms = false;
bool execute_50ms = false;
bool execute_100ms = false;
bool execute_250ms = false;

bool execute_LED = false;


//------------------------------------------------------------------------------
void pinToggle(volatile uint8_t *port, uint8_t pin){
    *port ^=  1 << pin;
}

void pinSet(volatile uint8_t *port, uint8_t pin){
    *port |=  1 << pin;
}

void pinReset(volatile uint8_t *port, uint8_t pin){
    *port &=  ~(1 << pin);
}

void sys_tick_handler(){

	counterLED--;
	counter10ms--;
	counter50ms--;
	counter100ms--;
	counter250ms--;

	
	if (ELAPSED_COUNTER == counterLED) {
		counterLED = RELOAD_LED_TICK;
		tickLED();
	}
	
	//Only to group 1ms tasks in a function
	tick1ms();
	
	if (ELAPSED_COUNTER == counter10ms) {
		counter10ms = RELOAD_10MS_TICK;
		tick10ms();
	}

	if (ELAPSED_COUNTER == counter50ms) {
		counter50ms = RELOAD_50MS_TICK;
		tick50ms();
	}

	if (ELAPSED_COUNTER == counter100ms) {
		counter100ms = RELOAD_100MS_TICK;
		tick100ms();
	}

	if (ELAPSED_COUNTER == counter250ms) {
		counter250ms = RELOAD_250MS_TICK;
		tick250ms();
	}


    if(softTimerCnt0 == 0){
		if(PORTC != 0){
			PORTC = 0;
		}
		else{
			//if in the meantime the position has been changed reset the current state before
			//the new one is updated. Typical XOR approach for toggling the led cannot be 
			//used as the led position changes asynchronously in relation to the systick
			PORTC = 0;	
			PORTC = ledPosition;			
		}
		
        softTimerCnt0 =  SW_TIMER_RELOAD;
    }
}

void tick1ms() {
	//Do critical but very short tasks here
	//...
	
	//Longer tasks should be executed in the main loop by polling this flag
	execute_1ms = true;
}

void tick10ms() {
	//Do critical but very short tasks here
	//...
		
	//Longer tasks should be executed in the main loop by polling this flag
	execute_10ms = true;
}

void tick50ms() {
	//Do critical but very short tasks here
	//...
	
	//Longer tasks should be executed in the main loop by polling this flag
	execute_50ms = true;
}

void tick100ms() {
	//Do critical but very short tasks here
	//...
	
	//Longer tasks should be executed in the main loop by polling this flag
	execute_100ms = true;
}

void tick250ms() {
	//Do critical but very short tasks here
	//...
	
	//Longer tasks should be executed in the main loop by polling this flag
	execute_250ms = true;	
}

void tickLED() {
	//Short task execute it here directly
	
	if(PORTC != 0){
		PORTC = 0;
	}
	else{
		//if in the meantime the position has been changed reset the current state before
		//the new one is updated. Typical XOR approach for toggling the led cannot be
		//used as the led position changes asynchronously in relation to the systick
		PORTC = 0;
		PORTC = ledPosition;
	}
}

void inLoop1msTask(){

}

void inLoop10msTask(){
	
}

void inLoop50msTask(){
	
}

void inLoop100msTask(){
	
}

void inLoop250msTask(){
	
}

//------------------------------------------------------------------------------
void processConsoleComm(SerialCom_t *serialCom){
	if (!serialCom->rxOngoing && !serialCom->txOngoing && !serialCom->unprocessedState) {
		//Listen for some input provideded over the console
		abtSerTerminalCharReception(serialCom, (uint8_t*)rxMessage, RX_TERM, MAX_RX_MESSAGE_LENGTH, true, true);
	}

	abtSerEvaluateReceptionStatus(serialCom);

	if (serialCom->unprocessedState) {
		//An event such as TxOK, RxOK, Timeout or BufferLimit ocurred
		if (serialCom->hwError) {
			serialCom->hwError = false;
			serialCom->rxOngoing = false;
			serialCom->rxComplete = false;
			serialCom->unprocessedState = false;
			serialCom->rxCounter = 0;
			sprintf(txMessage, "\n\rHW ERROR!\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}

		if (serialCom->rxBufferLimitError) {
			serialCom->rxBufferLimitError = false;
			serialCom->rxOngoing = false;
			serialCom->rxComplete = false;
			serialCom->unprocessedState = false;
			serialCom->rxCounter = 0;
			sprintf(txMessage, "\n\rRx buffer full!\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}

		if (serialCom->rxTransferTimeout) {
			serialCom->rxTransferTimeout = false;
			serialCom->rxOngoing = false;
			serialCom->rxComplete = false;
			serialCom->unprocessedState = false;
			sprintf(txMessage, "\n\rMessage timeout!\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}

		if (serialCom->rxCharTimeout) {
			serialCom->rxCharTimeout = false;
			serialCom->rxOngoing = false;
			serialCom->rxComplete = false;
			serialCom->unprocessedState = false;
			sprintf(txMessage, "\n\rChar timeout!\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}

		if (serialCom->rxComplete) {
			memcpy(txMessage, rxMessage, serialCom->rxLength);
			memcpy(txMessage + serialCom->rxLength, "\n\r>>", 4);
			abtSerTransmitData(serialCom, (uint8_t*)txMessage, 4 + serialCom->rxLength, false, false);
		}
		serialCom->unprocessedState = false;
	}
}
