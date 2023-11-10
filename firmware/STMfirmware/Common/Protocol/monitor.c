/*
 * monitor.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */




#include "protocol.h"
//#include "main.h"
#include  "monitor.h"
#include  "ring.h"
#include "stdio.h"
#include "string.h"
#include "runtime.h"
#include "test.h"


#if defined(USE_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#elif defined(USE_CHIBIOS)
#include "ch.h"
#include "hal.h"
#endif

static uint32_t start_time_val, stop_time_val;


#ifdef USE_CHIBIOS
//static StackType_t targetTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );
static THD_WORKING_AREA(targetTaskStack, configMINIMAL_STACK_SIZE*8);
//static StackType_t FuzzerTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );
static THD_WORKING_AREA(MonitorTaskStack, configMINIMAL_STACK_SIZE);
uint32_t CO3_TARGET_TIMEOUT;
#endif

void vStartMonitor( void );
void spawnNewTarget( void );
static void MonitorTask( void * pvParameters );
static void TargetTask( void * pvParameters );


void app_main( void )
{
	/* Start the MPU demo. */
	vStartMonitor();

#if defined USE_FREERTOS
	/* Start the scheduler. */
	vTaskStartScheduler();
#endif

	/* Should not get here. */
	for( ;; );
}


//creates the monitor task
void vStartMonitor( void )
{
#if defined USE_FREERTOS
	xTaskCreate(MonitorTask,
			    "Monitor",
				configMINIMAL_STACK_SIZE,
				NULL,
				10,
				&AFLfuzzer.xTaskMonitor);
#elif defined USE_CHIBIOS

	CO3_TARGET_TIMEOUT  = chTimeMS2I(2000);
	AFLfuzzer.xTaskMonitor = chThdCreateStatic(MonitorTaskStack, sizeof(MonitorTaskStack), NORMALPRIO+1, MonitorTask, NULL);
#endif

}

#if defined USE_CHIBIOS
void killTarget(){
    eventmask_t events = 0;
    events |= TARGET_SHOULD_KILL_SELF;
    chEvtSignal(AFLfuzzer.xTaskTarget, events);
    return;
}
#endif

//creates a target task
void spawnNewTarget( void )
{
#if defined USE_FREERTOS
	xTaskCreate(TargetTask,
				    "Target",
					configMINIMAL_STACK_SIZE*8,
					NULL,
					10,
					&AFLfuzzer.xTaskTarget);
#elif defined USE_CHIBIOS
	AFLfuzzer.xTaskTarget = chThdCreateStatic(targetTaskStack, sizeof(targetTaskStack), NORMALPRIO+1, TargetTask, NULL);
#endif
}


static void MonitorTask( void * pvParameters )
{


    spawnNewTarget();  //spawn a new target

#if defined USE_FREERTOS
    uint32_t notificationvalue;
    ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);
#elif defined USE_CHIBIOS
    eventmask_t evt = chEvtWaitAny(ALL_EVENTS);
    while(!(evt & FUZZER_TARGET_READY)){
        killTarget();
        chThdWait(AFLfuzzer.xTaskTarget); // wait for the target to finish
        spawnNewTarget();
        evt = chEvtWaitAny(ALL_EVENTS);
    }
#endif
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


#if defined USE_FREERTOS
		//notify the target that data has arrived, and it should start execution
		xTaskNotify(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite);
#elif defined USE_CHIBIOS
		eventmask_t events = 0;
		events |= TARGET_GO_AHEAD;
		chEvtSignal(AFLfuzzer.xTaskTarget, events);
#endif

		// when we have around 64 bytes ready to transmit in the buffer
		// the target task will send a notification to this
#if defined USE_FREERTOS
		notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		while(notificationvalue)
		{
			notiTarget = NOTI_TARGET;
			TransmitPack();
			notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
		}
#elif defined USE_CHIBIOS
		eventmask_t evt = chEvtWaitAnyTimeout(ALL_EVENTS, CO3_TARGET_TIMEOUT );
		while (evt == MORE_DATA_TO_COME){
			notiTarget = NOTI_TARGET;
			TransmitPack();
			evt = chEvtWaitAnyTimeout(ALL_EVENTS, CO3_TARGET_TIMEOUT );
		}
#endif
		notiTarget = NOTI_MONITOR;
		TransmitPack(); //transmit any remaining package in the buffer if any
#ifndef USE_SERIAL_OVER_USB
		ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT); // wait for the USB to finish transmission
#endif

#if defined USE_FREERTOS
		//delete the target
		vTaskDelete(AFLfuzzer.xTaskTarget);
		// lets the kernel clean artifacts
		taskYIELD();
#elif defined USE_CHIBIOS
		killTarget();
		chThdWait(AFLfuzzer.xTaskTarget);
		chThdYield();
#endif
		spawnNewTarget();
#if defined USE_FREERTOS
		ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);
#elif defined USE_CHIBIOS
		evt = chEvtWaitAny(ALL_EVENTS);
	    while(!(evt & FUZZER_TARGET_READY)){
		    killTarget();
		    chThdWait(AFLfuzzer.xTaskTarget); // wait for the target to finish
		    spawnNewTarget();
		    evt = chEvtWaitAny(ALL_EVENTS);
		}
#endif
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
#if defined USE_FREERTOS
	xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,1,eSetValueWithOverwrite); //notify the monitor task the target is ready
#elif defined USE_CHIBIOS
	eventmask_t events = 0;
	events |= FUZZER_TARGET_READY;
	chEvtSignal(AFLfuzzer.xTaskMonitor, events);
	events = 0; // clears out the bits
#endif
	while(1){
#if defined USE_FREERTOS
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the notification coming from the Monitor task
#elif defined USE_CHIBIOS
		if(chThdShouldTerminateX()){
		   break;
		}
		//ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the data coming from the fuzzer task
		eventmask_t evt = chEvtWaitAny(ALL_EVENTS);
		//check if the monitor tells me to exit
		if(evt & TARGET_SHOULD_KILL_SELF){
		    break;
		}
		// check if monitor tells me to continue
		if(!(evt & TARGET_GO_AHEAD)){
		    // something is wrong, but we cannot kill ourself
		    // I'll just deadloop here, so that when attached to a debugger, everyone would know something is wrong
		    while(1){}
		}
#endif
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
		printf("time:%d\n",stop_time_val - start_time_val);
	}
}


