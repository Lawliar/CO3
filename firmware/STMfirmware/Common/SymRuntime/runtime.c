/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */

#include "runtime.h"
#include "protocol.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))


#define lowByte(w) ((w) & 0xff)
#define highByte(w) ((w) >> 8)


#if DEBUGPRINT==1
#define txCommandtoMonitorF  txCommandtoMonitor(msgSize,msgCode)

#else

#define txCommandtoMonitorF  txCommandtoMonitor(msgSize)

#endif




#if DEBUGPRINT ==1

 char S_SYM_DUMMY           []="SYM_DUMMY         ";
 char S_SYM_BLD_INT_1       []="SYM_BLD_INT_1     ";
 char S_SYM_BLD_INT_2       []="SYM_BLD_INT_2     ";
 char S_SYM_BLD_INT_4       []="SYM_BLD_INT_4     ";
 char S_SYM_BLD_FLOAT       []="SYM_BLD_FLOAT     ";
 char S_SYM_BLD_FLOAT_DBL   []="SYM_BLD_FLOAT_DBL ";
 char S_SYM_BLD_BOOL        []="SYM_BLD_BOOL      ";
 char S_SYM_BLD_PATH_CNSTR  []="SYM_BLD_PATH_CNSTR";
 char S_SYM_BLD_MEMCPY      []="SYM_BLD_MEMCPY    ";
 char S_SYM_BLD_MEMSET      []="SYM_BLD_MEMSET    ";
 char S_SYM_BLD_MEMMOVE     []="SYM_BLD_MEMMOVE   ";
 char S_SYM_BLD_READ_MEM    []="SYM_BLD_READ_MEM  ";
 char S_SYM_BLD_WRITE_MEM   []="SYM_BLD_WRITE_MEM ";
 char S_SYM_SET_PAR_EXP     []="SYM_SET_PAR_EXP   ";
 char S_SYM_GET_PAR_EXP     []="SYM_GET_PAR_EXP   ";
 char S_SYM_SET_RET_EXP     []="SYM_SET_RET_EXP   ";
 char S_SYM_GET_RET_EXP     []="SYM_GET_RET_EXP   ";
 char S_SYM_NTFY_PHI        []="SYM_NTFY_PHI      ";
 char S_SYM_NTFY_CALL       []="SYM_NTFY_CALL     ";
 char S_SYM_NTFY_FUNC       []="SYM_NTFY_FUNC     ";
 char S_SYM_NTFY_RET        []="SYM_NTFY_RET      ";
 char S_SYM_NTFY_BBLK       []="SYM_NTFY_BBLK     ";
 char S_SYM_NTFY_BBLK1      []="SYM_NTFY_BBLK1    ";
 char S_SYM_INIT            []="SYM_INIT          ";

 char *fstrings[]={

		 S_SYM_DUMMY          ,
		 S_SYM_BLD_INT_1      ,
		 S_SYM_BLD_INT_2      ,
		 S_SYM_BLD_INT_4      ,
		 S_SYM_BLD_FLOAT      ,
		 S_SYM_BLD_FLOAT_DBL  ,
		 S_SYM_BLD_BOOL       ,
		 S_SYM_BLD_PATH_CNSTR ,
		 S_SYM_BLD_MEMCPY     ,
		 S_SYM_BLD_MEMSET     ,
		 S_SYM_BLD_MEMMOVE    ,
		 S_SYM_BLD_READ_MEM   ,
		 S_SYM_BLD_WRITE_MEM  ,
		 S_SYM_SET_PAR_EXP    ,
		 S_SYM_GET_PAR_EXP    ,
		 S_SYM_SET_RET_EXP    ,
		 S_SYM_GET_RET_EXP    ,
		 S_SYM_NTFY_PHI       ,
		 S_SYM_NTFY_CALL      ,
		 S_SYM_NTFY_FUNC      ,
		 S_SYM_NTFY_RET       ,
		 S_SYM_NTFY_BBLK      ,
		 S_SYM_NTFY_BBLK1     ,
		 S_SYM_INIT
 };

