#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <libserialport.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>


typedef struct _ReceivedBuf{
    unsigned char * buf;
    uint32_t len;
} ReceivedBuf;

typedef struct _OpenedSP{
    struct sp_event_set* events;
    struct sp_port* port;
} OpenedSP;

OpenedSP initSerialPort(const char * port_name, int);
void freeSerialPort(OpenedSP port);
void setRXReadyEvent(OpenedSP port);
int GetNumBytesWaiting(OpenedSP port);


void sendDataSerialPort(struct sp_port* port, uint8_t * buf, uint32_t size);
void flush_rx_tx_buffers(struct sp_port* port);
ReceivedBuf receiveData(struct sp_port* port,unsigned numBytesWaiting,unsigned timeout);

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