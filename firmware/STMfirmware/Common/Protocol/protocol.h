// This file is part of the CO3 runtime.
//
// The CO3 runtime is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The CO3 runtime is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CO3. If not, see <https://www.gnu.org/licenses/>.

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "ProtocolConfig.h"

#if defined CO3_USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#elif defined CO3_USE_CHIBIOS
#include "ch.h"
#include "hal.h"
#endif



#include "ring.h"

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
#ifdef CO3_USE_FREERTOS
    TaskHandle_t xTaskMonitor;
    TaskHandle_t xTaskTarget;
#elif defined CO3_USE_CHIBIOS
    thread_t *xTaskMonitor;
    thread_t *xTaskTarget;
#endif
    uint8_t  txbuffer[MAX_USB_FRAME];
    uint32_t txCurrentIndex;
    uint32_t txTotalFunctions;
    uint8_t  txFunctionIndexes[MAX_USB_FUNCTIONS-1]; //index0 is tacit


}Symex_t;

extern Symex_t AFLfuzzer;
extern uint8_t notiTarget;

#if defined CO3_USE_CHIBIOS
// control related communication
#define FUZZER_TARGET_READY EVENT_MASK(31)
#define TARGET_GO_AHEAD EVENT_MASK(30)
#define TARGET_SHOULD_KILL_SELF EVENT_MASK(29)
#define TRANSMIT_FINISHED EVENT_MASK(28)
// status related communication
#define ASAN_FAULT_CH EVENT_MASK(1)
#define FAULT_CRASH_CH EVENT_MASK(2)
#define NO_CHIBIOS_CH EVENT_MASK(3)
#define MORE_DATA_TO_COME EVENT_MASK(4)
#define NO_MORE_DATA_TO_COME EVENT_MASK(5)
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 512 )
#endif

void FuzzingInputHandler(uint8_t* Buf, uint32_t *Len);
void PackFunction(uint8_t *Buf, uint32_t Len);
void TransmitPack(void);
void notifyTXfinish();
void SerialReceiveInput();

// SHIFT stuff

#define b2(x)   (   (x) | (   (x) >> 1) )
#define b4(x)   ( b2(x) | ( b2(x) >> 2) )
#define b8(x)   ( b4(x) | ( b4(x) >> 4) )
#define b16(x)  ( b8(x) | ( b8(x) >> 8) )
#define b32(x)  (b16(x) | (b16(x) >>16) )
#define next_power_of_2(x)      (b32(x-1) + 1)

#endif /* PROTOCOL_H_ */
