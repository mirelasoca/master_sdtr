/*
* avr_sys_tick.c
*
* Created: 08.05.2020
* Author : Bogdan Tarnauca
*/

#include "avr_sys_tick.h"

void sys_tick_init(){
	sys_tick_cnt = 0;
	timer0_init();
}

void sys_tick(){
	sys_tick_cnt++;
	sys_tick_handler();
}

//Configure timer0 as a 1ms tick generator
void timer0_init(void)
{
#if defined __ATMEGA2560
	TCCR0A = 0x00; //stop
	TCCR0B = 0x00; //stop
	
	/*
	Finterrpt: expected tick frequency
	Finterrpt = Fclk/(N*(1+OCRx))
	N = prescaler value
	for CS22,CS21,CS10 = 100 => N = 64
	OCRx = Fclk/(Fint*N) - 1
	OCRx = Fclk*Tint/N - 1
	*/

	TCNT0 = 0x00; 
  
	#if (F_CPU == 11059200UL)
	    OCR0A  = 0xAB;	//for Fclck = 11.059.264 Hz
	#elif (F_CPU == 16000000UL)
	    OCR0A  = 0xF9;	//for Fclck = 16.000.000 Hz
	#elif (F_CPU == 7372800UL)
	    OCR0A  = 0x72;	//for Fclck = 7.372.800 Hz        
	#else
		#warning "!!!! F_CPU was not defined or is not supported by the sys_tick driver. Note that a different real CPU frequency will affect the timebase value. Timer settings set for a default F_CPU 11059200UL "
		OCR0A  = 0xAB;	//set as for Fclck = 11.059.264 Hz even if a wrong value has been set
	#endif
	
	
	TCCR0B |= 1<<CS02; //CS22,CS21,CS10 = 100 => 64 Fclk divider
	TCCR0A |= 1<<WGM01; //WGM22,WGM21,WGM20 = 010 start as CTC. Interrupt when TCNT2 reaches OCR2A		
	TIMSK0 |= 1<<OCIE0A; //
	
#elif defined __ATMEGA128
	TCCR0 = 0x00; //stop timer
	TCNT0 = 0x00; 
	
	#if (F_CPU == 11059200UL)
	    OCR0  = 0xAB;	//for Fclck = 11.059.264 Hz
	#elif (F_CPU == 16000000UL)
	    OCR0  = 0xF9;	//for Fclck = 16.000.000 Hz
	#elif (F_CPU == 7372800UL)
	    OCR0  = 0x72;	//for Fclck = 7.372.800 Hz
	#else
	#warning "!!!! F_CPU was not defined or is not supported by the sys_tick driver. Note that a different real CPU frequency will affect the timebase value. Timer settings set for a default F_CPU 11059200UL "
	    OCR0  = 0xAB;	//set as for Fclck = 11.059.264 Hz even if a wrong value has been set
	#endif
	
	TCCR0 |= 1<<WGM01 | 1<<CS02;
		//CS02,CS01,CS00 = 100 => 64 Fclk divider
		//WGM01 = 10 start as CTC. Interrupt when TCNT2 reaches OCR2A
	
	TIMSK |= 1<<OCIE0;	//Enable Timer2 CTC interrupt
	
#elif defined __ATMEGA328
	TCCR0A = 0x00; //stop
	TCCR0B = 0x00; //stop
	TCNT0 = 0x00;

	#if (F_CPU == 11059200UL)
	    OCR0A  = 0xAB;	//for Fclck = 11.059.264 Hz
	#elif (F_CPU == 16000000UL)
	    OCR0A  = 0xF9;	//for Fclck = 16.000.000 Hz
	#elif (F_CPU == 7372800UL)
	    OCR0A  = 0x72;	//for Fclck = 7.372.800 Hz
	#else
	#warning "!!!! F_CPU was not defined or is not supported by the sys_tick driver. Note that a different real CPU frequency will affect the timebase value. Timer settings set for a default F_CPU 11059200UL "
	    OCR0A  = 0xAB;	//set as for Fclck = 11.059.264 Hz even if a wrong value has been set
	#endif

	TCCR0B |= 1<<CS02; //CS22,CS21,CS10 = 100 => 64 Fclk divider
	TCCR0A |= 1<<WGM01; //WGM22,WGM21,WGM20 = 010 start as CTC. Interrupt when TCNT2 reaches OCR2A
	TIMSK0 |= 1<<OCIE0A; //
	
#endif
}

#ifdef __ICCAVR
#pragma interrupt_handler timer0_compa_isr:TIMER0_COMPA_ISR_VECTOR_NO
void timer0_compa_isr(void)
#else
#if (defined __ATMEGA2560)	
ISR(TIMER0_COMPA_vect)
#elif (defined __ATMEGA128)	
ISR(TIMER0_COMP_vect)
#elif (defined __ATMEGA328)	
ISR(TIMER0_COMPA_vect)
#else
#warning Check interrupt vector name for your device
ISR(TIMER0_COMP_vect)
#endif
#endif
{
	TCNT0 = 0x00;
	sys_tick();
}