/*
 * driver_lcd.h
 *
 * Created: 06/06/2020 11:53:41
 *  Author: Mirela
 */ 


#ifndef DRIVER_LCD_H_
#define DRIVER_LCD_H_
#include "main_config.h"
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include <stdbool.h>
typedef enum 
{
	LCD_SHIFT_CURSOR_LEFT,
	LCD_SHIFT_CURSOR_RIGHT,
	}LCD_SHIFT_CURSOR;
typedef enum
{
	LCD_NO_SHIFT_DISPLAY,
	LCD_SHIFT_ENTIRE_DISPLAY,
	}LCD_SHIFT_DISPLAY;
typedef enum
{
	LCD_DISPLAY_OFF,
	LCD_DISPLAY_ON,
	}LCD_DISPLAY_ON_OFF;
typedef enum
{
	LCD_CURSOR_OFF,
	LCD_CURSOR_ON,
	}LCD_CURSOR_ON_OFF;
typedef enum
{
	LCD_BLINK_OFF,
	LCD_BLINK_ON,
	}LCD_BLINK_ON_OFF;
typedef enum
{
	LCD_4_DATA_BUS_WIDTH,
	LCD_8_DATA_BUS_WIDTH,
} LCD_DATA_BUS_WIDTH;
typedef enum
{
	LCD_LINE_COUNT_1,
	LCD_LINE_COUNT_2,
	}LCD_LINE_COUNT;
typedef enum
{
	LCD_FONT_5x8,
	LCD_FONT_5x11,
	}LCD_FONT;

#ifndef LCD_CTRL_WR_INSTR_DELAY
#define LCD_CTRL_WR_INSTR_DELAY 42
#endif // LCD_CTRL_INSTR_DELAY
#ifndef LCD_CTRL_WR_MAX_INSTR_DELAY
#define LCD_CTRL_WR_MAX_INSTR_DELAY 2
#endif // LCD_CTRL_WR_MAX_INSTR_DELAY in ms
#define MAX_LENGTH 16
#define line_2 0x40

typedef struct
{
	volatile uint8_t * data_port;
	volatile uint8_t * e_port;
	volatile uint8_t * rs_port;
	volatile uint8_t * rw_port;
	uint8_t e_pin;
	uint8_t rs_pin;
	uint8_t rw_pin;
	uint8_t max_char_line;
	LCD_LINE_COUNT number_of_lines;
	LCD_FONT lcd_font;
	LCD_DATA_BUS_WIDTH lcd_data_bus_width;
	LCD_SHIFT_CURSOR lcd_shift_cursor;
	LCD_SHIFT_DISPLAY lcd_shift_display;
	}lcd_params;
lcd_params * params;
void lcd_write_instruction (uint8_t data);
void lcd_init(lcd_params * my_params);
void lcd_write_data(uint8_t data);
void lcd_blink (void);
void lcd_write_text(char *my_string, uint8_t position, LCD_LINE_COUNT line);
uint8_t lcd_cmd_clear_display(void);
uint8_t lcd_cmd_return_home(void);
uint8_t lcd_cmd_display_off(void);
uint8_t lcd_cmd_init_instr(void);
uint8_t lcd_cmd_cursor_display_shift(bool s, bool r);
uint8_t lcd_cmd_entry_mode_set(bool cursor_shift, bool display_shift);
uint8_t lcd_cmd_display_onoff_control( bool display, bool cursor, bool blink_);
uint8_t lcd_cmd_set_function(bool data_length, bool line_count, bool font);
uint8_t lcd_cmd_set_cgram_addr(uint8_t addr);
uint8_t lcd_cmd_set_ddram_addr(uint8_t addr);


#endif /* DRIVER_LCD_H_ */