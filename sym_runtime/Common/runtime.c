/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */

#include "runtime.h"
#include "ProtocolConfig.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

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





uint32_t *shadowram;
bool parameter_exp[NUMBER_PARAMETER_EXP];
bool return_exp;


int sockfd;
int clientfd;

uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t  txBuffer[TX_BUFFER_SIZE];
uint32_t txCur = 0; 

void  reportSymHelper(uint8_t msgCode, int size , char *dest, char *src, size_t length, uint16_t symID);

#if defined (CO3_NO_SHADOW)
#else
uint32_t AddressToShadow(char *addr);
#endif

//return true if byte is symbolic
bool checkSymbolic(char *addr);
bool checkSymbolicSetConcrete(char *);


void ReceiveInput()
{
    int result = read(clientfd, rxBuffer , sizeof(uint32_t));
    if (result != 4) {
        perror("read error");
        exit(1);
    }
    unsigned u32Tocopy = *(uint32_t*)rxBuffer - sizeof(uint32_t);
    read(clientfd, rxBuffer + sizeof(uint32_t), u32Tocopy);
    return;
}

// this should be only called by the monitor
void TransmitPack(void)
{
    txBuffer[0]= txCur;
    int n = write(clientfd, txBuffer, n);
    if (n < 0) {
        perror("write error");
        exit(1);
    }
    //reset
    txCur = TX_BUFFER_STARTING_POINT;
    memset(txBuffer,0,TX_BUFFER_SIZE);
}

#if DEBUGPRINT ==1
void txCommandtoMonitor(uint8_t size, uint8_t func)
#else
void txCommandtoMonitor(uint8_t size)
#endif
{
    //If we don't have more space in the buffer TX the packet
    if (txCur + size >= TX_BUFFER_SIZE  )
    {
        TransmitPack();
    }
}

void _sym_initialize()
{
    int i;
    struct sockaddr_un addr;
#if defined (CO3_NO_SHADOW)
#else 
    shadowram = (uint32_t *)malloc(SYM_SHADOW_RAM_LENGTH);
    memset((void*)shadowram,0x00,SYM_SHADOW_RAM_LENGTH);
#endif

    // initialize socket
    for(i=0; i<NUMBER_PARAMETER_EXP; i++)
    {
        parameter_exp[i]=false;
    }
    return_exp = false;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < -1) {
        perror("socket error");
        exit(1);
    }
    addr.sun_family = AF_UNIX;
#ifndef CO3_PROJECT_DIR
    #error "CO3_PROJECT_DIR" not defined
#else
    #define xSTR(x) STR(x)
    #define STR(x) #x
    sprintf(addr.sun_path, "%s%s", xSTR(CO3_PROJECT_DIR), "/CO3.sock");
#endif

    // create a UNIX socket file and bind to it
    unlink(addr.sun_path);
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr,"%s, bind error", addr.sun_path);
        exit(1);
    }
    listen(sockfd, 5);

    clientfd = accept(sockfd, NULL, NULL);
    if(clientfd < 0) {
        perror("accept error");
        exit(1);
    }

    // initialize global variables
    txCur=TX_BUFFER_STARTING_POINT;
    memset(txBuffer,0,TX_BUFFER_SIZE);
    memset(rxBuffer,0,RX_BUFFER_SIZE);
    ReceiveInput();
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

	txBuffer[txCur++]= arg_id;

	aux32 = (uint32_t*)(&txBuffer[txCur]);
    *aux32 = 0;

	for(uint8_t i=0; i<width; i++)
	{
		txBuffer[txCur++] = arg[i+2];
	}
	for(uint8_t i=0; i<(4-width); i++)
	{
			txCur++;
	}
}

#define ONE_BYTE_SYMID_MAX 255
#define ONE_BYTE_BBID_MAX 255

