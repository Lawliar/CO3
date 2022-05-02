/*
 * monitor.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */

#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"
#include "main.h"


void vStartMonitor( void );
void vStartTarget( void );
static void MonitorTask( void * pvParameters );
//static void TargetTask( void * pvParameters );

void app_main( void )
{
	/* Start the MPU demo. */
	vStartMonitor();


	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Should not get here. */
	for( ;; );
}


//creates the monitor task
void vStartMonitor( void )
{


	xTaskCreate(MonitorTask,
			    "Monitor",
				configMINIMAL_STACK_SIZE,
				NULL,
				10,
				&AFLfuzzer.xTaskFuzzer);

}


static void MonitorTask( void * pvParameters )
{

	while(1)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(250);

	}

}




//creates a target task
void vStartTarget( void )
{


}
