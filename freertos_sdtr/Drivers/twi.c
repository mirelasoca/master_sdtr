/*
 * twi.c
 *
 * Created: 10/25/2020 10:42:29 AM
 *  Author: Mirela
 */ 
#include "twi.h"
#include <util/twi.h>
#include "driver_lcd.h"
void twi_init(void)
{
	DDRD &= ~((1<<scl_pin)||(1<<sca_pin));
	PORTD |= (1<<scl_pin)||(1<<sca_pin); // set pull up
	
	TWBR = 18;// set frequency to 100khz
	TWAR = (10 << 1); // slave address
	
}

void printmyballs(const uint8_t *data, size_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		char t[3];
		snprintf(t, 3, "%d", data[i]);
		lcd_write_text(t, 2 * i, LCD_LINE_COUNT_1);
	}
}

void twi_slaveTransmit( uint8_t* data, size_t len)
{
	int i = 0;

	TWCR = (1<<TWEN)|(1<<TWEA);
	lcd_write_text("TTWI", 12, LCD_LINE_COUNT_2);
	int progress = 1;
	
	//printmyballs(data, len);
	
	char txt[7];

	while (progress)
	{
		_delay_ms(500);
		while (!(TWCR & (1<<TWINT)));
		unsigned int status = TWSR;
		switch (status)
		{
			case TW_ST_SLA_ACK:
				lcd_write_text("SLA", 12, LCD_LINE_COUNT_2);
				if (i != 0)
					goto protocol_error;
				
				TWDR = data[i];
				break;
			case TW_ST_DATA_ACK:
				snprintf(txt, 7, "ACK%1d", i);
				lcd_write_text(txt, 12, LCD_LINE_COUNT_2);
				if (i >= len -1)
				{
					progress = 0;
					break;
				}
				TWDR = data[++i];
				break;
			case TW_ST_DATA_NACK:
				lcd_write_text("NAK ", 12, LCD_LINE_COUNT_2);
				if (i >= len - 1)
					progress = 0;
				// TWDR = data[i];
				break;
			case TW_ST_LAST_DATA:
				lcd_write_text("END ", 12, LCD_LINE_COUNT_2);
				progress = 0;
				break;
			default:
				lcd_write_text("ERR ", 12, LCD_LINE_COUNT_2);
				progress = 0;
				break;
		}
		
		TWCR |= ((i == len-2) ? (1 << TWEA) : 0) | (1 << TWINT);
	}
	TWCR |= (1<<TWSTO);
	
	//printmyballs(data, len);
	return;
	
protocol_error:
	lcd_write_text("DIE", 12, LCD_LINE_COUNT_2);
}


void twi_slaveReceive(uint8_t* rdata, size_t lenr)
{
	int i = 0;

	TWCR = (1<<TWEN)|(1<<TWEA);
	lcd_write_text("RTWI", 12, LCD_LINE_COUNT_2);
	int progress = 1;
	
	//printmyballs(data, len);
	
	char txt[7];

	while (progress)
	{
		_delay_ms(500);
		while (!(TWCR & (1<<TWINT)));
		unsigned int status = TWSR;
		switch (status)
		{
			case TW_SR_SLA_ACK:
			lcd_write_text("SLA", 12, LCD_LINE_COUNT_2);
			if (i != 0)
				goto protocol_error;
			
			rdata[i]= TWDR;
			break;
			case TW_SR_DATA_ACK:
			snprintf(txt, 7, "ACK%1d", i);
			lcd_write_text(txt, 12, LCD_LINE_COUNT_2);
			if (i >= lenr -1)
			{
				progress = 0;
				break;
			}
			rdata[++i] = TWDR;
			snprintf(txt, 7, "%d", rdata[i]);
			//lcd_write_text(txt, 0, LCD_LINE_COUNT_1);
			//sprintf(txt,"data %u is %u",i,data[i]);
			//lcd_write_text(txt,0, LCD_LINE_COUNT_1);
			break;
			case TW_SR_DATA_NACK:
			lcd_write_text("NAK ", 12, LCD_LINE_COUNT_2);
			if (i >= lenr - 1)
			progress = 0;
			// TWDR = data[i];
			break;
			case TW_SR_STOP:
			lcd_write_text("END ", 12, LCD_LINE_COUNT_2);
			progress = 0;
			break;
			default:
			lcd_write_text("ERR ", 12, LCD_LINE_COUNT_2);
			progress = 0;
			break;
		}
		
		TWCR |= ((i == lenr-1) ? (1 << TWEA) : 0) | (1 << TWINT);
	}
	TWCR |= (1<<TWSTO);
	
	//printmyballs(data, len);
	return;
	
	protocol_error:
	lcd_write_text("DIE", 12, LCD_LINE_COUNT_2);
}