bool _sym_build_integer(uint32_t int_val, uint8_t numBits, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;

	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
	if(numBits==1)
	{
		if(isSmallSymID){
			msgSize = SIZE_SYM_BLD_INT_1;
			msgCode = SYM_BLD_INT_1;
		}else{
			msgSize = SIZE_SYM_BLD_INT_1_1;
			msgCode = SYM_BLD_INT_1_1;
		}

	}
	else if(numBits==2)
	{
		if(isSmallSymID){
			msgSize = SIZE_SYM_BLD_INT_2;
			msgCode = SYM_BLD_INT_2;
		}else{
			msgSize = SIZE_SYM_BLD_INT_2_1;
			msgCode = SYM_BLD_INT_2_1;
		}

	}
	else
	{
#if DEBUGPRINT ==1
	    if(numBits > 4){
	    	printf("Error! Integer more than 4 bytes! %d\n",(int)numBits );
	    }
#endif
	    if(isSmallSymID){
	    	msgSize = SIZE_SYM_BLD_INT_4;
	    	msgCode = SYM_BLD_INT_4;
	    }else{
	    	msgSize = SIZE_SYM_BLD_INT_4_1;
	    	msgCode = SYM_BLD_INT_4_1;
	    }

	}

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the ID
	byteval = (uint8_t *)(&symID);
	txBuffer[txCur++] = *byteval++;
	if(! isSmallSymID){
		txBuffer[txCur++] = *byteval;
	}
	//set the val
	byteval = (uint8_t *)(&int_val);
	for(int i=0;i<numBits;i++)
	{
		txBuffer[txCur++] = *byteval++; //set the function in the buffer
	}

	return true;
}


bool _sym_build_float(double double_val, bool is_double, uint16_t symID)
{
	// should never be called
	int msgSize=0;
	uint8_t msgCode;
    uint8_t *byteval;
    int numBits;

    bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;

    if(is_double)
    {
    	if(isSmallSymID){
    		msgSize = SIZE_SYM_BLD_FLOAT_DBL;
    		msgCode = SYM_BLD_FLOAT_DBL;
    	}else{
    		msgSize = SIZE_SYM_BLD_FLOAT_DBL_1;
    		msgCode = SYM_BLD_FLOAT_DBL_1;
    	}

    	numBits = 8;
    }
    else
    {
    	if(isSmallSymID){
    		msgSize = SIZE_SYM_BLD_FLOAT;
    		msgCode = SYM_BLD_FLOAT;
    	}else{
    		msgSize = SIZE_SYM_BLD_FLOAT_1;
    		msgCode = SYM_BLD_FLOAT_1;
    	}
    	numBits = 4;
    }

    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    txBuffer[txCur++] = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    txBuffer[txCur++] = *byteval++;
    if(! isSmallSymID){
    	txBuffer[txCur++] = *byteval;
    }
    //set the val
    byteval = (uint8_t *)(&double_val);
    for(int i=0;i<numBits;i++)
    {
        txBuffer[txCur++]  = *byteval++; //set the function in the buffer
    }
    return true;


}

bool _sym_build_bool(bool bool_val, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
    uint8_t *byteval;

    bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
    if(isSmallSymID){
    	msgSize = SIZE_SYM_BLD_BOOL;
    	msgCode = SYM_BLD_BOOL;
    }else{
    	msgSize = SIZE_SYM_BLD_BOOL_1;
    	msgCode = SYM_BLD_BOOL_1;
    }


    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    txBuffer[txCur++]  = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    txBuffer[txCur++]  = *byteval++;
    if(! isSmallSymID){
        txBuffer[txCur++]  = *byteval;
    }
   	//set the val
    txBuffer[txCur++]  = (uint8_t) bool_val;
   	return true;

}

void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;
	if(!input) return;

	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;

	if(isSmallSymID){
		msgSize = SIZE_SYM_BLD_PATH_CNSTR;
		msgCode = SYM_BLD_PATH_CNSTR;
	}else{
		msgSize = SIZE_SYM_BLD_PATH_CNSTR_1;
		msgCode = SYM_BLD_PATH_CNSTR_1;
	}


	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the ID
	byteval = (uint8_t *)(&symID);
	txBuffer[txCur++] = *byteval++;
	if( ! isSmallSymID){
		txBuffer[txCur++] = *byteval;
	}
	//set the val
	txBuffer[txCur++] = (uint8_t) runtimeVal;

}

