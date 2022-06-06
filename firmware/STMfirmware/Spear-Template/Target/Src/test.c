/*
 * test.c
 *
 *  Created on: Sep 3, 2021
 *      Author: alejandro
 */
#include "FreeRTOS.h"
#include "task.h"
#include "test.h"
#include "stdlib.h"



uint32_t bufferGlobal[10]; // accessing this buffer should trigger a memfault since there is no MPU region for it

int test(uint8_t *buf, uint32_t size)
{


	int i = 0;
	for (; i < size ; i ++ ){
		buf[0] += buf[0];
	}
	if(buf[0] == 'H'){
		return 0;
	}
	return 1;


}

