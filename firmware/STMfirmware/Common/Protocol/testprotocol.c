/*
 * testprotocol.c
 *
 *  Created on: May 3, 2022
 *      Author: alejandro
 */
#include "testprotocol.h"
#include "runtime.h"


volatile uint8_t  buffer[128];


void testprotocol(uint8_t numberfunc)
{
    for(int i=0; i<sizeof(buffer); i++)
    {
    	buffer[i]=i;
	}

    _sym_initialize();
    _spear_report1(0xDEADBEE1, (char*) &buffer[0]);
    _spear_report2(0xDEADBEE2, (char*) &buffer[0], (char*)&buffer[10]);
    _spear_report3(0xDEADBEE3, (char*)&buffer[0], (char*)&buffer[10], (char*)&buffer[20]);
    _spear_report4(0xDEADBEE4, (char*)&buffer[0], (char*)&buffer[10], (char*)&buffer[20], (char*)&buffer[30]);
    _spear_symbolize((char*)buffer, 128);
    _sym_notify_call(0xDEADBEE5);
    _sym_notify_ret(0xDEADBEE6);
    _sym_notify_basic_block(0xDEADBEE7);

}
