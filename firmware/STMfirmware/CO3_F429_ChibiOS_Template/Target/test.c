/*
 * test.c
 *
 *  Created on: Sep 3, 2021
 *      Author: alejandro
 */
#include "test.h"
//#include "stdio.h"
//#include "afl.h"
//#include "stdlib.h"
//#include "McuASAN.h"



//#if DUALCOREFUZZ

//#else
//uint8_t AFLfuzzerRegion[AFLINPUTREGION_SIZE ] __attribute__( ( aligned( AFLINPUTREGION_SIZE ) ) );
//#endif


#define SMALL_SEARCH_SPACE

uint32_t bufferGlobal[10]; // accessing this buffer should trigger a memfault since there is no MPU region for it

int test(uint8_t *buf, uint32_t size)
{

	uint8_t *localbuff;
    uint32_t arr32[4];
	uint16_t *ptr16;



	if(size<21)
	{
		return 0; //normal execution does not trigger any bug
	}

#ifndef SMALL_SEARCH_SPACE
	if(buf[0] == 'T' && \
		buf[1] == 'H' && \
		buf[2] == 'I' && \
		buf[3] == 'S' && \
		buf[4] == 'A' && \
		buf[5] == 'h' && \
		buf[6] == 'a' && \
		buf[7] == 'n' && \
		buf[8] == 'g' )
#else
	if(buf[0] == 'H' && \
				buf[1] == 'A' && \
				buf[2] == 'N' && \
				buf[3] == 'G' )
#endif
	{

		buf[100]='T';
		//while(1);  //Hang the task will be killed  by FreeRTOS
	}

	/***** MPU specific *******/
#ifndef SMALL_SEARCH_SPACE
	else if(buf[0] == 'S' && \
			buf[1] == 'e' && \
			buf[2] == 'G' && \
			buf[3] == 'F' && \
			buf[4] == 'A' && \
			buf[5] == 'u' && \
			buf[6] == 'l' && \
			buf[7] == 't' && \
			buf[8] == '!')
#else
	else if(buf[0] == 'S' \
					&& buf[1] == 'E' \
					&& buf[2] == 'G')
#endif
	{
		bufferGlobal[0]++; // Segmentation fault accessing a global variable outside the scope of the current task
		//return FAULT_CRASH; //this  line is not executed
	}

#ifndef SMALL_SEARCH_SPACE
	else if(buf[0] == 'n' && \
			buf[1] == 'U' && \
			buf[2] == 'l' && \
			buf[3] == 'L' && \
			buf[4] == 'd' && \
			buf[5] == 'e' && \
			buf[6] == 'r' && \
			buf[7] == 'e' && \
			buf[8] == 'f') //Null dereference
#else
		else if(buf[0] == 'N' && \
				buf[1] == 'U' && \
				buf[2] == 'L' && \
				buf[2] == 'L')
#endif
	{
			localbuff = 0x00;
			buf[0] = *localbuff;

	}

	/**** Cortex M specific *********/
#ifndef SMALL_SEARCH_SPACE
	else if(buf[0] == 'd' && \
			buf[1] == 'i' && \
			buf[2] == 'v' && \
			buf[3] == 'b' && \
			buf[4] == 'y' && \
			buf[5] == 'z' && \
			buf[6] == 'e' && \
			buf[7] == 'r' && \
			buf[8] == 'o' && \
			buf[9] == '\0')
#else
		else if(buf[0] == 'Z' \
				&& buf[1] == 'E' \
				&& buf[2] == 'R' \
				&& buf[3] == '0' \
				&& buf[9] == '\0')
#endif
	{
		arr32[0]= buf[0] / (buf[9]); //this should trigger a division by zero
	}
#ifndef SMALL_SEARCH_SPACE
	else if(buf[0] == 'u' && \
			buf[1] == 'N' && \
			buf[2] == 'd' && \
			buf[3] == 'e' && \
			buf[4] == 'f' && \
			buf[5] == 'i' && \
			buf[6] == 'n' && \
			buf[7] == 's' && \
			buf[8] == 't' && \
			buf[9] == 'r')
#else
		else if(buf[0] == 'U' \
				&& buf[1] == 'D' \
				&& buf[2] == 'F')
#endif
	{
		__asm volatile("nop"); //this should trigger an undefined instruction exception
	}

#ifndef SMALL_SEARCH_SPACE
	else if(buf[0] == 'U' && \
			buf[1] == 'n' && \
			buf[2] == 'a' && \
			buf[3] == 'g' && \
			buf[4] == 'l' && \
			buf[5] == 'I' && \
			buf[6] == 'n' && \
			buf[7] == 'e' && \
			buf[8] == 'd' && \
			buf[9] == 'r' )
#else
		else if(buf[0] == 'U' && \
				buf[1] == 'N' && \
				buf[2] == 'A' )
#endif
	{
		arr32[0]= (uint32_t)buf[3];
		arr32[1]= (uint32_t)buf[4];
		localbuff = (uint8_t*)&arr32[0];
		localbuff += 3;
		ptr16 = (uint16_t *)localbuff;
		buf[0]= *ptr16; // unaligned access
	}


	 /**** ASAN specific ********/

#ifndef SMALL_SEARCH_SPACE
    else if(buf[0] == 'U' && \
			buf[1] == 's' && \
			buf[2] == 'E' && \
			buf[3] == 'A' && \
			buf[4] == 'F' && \
			buf[5] == 't' && \
			buf[6] == 'E' && \
			buf[7] == 'r' && \
			buf[8] == 'f' && \
			buf[9] == 'R' && \
			buf[10] == 'e' && \
			buf[11] == 'E')
#else
        else if(buf[0] == 'U' && \
        		buf[1] == 'A' && \
    			buf[2] == 'F')
#endif
    {
    	//localbuff =  malloc(32);
    	localbuff = NULL;
    	if(localbuff)
    	{
    		localbuff[0] = buf[0];
    		localbuff[1] = buf[0] + buf[2];
    		//free(localbuff);
    		buf[0]=localbuff[1]; // use after free
    	}

    }
#ifndef SMALL_SEARCH_SPACE
    else if(buf[0] == 'd' && \
			buf[1] == 'O' && \
			buf[2] == 'u' && \
			buf[3] == 'B' && \
			buf[4] == 'l' && \
			buf[5] == 'E' && \
			buf[6] == 'F' && \
			buf[7] == 'R' && \
			buf[8] == 'e' && \
			buf[9] == 'E')
#else
        else if(buf[0] == '2' && \
        		buf[1] == 'F' && \
    			buf[2] == 'R')
#endif
    {

    		//localbuff =  malloc(32);
    	localbuff = NULL;
    		if(localbuff)
    		{
    			localbuff[0] = buf[0];
    			localbuff[1] = buf[0] + buf[2];
    			//free(localbuff);
    		}
    		//free(localbuff); //double free

    }

#ifndef SMALL_SEARCH_SPACE
    else if(buf[0] == 'i' && \
			buf[1] == 'N' && \
			buf[2] == 'v' && \
			buf[3] == 'a' && \
			buf[4] == 'L' && \
			buf[5] == 'i' && \
			buf[6] == 'D' && \
			buf[7] == 'F' && \
			buf[8] == 'r' && \
			buf[9] == 'e' && \
			buf[10] == 'E')
#else
        else if(buf[0] == 'I' \
        		&& buf[1] == 'F' \
    			&& buf[2] == 'R')
#endif
    {

    	//localbuff =  malloc(32);
    	localbuff = NULL;
    	if(localbuff)
    	{
    		localbuff[0] = buf[0];
    		localbuff[1] = buf[0] + buf[2];
    	}
    	localbuff+=10;
    	//free(localbuff); //invalid free

    }

#ifndef SMALL_SEARCH_SPACE
    else if(buf[0] == 's' && \
			buf[1] == 'T' && \
			buf[2] == 'a' && \
			buf[3] == 'c' && \
			buf[4] == 'O' && \
			buf[5] == 'v' && \
			buf[6] == 'e' && \
			buf[7] == 'R' && \
			buf[8] == 'F' && \
			buf[9] == 'L' && \
			buf[10] == 'o' && \
			buf[11] == 'w') //overflow in stack
#else
        else if(buf[0] == 'O' && \
        		buf[1] == 'F' && \
    			buf[2] == 'S')
#endif
    {

    	buf[0] = arr32[buf[4]];

    }
#ifndef SMALL_SEARCH_SPACE
    else if(buf[0] == 'h' && \
			buf[1] == 'E' && \
			buf[2] == 'a' && \
			buf[3] == 'P' && \
			buf[4] == 'o' && \
			buf[5] == 'V' && \
			buf[6] == 'E' && \
			buf[7] == 'r' && \
			buf[8] == 'f' && \
			buf[9] == 'l' && \
			buf[10] == 'O' && \
			buf[11] == 'W') //overflow in heap
#else
        else if(buf[0] == 'O' \
        		&& buf[1] == 'F' \
    			&& buf[2] == 'H' )
#endif
    {
    	//localbuff =  malloc(8);
    	localbuff = NULL;
    	if(localbuff)
    	{
    		localbuff[9]=10;//overflow in heap
    	//	free(localbuff);
    	}

    }

	return 0; //normal execution does not trigger any bug


}

