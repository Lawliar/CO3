#include "midi_lowlevel.h"
#include "leds.h" 
#include "main.h"
#include "protocol.h"
extern char midiBuffer[MIDI_BUF_SIZE] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );    /* for debugging */ //buffer
extern UART_HandleTypeDef huart4;
/*

char redzone1[ext_power_of_2(MIDI_BUF_SIZE)] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );

char redzone2[ext_power_of_2(MIDI_BUF_SIZE)] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );

MIDIMsgBuilder midiMsgBuilder __attribute__( ( aligned( next_power_of_2(sizeof(MIDIMsgBuilder) )  ) ) );                   //buffers

char redzone3[next_power_of_2(sizeof(MIDIMsgBuilder))] __attribute__( ( aligned( next_power_of_2(sizeof(MIDIMsgBuilder) )  ) ) );

MIDI_Router_Standard midiRouter __attribute__( ( aligned( next_power_of_2(sizeof(MIDI_Router_Standard) )  ) ) );                //buffers

char redzone4[next_power_of_2(sizeof(MIDI_Router_Standard))] __attribute__( ( aligned( next_power_of_2(sizeof(MIDI_Router_Standard) )  ) ) );
*/



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart4)
    {
       HAL_UART_DMAStop(&huart4);

        while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t*) midiBuffer, MIDI_BUF_SIZE) != HAL_OK)
        {
            HAL_UART_Abort(&huart4);
            HAL_UART_DeInit(&huart4);
            HAL_UART_Init(&huart4);
       }
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
#if DUALCOREFUZZ == 0

  //Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
#endif

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(huart==&huart4)
    {
        // notify with the buffer size
        xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,4,1,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}



void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* If configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2 then this
	function will automatically get called if a task overflows its stack. */
	( void ) pxTask;
	( void ) pcTaskName;
	for( ;; );
}


void vApplicationMallocFailedHook( void )
{
	/* If configUSE_MALLOC_FAILED_HOOK is set to 1 then this function will
	be called automatically if a call to pvPortMalloc() fails.  pvPortMalloc()
	is called automatically when a task, queue or semaphore is created. */
	for( ;; );
}


/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}