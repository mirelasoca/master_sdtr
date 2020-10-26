/*
 * twi.h
 *
 * Created: 10/25/2020 10:43:00 AM
 *  Author: Mirela
 */ 


#ifndef TWI_H_
#define TWI_H_
#include <avr/io.h>
#define TWI_PORT PORTD
#define TWI_DDR DDRD
#define sca_pin  PD1
#define scl_pin PD0

#include <stddef.h>
#include <stdio.h>

void twi_init(void);

void twi_slaveTransmit( uint8_t * data, size_t len);
void twi_slaveReceive(uint8_t* rdata, size_t lenr);

void twi_masterTransmit( uint8_t * data, size_t len);
void twi_masterReceive(uint8_t* rdata, size_t lenr);


#endif /* TWI_H_ */