/*
 * freertos_sdtr.c
 *
 * Created: 10/14/2020 8:16:52 PM
 * Author : Mirela
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "mytasks.h"
#include "freertosm128.h"


/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainLED_TASK_PRIORITY			( tskIDLE_PRIORITY )
#define mainButton_TASK_PRIORITY			( tskIDLE_PRIORITY +1)
#define  mainLCD_TASK_PRIORITY (tskIDLE_PRIORITY)

#include "LED.h"
#include "driver_lcd.h"
xSemaphoreHandle xButtonSemaphore=NULL;
portSHORT main(void)
{
	char welcomeln1[16] ="FreeRTOS DEMO";
	char buttonln1[16]="BT:";
	char tasksln1[16] ="TSKS:";
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

	lcd_init(&my_params);
	lcd_blink();
	lcd_write_text(welcomeln1, 0, LCD_LINE_COUNT_1);
	lcd_write_text(buttonln1, 0, LCD_LINE_COUNT_2);
	lcd_write_text(tasksln1, 7, LCD_LINE_COUNT_2);
	//sprintf(txMessage, "\n\r\n\rBasic console v.0.0.1\n\r>>");
	//abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
	vSemaphoreCreateBinary(xButtonSemaphore);
	if(xButtonSemaphore!=NULL)
	{
		//successfully created
		xTaskCreate( vButtonCheckTask, ( signed char * ) "Button", configMINIMAL_STACK_SIZE, NULL, mainButton_TASK_PRIORITY, NULL );
	}
	xTaskCreate( vLEDFlashTask, ( signed char * ) "LED", configMINIMAL_STACK_SIZE, NULL, mainLED_TASK_PRIORITY, NULL );
	xTaskCreate( vLCDUpdateTask, ( signed char * ) "LCD", configMINIMAL_STACK_SIZE, NULL, mainLCD_TASK_PRIORITY, NULL );
	lcd_write_text(welcomeln1, 0, LCD_LINE_COUNT_1);
	//
	//start scheduler
	vTaskStartScheduler();
	//you should never get here
	while(1)
	{
		//processConsoleComm(&serialCom0);
	}
	return 0;
}
