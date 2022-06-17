#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <libserialport.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "ring.h"

enum {
    SYM_BLD_INT_1 =      1,
    SYM_BLD_INT_2         ,
    SYM_BLD_INT_4         ,
    SYM_BLD_FLOAT         ,
    SYM_BLD_FLOAT_DBL     ,
    SYM_BLD_BOOL          ,
    SYM_BLD_PATH_CNSTR    ,
    SYM_BLD_MEMCPY        ,
    SYM_BLD_MEMSET        ,
    SYM_BLD_MEMMOVE       ,
    SYM_BLD_READ_MEM      ,
    SYM_BLD_WRITE_MEM     ,

    SYM_SET_PAR_EXP       ,
    SYM_GET_PAR_EXP       ,
    SYM_SET_RET_EXP       ,
    SYM_GET_RET_EXP       ,

    SYM_NTFY_PHI          ,
    SYM_NTFY_CALL         ,
    SYM_NTFY_FUNC         ,
    SYM_NTFY_RET          ,
    SYM_NTFY_BBLK         ,
    SYM_NTFY_BBLK1        ,

    SYM_INIT
};

typedef struct _OpenedSP{
    struct sp_event_set* events;
    struct sp_port* port;
} OpenedSP;

OpenedSP initSerialPort(const char * port_name, int);
void freeSerialPort(OpenedSP port);
void setRXReadyEvent(OpenedSP port);
unsigned GetNumBytesWaiting(OpenedSP port);


void sendDataSerialPort(struct sp_port* port, uint8_t * buf, uint32_t size);
void flush_rx_tx_buffers(struct sp_port* port);
void receiveData(struct sp_port* port,unsigned numBytesWaiting,unsigned timeout);

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