#endif





Symex_t AFLfuzzer;
uint32_t *sym_peripherals[NUMBER_SYM_PERI];
uint8_t total_sym_peripherals;
uint32_t *shadowram;
bool parameter_exp[NUMBER_PARAMETER_EXP];
bool return_exp;


//return address
uint32_t AddressToShadow(char *addr);

//return true if byte is symbolic
bool checkSymbolic(char *addr);


bool _sym_peripheral_symb(uint32_t *addr)
{
   if(total_sym_peripherals<NUMBER_SYM_PERI)
   {
	   sym_peripherals[total_sym_peripherals++] = addr;
	   return true;
   }
   return false;
}

#if DEBUGPRINT ==1
void txCommandtoMonitor(uint8_t size, uint8_t func)
#else
void txCommandtoMonitor(uint8_t size)
#endif
{
	//If we don't have more space in the buffer TX the packet
	if (AFLfuzzer.txCurrentIndex + size >= MAX_USB_FRAME  )
	{

    #if DEBUGPRINT ==1
	    printf("TX Nbytes: %d\n",(int)AFLfuzzer.txCurrentIndex );
    #endif
		xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,3,1,eSetValueWithOverwrite); //notify the Monitor to transmit
	    ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY); //get notification when transmission finishes to continue execution
	    //Note: the TxComplete callback will clean buffer after transmission so we will have space for the next function.
	    //It also notifies the target to continue execution


	}
#if DEBUGPRINT ==1
	printf("F.: %s, C.I.: %d\n",fstrings[func], (int)AFLfuzzer.txCurrentIndex );
#endif
	AFLfuzzer.txTotalFunctions++;
}

void _sym_initialize()
{
    int i;
	total_sym_peripherals = 0;
	shadowram = (uint32_t *)SYM_SHADOW_RAM_START;

	memset((void*)shadowram,0x00,SYM_SHADOW_RAM_LENGTH);

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

inline void set_id(uint32_t userID)
{
	union ubytes_t aux;
	aux.vuint32 = userID;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[0];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[1];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[2];
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= aux.vbytes[3];
}

inline void get_report(uint8_t * arg)
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
#if DEBUGPRINT ==1
	    if(numBits > 4){
	    	printf("Error! Integer more than 4 bytes! %d\n",(int)numBits );
	    }
#endif
		msgSize = SIZE_SYM_BLD_INT_4;
		msgCode = SYM_BLD_INT_4;
	}


	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
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


bool _sym_build_float(uint64_t double_val, bool is_double, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
    uint8_t *byteval;
    int numBits;

    if(is_double)
    {
    	msgSize = SIZE_SYM_BLD_FLOAT_DBL;
    	msgCode = SYM_BLD_FLOAT_DBL;
    	numBits = 8;
    }
    else
    {
    	msgSize = SIZE_SYM_BLD_FLOAT;
    	msgCode = SYM_BLD_FLOAT;
    	numBits = 4;
    }

    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;
   	//set the val
   	byteval = (uint8_t *)(&double_val);
   	for(int i=0;i<numBits;i++)
   	{
   		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++; //set the function in the buffer
   	}
   	return true;


}

bool _sym_build_bool(bool bool_val, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
    uint8_t *byteval;


    msgSize = SIZE_SYM_BLD_BOOL;
    msgCode = SYM_BLD_BOOL;

    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;
   	//set the val
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) bool_val;
   	return true;

}

void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;

	if(!input) return;

	msgSize = SIZE_SYM_BLD_PATH_CNSTR;
    msgCode = SYM_BLD_PATH_CNSTR;

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the ID
	byteval = (uint8_t *)(&symID);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;
	//set the val
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) runtimeVal;

}

