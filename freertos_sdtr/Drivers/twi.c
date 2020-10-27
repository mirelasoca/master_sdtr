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
	//TWAR = (10 << 1); // slave address
	
}
/*
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
*/
int twi_masterTransmit( uint8_t* data, size_t len)
{
	int i = 0;
	int j = 0;
	lcd_write_text("TTWI", 12, LCD_LINE_COUNT_2);
	int progress = 1;
	TWCR = (1<<TWEN)|(1<<TWSTA)|(1<<TWINT);
	
	
	
	//printmyballs(data, len);
	
	char txt[7];

	while (progress)
	{
		//_delay_ms(200);
		while (!(TWCR & (1<<TWINT)));
		
		volatile unsigned int status = TWSR;
		switch (status)
		{
			case TW_START:
			//TWCR &= ~(1<<TWSTA);
			TWDR = 24<<1; // address
			lcd_write_text("START", 12, LCD_LINE_COUNT_2);
			break;
			case TW_MT_SLA_ACK:
			lcd_write_text("SLA", 12, LCD_LINE_COUNT_2);
			if (i != 0)
			goto protocol_error;
			
			TWDR = data[i];
			break;
			case TW_MT_SLA_NACK:
			lcd_write_text("NSLA", 12, LCD_LINE_COUNT_2);
			j= -1;
			TWCR= (1<<TWSTA)|(1 << TWINT)|(1<<TWEN); //restart transm
			return 1;
			break;
			case TW_MT_DATA_ACK:
			snprintf(txt, 7, "ACK%1d", i);
			lcd_write_text(txt, 12, LCD_LINE_COUNT_2);
			if (i >= len-1)
			{
				progress = 0;
				i++;
				break;
			}
			TWDR = data[++i];
			break;
			case TW_MT_DATA_NACK:
			lcd_write_text("NAK ", 12, LCD_LINE_COUNT_2);
			if (i >= len-1)
			progress = 0;
			j= -1;
			TWCR= (1<<TWSTA)|(1 << TWINT)|(1<<TWEN); //restart transm
			return 1;
			break;
			default:
			lcd_write_text("ERR ", 12, LCD_LINE_COUNT_2);
			progress = 0;
			i= len;
			break;
		}
		
		TWCR =(i>=len) ?((1 << TWINT)|(1<<TWEN)|(1<<TWSTO)):((1 << TWINT)|(1<<TWEN));
		
		
	}

	
	//printmyballs(data, len);
	return 0;
	
	protocol_error:
	lcd_write_text("DIE", 12, LCD_LINE_COUNT_2);
	return 1;
}

/*

int twi_masterReceiveMirela(uint8_t* rdata, size_t lenr)
{
	int i = 0;
	static int count = 0;
	int j=0;
	TWCR = (1<<TWEN)|(1<<TWSTA)|(1<<TWINT);
	lcd_write_text("RTWI", 12, LCD_LINE_COUNT_2);
	int progress = 1;
	
	//printmyballs(data, len);
	
	char txt[7];

	while (progress)
	{
		count++;
		_delay_ms(500);
		j=0;
		while (!(TWCR & (1<<TWINT)));
		volatile unsigned int status = TWSR;
		switch (status)
		{
			case TW_START:
			lcd_write_text("START", 12, LCD_LINE_COUNT_2);
			TWDR = (24<<1)+1;
			break;
			case TW_MR_SLA_NACK:
			lcd_write_text("NSLA", 12, LCD_LINE_COUNT_2);
			j= -1;
			i=0;
			TWCR= (1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
			return 1;
			//TWCR= (1<<TWSTA)|(1 << TWINT)|(1<<TWEN); //restart transm
			break;
			case TW_MR_SLA_ACK:
			//TWDR = (24<<1)+1; // retransmit address
			lcd_write_text("SLA", 12, LCD_LINE_COUNT_2);
			if (i != 0)
				goto protocol_error;
			
			//rdata[i]= TWDR;
			break;
			case TW_MR_DATA_ACK:
			snprintf(txt, 7, "RACK%1d", i);
			lcd_write_text(txt, 10, LCD_LINE_COUNT_2);
			if (i >= lenr )
			{
				TWCR =(1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
				return 0;
				//i++;
				//break;
			}
			rdata[i++] = TWDR;
			//snprintf(txt, 7, "%d", rdata[i]);
			//lcd_write_text(txt, 0, LCD_LINE_COUNT_1);
			//sprintf(txt,"data %u is %u",i,data[i]);
			//lcd_write_text(txt,0, LCD_LINE_COUNT_1);
			break;
			case TW_MR_DATA_NACK:
			lcd_write_text("RNAK ", 12, LCD_LINE_COUNT_2);
			if (i >= lenr)
				{
					TWCR =(1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
					return 0;
					//i++;
					
				}
			j= -1;
			i=0;
			TWCR= (1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
			return 1;
			//TWCR= (1<<TWSTA)|(1 << TWINT)|(1<<TWEN); //restart transm
			break;
			default:
			lcd_write_text("RERR ", 12, LCD_LINE_COUNT_2);
			TWCR= (1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
			return 1;
			i= lenr+1;
			break;
		}
		
		TWCR =((1 << TWINT)|(1<<TWEN)|(1<<TWEA));
			
	}
	
	
	printmyballs(rdata, lenr);
	return 0;
	
	protocol_error:
	{
		lcd_write_text("DIE", 12, LCD_LINE_COUNT_2);
		TWCR =(1 << TWINT)|(1<<TWEN)|(1<<TWSTO);
	}
	
	return 1;
	
}
*/

int twi_masterReceive(uint8_t* rdata, size_t lenr)
{
	int i = 0;
	
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	lcd_write_text("RTWI", 12, LCD_LINE_COUNT_2);
	
	// printmyballs(rdata, lenr);
	
	char txt[7];

	while (1)
	{
		while (!(TWCR & (1<<TWINT)));
		volatile unsigned int status = TWSR;
		switch (status)
		{
			case TW_START:
			case TW_REP_START:
			if (i > 0 && status != TW_REP_START)
			{
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				return i + 20;
			}

			lcd_write_text("START", 12, LCD_LINE_COUNT_2);
			TWDR = (24 << 1) + 1;
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			break;
			case TW_MR_SLA_NACK:
			lcd_write_text("NSLA", 12, LCD_LINE_COUNT_2);
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			return 5;
			case TW_MR_SLA_ACK:
			lcd_write_text("SLA ", 12, LCD_LINE_COUNT_2);
			if (i != 0)
			{
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				return i + 10;
			}
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			break;
			case TW_MR_DATA_ACK:
			snprintf(txt, 7, "ACK%1d", i);
			lcd_write_text(txt, 10, LCD_LINE_COUNT_2);
			if (i > lenr)
			{
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				return i + 30;
			}

			rdata[i++] = TWDR;
			TWCR = (1 << TWINT) | (1 << TWEN) | (i == lenr ? 0 : (1 << TWEA));
			break;
			case TW_MR_DATA_NACK:
			lcd_write_text("RNAK", 12, LCD_LINE_COUNT_2);
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			if (i >= lenr)
			return 0;
			return 2;
			default:
			lcd_write_text("RERR ", 12, LCD_LINE_COUNT_2);
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			return 3;
		}
	}
	
	//printmyballs(data, len);
	return 0;
}