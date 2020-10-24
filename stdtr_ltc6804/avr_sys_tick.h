/*
* avr_sys_tick.h
*
* Created: 08.05.2020
* Author : Bogdan Tarnauca
*/

#ifndef __AVR_SYS_TICK_H__
#define __AVR_SYS_TICK_H__

#include "main_config.h"

uint16_t sys_tick_cnt;

void sys_tick_init(void);

void sys_tick();

extern __attribute__((weak))  void sys_tick_handler();

void timer0_init(void);


#ifdef __ICCAVR
#pragma interrupt_handler timer0_compa_isr:TIMER0_COMPA_ISR_VECTOR_NO
void timer0_compa_isr(void);
#else
#if (defined __ATMEGA2560)
ISR(TIMER0_COMPA_vect);
#elif (defined __ATMEGA128)
ISR(TIMER0_COMP_vect);
#elif (defined __ATMEGA328)
ISR(TIMER0_COMPA_vect);
#else
#warning "Check interrupt vector name for your device"
ISR(TIMER0_COMP_vect);
#endif
#endif

#endif	//__AVR_SYS_TICK_H__
