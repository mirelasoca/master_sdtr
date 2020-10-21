/*
 * spi.c
 *
 * Created: 10/19/2020 3:54:45 PM
 *  Author: Mirela
 */ 
#include "spi.h"
void SPI_SStoggle(void)
{
	SPI_PORT ^= (1<<pin_SS);
}
void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<pin_SS);
	//DDR_SPI &= ~(1<<pin_MISO);
	//SPI_PORT |= (1<<pin_MISO); //activate pull up
	/* Enable SPI, Master, set clock rate fck/128 */
	SPCR = (1<<SPR0)|(1<<SPR1)|(1<<MSTR)|(1<<SPE)|(1<<CPOL)|(1<CPHA);
	
}
void SPI_MasterDisable(void)
{
	SPCR &= ~ (1<<SPE);
}
char SPI_MasterTransfer(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}
char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return data register */
	return SPDR;
}
void SPI_returnMaster(void)
{
	SPCR |= (1<<MSTR);
}