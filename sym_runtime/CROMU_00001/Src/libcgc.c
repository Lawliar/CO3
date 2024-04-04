/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//#include <sys/mman.h>
#include "libcgc.h"
#include "stdlib.h" // for free and malloc
#include "string.h"
#include "stdint.h"
#include "ProtocolConfig.h"
// lame so we don't have to include over-riding .h files
#define MAP_ANONYMOUS 0x20
#define MAP_PRIVATE 0x02
// #define PROT_READ 0x01
// #define PROT_WRITE 0x02

#define _SC_PAGESIZE 128
// todo prot_exec



/*
void _terminate(unsigned int status){
	_exit(1);
}*/

int transmit(int fd, const void *buf, size_t_cgc count, size_t_cgc *tx_bytes){
	//ssize_t_cgc txb = write(fd, buf, count);
	//if(txb < 0)
	//	return EFAULT;
	//*tx_bytes = (size_t_cgc) txb;
	//return 0;
	return 0;
}
int input_cur = 0;
extern char * rxBuffer;
int receive_cgc( int fd, void *buf, size_t count, size_t *rx_bytes){
    uint32_t total_available = *(uint32_t*)rxBuffer - RX_BUFFER_STARTING_POINT;
    if(input_cur >= total_available){
        return EFAULT;
    }
    else if( (total_available - input_cur) < count ){
        *rx_bytes = (size_t_cgc) total_available - input_cur;
        memcpy(buf, rxBuffer + RX_BUFFER_STARTING_POINT + input_cur, *rx_bytes);
        input_cur = total_available;
        return 0;
    }else{
        memcpy(buf, rxBuffer + RX_BUFFER_STARTING_POINT + input_cur,count);
        input_cur += count;
        *rx_bytes = count;
        return 0;
    }
    return 0;
}


int allocate_cgc(size_t_cgc length, int is_X, void **addr){
	size_t_cgc ps = _SC_PAGESIZE;
	if(length % ps != 0)
		length += (ps - (length % ps));


	//void *c = mmap(NULL, length, PROT_READ | PROT_WRITE,  MAP_ANONYMOUS|MAP_PRIVATE, -1, NULL);
	
	void *c = w_malloc(length);
	if(c != NULL && c != (void *) -1){
		*addr = c;
		return 0;
	}
	// todo see if we can bring errno in ex = errno
	int ex = EFAULT;
	return ex;

}

int deallocate_cgc(void *addr, size_t_cgc length){
	// warning! length not used!

	size_t_cgc ps = _SC_PAGESIZE;//sysconf(_SC_PAGESIZE);
	if(length % ps != 0)
		length += (ps - (length % ps));

	w_free(addr);
	return 0;
	//return munmap(addr, length);
}

// int random_cgc(void *buf, size_t_cgc count, size_t_cgc *rnd_bytes){
// 	size_t_cgc read_sz = 0;
// 	// 0x00 is for rdonly
// 	int rndfd = open("/dev/urandom", 0x00);
	
// 	ssize_t_cgc res = read(rndfd, (char *) buf, count);
// 	if(res < 0)
// 		return EFAULT;
// 	*rnd_bytes = (size_t_cgc) res;
// 	return 0;

// }
