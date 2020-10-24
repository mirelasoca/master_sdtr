/*
* avr_usart.h
*
* Created: 01.01.2009
* Author : Bogdan Tarnauca
*/

#include "serial_com_config.h"
#include "avr_serial_com_hal.h"

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
                uint16_t  nValoareTimeoutChar)
{
    serialComPtr0 = serialCom;

	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = paramUCSR0A;
	UCSR0C = paramUCSR0C;
	UBRR0L = paramUBRR0L; //set baud rate lo
	UBRR0H = paramUBRR0H; //set baud rate hi
	UCSR0B = paramUCSR0B;
	
    abtSerClearTxDataRegisterEmptyFlag(serialCom);
	
	//Daca pointerul catre portul care selecteaza directia de transmisie
	//pe RS485 este NULL se considera ca transmisia se face prin RS232 si ca
	//atare nu mai este necesara selectarea directiei driverului
	if(PORT485_DIR != NULL)
	{
    	serialCom->extUsartHandle->driverDirectionControlPort = PORT485_DIR;
    	*(serialCom->extUsartHandle->driverDirectionControlPort-1) = *(serialCom->extUsartHandle->driverDirectionControlPort-1) | Port485DirMask;
    	//Setez modul de functionare (IESIRE) pentru linia portului care stabileste
    	//directia pe RS485. Se modifica DDRn care se afla la adresa imediat inferioara
    	//registrului PORTn
    	serialCom->extUsartHandle->driverDirectionControlMask = Port485DirMask;
    	serialCom->extUsartHandle->invertedDriverDirectionControl = bRS485Inversat;
	}
}

/******************************************************************************/

#ifdef __ICCAVR
#pragma interrupt_handler uart0_udre_isr: uart0_UDRE_ISR_VECTOR_NO
void uart0_udre_isr(void)
#else
	#ifdef __ATMEGA328
	ISR(USART_UDRE_vect)
	#else
	ISR(USART0_UDRE_vect)
	#endif
#endif
{
    abtSerTxDataRegisterEmtpyHandler(serialComPtr0);
}

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart0_tx_isr: uart0_TX_ISR_VECTOR_NO
void uart0_tx_isr(void)
#else
	#ifdef __ATMEGA328
	ISR(USART_TX_vect)
	#else
	ISR(USART0_TX_vect)
	#endif
#endif
{
    abtSerTxDataSentHandler(serialComPtr0);
}


/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart0_rx_isr: uart0_RX_ISR_VECTOR_NO
void uart0_rx_isr(void)
#else
	#ifdef __ATMEGA328
	ISR(USART_RX_vect)
	#else
	ISR(USART0_RX_vect)
	#endif
#endif
{
    abtSerRxDataReceivedHandler(serialComPtr0, UDR0);
}

#endif


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
                uint16_t  nValoareTimeoutChar)
{
    serialComPtr1 = serialCom;
    
	UCSR1B = 0x00; //disable while setting baud rate
	UCSR1A = paramUCSR0A;
	UCSR1C = paramUCSR0C;
	UBRR1L = paramUBRR0L; //set baud rate lo
	UBRR1H = paramUBRR0H; //set baud rate hi
	UCSR1B = paramUCSR0B;
	
	abtSerClearTxDataRegisterEmptyFlag(serialCom);
	
	//Daca pointerul catre portul care selecteaza directia de transmisie
	//pe RS485 este NULL se considera ca transmisia se face prin RS232 si ca
	//atare nu mai este necesara selectarea directiei driverului
	if(PORT485_DIR != NULL)
	{
        serialCom->extUsartHandle->driverDirectionControlPort = PORT485_DIR;
		*(serialCom->extUsartHandle->driverDirectionControlPort-1) = *(serialCom->extUsartHandle->driverDirectionControlPort-1) | Port485DirMask;
		//Setez modul de functionare (IESIRE) pentru linia portului care stabileste
		//directia pe RS485. Se modifica DDRn care se afla la adresa imediat inferioara
		//registrului PORTn
        serialCom->extUsartHandle->driverDirectionControlMask = Port485DirMask;
		serialCom->extUsartHandle->invertedDriverDirectionControl = bRS485Inversat;
	}
}

/******************************************************************************/

#ifdef __ICCAVR
#pragma interrupt_handler uart1_udre_isr: uart1_UDRE_ISR_VECTOR_NO
void uart1_udre_isr(void)
#else
ISR(USART1_UDRE_vect)
#endif
{
    abtSerTxDataRegisterEmtpyHandler(serialComPtr1);
}

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart1_tx_isr: uart1_TX_ISR_VECTOR_NO
void uart1_tx_isr(void)
#else
ISR(USART1_TX_vect)
#endif
{
    abtSerTxDataSentHandler(serialComPtr1);
}


/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart1_rx_isr: uart1_RX_ISR_VECTOR_NO
void uart1_rx_isr(void)
#else
ISR(USART1_RX_vect)
#endif
{
    abtSerRxDataReceivedHandler(serialComPtr1, UDR1);
}

