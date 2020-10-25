/*
 * main_mcu_init.c
 *
 * Created: 01.01.2009
 * Author : Bogdan Tarnauca
 */

#include <string.h>

#include "main_mcu_init.h"
#include "main_defines.h"


void init_data(void){	
	btnLeftState = 0;
	btnRightState = 0;
	
	btnLeftClicks = 0;
	btnRightClicks = 0;
	
	ledPosition = 0x01;
	
	btnLeftPressedEvt = false;
	btnRightPressedEvt = false;	

    softTimerCnt0 = SW_TIMER_RELOAD;

	usart0Handle.usartIndex = 0;
	serialComInitStr.halUsartHandle = NULL;
	serialComInitStr.extUsartHandle = &usart0Handle;
	//serialComInitStr.usartIndex = 0;
	serialComInitStr.rxTransferTimeoutTicks = 0;
	serialComInitStr.rxCharTimeoutTicks = 0;
	serialComInitStr.usingDriverDirectionControl = false;
	serialComInitStr.switchDriverToReceiveMode = NULL;	//switchDriverToReceiveMode;
	serialComInitStr.switchDriverToTransmitMode = NULL; //switchDriverToTransmitMode;
	
	abtSerCommInit(&serialCom0, &serialComInitStr);	
	
	strcpy(newLine,"\n\r");
}

//Init MCU peripherals and any devices connected to the MCU
void init_devices(void)
{
    //watchdog_init();
    
    //stop errant interrupts until set up
    cli(); //disable all interrupts
    
    //port_init();
    
	//Timer0 is used as a tick generator.
    sys_tick_init();
	
//
// NOTE:
// Timer2 is also used by the UART driver in order to generate a 10ms timebase for the timeout mechanism
// The timer is initialized inside uartX_init
//
uart0_init( &serialCom0,
			NULL,
			0x00,
			false,
			0x00,	//UCSR0A
			0x06,	//UCSR0C
			//0x0B,	//UBRR0L = set baud rate lo //57600 la 11.0592 Mhz
			0x33,	//UBRR0L = set baud rate lo //19200 la 16.000 Mhz			
			//0x10,	//UBRR0L = set baud rate lo //57600 la 16.000 Mhz
			//0x07,	//UBRR0L = set baud rate lo //57600 la 7.3728 Mhz
			0x00,	//UBRR0H = set baud rate hi
			0xF8,	//UCSR0B
			0,		
			0
);
    
    sei(); //re-enable interrupts
    //all peripherals are now initialized
    
    //Init peripherals and connected devices
    //
    //init_adc(........);
    //init_lcd(........);
    //init_lm75(........);
}

//Watchdog initialization. Prescale: 16K cycles
void watchdog_init(void)
{
	wdt_reset();	//this prevents a timeout on enabling
	wdt_enable(WDTO_500MS);	//ATENTION: Watchdog functionality has to be enabled also from the fuse bits WDTON - fuse set ti o 0 (zero) to enable it.
}

void port_init(void)
{
	DDRC  = 0xFF;
	PORTC = 0x01;
}

