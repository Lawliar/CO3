#ifndef SERIALPORT_H
#define SERIALPORT_H


#include "ring.h"

#include <string.h>
#include <stdbool.h>

#if defined(CO3_SER2NET)
#include "co3_socket.h"
#else
#include "co3_serialport.h"
#endif


typedef struct _CO3_SER{
#if defined(CO3_SER2NET)
    int tcp_handle;
#else
    OpenedSP sp;
#endif
    bool used;
    uint64_t total_bytes;
} CO3_SER;

void sendData(CO3_SER* ser, uint8_t * buf, uint32_t size);

int receiveData(CO3_SER* ser);


#if defined(CO3_SER2NET)
CO3_SER* initSer(const char *);
#else
CO3_SER* initSer(const char * port_name, int baud_rate);
#endif

void releaseSer(CO3_SER*);

#define CO3_FRAME_LEN 256
/* Get unix time in milliseconds */


/*
static unsigned long get_cur_time_1(void) {

  struct timeval tv;

  gettimeofday(&tv, NULL);

  return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);

}

static unsigned long  get_cur_time_us_1(void) {

  struct timeval tv;
  struct timezone tz;

  gettimeofday(&tv, &tz);

  return (tv.tv_sec * 1000000ULL) + tv.tv_usec;

}*/


#endif 