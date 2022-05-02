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
#include  "monitor.h"
#include  "ring.h"


void vStartMonitor( void );
void spawnNewTarget( void );
static void MonitorTask( void * pvParameters );
static void TargetTask( void * pvParameters );

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
				&AFLfuzzer.xTaskMonitor);



}

//creates a target task
void spawnNewTarget( void )
{
	xTaskCreate(TargetTask,
				    "Target",
					configMINIMAL_STACK_SIZE,
					NULL,
					10,
					&AFLfuzzer.xTaskTarget);
}


static void MonitorTask( void * pvParameters )
{
    uint32_t notificationvalue;

    spawnNewTarget();  //spawn a new target

	// wait for the target task notification when ready
    ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);

	while(1)
	{
		// we will wait for a notification on index 1 when fuzzing data has arrived
		ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY);
		//notify the target that data has arrived
		xTaskNotify(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite);
		// wait for the target finishing execution
		notificationvalue = ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT);

		//delete the target
		vTaskDelete(AFLfuzzer.xTaskTarget);
		// lets the kernel clean artifacts
		taskYIELD();
		// create a new target
		spawnNewTarget();
		// wait for the target to start
		ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);

		AFLfuzzer.bRXcomplete = false;
		AFLfuzzer.inputLength = 0;
		RingZeroes(&AFLfuzzer.inputAFL);


	}
}

static void TargetTask( void * pvParameters )
{

	xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,1,eSetValueWithOverwrite); //notify the monitor task the target is ready
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the notification coming from the Monitor task
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		//here we should call the instrumented code

		xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,10,eSetValueWithOverwrite);//notify that the test finished


	}
}



