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
#include "protocol.h"
//#include "stdio.h"
// lame so we don't have to include over-riding .h files
#define MAP_ANONYMOUS 0x20
#define MAP_PRIVATE 0x02
// #define PROT_READ 0x01
// #define PROT_WRITE 0x02

#define _SC_PAGESIZE 128
// todo prot_exec

#define USE_USB_AS_INPUT

#define string_literal "123456789"
#ifdef USE_USB_AS_INPUT
extern Symex_t AFLfuzzer;
#else
char * input = string_literal;
unsigned available_input = sizeof(string_literal) - 1;
#endif

unsigned int cur = 0;

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

int receive_cgc( void *buf, size_t_cgc count, size_t_cgc *rx_bytes){
#ifdef USE_USB_AS_INPUT
	//printf("cur:%d\n",cur);
	if(cur >= AFLfuzzer.inputAFL.u32available){
		return EFAULT;
	}
	else if( (AFLfuzzer.inputAFL.u32available - cur) < count ){
		memcpy(buf, AFLfuzzer.inputAFL.uxBuffer + 1 + cur, AFLfuzzer.inputAFL.u32available - cur);
		cur = AFLfuzzer.inputAFL.u32available;
		*rx_bytes = (size_t_cgc) AFLfuzzer.inputAFL.u32available - cur;
		return 0;
	}else{
		memcpy(buf, AFLfuzzer.inputAFL.uxBuffer + 1 + cur,count);
		cur += count;
		*rx_bytes = count;
		return 0;
	}
#else
	if(cur >= available_input){
		return EFAULT;
	}
	else if( (available_input - cur) < count ){
		memcpy(buf, input + cur, available_input - cur);
		cur = available_input;
		*rx_bytes = (size_t_cgc) available_input - cur;
		return 0;
	}else{
		memcpy(buf, input + cur,count);
		cur += count;
		*rx_bytes = count;
		return 0;
	}
#endif
		
	return 0;
}


int allocate_cgc(size_t_cgc length, int is_X, void **addr){
	size_t_cgc ps = _SC_PAGESIZE;
	if(length % ps != 0)
		length += (ps - (length % ps));


	//void *c = mmap(NULL, length, PROT_READ | PROT_WRITE,  MAP_ANONYMOUS|MAP_PRIVATE, -1, NULL);
	
	void *c = malloc(length);
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

	free(addr);
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