void _sym_notify_phi(uint8_t branchNo, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;


	msgSize = SIZE_SYM_NTFY_PHI;
	msgCode = SYM_NTFY_PHI;

    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;
    //set the val
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) branchNo;
}

void _sym_notify_call(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_CALL;
    msgCode = SYM_NTFY_CALL;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the val
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) call_inst_id;
}


void _sym_notify_func(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_FUNC;
    msgCode = SYM_NTFY_FUNC;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the val
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) call_inst_id;
}


void _sym_notify_ret(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_RET;
    msgCode = SYM_NTFY_RET;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the val
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) call_inst_id;
}

void _sym_notify_basic_block(uint8_t bbid)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_BBLK;
    msgCode = SYM_NTFY_BBLK;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the val
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = (uint8_t) bbid;

}

void _sym_notify_basic_block1(uint16_t bbid)
{
	int msgSize=0;
    uint8_t msgCode;
	uint8_t *byteval;

    msgSize = SIZE_SYM_NTFY_BBLK;
    msgCode = SYM_NTFY_BBLK;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer
	//set the val
	byteval = (uint8_t *)(&bbid);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;
}


//return shadow address
uint32_t AddressToShadow(char *addr)
{
	uint32_t adr32 = (uint32_t)addr;
	return  ((adr32>>3) + SYM_SHADOW_RAM_OFFSET);
}

//return true if byte is symbolic
bool checkSymbolic(char *addr)
{
   char *addrShadow = (char *)AddressToShadow(addr);
   char val = *addrShadow;
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   return bitRead(val,bitnum);
}


//return true if byte was symbolic and convert it to concrete
bool checkSymbolicSetConcrete(char *addr)
{
   char *addrShadow = (char *)AddressToShadow(addr);
   char val = *addrShadow;
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bool symbolic  = bitRead(val,bitnum);
   if(symbolic) bitClear(*addrShadow,  bitnum);
   return symbolic;
}


//return true if byte was symbolic and convert it to symbolic
bool checkSymbolicSetSymbolic(char *addr)
{
   char *addrShadow =  (char *)AddressToShadow(addr);
   char val = *addrShadow;
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bool symbolic  = bitRead(val,bitnum);
   if(!symbolic)bitSet(*addrShadow,  bitnum);
   return symbolic;
}


void SetSymbolic(char *addr)
{
   char *addrShadow =  (char *)AddressToShadow (addr);
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bitSet(*addrShadow,  bitnum);
}

void SetConcrete(char *addr)
{
   char *addrShadow =  (char *)AddressToShadow (addr);
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bitClear(*addrShadow,  bitnum);
}


void  reportSymHelper(uint8_t msgCode, int size , char *dest, char *src, size_t length, uint16_t symID)
{

	int msgSize=0;
	msgSize = size;
	uint32_t addr;
	uint8_t *byteval;


	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = msgCode; //set the function in the buffer

	byteval = (uint8_t*)(&symID);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval;


	addr = (uint32_t) dest;
	byteval = (uint8_t*)(&addr);
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval;


	if(msgCode == SYM_BLD_MEMCPY || msgCode == SYM_BLD_MEMMOVE)
	{
		addr = (uint32_t) src;
		byteval = (uint8_t*)(&addr);
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
		AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval;
	}

    if(msgCode != SYM_BLD_READ_MEM  && msgCode != SYM_BLD_WRITE_MEM )
    {
    	byteval = (uint8_t*)(&length);
    	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval++;
    	AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++]= *byteval;
    }



}


void _sym_build_memcpy(char * dest, char * src, size_t length, uint16_t symID)
{
	char *pCharDest = dest;
	char *pCharSrc = src;
    bool report;
    report =false;

    for(size_t i=0; i<length; i++)
    {
    	if(!checkSymbolic(pCharSrc))
    	{
    		if(checkSymbolicSetConcrete(pCharDest))
    		{
    			report=true;
    		}
    	}
    	else
    	{
    		SetSymbolic(pCharDest);
    		report=true;
    	}

    	pCharDest++;
    	pCharSrc++;
    }

    if(report)
    {
    	reportSymHelper( SYM_BLD_MEMCPY, SIZE_SYM_BLD_MEMCPY, dest, src, length, symID);
    }

}

