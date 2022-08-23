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
#include "stdio.h"
#include "string.h"
#include "test.h"
#include "runtime.h"


//static uint32_t start_time_val, stop_time_val;

// for getting time
//#include "stm32h7xx_hal.h"

void vStartMonitor( void );
void spawnNewTarget( void );
static void MonitorTask( void * pvParameters );
static void TargetTask( void * pvParameters );


extern Symex_t AFLfuzzer;

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
					configMINIMAL_STACK_SIZE*8,
					NULL,
					10,
					&AFLfuzzer.xTaskTarget);
}


static void MonitorTask( void * pvParameters )
{
    uint32_t notificationvalue;

    spawnNewTarget();  //spawn a new target

    //vTaskDelay(1000000);
	// wait for the target task notification when ready
    ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);

    // notification indexes Monitor
    // 0: notification from target when ready to execute
    // 1: notification from USB CDC ISR when fuzzing data has arrived
    // 2: notification from USB CDC ISR when TX completed
    // 3: notification from target to transmit function packet

    for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
    {
    	AFLfuzzer.txbuffer[j]=0;
    }

    _sym_initialize();

    //enable the cycle counter
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    while(1)
	{
		// we will wait for a notification on index 1 when fuzzing data has arrived
		ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY);
		//cleaning the packet buffer
		//AFLfuzzer.txTotalFunctions=0;
		/*
    	for(uint8_t i=1; i<8; i++)
		{
			AFLfuzzer.txbuffer[i]=0;
		}
		*/

    	AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT; //Index Zero is reserved for the total length of the message, which  includes the first byte



		//notify the target that data has arrived, and it should start execution
		xTaskNotify(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite);

		// wait for the target command to transmit
		// this command is generated when we have around 64 bytes ready to transmit in the buffer
		notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		while(notificationvalue)
		{
			notiTarget = NOTI_TARGET;
			TransmitPack();
			notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		}
		notiTarget = NOTI_MONITOR;
		TransmitPack(); //transmit any remaining package in the buffer if any

		ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT); // wait for TX finish

		//delete the target
		vTaskDelete(AFLfuzzer.xTaskTarget);
		// lets the kernel clean artifacts
		taskYIELD();
		// create a new target
		spawnNewTarget();
		// wait for the target to start
		ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);


		_sym_initialize();


		//clean the buffers
		AFLfuzzer.txTotalFunctions=0;
		for(uint8_t i=1; i<8; i++)
		{
			AFLfuzzer.txbuffer[i]=0;
		}
		AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT;
		for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
		{
			AFLfuzzer.txbuffer[j]=0;
		}
		AFLfuzzer.bRXcomplete = false;
		AFLfuzzer.inputLength = 0;
		RingZeroes(&AFLfuzzer.inputAFL);

	}
}

#define CGC_BENCHMARK

#ifdef CGC_BENCHMARK
extern unsigned int input_cur;
#endif
static void TargetTask( void * pvParameters )
{
	//printf("\n new target spawned\n");
	xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,1,eSetValueWithOverwrite); //notify the monitor task the target is ready
	while(1){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the notification coming from the Monitor task
		//HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		//here we should call the instrumented code
//#if DEBUGPRINT ==1
		printf("\nStart\n");
//#endif

#ifdef CGC_BENCHMARK
        input_cur = 0;
#endif
        _sym_symbolize_memory((char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT),AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT);
#ifdef CGC_BENCHMARK
        test();
#else
        modbusparsing(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4 );
#endif
        _sym_end();

        //stop_time_val = DWT->CYCCNT;


//#if DEBUGPRINT ==1
		printf("\nFinish\n");
//#endif
		//printf("clocks:%u\n\n", stop_time_val - start_time_val);

		//xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,10,eSetValueWithOverwrite);//notify that the test finished
        //vTaskDelay(10);

	}
}


