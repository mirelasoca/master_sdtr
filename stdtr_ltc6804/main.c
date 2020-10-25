/*
 * stdtr_ltc6804.c
 *
 * Created: 10/19/2020 8:21:41 PM
 * Author : Mirela
 */ 
#include "ltc6804.h"
#include <avr/io.h>

#include "driver_lcd.h"
#include <util/delay.h>
#include <stdio.h>
int main(void)
{
    /* Replace with your application code */
	//SPI_MasterInit();
	uint8_t RDCVA[4] = {CMD0_RDCVA, CMD1_RDCVA, PEC0_RDCVA, PEC1_RDCVA };
	uint8_t RDCVC[4] = {CMD0_RDCVC, CMD1_RDCVC, PEC0_RDCVC, PEC1_RDCVC };
	command_message_t command;
	config_message_t config;
	uint16_t cells[4]= {0};
	uint8_t data_read1[8]= {0};
	uint8_t data_read2[8]= {0};
	char tx_message[16];
	lcd_params my_params =
	{
		.data_port = &PORTC,
		.e_port = &PORTB,
		.rs_port = &PORTB,
		.rw_port = &PORTB,
		.e_pin = 6,
		.rs_pin = 4,
		.rw_pin = 5,
		.max_char_line = 16,
		.number_of_lines = LCD_LINE_COUNT_2,
		.lcd_font = LCD_FONT_5x8,
		.lcd_data_bus_width = LCD_8_DATA_BUS_WIDTH,
		.lcd_shift_cursor = LCD_SHIFT_CURSOR_RIGHT,
		.lcd_shift_display = LCD_NO_SHIFT_DISPLAY,
	};
	SPI_PORT |= (1<<pin_SS);
	SPI_PORT |= (1<<pin_MOSI);
	DDRB |= (1<<7);
	lcd_init(&my_params);
	lcd_blink();
	lcd_write_text("cacamac",1, LCD_LINE_COUNT_1);
	_delay_ms(1000);
	lcd_write_instruction(lcd_cmd_clear_display());
	set_command(&command, WRCFGt);
	init_data();
	init_devices();
	sprintf(txMessage, "\n\r\n\rBasic console v.0.0.1\n\r>>");
	abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
    while (1) 
    {
		


		set_command(&command, WRCFGt);
		
		
		set_config(&config, cell8, halfmin);
		wakeup();
		write_read(&config, &command, 0);
		//write_read_config(cell8, halfmin, cell8halfmin);
		_delay_ms(500);
		set_config(&config, cell2, halfmin);
		wakeup();
		write_read(&config, &command, 0);
		//write_read_config(cell2, halfmin, cell2halfmin);
		_delay_ms(500);
		set_config(&config, cell3, halfmin);
		wakeup();
		write_read(&config, &command, 0);
		//write_read_config(cell3, halfmin, cell3halfmin);
		_delay_ms(500);
		set_config(&config, cell7, halfmin);
		wakeup();
		write_read(&config, &command, 0);
		//write_read_config(cell7, halfmin, cell7halfmin);
		_delay_ms(500);
		set_config(&config, no_cell, disabled);
		wakeup();
		write_read(&config, &command, 0);
		
		//write_read_config(no_cell, disabled, nodischarge);
		//_delay_ms(500);
		
		
/*

		wakeup();
		adcv(allcells);
		
			// measure cells
		wakeup();

		readstat(RDCVA, data_read1); // read register with values for cells 1 and 2
		cells[cell2-1]= calculate_voltage(data_read1[2], data_read1[3]); // assembly the values
		cells[cell3-1]= calculate_voltage(data_read1[4], data_read1[5]);
		*/

		set_command(&command, RDCFGt);
		
		wakeup();
		write_read(0, &command, data_read1);
		uint16_t pec = calculate_pec(data_read1, 6);
		if ((data_read1[6]==pec>>8)||(data_read1[7]==(pec&0xFF)))
		{
			sprintf(txMessage,"\n\r\n\Data 1 pec correct\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}
		else
		{
			sprintf(txMessage,"\n\r\n\Data 1 pec  NOT correct\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}
/*

		_delay_ms(4);
		wakeup();

		readstat(RDCVC, data_read2); // read register with values for cells 3 and 4

		cells[cell8-1]= calculate_voltage(data_read2[2], data_read2[3]);
		cells[cell7-1]= calculate_voltage(data_read2[0], data_read2[1]);
		pec = calculate_pec(data_read2, 6);
		if ((data_read2[7]==pec>>8)||(data_read2[6]==(pec&0xFF)))
		{
			sprintf(txMessage, "\n\r\n\Data 2 pec correct\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}
		else
		{
			sprintf(txMessage, "\n\r\n\Data 2 pec  NOT correct\n\r>>");
			abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		}
		lcd_init(&my_params);
		lcd_blink();
		//_delay_ms(1000);
		
		lcd_write_instruction(lcd_cmd_clear_display());
		sprintf(tx_message, "2:%5u 3:%5u", cells[cell2-1], cells[cell3-1]);
		lcd_write_text(tx_message, 0, LCD_LINE_COUNT_1);
		sprintf(tx_message, "7:%5u 8:%5u", cells[cell7-1], cells[cell8-1]);
		lcd_write_text(tx_message, 0, LCD_LINE_COUNT_2);
*/

		
		/*for(int i=0; i<8;i++)
		{
			sprintf(tx_message, "%0.2X",data_read[i]);
			lcd_write_text(tx_message,2*i, LCD_LINE_COUNT_1);
			//_delay_ms(1000);
		}
		for(int i=8; i<16;i++)
		{
			sprintf(tx_message, "%0.2X",data_read2[i-8]);
			lcd_write_text(tx_message,2*(i-8), LCD_LINE_COUNT_2);
		}*/
		processConsoleComm(&serialCom0);
		//_delay_ms(2000);
		
    }
}

