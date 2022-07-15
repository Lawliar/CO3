/*
 * protocol.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */


#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"
#include "stdint.h"
#include "usbd_cdc_if.h"


extern Symex_t AFLfuzzer;

uint8_t notiTarget;

// Callback executed on USB TX complete ISR
void notifyTXfinish()
{
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;


	//cleaning the packet buffer to receive new messages
	/*
	AFLfuzzer.txTotalFunctions=0;
	for(uint8_t i=1; i<8; i++)
	{
		AFLfuzzer.txbuffer[i]=0;
	}
	}*/

	AFLfuzzer.txCurrentIndex=1;  //we reserve the first byte for size
	//clean the tx buffer
	for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
	{
		AFLfuzzer.txbuffer[j]=0;
	}
	AFLfuzzer.txTotalFunctions=0;



	//notify the target to continue execution
	if (notiTarget == NOTI_TARGET)
	{
	xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskTarget,
  	  	    				1, //index
  							1, //value = 1 data TX complete
  							eSetBits,
  							&xHigherPriorityTaskWoken);
	}
	else
	{
		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,
		  	  	    				2, //index
		  							1, //value = 1 data TX complete
		  							eSetBits,
		  							&xHigherPriorityTaskWoken);
	}

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

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
		AFLfuzzer.txbuffer[0]= AFLfuzzer.txCurrentIndex-1; //set the total length of the payload without considering size itself
		CDC_Transmit_FS(AFLfuzzer.txbuffer, AFLfuzzer.txCurrentIndex); //transmit data
	}

}


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
