/*
 * protocol.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */


#include "protocol.h"
#include "stdint.h"


#ifdef USE_SERIAL_OVER_USB


#if defined USE_FREERTOS
extern UART_HandleTypeDef huart2; //PUT the usart that you wanna use here
UART_HandleTypeDef* co3_huart = &huart2;
#elif defined USE_CHIBIOS
extern SerialDriver SD2;
SerialDriver* co3_huart = &SD2;
#endif
uint8_t co3_usart_input_buffer[MAX_BUFFER_INPUT];


#else
#include "usbd_cdc_if.h"
#endif


extern Symex_t AFLfuzzer;

uint8_t notiTarget;

// Callback executed on USB TX complete ISR
void notifyTXfinish()
{
#if defined USE_FREERTOS
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
#endif
	//cleaning the packet buffer to receive new messages
	/*
	AFLfuzzer.txTotalFunctions=0;
	for(uint8_t i=1; i<8; i++)
	{
		AFLfuzzer.txbuffer[i]=0;
	}
	}*/

	AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT;  //we reserve the first byte for size
	//clean the tx buffer
	for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
	{
		AFLfuzzer.txbuffer[j]=0;
	}
	AFLfuzzer.txTotalFunctions=0;



	//notify the target to continue execution
	if (notiTarget == NOTI_TARGET)
	{
#if defined USE_FREERTOS
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskTarget,
  	  	    				1, //index
  							1, //value = 1 data TX complete
  							eSetBits,
  							&xHigherPriorityTaskWoken);
#elif defined USE_CHIBIOS
		chEvtSignalI(AFLfuzzer.xTaskTarget, TRANSMIT_FINISHED);
#endif
	}
	else
	{
#if defined USE_FREERTOS
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,
		  	  	    				2, //index
		  							1, //value = 1 data TX complete
		  							eSetBits,
		  							&xHigherPriorityTaskWoken);
#elif defined USE_CHIBIOS
		chEvtSignalI(AFLfuzzer.xTaskMonitor, TRANSMIT_FINISHED);
#endif
	}
#if defined USE_FREERTOS
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
#endif
}


// this should be only called by the monitor
void TransmitPack(void)
{
    // Transmit all functions in output buffer if any
	if(AFLfuzzer.txTotalFunctions)
	{
#if DEBUGPRINT==1
		printf("TX buffer f. num: %d\n", AFLfuzzer.txTotalFunctions);
#endif
		AFLfuzzer.txbuffer[0]= AFLfuzzer.txCurrentIndex; //set the total length of the payload without considering size itself
#ifdef USE_SERIAL_OVER_USB

#if defined USE_FREERTOS
		HAL_UART_Transmit(co3_huart,AFLfuzzer.txbuffer,AFLfuzzer.txCurrentIndex,HAL_MAX_DELAY);
#elif defiend USE_CHIBIOS
		sdWrite(co3_huart, (uint8_t *)AFLfuzzer.txbuffer, AFLfuzzer.txCurrentIndex);
#endif
		AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT;  //we reserve the first byte for size
		//clean the tx buffer
		for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
		{
			AFLfuzzer.txbuffer[j]=0;
		}
		AFLfuzzer.txTotalFunctions=0;
#if defined USE_FREERTOS
		// wake up the target who is waiting on the transmission
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskTarget,
		  	  	    				1, //index
		  							1, //value = 1 data TX complete
		  							eSetBits,
		  							&xHigherPriorityTaskWoken);
		//sanity check, since only the monitor uses usart to transmit this.
		if(xTaskGetCurrentTaskHandle() != AFLfuzzer.xTaskMonitor){
			while(1){}
		}
#elif defined USE_CHIBIOS
		eventmask_t events = 0;
		events |= TRANSMIT_FINISHED;
		chEvtSignal(AFLfuzzer.xTaskTarget, events);
#endif

#else
		CDC_Transmit_FS(AFLfuzzer.txbuffer, AFLfuzzer.txCurrentIndex); //transmit data
#endif
	}

}

