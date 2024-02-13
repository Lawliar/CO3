#include "transmit.h"

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

unsigned long long exec_tmout = 1000;
u32 write_timeout = 1000;

#define DUMMY_STR "dummy"
ring_buffer_t RingBuffer;

#if defined(CO3_SER2NET)
CO3_SER initSer(const char * port){
    if(strcmp(port, DUMMY_STR) == 0){
        CO3_SER ret = {-1,false};
        return ret;
    }
    int sock_no =  initTCPSocket(port);
    CO3_SER ret ={sock_no,true};
    return ret;
}
#else
CO3_SER initSer(const char * port, int baud_rate){
    if(strcmp(port,"dummy") == 0){
        OpenedSP sp = {NULL, NULL};
        CO3_SER ret = {sp, false};
        return ret;
    }
    OpenedSP sp = initSerialPort(port, baud_rate);
    CO3_SER ret = {sp, true};
    return ret;
}
#endif

void releaseSer(CO3_SER ser){
#if defined(CO3_SER2NET)
      close(ser.tcp_handle);
#else
      freeSerialPort(ser.sp);
#endif
}

inline void sendData(CO3_SER ser, uint8_t * buf, uint32_t size ){
#if defined(CO3_SER2NET)
    int result = write(ser.tcp_handle, buf, size);
#else
    int result = check(sp_blocking_write(ser.sp.port, buf, size, write_timeout));
#endif
    if (result != size){
        fprintf(stderr,"Serial Port Timed out, %d/%d bytes sent.\n", result, size);
        abort();
    }
}

#define FRAME_LEN 64
#define HEADER_LEN 1
int receiveData(CO3_SER ser){
    u8 buf[FRAME_LEN];
#if defined(CO3_SER2NET)
    int result = read(ser.tcp_handle,buf,HEADER_LEN);
    if(result < 0){
        fprintf(stderr,"reading 1-byte long header failed");
        abort();
    }
#else 
    int result = check(sp_blocking_read(ser.sp.port, buf, HEADER_LEN, 1000));
#endif
    if(result == 0){
        return 0;
    }
    int packet_len = (int)*(char *)buf;
    if(packet_len == 1){
        // just the header itself
        return HEADER_LEN;
    }
    // overwriting the header
    int cur = 0;
    while (cur < packet_len - HEADER_LEN){
#if defined(CO3_SER2NET)
        result = read(ser.tcp_handle,buf + cur, packet_len - HEADER_LEN - cur);
#else
        result = check(sp_blocking_read(ser.sp.port, buf, packet_len - HEADER_LEN, 1000));
#endif
        cur += result;
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
