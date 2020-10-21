/*
 * spi.h
 *
 * Created: 10/19/2020 3:51:34 PM
 *  Author: Mirela
 */ 


#ifndef SPI_H_
#define SPI_H_

#include<avr/io.h>

#define DDR_SPI DDRB
#define DD_MOSI PB2
#define DD_SCK PB1

#define SPI_PORT PORTB
#define pin_MOSI PB2
#define pin_SCK PB1
#define pin_SS PB0
#define pin_MISO PB3

void SPI_returnMaster(void);
void SPI_MasterInit(void);
char SPI_MasterTransfer(char cData);
char SPI_SlaveReceive(void);
void SPI_SStoggle(void);
void SPI_MasterDisable(void);
#endif /* SPI_H_ */