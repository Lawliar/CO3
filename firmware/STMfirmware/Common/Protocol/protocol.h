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
#include "timers.h"
#include "ring.h"
#include "ProtocolConfig.h"
#include "runtime.h"

#define NOTI_TARGET 0
#define NOTI_MONITOR 1

union ubytes_t{
	 uint8_t vbytes [4];
	 uint16_t vuwords[2];
	 int16_t  vwords[2];
	 int32_t  vint32;
	 uint32_t vuint32;

};


//SHIFT STUFF
enum {
  /* 00 */ FAULT_NONE,
  /* 01 */ FAULT_TMOUT,
  /* 02 */ FAULT_CRASH,
  /* 03 */ FAULT_ERROR,
  /* 04 */ FAULT_NOINST,
  /* 05 */ FAULT_NOBITS,
  /* 06 */ FAULT_COMM,
  /* 07 */ FAULT_INLEGTH,
  /* 08 */ FAULT_NONE_RTOS,
  /* 09 */ FAULT_ASAN,
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

    uint8_t  txbuffer[MAX_USB_FRAME];
    uint32_t txCurrentIndex;
    uint32_t txTotalFunctions;
    uint8_t  txFunctionIndexes[MAX_USB_FUNCTIONS-1]; //index0 is tacit


}Symex_t;

extern Symex_t AFLfuzzer;
extern uint8_t notiTarget;

void FuzzingInputHandler(uint8_t* Buf, uint32_t *Len);
void PackFunction(uint8_t *Buf, uint32_t Len);
void TransmitPack(void);
void notifyTXfinish();


#endif /* PROTOCOL_H_ */
