/*
 * protocol.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */


#include"protocol.h"
#include "stdint.h"

Fuzzer_t AFLfuzzer;

void FuzzingInputHandler(uint8_t* Buf, uint32_t *Len)
{

	  BaseType_t xHigherPriorityTaskWoken;
	  xHigherPriorityTaskWoken = pdFALSE;
	  union ubytes_t auxbytes;
	  uint32_t u32Tocopy;
	  uint8_t error;


	  error = 0;


	  if( AFLfuzzer.inputLength == 0 && error == 0 )
	  {

	      AFLfuzzer.inputLength = Buf[0];
	      AFLfuzzer.inputLengthpadded  = Buf[0];;

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
