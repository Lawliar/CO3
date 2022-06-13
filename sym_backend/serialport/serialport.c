#include "serialport.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



//#define MEASSURE_READ_ATTEMPTS

#ifdef MEASSURE_READ_ATTEMPTS

FILE *read_num_File = NULL;

#endif
typedef uint32_t u32;
typedef uint8_t u8;
typedef uint64_t u64;

unsigned long long exec_tmout = 1000;
u32 write_timeout = 1000;


ring_buffer_t RingBuffer;

/* Helper function for error handling. */
int check(enum sp_return result)
{
	/* For this example we'll just exit on any error by calling abort(). */
	char *error_message;

	switch (result) {
	case SP_ERR_ARG:
		printf("Error: Invalid argument.\n");
		abort();
	case SP_ERR_FAIL:
		error_message = sp_last_error_message();
		printf("Error: Failed: %s\n", error_message);
		sp_free_error_message(error_message);
		abort();
	case SP_ERR_SUPP:
		printf("Error: Not supported.\n");
		abort();
	case SP_ERR_MEM:
		printf("Error: Couldn't allocate memory.\n");
		abort();
	case SP_OK:
	default:
		return result;
	}
}

void flush_rx_tx_buffers(struct sp_port* port){
	check(sp_flush(port, SP_BUF_BOTH));
}

OpenedSP initSerialPort(const char * port_name, int baud_rate){
    OpenedSP ret = {.port = NULL, .events=NULL};
    if(strcmp(port_name,"dummy") == 0){
        return ret;
    }
    printf("Looking for port %s.\n", port_name);
    check(sp_get_port_by_name(port_name, &ret.port));

    printf("Opening port.\n");
    check(sp_open(ret.port, SP_MODE_READ_WRITE));

    printf("Setting port to 9600 8N1, no flow control.\n");
	if (baud_rate == 0){
		check(sp_set_baudrate(ret.port, 9600));
	}
	else{
		check(sp_set_baudrate(ret.port, baud_rate));
	}

	check(sp_set_bits(ret.port, 8));
    check(sp_set_parity(ret.port, SP_PARITY_NONE));
    check(sp_set_stopbits(ret.port, 1));
    check(sp_set_flowcontrol(ret.port, SP_FLOWCONTROL_NONE));

    sp_new_event_set(&ret.events);
    check(sp_add_port_events(ret.events, ret.port, SP_EVENT_RX_READY));
    //init the ring buffer as well
    ring_buffer_init(&RingBuffer);
    return ret;
}

void freeSerialPort(OpenedSP sp){
    if(sp.events != NULL){
        sp_free_event_set(sp.events);
    }
    if(sp.port != NULL){
        check(sp_close(sp.port));
        sp_free_port(sp.port);
    }

}


int GetNumBytesWaiting(OpenedSP sp){
    int bytes_waiting = check(sp_input_waiting(sp.port));
    return bytes_waiting;
}


void setRXReadyEvent(OpenedSP sp){
    check(sp_add_port_events(sp.events, sp.port, SP_EVENT_RX_READY));
}
inline void sendDataSerialPort(struct sp_port* port, uint8_t * buf, uint32_t size ){
    int result = check(sp_blocking_write(port, buf, size, write_timeout));
    if (result != size){
        fprintf(stderr,"Serial Port Timed out, %d/%d bytes sent.\n", result, size);
        abort();
    }
}


void receiveData(struct sp_port* port, unsigned numBytesWaiting, unsigned timeout){
    if(numBytesWaiting == 0 ){
        return;
    }
    u8 * buf = (u8 *)malloc(numBytesWaiting + 1);
    int result = check(sp_blocking_read(port, buf, numBytesWaiting, timeout));
    buf[result] = '\0';
    unsigned freeBytes = ring_buffer_num_empty_items(&RingBuffer);
    if(freeBytes < numBytesWaiting){
        fprintf(stderr,"no enough space in the ringbuffer to write %d bytes!\n", freeBytes);
        abort();
    }

    ring_buffer_queue_arr(&RingBuffer, buf, numBytesWaiting);
    return;
}
