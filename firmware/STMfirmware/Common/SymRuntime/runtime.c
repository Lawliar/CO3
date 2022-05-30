

#include "runtime.h"
#include "protocol.h"
#include "stdlib.h"
#include "string.h"

Symex_t AFLfuzzer;
uint32_t *sym_peripherals[NUMBER_SYM_PERI];
uint8_t total_sym_peripherals;
uint32_t *shadowram;
bool parameter_exp[NUMBER_PARAMETER_EXP];
bool return_exp;


bool _sym_peripheral_symb(uint32_t *addr)
{
   if(total_sym_peripherals<NUMBER_SYM_PERI)
   {
	   sym_peripherals[total_sym_peripherals++] = addr;
	   return true;
   }
   return false;
}


static inline void txCommandtoMonitor(uint8_t size)
{
	//If we don't have more space in the buffer TX the packet
	if (AFLfuzzer.txCurrentIndex + size >= MAX_USB_FRAME  )
	{
	    xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,3,1,eSetValueWithOverwrite); //notify the Monitor to transmit
	    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //get notification when transmission finishes to continue execution
	    //Note: the TxComplete callback will clean buffer after transmission so we will have space for the next function.
	    //It also notifies the target to continue execution
	}
}

void _sym_initialize()
{
    int i;
	total_sym_peripherals = 0;
	shadowram = (uint32_t *)SHADOW_RAM_START;

	memset((void*)shadowram,0x00,SHADOW_RAM_LENGTH);

	for(i=0; i<NUMBER_PARAMETER_EXP; i++)
	{
		parameter_exp[i]=false;
	}
	return_exp = false;
}


void _sym_set_parameter_expression(int8_t para_index, bool input)
{
	if(para_index<NUMBER_PARAMETER_EXP)
	{
		parameter_exp[para_index]= input;
	}
}

bool _sym_get_parameter_expression(int8_t para_index)
{
	if(para_index<NUMBER_PARAMETER_EXP)
	{
		return parameter_exp[para_index];
	}
	return false;
}


void _sym_set_return_expression(bool input)
{
	return_exp = input;
}

bool _sym_get_return_expression()
{
	return return_exp;
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



bool _sym_build_integer(uint32_t int_val, uint8_t numBits, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;

	if(numBits==1)
	{
		msgSize = SIZE_SYM_BLD_INT_1;
		msgCode = SYM_BLD_INT_1;
	}
	else if(numBits==2)
	{
		msgSize = SIZE_SYM_BLD_INT_2;
		msgCode = SYM_BLD_INT_2;
	}
	else
	{
		msgSize = SIZE_SYM_BLD_INT_4;
		msgCode = SYM_BLD_INT_4;
	}


	txCommandtoMonitor(msgSize);                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the ID
	byteval = (uint8_t *)(&symID);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;

	//set the val
	byteval = (uint8_t *)(&int_val);
	for(int i=0;i<numBits;i++)
	{
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++; //set the function in the buffer
	}

	return true;

}


/*
bool _sym_build_float(uint64_t double_val, bool is_double, uint16_t symID);
bool _sym_build_bool(bool bool_val, uint16_t symID);
void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID);
*/


/*

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

void _sym_notify_basic_block(uint8_t id)
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
*/

