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

