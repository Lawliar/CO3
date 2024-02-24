//
// Created by lcm on 2/12/24.
//

#ifndef SYMBACKEND_CO3_SERIALPORT_H
#define SYMBACKEND_CO3_SERIALPORT_H

#include <libserialport.h>

#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct _OpenedSP{
    struct sp_event_set* events;
    struct sp_port* port;
} OpenedSP;

int check(enum sp_return result);
OpenedSP initSerialPort(const char * port_name, int);
void freeSerialPort(OpenedSP port);

void setRXReadyEvent(OpenedSP port);
unsigned GetNumBytesWaiting(OpenedSP port);
void flush_rx_tx_buffers(struct sp_port* port);

#endif //SYMBACKEND_CO3_SERIALPORT_H
