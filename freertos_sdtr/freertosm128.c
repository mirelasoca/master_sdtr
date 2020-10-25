/*
 * freertosm128.c
 *
 * Created: 7/4/2011 18:43:58
 *  Author: MMM
 */ 

#include <avr/io.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "mytasks.h"
#include "freertosm128.h"
/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainLED_TASK_PRIORITY			( tskIDLE_PRIORITY )
#define mainButton_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainLCD_TASK_PRIORITY				( tskIDLE_PRIORITY )
xSemaphoreHandle xButtonSemaphore=NULL;
portSHORT main(void)
{
	vSemaphoreCreateBinary(xButtonSemaphore);
	if(xButtonSemaphore!=NULL)
	{
		//successfully created
		xTaskCreate( vButtonCheckTask, ( signed char * ) "Button", configMINIMAL_STACK_SIZE, NULL, mainButton_TASK_PRIORITY, NULL );	
	}
	xTaskCreate( vLCDUpdateTask, ( signed char * ) "LCD", configMINIMAL_STACK_SIZE, NULL, mainLCD_TASK_PRIORITY, NULL );	
	xTaskCreate( vLEDFlashTask, ( signed char * ) "LED", configMINIMAL_STACK_SIZE, NULL, mainLED_TASK_PRIORITY, NULL );	
	//start scheduler
	vTaskStartScheduler();
	//you should never get here
	while(1)
    {
        
    }
		return 0;
}