void _sym_notify_phi(uint8_t branchNo, uint16_t symID, bool isSym, char * base_addr, uint8_t offset)
{
	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;


	uint8_t byteOffset = offset / 8;
	uint8_t bitOffsetInByte = offset % 8;
	bool prev_state = bitRead( *(base_addr + byteOffset), bitOffsetInByte );

	if(prev_state == false && isSym == false){
		return;
	}
	bitWrite(*(base_addr + byteOffset), bitOffsetInByte, isSym);
	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;

	if(isSmallSymID){
		msgSize = SIZE_SYM_NTFY_PHI;
		msgCode = SYM_NTFY_PHI;
	}else{
		msgSize = SIZE_SYM_NTFY_PHI_1;
		msgCode = SYM_NTFY_PHI_1;
	}


    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    txBuffer[txCur++] = msgCode; //set the function in the buffer
    //set the ID
    byteval = (uint8_t *)(&symID);
    txBuffer[txCur++] = *byteval++;
    if(! isSmallSymID){
        txBuffer[txCur++] = *byteval;
    }

    //set the val
    txBuffer[txCur++] = (uint8_t) branchNo;
}

void _sym_notify_call(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_CALL;
    msgCode = SYM_NTFY_CALL;
    txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
    txBuffer[txCur++] = msgCode; //set the function in the buffer
    //set the val
    txBuffer[txCur++] = (uint8_t) call_inst_id;
}

void _sym_try_alternative(bool destSym, bool concSym, uint16_t symID)
{
/*
	if(! destSym || !concSym){
		return;
	}

	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;

	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;

	if(isSmallSymID){
		msgSize = SIZE_SYM_TRY_ALTERNATIVE;
		msgCode = SYM_TRY_ALT;
	}else{
		msgSize = SIZE_SYM_TRY_ALTERNATIVE_1;
		msgCode = SYM_TRY_ALT_1;
	}

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the val
	byteval = (uint8_t *)(&symID);
	txBuffer[txCur++] = *byteval++;
	if(! isSmallSymID){
	    txBuffer[txCur++] = *byteval;
	}
*/
}

void _sym_notify_func(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_FUNC;
    msgCode = SYM_NTFY_FUNC;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the val
	txBuffer[txCur++] = (uint8_t) call_inst_id;
}

void _sym_end(){
	int msgSize = 0;
	uint8_t msgCode;
	msgSize = SIZE_SYM_END;
	msgCode = SYM_END;
	txCommandtoMonitorF;
	txBuffer[txCur++] = msgCode;
	close(clientfd);
	close(sockfd);
	return;
}

void _sym_notify_ret(uint8_t call_inst_id)
{
	int msgSize=0;
    uint8_t msgCode;

    msgSize = SIZE_SYM_NTFY_RET;
    msgCode = SYM_NTFY_RET;
	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the val
	txBuffer[txCur++] = (uint8_t) call_inst_id;
}

void _sym_notify_select(bool cond, bool isSym1, bool isSym2, char * base_addr, uint8_t offset, uint16_t symID){

	int msgSize=0;
	uint8_t msgCode;
	uint8_t *byteval;

	uint8_t byteOffset = offset / 8;
	uint8_t bitOffsetInByte = offset % 8;
	bool prev_state = bitRead( *(base_addr + byteOffset), bitOffsetInByte );

	bool cur_state = isSym1 | isSym2;
	if(prev_state == false && cur_state == false){
		return;
	}


	bitWrite(*(base_addr + byteOffset), bitOffsetInByte, cur_state);
	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
	if(isSmallSymID){
	    msgSize = SIZE_SYM_NTFY_SELECT;
	    msgCode = SYM_NTFY_SELECT;
	}else{
	    msgSize = SIZE_SYM_NTFY_SELECT_1;
	    msgCode = SYM_NTFY_SELECT_1;
	}

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	//set the msg type
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the symid
	byteval = (uint8_t *)(&symID);
	txBuffer[txCur++] = *byteval++;
	if(!isSmallSymID){
		txBuffer[txCur++] = *byteval;
	}
	// set the cond
	txBuffer[txCur++] = (char)cond;

}
void _sym_notify_basic_block(uint16_t bbid, bool isSym, char * base_addr, uint8_t offset )
{
	int msgSize=0;
    uint8_t msgCode;
    uint8_t *byteval;

    uint8_t byteOffset = offset / 8;
    uint8_t bitOffsetInByte = offset % 8;
    bool prev_state = bitRead( *(base_addr + byteOffset), bitOffsetInByte );

    if(prev_state == false && isSym == false){
    	return;
    }
    bitWrite(*(base_addr + byteOffset), bitOffsetInByte, isSym);
    bool isSmallBBID = bbid <= ONE_BYTE_BBID_MAX ? true : false;
    if(isSmallBBID){
    	msgSize = SIZE_SYM_NTFY_BBLK;
    	msgCode = SYM_NTFY_BBLK;
    }else{
    	msgSize = SIZE_SYM_NTFY_BBLK1;
    	msgCode = SYM_NTFY_BBLK1;
    }

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer
	txBuffer[txCur++] = msgCode; //set the function in the buffer
	//set the val
	byteval = (uint8_t *)(&bbid);
	txBuffer[txCur++] = *byteval++;
	if(!isSmallBBID){
		txBuffer[txCur++] = *byteval;
	}
}



