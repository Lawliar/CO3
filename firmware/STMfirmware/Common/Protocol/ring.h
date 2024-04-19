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

#ifndef SRC_RING_H_
#define SRC_RING_H_



#include "stdint.h"
#include "stdbool.h"
#include "ProtocolConfig.h"


typedef struct
{
uint8_t uxBuffer[MAX_BUFFER_INPUT];
uint32_t u32start;
uint32_t u32end;
uint32_t u32available;
uint32_t u32availablenopad;
bool    overflow;
}RingBuffer_t;


//Function prototypes for ModbusRingBuffer
void RingAdd(RingBuffer_t *xRingBuffer, uint8_t u8Val); // adds a byte to the ring buffer
uint32_t RingGetAllBytes(RingBuffer_t *xRingBuffer, uint8_t *buffer); // gets all the available bytes into buffer and return the number of bytes read
uint32_t RingGetNBytes(RingBuffer_t *xRingBuffer, uint8_t *buffer, uint32_t uNumber); // gets uNumber of bytes from ring buffer, returns the actual number of bytes read
uint32_t RingCountBytes(RingBuffer_t *xRingBuffer); // return the number of available bytes
void RingClear(RingBuffer_t *xRingBuffer); // flushes the ring buffer
void RingZeroes(RingBuffer_t *xRingBuffer); // clear the ring buffer and zeroes the content
void RingCopy(RingBuffer_t *xRingBuffer, uint8_t* u8Buff, uint32_t len);


#endif /* SRC_RING_H_ */
