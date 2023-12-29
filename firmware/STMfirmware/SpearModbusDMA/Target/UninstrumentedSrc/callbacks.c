/*
 * callbacks.c
 *
 *  Created on: Jan 28, 2022
 *      Author: alejandro
 */



#include "modbus_rtu.h"
#include "string.h"
//#include "afl.h"
//#include "fuzzing.h"
#include "stm32h7xx_hal.h"
#include "protocol.h"
extern uint16_t modbusMemory[MODBUS_SLAVE_REGISTERS_NUM];

extern modbus_t  modbusVars;
extern UART_HandleTypeDef huart4;


//#if DUALCOREFUZZ == 0
//extern uint8_t AFLfuzzerRegion[AFLINPUTREGION_SIZE ] __attribute__( ( aligned( AFLINPUTREGION_SIZE ) ) );
//#endif


#define modbusRxCount modbusVars.modbusRxCountV
#define modbusSlaveFrameReadyFlag modbusVars.modbusSlaveFrameReadyFlagV
#define xFrameReadySemaphore modbusVars.xFrameReadySemaphoreV
#define xModbusSlaveTask modbusVars.xModbusSlaveTaskV




void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(huart==&huart4)
	{
		modbusRxCount = Size;  //MODBUS_MAX_FRAME_SIZE - DMA1_Channel5->CNDTR;
		//xSemaphoreGiveFromISR(xFrameReadySemaphore, NULL);
		//notify the fuzzer that data has arrived to through USART
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,4,1,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
		//portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		//xTaskNotifyFromISR(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite, &xHigherPriorityTaskWoken); //notify that data arrived
	}
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#if DUALCOREFUZZ == 0

  //Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
#endif

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;


	if(huart==&huart4)
	{

		while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, modbusRxTxBuffer, MODBUS_MAX_FRAME_SIZE) != HAL_OK)
		{
			HAL_UART_DMAStop(&huart4);
		}
		modbusRxCount = 0;
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,4,FAULT_NONE_RTOS,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);

	}

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

	 if(huart==&huart4)
	 {
		HAL_UART_DMAStop(&huart4);
		while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, modbusRxTxBuffer, MODBUS_MAX_FRAME_SIZE) != HAL_OK)
		{
		    HAL_UART_Abort(&huart4);
		    HAL_UART_DeInit(&huart4);
		    HAL_UART_Init(&huart4);
		}

	}

/*
  #if DUALCOREFUZZ == 0

  Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
  #endif

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;


  if(huart==&huart4)
  {

	    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, modbusRxTxBuffer, MODBUS_MAX_FRAME_SIZE); //wait for more data
	    modbusRxCount = 0;
	    xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskFuzzer,0,FAULT_NONE_RTOS,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
	    //notify that the test finished

  }

  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

*/

}




void cleanInitShadow()
{

#if DUALCOREFUZZ == 0

  //Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
#endif
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


void SytemCall_1_code()
{
	modbusSlaveHardwareInit();
}
//void _sym_symbolize_memory(char * addr, size_t length);

uint8_t modbusSlaveHardwareInit(void)
{

	uint8_t status = 0;
	modbusRxCount = 0;

	HAL_UART_Abort(&huart4);
	HAL_UART_DeInit(&huart4);
	HAL_UART_Init(&huart4);


	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, modbusRxTxBuffer, MODBUS_MAX_FRAME_SIZE) != HAL_OK)
	{
		  HAL_UART_DMAStop(&huart4);
	}

    status = 1;
    return status;
}


void SytemCall_2_code()
{
	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, modbusRxTxBuffer, MODBUS_MAX_FRAME_SIZE) != HAL_OK)
	{
	 					HAL_UART_DMAStop(&huart4);
	}
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

