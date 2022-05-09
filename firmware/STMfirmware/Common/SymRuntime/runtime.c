

#include "runtime.h"
#include "protocol.h"

Symex_t AFLfuzzer;

static inline void txCommandtoMonitor(uint8_t size)
{
	//If we don't have more space in the buffer or we already have 8 functions TX the packet
	if (AFLfuzzer.txCurrentIndex + size >= MAX_USB_FRAME || AFLfuzzer.txTotalFunctions>=8 )
	{
	    xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,3,1,eSetValueWithOverwrite); //notify the Monitor to transmit
	    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //get notification when transmission finishes to continue execution
	    //Note: Monitor will clean buffer after transmission so we will have space for the next function
	}
}



void _sym_initialize()
{

	txCommandtoMonitor(SIZE_SYM_INIT);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SYM_INIT; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txCurrentIndex++;
	AFLfuzzer.txTotalFunctions++;


}


static inline void set_id(uint32_t userID)
{

	union ubytes_t aux;
	aux.vuint32 = userID;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[0];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[1];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[2];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[3];

}


static inline void get_report(uint8_t * arg)
{

	uint8_t arg_id, width;
	arg_id = arg[0];
	width = arg[1];

	if(width>4)
	{
		width = 4;
	}
	if(width==0)
	{
		width = 1;
	}


	uint32_t  *aux32;

	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= arg_id;

	aux32 = (uint32_t*)(&AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex]);
    *aux32 = 0;

	for(uint8_t i=0; i<width; i++)
	{
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= arg[i+2];
	}
	for(uint8_t i=0; i<(4-width); i++)
	{
			AFLfuzzer.txCurrentIndex++;
	}



}


void _spear_report1(uint32_t userID, char * arg1)
{

	txCommandtoMonitor(SIZE_SPEAR_RPT1);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SPEAR_RPT1; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
}


void _spear_report2(uint32_t userID, char * arg1,char * arg2)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT2);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SPEAR_RPT2; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
	get_report( (uint8_t*) arg2);

}


void _spear_report3(uint32_t userID, char * arg1,char * arg2,char * arg3)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT3);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SPEAR_RPT3; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
	get_report( (uint8_t*) arg2);
	get_report( (uint8_t*) arg3);
}


void _spear_report4(uint32_t userID, char * arg1,char * arg2,char * arg3,char * arg4)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT4);
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SPEAR_RPT4; //set the function in the buffer
    if(AFLfuzzer.txTotalFunctions)
    {
    	//write the index where the current function starts
    	AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
    }
    AFLfuzzer.txTotalFunctions++;
    AFLfuzzer.txCurrentIndex++;
    set_id(userID);
    get_report( (uint8_t*) arg1);
    get_report( (uint8_t*) arg2);
    get_report( (uint8_t*) arg3);
    get_report( (uint8_t*) arg4);
}

void _spear_symbolize(char *address, uint32_t len)
{
	txCommandtoMonitor(SIZE_SPEAR_SYM);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SPEAR_SYM; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id((uint32_t)address);
	set_id((uint32_t)len);

}


void _sym_notify_call(uint32_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_CALL);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SYM_N_CALL; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(id);
}

void _sym_notify_ret(uint32_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_RET);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SYM_N_RET; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(id);
}
void _sym_notify_basic_block(uint32_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_BB);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex] = SYM_N_BB; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		AFLfuzzer.txbuffer[AFLfuzzer.txTotalFunctions]=AFLfuzzer.txCurrentIndex;
	}
	AFLfuzzer.txTotalFunctions++;
	AFLfuzzer.txCurrentIndex++;
	set_id(id);
}

