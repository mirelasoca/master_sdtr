/*
 * driver_lcd.c
 *
 * Created: 06/06/2020 11:53:16
 *  Author: Mirela
 */ 
#include "driver_lcd.h"
lcd_params * params;
uint8_t lcd_cmd_clear_display(void)
{
	return 0x1;
}
uint8_t lcd_cmd_return_home(void)
{
	return (1<<1);
}
uint8_t lcd_cmd_display_off(void)
{
	return (1<<3);
}
uint8_t lcd_cmd_init_instr(void)
{
	return (1<<4)|(1<<5);
}
uint8_t lcd_cmd_cursor_display_shift(bool s, bool r)
{
	return (1<<4)|(s<<3)|(r<<2);
}
uint8_t lcd_cmd_entry_mode_set(bool cursor_shift, bool display_shift)
{
	return (1<<2)|(cursor_shift<<1)|(display_shift<<0);
}

uint8_t lcd_cmd_display_onoff_control( bool display, bool cursor, bool blink_)
{
	return (1<<3)|(display<<2)|(cursor<<1)|(blink_<<0);
}
uint8_t lcd_cmd_set_function(bool data_length, bool line_count, bool font)
{
	return (1<<5)|(data_length<<4)|(line_count<<3)|(font<<2);
}
uint8_t lcd_cmd_set_cgram_addr(uint8_t addr)
{
	return (1<<6)|(addr & 0x3F);
}
uint8_t lcd_cmd_set_ddram_addr(uint8_t addr)
{
	return (1<<7)|(addr & 0x7F);
}
void lcd_write_instruction(uint8_t data)
{
	* params->rs_port &= ~(1<<params->rs_pin);
	* params->rw_port &= ~(1<<params->rw_pin);
	* params->e_port &= ~(1<<params->e_pin);
	
	* params->data_port = data;
	_delay_us(1);
	* params->e_port |= (1<<params->e_pin);
	_delay_us(1); // toggle enable
	* params->e_port &= ~(1<<params->e_pin);
	_delay_ms(LCD_CTRL_WR_MAX_INSTR_DELAY);
	if(!params->lcd_data_bus_width)
	{
		* params->data_port = (data<<4)&0xF0;
		_delay_us(1);
		* params->e_port |= (1<<params->e_pin);
		_delay_us(1); // toggle enable
		* params->e_port &= ~(1<<params->e_pin);
		_delay_ms(LCD_CTRL_WR_MAX_INSTR_DELAY);
	}

}
void lcd_write_data(uint8_t data) {
	* params->rw_port &= ~(1<<params->rw_pin);
	* params->e_port &= ~(1<<params->e_pin);
	* params->rs_port |= (1<<params->rs_pin);
	
	* params->data_port = data;
	_delay_ms(1);
	* params->e_port |= (1<<params->e_pin);
	_delay_us(1); // toggle enable
	*params->e_port &= ~(1<<params->e_pin);
	_delay_us(LCD_CTRL_WR_INSTR_DELAY);
	if (!(params->lcd_data_bus_width))
	{
		* params->data_port = (data<<4)&0xF0;
		_delay_us(1);
		* params->e_port|= (1<<params->e_pin);
		_delay_us(1); // toggle enable
		* params->e_port &= ~(1<<params->e_pin);
		_delay_us(LCD_CTRL_WR_INSTR_DELAY);
		
	}
	
}
void lcd_init(lcd_params * my_params)
{
	params = my_params;
	*(params->data_port-1) = 0xFF; // set data direction as output
	*(params->e_port-1) |= (1<<params->e_pin);
	*(params->rs_port-1) |= (1<< params->rs_pin);
	*(params->rw_port-1) |= (1<< params->rw_pin);
	if(!params->lcd_data_bus_width) // if 4 bit interface
	{
		params->lcd_data_bus_width = LCD_8_DATA_BUS_WIDTH;
		
		_delay_ms(15);// wait on power on
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_ms(5);
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_us(150);
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_us(LCD_CTRL_WR_INSTR_DELAY);
		lcd_write_instruction(lcd_cmd_set_function(LCD_4_DATA_BUS_WIDTH, 0, 0));
		
		params->lcd_data_bus_width = LCD_4_DATA_BUS_WIDTH;
		
		lcd_write_instruction(lcd_cmd_set_function(LCD_4_DATA_BUS_WIDTH,params->number_of_lines, params->lcd_font));
		lcd_write_instruction(lcd_cmd_display_off());
		lcd_write_instruction(lcd_cmd_clear_display());
		lcd_write_instruction(lcd_cmd_entry_mode_set(params->lcd_shift_cursor, params->lcd_shift_display));
	}
	else
	{	_delay_ms(15);// wait on power on
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_ms(5);
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_us(150);
		lcd_write_instruction(lcd_cmd_init_instr());
		_delay_us(LCD_CTRL_WR_INSTR_DELAY);
		lcd_write_instruction(lcd_cmd_set_function(LCD_8_DATA_BUS_WIDTH,params->number_of_lines,params->lcd_font));
		lcd_write_instruction(lcd_cmd_display_off());
		lcd_write_instruction(lcd_cmd_clear_display());
		lcd_write_instruction(lcd_cmd_entry_mode_set(params->lcd_shift_cursor, params->lcd_shift_display));
	
	}
	
}


void lcd_write_text(char *my_string, uint8_t position, LCD_LINE_COUNT line)
{
	uint8_t actual_position, i, length_left = 0;
	length_left =params->max_char_line - position;
	if (line) //if 2nd line
	{
		actual_position = line_2 + position;
	}
	else
	{
		actual_position = position;
	}
	lcd_write_instruction(lcd_cmd_set_ddram_addr(actual_position));
	if (strlen(my_string) <= length_left)
	{
		length_left = strlen(my_string);
	}
	for(i=0; i<length_left;i++)
		{
			lcd_write_data(my_string[i]);
		}
}
void lcd_blink(void)
{
		lcd_write_instruction(lcd_cmd_display_onoff_control(LCD_DISPLAY_ON,LCD_CURSOR_ON,LCD_BLINK_ON));
		_delay_ms(1000);
}