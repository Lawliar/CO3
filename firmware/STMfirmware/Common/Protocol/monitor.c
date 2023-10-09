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
#include "runtime.h"
#include "test.h"



static uint32_t start_time_val, stop_time_val;

// for getting time
//#include "stm32h7xx_hal.h"

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
#ifdef USE_SERIAL_OVER_USB
    	SerialReceiveInput();
#else
    	ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY);
#endif
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


		// when we have around 64 bytes ready to transmit in the buffer
		// the target task will send a notification to this
		notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		while(notificationvalue)
		{
			notiTarget = NOTI_TARGET;
			TransmitPack();
			notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		}
		notiTarget = NOTI_MONITOR;
		TransmitPack(); //transmit any remaining package in the buffer if any
#ifndef USE_SERIAL_OVER_USB
		ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT); // wait for the USB to finish transmission
#endif

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

#ifdef CGC_BENCHMARK
extern unsigned int input_cur;
#endif

extern uint8_t GPSHandleRegion[];
static void TargetTask( void * pvParameters )
{
	//printf("\n new target spawned\n");
	xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,1,eSetValueWithOverwrite); //notify the monitor task the target is ready
	while(1){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the notification coming from the Monitor task
		//HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		//here we should call the instrumented code
//#if DEBUGPRINT ==1
		//printf("\nStart\n");
//#endif
		start_time_val = DWT->CYCCNT;
#ifdef CGC_BENCHMARK
        input_cur = 0;
#endif
        //if(AFLfuzzer.inputAFL.u32available <= AFL_BUFFER_STARTING_POINT){
        	// THE DATA IS NOT READY, but the monitor said it is, what is going on?
        //	while(1){}
        //}
		_sym_symbolize_memory((char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT),AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT, false);
#ifdef CGC_BENCHMARK
		test();
#else

		//modbusparsing(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4 );
        test(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4);
		//HAL_UART_Transmit_test(&huart2,  (unsigned char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT), AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT,  HAL_MAX_DELAY);
		//gps_init((gps_t*)GPSHandleRegion);
        //gps_process((gps_t*)GPSHandleRegion,&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4 );
		//memset(GPSHandleRegion,0, next_power_of_2(gps_handle_t_size));
#endif
        _sym_end();

        stop_time_val = DWT->CYCCNT;


//#if DEBUGPRINT ==1
		//printf("\nFinish\n");
//#endif
		printf("time:%d\n",stop_time_val - start_time_val);

		//xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,10,eSetValueWithOverwrite);//notify that the test finished
        //vTaskDelay(10);

	}
}