#if defined(CO3_NO_SHADOW)
#else 
uint32_t AddressToShadow(char *addr)
{
	uint32_t adr32 = (uint32_t)addr;
	return ((adr32>>3) + SYM_SHADOW_RAM_OFFSET);
}
#endif

//return true if byte is symbolic
bool checkSymbolic(char *addr)
{
#if defined (CO3_NO_SHADOW) 
	return true;
	
#else
	char *addrShadow = (char *)AddressToShadow(addr);
	char val = *addrShadow;
	uint32_t bitnum = ((uint32_t)addr) & 0x07;
	return bitRead(val,bitnum);
#endif
}


//return true if byte was symbolic and convert it to concrete
bool checkSymbolicSetConcrete(char *addr)
{
#ifndef CO3_NO_SHADOW
	char *addrShadow = (char *)AddressToShadow(addr);
	char val = *addrShadow;
	uint32_t bitnum = ((uint32_t)addr) & 0x07;
	bool symbolic  = bitRead(val,bitnum);
	if(symbolic) bitClear(*addrShadow,  bitnum);
	return symbolic;
#else
	return true;
#endif
}

/*
//return true if byte was symbolic and convert it to symbolic
bool checkSymbolicSetSymbolic(char *addr)
{
   char *addrShadow =  (char *)AddressToShadow(addr);
   char val = *addrShadow;
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bool symbolic  = bitRead(val,bitnum);
   if(!symbolic)bitSet(*addrShadow,  bitnum);
   return symbolic;
}*/


void SetSymbolic(char *addr)
{
#ifndef CO3_NO_SHADOW
	char *addrShadow =  (char *)AddressToShadow (addr);
	uint32_t bitnum = ((uint32_t)addr) & 0x07;
	bitSet(*addrShadow,  bitnum);
#else 
	(void) addr;
	return;
#endif
}

/*
void SetConcrete(char *addr)
{
   char *addrShadow =  (char *)AddressToShadow (addr);
   uint32_t bitnum = ((uint32_t)addr) & 0x07;
   bitClear(*addrShadow,  bitnum);
}*/