void _sym_build_memset(char * mem, bool input, size_t length, uint16_t symID)
{
	char *pChar=mem;
    bool report;
    report =false;

    for(size_t i=0; i<length; i++)
    {
    	if(!input)
    	{
    		if(checkSymbolicSetConcrete(pChar)) // if input is concrete and at least one byte is symbolic
    		{
    			report=true;
    		}
    	}
    	else
    	{
    		SetSymbolic(pChar);
    		report=true;
    	}

    	pChar++;
    }

    if(report)
    {
    	reportSymHelper( SYM_BLD_MEMSET, SIZE_SYM_BLD_MEMSET,mem,NULL, length, symID);
    }
}

void _sym_build_memmove(char * dest, char * src, size_t length, uint16_t symID)
{
	char *pCharDest = dest;
	char *pCharSrc = src;
    bool report;
    report =false;

    for(size_t i=0; i<length; i++)
    {
    	if(!checkSymbolic(pCharSrc))
    	{
    		if(checkSymbolicSetConcrete(pCharDest))
    		{
    			report=true;
    		}
    	}
    	else
    	{
    		SetSymbolic(pCharDest);
    		report=true;
    	}

    	pCharDest++;
    	pCharSrc++;
    }

    if(report)
    {
    	reportSymHelper( SYM_BLD_MEMMOVE, SIZE_SYM_BLD_MEMMOVE, dest, src, length, symID);
    }
}



bool _sym_build_read_memory(char * addr, size_t length, bool is_little_edian, uint16_t symID)
{
	if((uint32_t)addr>= SYM_PERIPHERAL_ADDR_START && (uint32_t)addr <=(SYM_PERIPHERAL_ADDR_START + SYM_PERIPHERAL_SIZE)  )
	{
		//address is a peripheral
		//here check the list of addresses in the initialization
		//we only check the first byte TODO: check if this works with the instrumentation
		for(int i=0; i<total_sym_peripherals; i++)
		{
			if((uint32_t*)addr == sym_peripherals[i])
			{
				reportSymHelper( SYM_BLD_READ_MEM, SIZE_SYM_BLD_READ_MEM, addr, NULL, 0, symID);
				return true;
			}
		}
		return false;
	}
	if((uint32_t)addr>= SYM_FLASH_ADDR_START && (uint32_t)addr <=(SYM_FLASH_ADDR_START + SYM_FLASH_SIZE)  )
	{
		//flash is always concrete
		return false;
	}

	char *pChar=addr;
	for(size_t i=0; i<length; i++)
	{
		if(checkSymbolic(pChar))
		{
			reportSymHelper( SYM_BLD_READ_MEM, SIZE_SYM_BLD_READ_MEM, addr, NULL, 0, symID);
			return true;
		}
		pChar++;
	}
	return false;

}

void _sym_build_write_memory(char * addr, size_t length, bool input, uint16_t symID )
{
	char *pChar=addr;
	bool report;
	report =false;

	for(size_t i=0; i<length; i++)
	{
		if(!input)
		{
			if(checkSymbolicSetConcrete(pChar)) // if input is concrete and at least one byte is symbolic
			{
				report=true;
			}
		}
		else
		{
			SetSymbolic(pChar);
			report=true;
		}

		pChar++;
	}

	if(report)
	{
		reportSymHelper( SYM_BLD_WRITE_MEM, SIZE_SYM_BLD_WRITE_MEM ,addr,NULL,0, symID);
	}

}


void _sym_symbolize_memory(char * addr, size_t length)
{
	char *pChar=addr;

	for(size_t i=0; i<length; i++)
	{
		SetSymbolic(pChar);
		pChar++;
	}

}



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