#endif


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
                uint16_t  nValoareTimeoutChar)
{
    serialComPtr2 = serialCom;
    
	UCSR2B = 0x00; //disable while setting baud rate
	UCSR2A = paramUCSR0A;
	UCSR2C = paramUCSR0C;
	UBRR2L = paramUBRR0L; //set baud rate lo
	UBRR2H = paramUBRR0H; //set baud rate hi
	UCSR2B = paramUCSR0B;
	
	abtSerClearTxDataRegisterEmptyFlag(serialCom);
	
	//Daca pointerul catre portul care selecteaza directia de transmisie
	//pe RS485 este NULL se considera ca transmisia se face prin RS232 si ca
	//atare nu mai este necesara selectarea directiei driverului
	if(PORT485_DIR != null)
	{
    	serialCom->extUsartHandle->driverDirectionControlPort = PORT485_DIR;
    	*(serialCom->extUsartHandle->driverDirectionControlPort-1) = *(serialCom->extUsartHandle->driverDirectionControlPort-1) | Port485DirMask;
    	//Setez modul de functionare (IESIRE) pentru linia portului care stabileste
    	//directia pe RS485. Se modifica DDRn care se afla la adresa imediat inferioara
    	//registrului PORTn
    	serialCom->extUsartHandle->driverDirectionControlMask = Port485DirMask;
    	serialCom->extUsartHandle->invertedDriverDirectionControl = bRS485Inversat;
	}
}

/******************************************************************************/

#ifdef __ICCAVR
#pragma interrupt_handler uart2_udre_isr: uart2_UDRE_ISR_VECTOR_NO
void uart2_udre_isr(void)
#else
ISR(USART2_UDRE_vect)
#endif
{
    abtSerTxDataRegisterEmtpyHandler(serialComPtr2);
}

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart2_tx_isr: uart2_TX_ISR_VECTOR_NO
void uart2_tx_isr(void)
#else
ISR(USART2_TX_vect)
#endif
{
	abtSerTxDataSentHandler(serialComPtr2);
}


/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart2_rx_isr: uart2_RX_ISR_VECTOR_NO
void uart2_rx_isr(void)
#else
ISR(USART2_RX_vect)
#endif
{
    abtSerRxDataReceivedHandler(serialComPtr2, UDR2);
}

#endif


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
                uint16_t  nValoareTimeoutChar)
{
    serialComPtr3 = serialCom;
    
	UCSR3B = 0x00; //disable while setting baud rate
	UCSR3A = paramUCSR0A;
	UCSR3C = paramUCSR0C;
	UBRR3L = paramUBRR0L; //set baud rate lo
	UBRR3H = paramUBRR0H; //set baud rate hi
	UCSR3B = paramUCSR0B;
	
	abtSerClearTxDataRegisterEmptyFlag(serialCom);
	
	//Daca pointerul catre portul care selecteaza directia de transmisie
	//pe RS485 este NULL se considera ca transmisia se face prin RS232 si ca
	//atare nu mai este necesara selectarea directiei driverului
	if(PORT485_DIR != null)
	{
    	serialCom->extUsartHandle->driverDirectionControlPort = PORT485_DIR;
    	*(serialCom->extUsartHandle->driverDirectionControlPort-1) = *(serialCom->extUsartHandle->driverDirectionControlPort-1) | Port485DirMask;
    	//Setez modul de functionare (IESIRE) pentru linia portului care stabileste
    	//directia pe RS485. Se modifica DDRn care se afla la adresa imediat inferioara
    	//registrului PORTn
    	serialCom->extUsartHandle->driverDirectionControlMask = Port485DirMask;
    	serialCom->extUsartHandle->invertedDriverDirectionControl = bRS485Inversat;
	}
}

/******************************************************************************/

#ifdef __ICCAVR
#pragma interrupt_handler uart3_udre_isr: uart3_UDRE_ISR_VECTOR_NO
void uart3_udre_isr(void)
#else
ISR(USART3_UDRE_vect)
#endif
{
    abtSerTxDataRegisterEmtpyHandler(serialComPtr3);
}

/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart3_tx_isr: uart3_TX_ISR_VECTOR_NO
void uart3_tx_isr(void)
#else
ISR(USART3_TX_vect)
#endif
{
    abtSerTxDataSentHandler(serialComPtr3);
}


/******************************************************************************/
#ifdef __ICCAVR
#pragma interrupt_handler uart3_rx_isr: uart3_RX_ISR_VECTOR_NO
void uart3_rx_isr(void)
#else
ISR(USART3_RX_vect)
#endif
{
    abtSerRxDataReceivedHandler(serialComPtr3, UDR3);
}

#endif


