/*
 * freertos_sdtr.c
 *
 * Created: 10/14/2020 8:16:52 PM
 * Author : Mirela
 */ 

#include <avr/io.h>


#include "FreeRTOS.h"
#include "task.h"
#include "mytasks.h"

/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainLED_TASK_PRIORITY			( tskIDLE_PRIORITY )

portSHORT main(void)
{
	
	xTaskCreate( vLEDFlashTask, ( signed char * ) "LED", configMINIMAL_STACK_SIZE, NULL, mainLED_TASK_PRIORITY, NULL );
	//start scheduler
	vTaskStartScheduler();
	//you should never get here
	while(1)
	{
		
	}
	return 0;
}
