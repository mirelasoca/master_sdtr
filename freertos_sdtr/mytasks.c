/*
 * mytasks.c
 *
 * Created: 7/5/2011 23:45:23
 *  Author: MMM
 */ 
#include <avr/pgmspace.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "LED.h"
#include "button.h"
#include "driver_lcd.h"
#include "mytasks.h"
#include "freertosm128.h"
#include "mytasks.h"



/*
lcd_params my_params =
{
	.data_port = &PORTC,
	.e_port = &PORTB,
	.rs_port = &PORTB,
	.rw_port = &PORTB,
	.e_pin = 3,
	.rs_pin = 1,
	.rw_pin = 2,
	.max_char_line = 16,
	.number_of_lines = LCD_LINE_COUNT_2,
	.lcd_font = LCD_FONT_5x8,
	.lcd_data_bus_width = LCD_8_DATA_BUS_WIDTH,
	.lcd_shift_cursor = LCD_SHIFT_CURSOR_RIGHT,
	.lcd_shift_display = LCD_NO_SHIFT_DISPLAY,
};*/
void vLEDFlashTask( void *pvParameters )
{
vLEDInit();
portTickType xLastWakeTime;
const portTickType xFrequency = 1000;
xLastWakeTime=xTaskGetTickCount();
	for( ;; )
	{
		//sprintf(txMessage, "\n\r\LED activated\n\r>>");
		//abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		vLEDToggle();
		_delay_ms(1);
		vTaskDelayUntil(&xLastWakeTime,xFrequency);		
	}
	
}
void vButtonCheckTask( void *pvParameters )
{
portTickType xLastWakeTime;
const portTickType xFrequency = 20;
xLastWakeTime=xTaskGetTickCount();
xSemaphoreTake(xButtonSemaphore, (portTickType)0);
vButtonInit();
	for (;;)
	{
	if (xButtonGetStatus()==pdTRUE)
		{
			//sprintf(txMessage, "\n\r\n\rBUTTON activated\n\r>>");
			//abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
			xSemaphoreGive(xButtonSemaphore);
		}
	vTaskDelayUntil(&xLastWakeTime,xFrequency);	
	}
}
void vLCDUpdateTask( void *pvParameters )
{
/*
 char welcomeln1[16] ="FreeRTOS DEMO";
 char buttonln1[16]="BT:";
 char tasksln1[16] ="TSKS:";*/
char afis[1];
uint8_t afisare = 0;
portTickType xLastWakeTime;
const portTickType xFrequency = 500;
xLastWakeTime=xTaskGetTickCount();
unsigned portBASE_TYPE uxTasks;
/*
lcd_init(&my_params);
lcd_blink();
lcd_write_instruction(lcd_cmd_clear_display());
lcd_write_text(welcomeln1, 0, LCD_LINE_COUNT_1);
lcd_write_text(buttonln1, 0, LCD_LINE_COUNT_2);
lcd_write_text(tasksln1, 7, LCD_LINE_COUNT_2);
*/

	for (;;)
	{
		//sprintf(txMessage, "\n\r\n\rLCD activated\n\r>>");
		//abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		uxTasks=uxTaskGetNumberOfTasks();
		//LCDGotoXY(13,1);
		sprintf(afis,"%u", uxTasks);
		lcd_write_text(afis, 13, LCD_LINE_COUNT_2);
		//works only up to 9 tasks
		//LCDsendChar(uxTasks+48);
		if (xButtonSemaphore != NULL)
		{
			//poll
			afisare= (xSemaphoreTake(xButtonSemaphore, (portTickType)0)==pdTRUE)?1:0;
			sprintf(afis,"%u", afisare);
			lcd_write_text(afis, 3, LCD_LINE_COUNT_2);
		}
	vTaskDelayUntil(&xLastWakeTime,xFrequency);	
	}
}