/**************************************************/
//Implementation of the serial_com.h driver functions
// Define this externally as these are platform dependent
void abtSerSendChar(SerialCom_t* serialCom, uint8_t charToSend)
{
	#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)
    if(serialCom->extUsartHandle->usartIndex == 0){
        UDR0 = charToSend;
        UCSR0B = UCSR0B | 1<<UDRIE0;
    }
	#endif
	
	#if (defined __ATMEGA128) || (defined __ATMEGA2560)
    if(serialCom->extUsartHandle->usartIndex == 1){
        UDR1 = charToSend;
        UCSR1B = UCSR1B | 1<<UDRIE1;
    }
	#endif
	
	#if (defined __ATMEGA2560)
    if(serialCom->extUsartHandle->usartIndex == 2){
        UDR2 = charToSend;
        UCSR2B = UCSR2B | 1<<UDRIE2;
    }
	#endif


	#if (defined __ATMEGA2560)
    if(serialCom->extUsartHandle->usartIndex == 3){
        UDR3 = charToSend;
        UCSR3B = UCSR3B | 1<<UDRIE3;
    }
	#endif
}

void abtSerClearTxDataRegisterEmptyFlag(SerialCom_t* serialCom){
	#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)
	if(serialCom->extUsartHandle->usartIndex == 0){
        UCSR0B ^= 1<<UDRIE0; //Dezactivez bitul UDRIE	
        }
	#endif
	
	#if (defined __ATMEGA128) || (defined __ATMEGA2560)
	if(serialCom->extUsartHandle->usartIndex == 1){
        UCSR1B ^= 1<<UDRIE1; //Dezactivez bitul UDRIe	
        }
	#endif
	
	#if (defined __ATMEGA2560)
	if(serialCom->extUsartHandle->usartIndex == 2){
        UCSR2B ^= 1<<UDRIE2; //Dezactivez bitul UDRIE
        }
	#endif


	#if (defined __ATMEGA2560)
	if(serialCom->extUsartHandle->usartIndex == 3){
        UCSR3B ^= 1<<UDRIE3; //Dezactivez bitul UDRIE
        }
	#endif    
}

void abtSerWatchdogReset(){
    //TODO: this is platform dependent. Provide a generic mechanism.
    wdt_reset();
}

void switchDriverToReceiveMode(SerialCom_t *serialCom){
    
	#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 0){
            switchPinReceiveMode(serialCom);
        }         
	#endif
	
	#if (defined __ATMEGA128) || (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 1){
            switchPinReceiveMode(serialCom);
        }            
	#endif
	
	#if (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 2){
            switchPinReceiveMode(serialCom);
        }            
	#endif

	#if (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 3){
            switchPinReceiveMode(serialCom);
        }            
	#endif    
    
}
void switchDriverToTransmitMode(SerialCom_t *serialCom){
	#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 0){
            switchPinTransmitMode(serialCom);
        }        
	#endif
	
	#if (defined __ATMEGA128) || (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 1){
            switchPinTransmitMode(serialCom);
        }
	#endif
	
	#if (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 2){
            switchPinTransmitMode(serialCom);
        }
	#endif

	#if (defined __ATMEGA2560)
        if(serialCom->extUsartHandle->usartIndex == 3){
            switchPinTransmitMode(serialCom);
        }	
    #endif    
}

void abtSerClearHWErrorFlags(SerialCom_t* serialCom){
    //TODO: Check AVR error flags and implement their reset
	#if (defined __ATMEGA8) || (defined __ATMEGA328) || (defined __ATMEGA128) || (defined __ATMEGA2560)

	#endif
	
	#if (defined __ATMEGA128) || (defined __ATMEGA2560)

	#endif
	
	#if (defined __ATMEGA2560)

	#endif

	#if (defined __ATMEGA2560)

	#endif
}


void switchPinTransmitMode(SerialCom_t *serialCom){
    uint8_t nAux;
    
    if(serialCom->usingDriverDirectionControl)
    {
        //salvez valoarea portului prin care stabileste directia, resetand insa bitul care este
        //alocat semnalului de directie. Fac asta pentru a permite operatiile logice de mai jos
        nAux = *serialCom->extUsartHandle->driverDirectionControlPort & ~(serialCom->extUsartHandle->driverDirectionControlMask);

        //Setez in 1 pinul care seteaza directia sau il las in zero (vezi comanda de mai sus)
        //daca semnalul de comanda trebuie inversat).
        *serialCom->extUsartHandle->driverDirectionControlPort = nAux | (serialCom->extUsartHandle->invertedDriverDirectionControl?0x00:serialCom->extUsartHandle->driverDirectionControlMask);
    }   
}

void switchPinReceiveMode(SerialCom_t *serialCom){
    uint8_t nAux;
    
	if(serialCom->usingDriverDirectionControl)
	{
    	//salvez valoarea portului prin care stabileste directia, resetand insa bitul care este
    	//alocat semnalului de directie. Fac asta pentru a permite operatiile logice de mai jos
    	nAux = *serialCom->extUsartHandle->driverDirectionControlPort & ~(serialCom->extUsartHandle->driverDirectionControlMask);
    	//Setez in 1 pinul care seteaza directia sau il las in zero (vezi comanda de mai sus)
    	//daca semnalul de comanda trebuie inversat).
    	*serialCom->extUsartHandle->driverDirectionControlPort = nAux | (serialCom->extUsartHandle->invertedDriverDirectionControl?serialCom->extUsartHandle->driverDirectionControlMask:0x00);
	}
}            