void  reportSymHelper(uint8_t msgCode, int size , char *dest, char *src, size_t length, uint16_t symID)
{

	int msgSize=0;
	msgSize = size;
	//uint32_t addr;
	uint8_t *byteval;

	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;

	txCommandtoMonitorF;                              //check if we have space otherwise send the buffer

	txBuffer[txCur++] = msgCode;


	byteval = (uint8_t*)(&symID);
	txBuffer[txCur++]= *byteval++;
	if( ! isSmallSymID){
		txBuffer[txCur++]= *byteval;
	}



	//addr = (uint32_t) dest;
	//byteval = (uint8_t*)(&addr);
	//txBuffer[txCur++]= *byteval++;
	//txBuffer[txCur++]= *byteval++;
	//txBuffer[txCur++]= *byteval++;
	//txBuffer[txCur++]= *byteval;
	*(uint32_t*)(txBuffer + txCur) = (uint32_t) dest;
	txCur += 4;


	if(msgCode == SYM_BLD_MEMCPY || msgCode == SYM_BLD_MEMCPY_1  || msgCode == SYM_BLD_MEMMOVE || msgCode == SYM_BLD_MEMMOVE_1)
	{
		//addr = (uint32_t) src;
		//byteval = (uint8_t*)(&addr);
		//txBuffer[txCur++]= *byteval++;
		//txBuffer[txCur++]= *byteval++;
		//txBuffer[txCur++]= *byteval++;
		//txBuffer[txCur++]= *byteval;
		*(uint32_t*)(txBuffer + txCur) = (uint32_t) src;
		txCur += 4;
	}

    if(msgCode != SYM_BLD_READ_MEM  && msgCode != SYM_BLD_READ_MEM_1 \
    	&&	msgCode != SYM_BLD_READ_MEM_HW  && msgCode != SYM_BLD_READ_MEM_HW_1 \
    	&&	msgCode != SYM_BLD_READ_MEM_W  && msgCode != SYM_BLD_READ_MEM_W_1 \
    		&& msgCode != SYM_BLD_WRITE_MEM && msgCode != SYM_BLD_WRITE_MEM_1 ) // we don't need the length because it is already known on PC side for these functions
    {
    	byteval = (uint8_t*)(&length);
    	txBuffer[txCur++]= *byteval++;
    	txBuffer[txCur++]= *byteval;
    }

    if(msgCode == SYM_BLD_READ_MEM_W || msgCode == SYM_BLD_READ_MEM_W_1) // we need to send the concrete word
    {
    		byteval = (uint8_t*)(dest);
    		//txBuffer[txCur++]= *byteval++;
    		//txBuffer[txCur++]= *byteval++;
    		//txBuffer[txCur++]= *byteval++;
    		//txBuffer[txCur++]= *byteval;
    		*(uint32_t*)(txBuffer + txCur) = *(uint32_t*)(dest);
    		txCur += 4;
    }

    if(msgCode == SYM_BLD_READ_MEM_HW || msgCode ==  SYM_BLD_READ_MEM_HW_1) // we need to send the concrete half  word
    {
       		byteval = (uint8_t*)(dest);
       		txBuffer[txCur++]= *byteval++;
       		txBuffer[txCur++]= *byteval;
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
    	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
    	if(isSmallSymID){
    		reportSymHelper( SYM_BLD_MEMCPY, SIZE_SYM_BLD_MEMCPY, dest, src, length, symID);
    	}else{
    		reportSymHelper( SYM_BLD_MEMCPY_1, SIZE_SYM_BLD_MEMCPY_1, dest, src, length, symID);
    	}

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
    	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
    	if(isSmallSymID){
    		reportSymHelper( SYM_BLD_MEMSET, SIZE_SYM_BLD_MEMSET,mem,NULL, length, symID);
    	}else{
    		reportSymHelper( SYM_BLD_MEMSET_1, SIZE_SYM_BLD_MEMSET_1,mem,NULL, length, symID);
    	}

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
    	bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
    	if(isSmallSymID){
    		reportSymHelper( SYM_BLD_MEMMOVE, SIZE_SYM_BLD_MEMMOVE, dest, src, length, symID);
    	}else{
    		reportSymHelper( SYM_BLD_MEMMOVE_1, SIZE_SYM_BLD_MEMMOVE_1, dest, src, length, symID);
    	}

    }
}



bool _sym_build_read_memory(char * addr, size_t length, bool is_little_edian, uint16_t symID)
{
	char *pChar=addr;
	uint8_t countSymbols;
	size_t i;

	countSymbols =0;

	for(i=0; i<length; i++)
	{
		if(checkSymbolic(pChar))
		{
			countSymbols++;
			//reportSymHelper( SYM_BLD_READ_MEM, SIZE_SYM_BLD_READ_MEM, addr, NULL, 0, symID);

		}
		pChar++;
	}

	if(countSymbols == length) // all bytes are symbols report only the address
	{
		bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
		if(isSmallSymID){
			reportSymHelper( SYM_BLD_READ_MEM, SIZE_SYM_BLD_READ_MEM, addr, NULL, 0, symID);
		}
		else{
			reportSymHelper( SYM_BLD_READ_MEM_1, SIZE_SYM_BLD_READ_MEM_1, addr, NULL, 0, symID);
		}
		return true;
	}
	else if( length == 2 && countSymbols>0 ) // report address + value HalfWord
	{
		bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
		if(isSmallSymID){
			reportSymHelper(SYM_BLD_READ_MEM_HW, SIZE_SYM_BLD_READ_MEM_HW, addr, NULL, 0, symID);
		}else{
			reportSymHelper(SYM_BLD_READ_MEM_HW_1, SIZE_SYM_BLD_READ_MEM_HW_1, addr, NULL, 0, symID);
		}

		return true;
	}
	else if( length == 4 && countSymbols>0 ) // report address + value Word
	{
		bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
		if(isSmallSymID){
			reportSymHelper(SYM_BLD_READ_MEM_W, SIZE_SYM_BLD_READ_MEM_W, addr, NULL, 0, symID);
		}else{
			reportSymHelper(SYM_BLD_READ_MEM_W_1, SIZE_SYM_BLD_READ_MEM_W_1, addr, NULL, 0, symID);
		}

		return true;
	}


	return false;

}

