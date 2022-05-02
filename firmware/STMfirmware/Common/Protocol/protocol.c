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

	  if( (AFLfuzzer.inputLength != 0 && AFLfuzzer.inputLengthpadded == 0) ||
	   	   AFLfuzzer.inputLength > MAX_BUFFER_INPUT || 	AFLfuzzer.inputLengthpadded> MAX_BUFFER_INPUT )
	  {
	    	  //this is a wrong condition that should never happens,
	    	  //however fuzzing is stressing the whole system so we should clean the whole thing here
	    	  //SendBackFault(FAULT_INLEGTH);
	    	  error = 1;
	  }

	  if( AFLfuzzer.inputLength == 0 && error == 0 )
	  {
	      auxbytes.vbytes[0]=Buf[0];
	      auxbytes.vbytes[1]=Buf[1];
	      auxbytes.vbytes[2]=Buf[2];
	      auxbytes.vbytes[3]=Buf[3];
	      AFLfuzzer.inputLength = auxbytes.vint32;
	      //offset = 4;
	      if(AFLfuzzer.inputLength % 4)
	      {
	      	  AFLfuzzer.inputLengthpadded = AFLfuzzer.inputLength + 4 - AFLfuzzer.inputLength % 4;
	      }
	      else
	      {
	      	  AFLfuzzer.inputLengthpadded = AFLfuzzer.inputLength;
	      }

	      if((AFLfuzzer.inputLengthpadded + 4 )> MAX_BUFFER_INPUT)
	      {
        	  //u32copied = 0;
        	  //SendBackFault(FAULT_INLEGTH);
        	  error = 1;

          }

      }

      if(AFLfuzzer.inputLengthpadded && (error == 0) )
      {


	    	 u32Tocopy = (AFLfuzzer.inputLengthpadded + 4) - AFLfuzzer.inputAFL.u32available;
	    	 if (u32Tocopy > *Len)
	    	 {
	    		 u32Tocopy = *Len;

	    	 }
	    	 RingCopy(&AFLfuzzer.inputAFL, Buf, u32Tocopy);


	  	     if( (AFLfuzzer.inputLengthpadded + 4) == AFLfuzzer.inputAFL.u32available)
	  	     {

	  	    	//if( checkCRC(&AFLfuzzer.inputAFL) )
	  	    	//{

	  	    		AFLfuzzer.inputAFL.u32availablenopad = AFLfuzzer.inputLength;
	  	    		AFLfuzzer.bRXcomplete = 1;

	  	    		//if(AFLfuzzer.inputAFL.u32availablenopad ==0)printf("Zero USB \n");

	  	    		xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskFuzzer,
	  	    				1, //index
							1, //value = 1 data received
							eSetBits,
							&xHigherPriorityTaskWoken);
	  	    	//}
	  	    	//else
	  	    	//{
	  	    		// wrong CRC
	  	    		//u32copied = 0;
	  	    		//SendBackFault(FAULT_COMM);

	  	    	//}

	  	     }


      }
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}
