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
#include "portmacro.h"
#include "semphr.h"
#include "LED.h"
#include "button.h"
#include "driver_lcd.h"
#include "ltc6804.h"
#include "twi.h"
#include "mytasks.h"
#include "freertosm128.h"
#include "mytasks.h"

bool ON = false;
bool STOP = false;
uint8_t cfgg4;
uint8_t i2c_discells[8]= {0};
uint8_t i2c_nodis[8]= {0};
uint8_t i2c_rcells[10]= {0};
uint8_t txtt[16]= {0};
	
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

void vLTCreadCells( void *pvParameters )
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 200;
	xLastWakeTime=xTaskGetTickCount();
	
	for( ;; )
	{
		//sprintf(txMessage, "\n\r\LED activated\n\r>>");
		//abtSerTransmitData(&serialCom0, (uint8_t *)txMessage, strlen(txMessage), true, true);
		if( xMutexu != NULL )
		//if(1)
		{
        /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
			if( xSemaphoreTake( xMutexu, ( portTickType ) 0) == pdTRUE )
			//if(1)
			{
			int ret = 0;
			while (ret = twi_masterReceive(i2c_rcells, 8))
			{
				//vTaskDelayUntil(xLastWakeTime, 10);
				 
				//snprintf(txtt, 10, "EXIT:%d", ret);
				//lcd_write_text(txtt, 0, LCD_LINE_COUNT_1);
				
			}
			//printmyballs(i2c_rcells, 8);
			//lcd_write_text("got it R ", 0, LCD_LINE_COUNT_1);
			xSemaphoreGive(xMutexu);
			//printmyballs(i2c_rcells, 8);
			}
	
					
		}
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
	
}
void vLTCsendDischarge( void *pvParameters )
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1000; // update discharge pattern every second
	xLastWakeTime=xTaskGetTickCount();
	set_config2((config_message_t*)i2c_nodis);
	for( ;; )
	{

		if( xMutexu != NULL )
		{
	
         if( xSemaphoreTake( xMutexu, (portTickType ) 0) == pdTRUE )
		 {
				ON ^=1; // toggle discharge on and off for better heat management
				if(ON && (!STOP)) // button was not pressed to stop the process
				
				{
					int ret =0;
					while (ret = twi_masterReceive(i2c_rcells, 8)); //ask for voltage readings until received and right before the cells are discharged, so that measurement is precise
					twi_masterTransmit(i2c_discells, 8); // send updated discharge pattern (last from updateDischarge task)
					
				}
				else
				{
				twi_masterTransmit(i2c_nodis, 8); // send discharge off for 1s
				
				
				}
			/*	snprintf(txtt,5,"conf %u", i2c_discells[0]);
				lcd_write_text(txtt, 0, LCD_LINE_COUNT_1);*/
				xSemaphoreGive(xMutexu);
				
		 }
		
	}	
	vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
	
	
}

void vLTCupdateDischarge( void *pvParameters )
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 70;
	xLastWakeTime=xTaskGetTickCount();
	int i = 0;
	
	for( ;; )
	{
		if( xMutexu != NULL )
		{
			
        /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
        if( xSemaphoreTake( xMutexu, (portTickType ) 0 ) == pdTRUE )
        {
			
			for(int i= 0;i<4; i++)
			{
				
				vcells[i] = (uint16_t)(i2c_rcells[2*i]<<8)|(i2c_rcells[2*i+1]); // compute cell readings from incoming data
				//vcells[i] |= (i2c_rcells[i+1]);
				//i2c_discells[0] = (1<<1)|(1<<7);
				
			}
			vcells[0] -= 339; // update values based on R*I calculation
			vcells[1] -= 332;
			vcells[2] -= 347;
			vcells[3] -= 356;
			
			
			calculate_min(); // calculate ref cell ( min value)
		xSemaphoreGive(xMutexu);}
			if( xSemaphoreTake( xMutexu, (portTickType ) 0 ) == pdTRUE )
			{ 
			// compute configuration pattern based on cells that are higher than the ref cell
			  cfgg4 =((vcells[cell2]-3 > vcells[vref])<<1)|((vcells[cell3]-3 > vcells[vref])<<2)|((vcells[cell7]-3> vcells[vref])<<6)|((vcells[cell8]-3> vcells[vref])<<7);
			  set_config((config_message_t*)i2c_discells, cfgg4); // update discharge
			xSemaphoreGive(xMutexu);}

			
		}
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}
// for debug purpose, not used	
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
char afis[10];
xLastWakeTime=xTaskGetTickCount();
xSemaphoreTake(xButtonSemaphore, (portTickType)0);
vButtonInit();
	for (;;)
	{
	if (xButtonGetStatus()==pdTRUE)
	{
		
	
	if( xSemaphoreTake( xMutexu, (portTickType ) 0) == pdTRUE )

		{
			
			STOP^=1; // toggle stop/start when button pressed
			
			if(STOP)
			{
				
					twi_masterTransmit(i2c_nodis, 8); // send discharge off immediately
				
				
				sprintf(afis,"STOP");
				lcd_init(&my_params);
				lcd_blink();
				lcd_write_text(afis, 0, LCD_LINE_COUNT_1);
				
			}
			_delay_ms(200);
			xSemaphoreGive(xMutexu);
		}
			else
			{
				sprintf(afis,"START");
				lcd_init(&my_params);
				lcd_blink();
				lcd_write_text(afis, 0, LCD_LINE_COUNT_1);
			}
			
		}
	vTaskDelayUntil(&xLastWakeTime,xFrequency);	
	}
}
// for debug purposes, not used
void vLCDUpdateTask( void *pvParameters )
{
/*
 char welcomeln1[16] ="FreeRTOS DEMO";
 char buttonln1[16]="BT:";
 char tasksln1[16] ="TSKS:";*/
char afis[10];
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
		sprintf(afis,"ref is %u", vref);
		lcd_write_text(afis, 0, LCD_LINE_COUNT_1);
		//works only up to 9 tasks
		//LCDsendChar(uxTasks+48);
		//if (xButtonSemaphore != NULL)
		//{
			//poll
			//afisare= (xSemaphoreTake(xButtonSemaphore, (portTickType)0)==pdTRUE)?1:0;
			//sprintf(afis,"%u", afisare);
			//lcd_write_text(afis, 3, LCD_LINE_COUNT_2);
		//}
	vTaskDelayUntil(&xLastWakeTime,xFrequency);	
	}
}