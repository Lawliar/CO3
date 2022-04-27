#include "serialport.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


unsigned int receiveBufSize = 16 * 1024;

//#define MEASSURE_READ_ATTEMPTS

#ifdef MEASSURE_READ_ATTEMPTS

FILE *read_num_File = NULL;

#endif
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned long long u64;

unsigned long long exec_tmout = 1000;
u32 write_timeout = 1000;


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
    return ret;
}

void freeSerialPort(OpenedSP port){
    sp_free_event_set(port.events);
    check(sp_close(port.port));
    sp_free_port(port.port);
}

inline void sendDataSerialPort(struct sp_port* port, uint8_t * buf, uint32_t size ){
    int result = check(sp_blocking_write(port, buf, size, write_timeout));
    if (result != size){
        fprintf(stderr,"Serial Port Timed out, %d/%d bytes sent.\n", result, size);
        abort();
    }
}


ReceivedBuf receiveDataSerial(struct sp_port* port){
	ReceivedBuf ret = {.buf = NULL, .len = 0};

	u32 unfilled_bf_sz = sizeof(u8) * receiveBufSize;
	u8 * buf = (u8 *)malloc(unfilled_bf_sz);

	// read in the header first
	u32 single_transfer = 0;
	u32 total_transfer = 0;
	u64 start_read_8bytes_time = get_cur_time_us_1();
	total_transfer = single_transfer = check(sp_blocking_read(port, buf, 8, exec_tmout));
	if(single_transfer < 8){
		fprintf(stderr,"cannot read anything from usb timed out %lld ms\n", exec_tmout);
		free(buf);
		return ret;
	}
	u64 end_read_8bytes_time = get_cur_time_us_1();
	
#ifdef MEASSURE_READ_ATTEMPTS
	if (read_num_File == NULL){
		read_num_File = fopen("read_attempts.txt", "a");
	}
	fprintf(read_num_File, "read 8 bytes : %lld us\n", end_read_8bytes_time - start_read_8bytes_time);
#endif
	// get the payload length
	u32 exit_code = *((u32*)buf);
	u32 bm_sz = *((u32*)buf + 1);
	u32 rectified_bf_sz = bm_sz + 8;
	// some sanity check
	if(rectified_bf_sz < total_transfer){
		fprintf(stderr,"rectified length is less than already transferred?\n");
		free(buf);
		return ret;
	}
	
	buf = (u8 *)realloc(buf, rectified_bf_sz);
	if(buf == NULL){
        fprintf(stderr, "realloc the receiving buffer failed");
	}
	// adjust unfilled buf size 
	unfilled_bf_sz = rectified_bf_sz - total_transfer;

	int result = check(sp_blocking_read(port, buf + total_transfer, unfilled_bf_sz, exec_tmout));
	if(result != unfilled_bf_sz){
		fprintf(stderr,"only read the header but the payload timed out  %lld\n", exec_tmout);
		free(buf);
		return ret;
	}
	total_transfer += unfilled_bf_sz;
	ret.buf = buf;
	ret.len = total_transfer;
	return ret;
}
