// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.


#include "transmit.h"
#include "getTimeStamp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


//#define MEASSURE_READ_ATTEMPTS

#ifdef MEASSURE_READ_ATTEMPTS

FILE *read_num_File = NULL;

#endif
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint64_t u64;

unsigned long long read_timeout = 2000;
u32 write_timeout = 1000;

#define DUMMY_STR "dummy"
ring_buffer_t RingBuffer;

#if defined(CO3_SER2NET)
CO3_SER* initSer(const char * port){
    CO3_SER* ret = (CO3_SER*) malloc(sizeof(CO3_SER));
    if(strcmp(port, DUMMY_STR) == 0){
        ret->tcp_handle = -1;
        ret->used = false;
        ret->total_bytes = 0;
        return ret;
    }
    int sock_no =  initSock(port);
    ret->tcp_handle = sock_no;
    ret->used = true;
    ret->total_bytes = 0;
    return ret;
}
#else
CO3_SER* initSer(const char * port, int baud_rate){
    CO3_SER* ret = (CO3_SER*) malloc(sizeof(CO3_SER));
    if(strcmp(port,"dummy") == 0){
        OpenedSP sp = {NULL, NULL};
        ret->sp = sp;
        ret->used = false;
        ret->total_bytes = 0;
        return ret;
    }
    OpenedSP sp = initSerialPort(port, baud_rate);
    ret->sp = sp;
    ret->used = true;
    ret->total_bytes = 0;
    return ret;
}
#endif

void releaseSer(CO3_SER* ser){
#if defined(CO3_SER2NET)
      close(ser->tcp_handle);
#else
      freeSerialPort(ser->sp);
#endif
    free(ser);
}

inline void sendData(CO3_SER* ser, uint8_t * buf, uint32_t size ){
#if defined(CO3_SER2NET)
    int result = write(ser->tcp_handle, buf, size);
#else
    int result = check(sp_blocking_write(ser->sp.port, buf, size, write_timeout));
#endif
    ser->total_bytes += result;
    if (result != size){
        fprintf(stderr,"Serial Port Timed out, %d/%d bytes sent.\n", result, size);
        abort();
    }
}


#define HEADER_LEN 1
int receiveData(CO3_SER* ser){
    u8 buf[CO3_FRAME_LEN];
#if defined(CO3_SER2NET)
    int result = read(ser->tcp_handle,buf,HEADER_LEN);
    if(result < 0){
        fprintf(stderr,"reading 1-byte long header failed");
        abort();
    }
#else 
    int result = check(sp_blocking_read(ser->sp.port, buf, HEADER_LEN, 1000));
#endif
    ser->total_bytes += result;
    if(result == 0){
        return 0;
    }
    unsigned int packet_len = *(unsigned char *)buf;
    if(packet_len == 1){
        // just the header itself
        return HEADER_LEN;
    }
    // overwriting the header
    unsigned cur = 0;
    uint64_t read_start_time = getTimeStamp();
    while (cur < packet_len - HEADER_LEN){
#if defined(CO3_SER2NET)
        result = read(ser->tcp_handle,buf + cur, packet_len - HEADER_LEN - cur);
#else
        result = check(sp_blocking_read(ser->sp.port, buf, packet_len - HEADER_LEN, 1000));
#endif
        if(result < 0){
            fprintf(stderr,"reading %d bytes failed\n", packet_len - HEADER_LEN - cur);
            abort();
        }
        cur += result;
        ser->total_bytes += result;
        if(getTimeStamp() - read_start_time > read_timeout){
            fprintf(stderr,"reading %u bytes timed out\n", packet_len - HEADER_LEN);
            abort();
        }
    }
    if(cur != packet_len - HEADER_LEN){
        fprintf(stderr,"payload is not as long as what the header says\n");
        abort();
    }
    unsigned freeBytes = ring_buffer_num_empty_items(&RingBuffer);
    if(freeBytes < packet_len){
        fprintf(stderr,"no enough space in the ringbuffer to write %d bytes!\n", freeBytes);
        abort();
    }

    ring_buffer_queue_arr(&RingBuffer, buf, packet_len - HEADER_LEN);
    return packet_len;
}
