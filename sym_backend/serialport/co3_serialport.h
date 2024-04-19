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