void _sym_build_write_memory(char * addr, size_t length, bool input, bool little_endian, uint16_t symID )
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
		bool isSmallSymID = symID <= ONE_BYTE_SYMID_MAX ? true : false;
		if(isSmallSymID){
			reportSymHelper( SYM_BLD_WRITE_MEM, SIZE_SYM_BLD_WRITE_MEM ,addr,NULL,0, symID);
		}else{
			reportSymHelper( SYM_BLD_WRITE_MEM_1, SIZE_SYM_BLD_WRITE_MEM_1 ,addr,NULL,0, symID);
		}
	}
}


void _sym_symbolize_memory(char * addr, size_t length, bool DR)
{
	char *pChar=addr;

	for(size_t i=0; i<length; i++)
	{
		SetSymbolic(pChar);
		pChar++;
	}
	// send the addr value
	int msgSize = 0;
	uint8_t msgCode;

	if(DR == true){
		msgSize = SIZE_SYM_INIT_DR;
		msgCode = SYM_INIT_DR;
	}else{
		msgSize = SIZE_SYM_INIT;
		msgCode = SYM_INIT;

	}

	txCommandtoMonitorF;
	txBuffer[txCur++] = msgCode;
	*(uint32_t*)(txBuffer + txCur) = (uint32_t) addr;
	txCur += 4;
}


uintptr_t prev_site_id = 0;

void _sym_notify_basic_block_cc(uintptr_t site_id) {
    //FILE * f= fopen("coverage", "a");
    //fprintf(f,"%lx\n",site_id ^ prev_site_id);
    //fprintf(f,"%lx\n",site_id);

	uintptr_t r = site_id ^ prev_site_id;
	int msgSize = sizeof(uintptr_t) + 1;
	txCommandtoMonitorF;
	*(uintptr_t*)(txBuffer + txCur) = r;
	txCur += sizeof(uintptr_t);
	txBuffer[txCur++] = '\n';
    prev_site_id = site_id >> 1;
}

/*
void _spear_report1(uint32_t userID, char * arg1)
{

	txCommandtoMonitor(SIZE_SPEAR_RPT1);
	txBuffer[txCur] = SPEAR_RPT1; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
}


void _spear_report2(uint32_t userID, char * arg1,char * arg2)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT2);
	txBuffer[txCur] = SPEAR_RPT2; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
	get_report( (uint8_t*) arg2);

}


void _spear_report3(uint32_t userID, char * arg1,char * arg2,char * arg3)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT3);
	txBuffer[txCur] = SPEAR_RPT3; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(userID);
	get_report( (uint8_t*) arg1);
	get_report( (uint8_t*) arg2);
	get_report( (uint8_t*) arg3);
}


void _spear_report4(uint32_t userID, char * arg1,char * arg2,char * arg3,char * arg4)
{
	txCommandtoMonitor(SIZE_SPEAR_RPT4);
    txBuffer[txCur] = SPEAR_RPT4; //set the function in the buffer
    if(AFLfuzzer.txTotalFunctions)
    {
    	//write the index where the current function starts
    	txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
    }
    AFLfuzzer.txTotalFunctions++;
    txCur++;
    set_id(userID);
    get_report( (uint8_t*) arg1);
    get_report( (uint8_t*) arg2);
    get_report( (uint8_t*) arg3);
    get_report( (uint8_t*) arg4);
}

void _spear_symbolize(char *address, uint32_t len)
{
	txCommandtoMonitor(SIZE_SPEAR_SYM);
	txBuffer[txCur] = SPEAR_SYM; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id((uint32_t)address);
	set_id((uint32_t)len);

}


void _sym_notify_call(uint32_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_CALL);
	txBuffer[txCur] = SYM_N_CALL; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(id);
}

void _sym_notify_ret(uint32_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_RET);
	txBuffer[txCur] = SYM_N_RET; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(id);
}

void _sym_notify_basic_block(uint8_t id)
{
	txCommandtoMonitor(SIZE_SYM_N_BB);
	txBuffer[txCur] = SYM_N_BB; //set the function in the buffer
	if(AFLfuzzer.txTotalFunctions)
	{
		//write the index where the current function starts
		txBuffer[AFLfuzzer.txTotalFunctions]=txCur;
	}
	AFLfuzzer.txTotalFunctions++;
	txCur++;
	set_id(id);
}
*/




