/*
 * stdtr_ltc6804.c
 *
 * Created: 10/19/2020 8:21:41 PM
 * Author : Mirela
 */ 

#include <avr/io.h>
#include "ltc6804.h"
#include "driver_lcd.h"
#include <util/delay.h>
#include <stdio.h>
int main(void)
{
    /* Replace with your application code */
	//SPI_MasterInit();
	uint8_t RDCVA[4] = {CMD0_RDCVA, CMD1_RDCVA, PEC0_RDCVA, PEC1_RDCVA };
	uint8_t RDCVC[4] = {CMD0_RDCVC, CMD1_RDCVC, PEC0_RDCVC, PEC1_RDCVC };
	uint16_t cells[4]= {0};
	uint8_t data_read[8]= {0};
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
	lcd_init(&my_params);
	lcd_blink();
	lcd_write_text("cacamac",1, LCD_LINE_COUNT_1);
	_delay_ms(1000);
	lcd_write_instruction(lcd_cmd_clear_display());
    while (1) 
    {
		


		wakeup();
		write_read_config(cell8, halfmin, cell8halfmin);
		_delay_ms(1000);
		wakeup();
		write_read_config(cell2, halfmin, cell2halfmin);
		_delay_ms(1000);
		wakeup();
		write_read_config(cell3, halfmin, cell3halfmin);
		_delay_ms(1000);

		wakeup();
		write_read_config(cell7, halfmin, cell7halfmin);
		_delay_ms(1000);
		
		wakeup();
		
		write_read_config(no_cell, disabled, nodischarge);
		_delay_ms(1000);
		

		wakeup();
		adcv(celll2_8);
		_delay_ms(50);
		adcv(celll7);
		_delay_ms(50);
		adcv(celll3);
		_delay_ms(50);
			// measure cells
		wakeup();

		readstat(RDCVA, data_read); // read register with values for cells 1 and 2
		cells[cell2-1]= calculate_voltage(data_read[2], data_read[3]); // assembly the values
		cells[cell3-1]= calculate_voltage(data_read[4], data_read[5]);

		_delay_ms(4);
		wakeup();

		readstat(RDCVC, data_read2); // read register with values for cells 3 and 4

		cells[cell8-1]= calculate_voltage(data_read[2], data_read[3]);
		cells[cell7-1]= calculate_voltage(data_read[0], data_read[1]);
		lcd_init(&my_params);
		lcd_blink();
		//_delay_ms(1000);



		for(int i=0; i<8;i++)
		{
			sprintf(tx_message, "data %u is %u", i,data_read[i]);
			lcd_write_text(tx_message,1, LCD_LINE_COUNT_1);
			_delay_ms(2000);
			lcd_write_instruction(lcd_cmd_clear_display());
			//_delay_ms(1000);
		}
		for(int i=8; i<16;i++)
		{
			sprintf(tx_message, "data %u is %u", i,data_read2[i-8]);
			lcd_write_text(tx_message,1, LCD_LINE_COUNT_1);
			_delay_ms(2000);
			lcd_write_instruction(lcd_cmd_clear_display());
			//_delay_ms(1000);
		}
		
    }
}

