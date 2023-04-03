/*
 * wrapped_malloc.h
 *
 *  Created on: Feb 24, 2023
 *      Author: lcm
 */

#ifndef WRAPPED_MALLOC_H_
#define WRAPPED_MALLOC_H_


#define MAX_ALLOC 100


void * w_malloc(int numBytes);
void w_free(void *);
void whole_free();

#endif /* WRAPPED_MALLOC_H_ */
