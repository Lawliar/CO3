/*
 * protocol.h
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "FreeRTOS.h"
#include "task.h"
#include "ring.h"


union ubytes_t{
	 uint8_t vbytes [4];
	 uint16_t vuwords[2];
	 int16_t  vwords[2];
	 int32_t  vint32;
	 uint32_t vuint32;

};



typedef struct
{

	RingBuffer_t inputAFL;
	uint32_t inputLength;
    uint32_t inputLengthpadded;
    uint16_t indexdif;
    volatile bool bRXcomplete;  // variable must be declared as volatile otherwise the compiler may optimize out it
    volatile bool bTXcomplete;
    volatile bool breceiving;
    TaskHandle_t xTaskMonitor;
    TaskHandle_t xTaskTarget;

}Fuzzer_t;

extern Fuzzer_t AFLfuzzer;

void FuzzingInputHandler(uint8_t* Buf, uint32_t *Len);

#endif /* PROTOCOL_H_ */