#ifdef USE_SERIAL_OVER_USB
void SerialReceiveInput(uint8_t* Buf, uint32_t *Len)
{

	  //union ubytes_t auxbytes;
	  uint32_t u32Tocopy;
	  uint8_t error = 0;
	  if(AFLfuzzer.inputLength == 0)
	  {
#if defined USE_FREERTOS
	      HAL_UART_Receive(co3_huart, (uint8_t * )&AFLfuzzer.inputLength, sizeof(uint32_t), HAL_MAX_DELAY );
#elif defined USE_CHIBIOS
	      sdReadTimeout(co3_huart, (uint8_t * )&AFLfuzzer.inputLength, sizeof(uint32_t) , TIME_INFINITE);
#endif
	      AFLfuzzer.inputLengthpadded  = AFLfuzzer.inputLength;
	      if((AFLfuzzer.inputLengthpadded)> MAX_BUFFER_INPUT)
	      {
        	  error = 1;
          }
      }

      if( AFLfuzzer.inputLengthpadded && (error == 0) )
      {
	    	 //u32Tocopy = (AFLfuzzer.inputLengthpadded) - AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT;
    	     u32Tocopy = (AFLfuzzer.inputLengthpadded) - AFLfuzzer.inputAFL.u32available - sizeof(uint32_t);
#if defined USE_FREERTOS
	    	 HAL_UART_Receive(co3_huart,co3_usart_input_buffer,u32Tocopy,HAL_MAX_DELAY);
#elif defined USE_CHIBIOS
	    	 sdReadTimeout(co3_huart, (unsigned char *) co3_usart_input_buffer, u32Tocopy, TIME_INFINITE);
#endif
	    	 RingCopy(&AFLfuzzer.inputAFL, (uint8_t * )&AFLfuzzer.inputLength, AFL_BUFFER_STARTING_POINT);
	    	 RingCopy(&AFLfuzzer.inputAFL, Buf, u32Tocopy);


	  	     if( AFLfuzzer.inputLengthpadded == AFLfuzzer.inputAFL.u32available)
	  	     {
                 AFLfuzzer.inputAFL.u32availablenopad = AFLfuzzer.inputLength;
	  	    	 AFLfuzzer.bRXcomplete = 1;
	  	     }
      }
}
#else

void FuzzingInputHandler(uint8_t* Buf, uint32_t *Len)
{

	  BaseType_t xHigherPriorityTaskWoken;
	  xHigherPriorityTaskWoken = pdFALSE;
	  //union ubytes_t auxbytes;
	  uint32_t u32Tocopy;
	  uint8_t error;


	  error = 0;

	  if( AFLfuzzer.inputLength == 0 && error == 0 )
	  {

	      AFLfuzzer.inputLength = *(uint32_t*)Buf;
	      AFLfuzzer.inputLengthpadded  = *(uint32_t*)Buf;

	      if((AFLfuzzer.inputLengthpadded)> MAX_BUFFER_INPUT)
	      {
        	  //u32copied = 0;
        	  //SendBackFault(FAULT_INLEGTH);
        	  error = 1;
          }
      }

      if( AFLfuzzer.inputLengthpadded && (error == 0) )
      {
	    	 u32Tocopy = (AFLfuzzer.inputLengthpadded) - AFLfuzzer.inputAFL.u32available;
	    	 if (u32Tocopy > *Len)
	    	 {
	    		 u32Tocopy = *Len;

	    	 }
	    	 RingCopy(&AFLfuzzer.inputAFL, Buf, u32Tocopy);


	  	     if( AFLfuzzer.inputLengthpadded == AFLfuzzer.inputAFL.u32available)
	  	     {
                 AFLfuzzer.inputAFL.u32availablenopad = AFLfuzzer.inputLength;
	  	    	 AFLfuzzer.bRXcomplete = 1;
    	    	 xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,
	  	    				1, //index
							1, //value = 1 data received
							eSetBits,
							&xHigherPriorityTaskWoken);
	  	     }

      }
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
#endif
