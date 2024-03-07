/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */

#include "runtime_no_replace.h"
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

 char S_SYM_DUMMY           []="SYM_DUMMY           ";
 char S_SYM_BLD_INT_1       []="SYM_BLD_INT_1       ";
 char S_SYM_BLD_INT_1_1     []="SYM_BLD_INT_1_1     ";
 char S_SYM_BLD_INT_2       []="SYM_BLD_INT_2       ";
 char S_SYM_BLD_INT_2_1     []="SYM_BLD_INT_2_1     ";
 char S_SYM_BLD_INT_4       []="SYM_BLD_INT_4       ";
 char S_SYM_BLD_INT_4_1     []="SYM_BLD_INT_4_1     ";
 char S_SYM_BLD_FLOAT       []="SYM_BLD_FLOAT       ";
 char S_SYM_BLD_FLOAT_1     []="SYM_BLD_FLOAT_1     ";

 char S_SYM_BLD_FLOAT_DBL   []="SYM_BLD_FLOAT_DBL   ";
 char S_SYM_BLD_FLOAT_DBL_1 []="SYM_BLD_FLOAT_DBL_1 ";

 char S_SYM_BLD_BOOL        []="SYM_BLD_BOOL        ";
 char S_SYM_BLD_BOOL_1      []="SYM_BLD_BOOL_1      ";

 char S_SYM_BLD_PATH_CNSTR  []="SYM_BLD_PATH_CNSTR  ";
 char S_SYM_BLD_PATH_CNSTR_1[]="SYM_BLD_PATH_CNSTR_1";

 char S_SYM_BLD_MEMCPY      []="SYM_BLD_MEMCPY      ";
 char S_SYM_BLD_MEMCPY_1    []="SYM_BLD_MEMCPY_1    ";

 char S_SYM_BLD_MEMSET      []="SYM_BLD_MEMSET      ";
 char S_SYM_BLD_MEMSET_1    []="SYM_BLD_MEMSET_1    ";

 char S_SYM_BLD_MEMMOVE     []="SYM_BLD_MEMMOVE     ";
 char S_SYM_BLD_MEMMOVE_1   []="SYM_BLD_MEMMOVE_1   ";

 char S_SYM_BLD_READ_MEM    []="SYM_BLD_READ_MEM    ";
 char S_SYM_BLD_READ_MEM_1  []="SYM_BLD_READ_MEM_1  ";

 char S_SYM_BLD_READ_MEM_HW   []="SYM_BLD_READ_MEM_HW  ";
 char S_SYM_BLD_READ_MEM_HW_1 []="SYM_BLD_READ_MEM_HW_1";

 char S_SYM_BLD_READ_MEM_W  []="SYM_BLD_READ_MEM_W  ";
 char S_SYM_BLD_READ_MEM_W_1[]="SYM_BLD_READ_MEM_W_1";



 char S_SYM_BLD_WRITE_MEM   []="SYM_BLD_WRITE_MEM   ";
 char S_SYM_BLD_WRITE_MEM_1 []="SYM_BLD_WRITE_MEM_1 ";

 //char S_SYM_SET_PAR_EXP     []="SYM_SET_PAR_EXP     ";
 //char S_SYM_GET_PAR_EXP     []="SYM_GET_PAR_EXP     ";
 //char S_SYM_SET_RET_EXP     []="SYM_SET_RET_EXP     ";
 //char S_SYM_GET_RET_EXP     []="SYM_GET_RET_EXP     ";
 char S_SYM_NTFY_PHI        []="SYM_NTFY_PHI        ";
 char S_SYM_NTFY_PHI_1      []="SYM_NTFY_PHI_1      ";
 char S_SYM_NTFY_CALL       []="SYM_NTFY_CALL       ";
 char S_SYM_NTFY_FUNC       []="SYM_NTFY_FUNC       ";
 char S_SYM_NTFY_RET        []="SYM_NTFY_RET        ";
 char S_SYM_NTFY_BBLK       []="SYM_NTFY_BBLK       ";
 char S_SYM_NTFY_BBLK1      []="SYM_NTFY_BBLK1      ";

 char S_SYM_TRY_ALT         []= "SYM_TRY_ALT        ";
 char S_SYM_TRY_ALT_1       []= "SYM_TRY_ALT_1      ";

 char S_SYM_INIT            []="SYM_INIT            ";
 char S_SYM_INIT_DR         []="SYM_INIT_DR         ";
 char S_SYM_END             []="SYM_END            ";

 char *fstrings[]={

		 S_SYM_DUMMY            ,
		 S_SYM_BLD_INT_1        ,
		 S_SYM_BLD_INT_1_1      ,
		 S_SYM_BLD_INT_2        ,
		 S_SYM_BLD_INT_2_1      ,
		 S_SYM_BLD_INT_4        ,
		 S_SYM_BLD_INT_4_1      ,
		 S_SYM_BLD_FLOAT        ,
		 S_SYM_BLD_FLOAT_1      ,
		 S_SYM_BLD_FLOAT_DBL    ,
		 S_SYM_BLD_FLOAT_DBL_1  ,
		 S_SYM_BLD_BOOL         ,
		 S_SYM_BLD_BOOL_1       ,
		 S_SYM_BLD_PATH_CNSTR   ,
		 S_SYM_BLD_PATH_CNSTR_1 ,
		 S_SYM_BLD_MEMCPY       ,
		 S_SYM_BLD_MEMCPY_1     ,
		 S_SYM_BLD_MEMSET       ,
		 S_SYM_BLD_MEMSET_1     ,
		 S_SYM_BLD_MEMMOVE      ,
		 S_SYM_BLD_MEMMOVE_1    ,
		 S_SYM_BLD_READ_MEM     ,
		 S_SYM_BLD_READ_MEM_1   ,

		 S_SYM_BLD_READ_MEM_HW  ,
		 S_SYM_BLD_READ_MEM_HW_1,

		 S_SYM_BLD_READ_MEM_W   ,
		 S_SYM_BLD_READ_MEM_W_1 ,

		 S_SYM_BLD_WRITE_MEM    ,
		 S_SYM_BLD_WRITE_MEM_1  ,
		 //S_SYM_SET_PAR_EXP      ,
		 //S_SYM_GET_PAR_EXP      ,
		 //S_SYM_SET_RET_EXP      ,
		 //S_SYM_GET_RET_EXP      ,
		 S_SYM_NTFY_PHI         ,
		 S_SYM_NTFY_PHI_1       ,
		 S_SYM_NTFY_CALL        ,
		 S_SYM_NTFY_FUNC        ,
		 S_SYM_NTFY_RET         ,
		 S_SYM_NTFY_BBLK        ,
		 S_SYM_NTFY_BBLK1       ,

		 S_SYM_TRY_ALT          ,
		 S_SYM_TRY_ALT_1        ,
		 S_SYM_INIT             ,
		 S_SYM_INIT_DR          ,
		 S_SYM_END
 };

#endif





Symex_t AFLfuzzer;
uint32_t *sym_peripherals[NUMBER_SYM_PERI];
uint8_t total_sym_peripherals;
uint32_t *shadowram;
bool parameter_exp[NUMBER_PARAMETER_EXP];
bool return_exp;



void  reportSymHelper(uint8_t msgCode, int size , char *dest, char *src, size_t length, uint16_t symID);

#ifndef CO3_NO_MCU_SIDE_SHADOW
//return address
uint32_t AddressToShadow(char *addr);
#endif

//return true if byte is symbolic
bool checkSymbolic(char *addr);
bool checkSymbolicSetConcrete(char *);

bool _sym_peripheral_symb(uint32_t *addr)
{
   if(total_sym_peripherals<NUMBER_SYM_PERI)
   {
	   sym_peripherals[total_sym_peripherals++] = addr;
	   return true;
   }
   return false;
}

int checkSymPeripheral(uint32_t * addr){
	if((uint32_t)addr>= SYM_PERIPHERAL_ADDR_START && (uint32_t)addr <=(SYM_PERIPHERAL_ADDR_START + SYM_PERIPHERAL_SIZE)  )
	{
		//address is a peripheral
		//here check the list of addresses in the initialization
		//we only check the first byte TODO: check if this works with the instrumentation
		for(int i=0; i<total_sym_peripherals; i++)
		{
			if((uint32_t*)addr == sym_peripherals[i])
			{
				//reportSymHelper( SYM_BLD_READ_MEM, SIZE_SYM_BLD_READ_MEM, addr, NULL, 0, symID);
				return 1;
			}
		}
		return 0;
	}
	return -1;
}

int checkSymFlash(uint32_t * addr){
	if((uint32_t)addr>= SYM_FLASH_ADDR_START && (uint32_t)addr <=(SYM_FLASH_ADDR_START + SYM_FLASH_SIZE)  )
	{
		//flash is always concrete
		return 0;
	}
	return -1;
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
#if defined CO3_USE_FREERTOS
	    xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,3,1,eSetValueWithOverwrite); //notify the Monitor to transmit
	    ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY); //get notification when transmission finishes to continue execution
#elif defined CO3_USE_CHIBIOS
	    chEvtSignal(AFLfuzzer.xTaskMonitor, MORE_DATA_TO_COME);
	    eventmask_t evt = chEvtWaitAny(ALL_EVENTS);
	    if(evt != TRANSMIT_FINISHED){
	    	while(1) {}
	    }
#endif
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
}

#define REPORT_SYMID(PREFIX) \
    symID = (PREFIX << 13 & 0xe000) | (symID & 0x1fff);              \
    byteval = (uint8_t *)(&symID);                                  \
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval++;    \
    AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *byteval;


#define REPORT_OP(OP)                                                    \
    byteval = (uint8_t *)(&OP);                                      \
    for( int i = 0 ; i < sizeof(OP); i ++)                               \
        AFLfuzzer.txbuffer[AFLfuzzer.txCurrentIndex++] = *(byteval++);


#define CO3_REPORT_0_OP                                     \
    unsigned msgSize = 2;                                   \
    uint8_t *byteval;                                             \
    txCommandtoMonitorF;                                          \
    REPORT_SYMID(0b000)



#define CO3_REPORT_1_OP(OP1)                                      \
    unsigned msgSize = 2 + sizeof(OP1);                           \
    uint8_t *byteval;                                             \
    txCommandtoMonitorF;                                          \
    if(msgSize == 3){                                             \
        REPORT_SYMID(0b001)                                        \
    }else if(msgSize == 4){                                        \
        REPORT_SYMID(0b010)                                        \
    }else if(msgSize == 6){                                        \
        REPORT_SYMID(0b011)                                         \
    }else{                                                        \
         while(1){}                                                              \
    }                                                              \
    REPORT_OP(OP1)


#define CO3_REPORT_2_OPS(OP1, OP2) \
    unsigned msgSize = 2 + sizeof(OP1) + sizeof(OP2); \
    uint8_t *byteval;              \
    txCommandtoMonitorF;                                \
    if(msgSize != 10){                                 \
        while(1){}                                     \
    }                                                  \
    REPORT_SYMID(0b100)                                 \
    REPORT_OP(OP1)                                     \
    REPORT_OP(OP2)

#define CO3_REPORT_3_OPS(OP1, OP2, OP3) \
    unsigned msgSize = 2 + sizeof(OP1) + sizeof(OP2) + sizeof(OP3); \
    uint8_t *byteval;                   \
    txCommandtoMonitorF;                            \
    if(msgSize != 14){                              \
        while(1){}                                  \
    }                                              \
    REPORT_SYMID(0b101)                            \
    REPORT_OP(OP1)                                 \
    REPORT_OP(OP2)                                 \
    REPORT_OP(OP3)

#define CO3_DEFINE_BINARY_OPERATION(NAME) \
    bool NAME(bool input1, bool input2, uint16_t symID){  \
        CO3_REPORT_0_OP                                                \
        return true;                                  \
    }


bool _sym_build_integer(uint32_t val, uint8_t is_signed, uint16_t symID){
    CO3_REPORT_1_OP(val)
    return true;
}
//void _sym_build_integer128
//void _sym_build_float

bool _sym_build_null_pointer(uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_build_true(uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_build_false(uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_build_bool(bool input, uint16_t symID){
    CO3_REPORT_1_OP(input)
    return true;
}

bool _sym_build_sext(bool input, int8_t ext, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_build_zext(bool input, int8_t ext, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}
bool _sym_build_trunc(bool input, int8_t ext, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}
bool _sym_build_bswap(bool input, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}


CO3_DEFINE_BINARY_OPERATION(_sym_build_bool_and)
CO3_DEFINE_BINARY_OPERATION(_sym_build_bool_or)
CO3_DEFINE_BINARY_OPERATION(_sym_build_bool_xor)

CO3_DEFINE_BINARY_OPERATION(_sym_build_add)
CO3_DEFINE_BINARY_OPERATION(_sym_build_sub)
CO3_DEFINE_BINARY_OPERATION(_sym_build_mul)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_div)
CO3_DEFINE_BINARY_OPERATION(_sym_build_signed_div)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_rem)
CO3_DEFINE_BINARY_OPERATION(_sym_build_signed_rem)
CO3_DEFINE_BINARY_OPERATION(_sym_build_shift_left)
CO3_DEFINE_BINARY_OPERATION(_sym_build_logical_shift_right)
CO3_DEFINE_BINARY_OPERATION(_sym_build_arithmetic_shift_right)
CO3_DEFINE_BINARY_OPERATION(_sym_build_and)
CO3_DEFINE_BINARY_OPERATION(_sym_build_or)
CO3_DEFINE_BINARY_OPERATION(_sym_build_xor)


CO3_DEFINE_BINARY_OPERATION(_sym_build_equal)
CO3_DEFINE_BINARY_OPERATION(_sym_build_not_equal)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_greater_than)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_greater_equal)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_less_than)
CO3_DEFINE_BINARY_OPERATION(_sym_build_unsigned_less_equal)
CO3_DEFINE_BINARY_OPERATION(_sym_build_signed_greater_than)
CO3_DEFINE_BINARY_OPERATION(_sym_build_signed_greater_equal)
CO3_DEFINE_BINARY_OPERATION(_sym_build_signed_less_than)
CO3_DEFINE_BINARY_OPERATION(_sym_build_less_equal)



bool _sym_build_bool_to_bits(bool input, int8_t bits, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}


void _sym_build_path_constraint(bool input, bool cond, uint16_t symID){
    CO3_REPORT_1_OP(cond);
}


void _sym_set_parameter_expression(int8_t para_index, bool input, uint16_t symID)
{
    CO3_REPORT_0_OP
}

bool _sym_get_parameter_expression(int8_t para_index, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

void _sym_set_return_expression(bool input, uint16_t symID)
{
    CO3_REPORT_0_OP
}

bool _sym_get_return_expression(uint16_t symID)
{
    CO3_REPORT_0_OP
    return true;
}

void _sym_build_memcpy(char * dest, char * src, int len, uint16_t symID){
    CO3_REPORT_3_OPS(dest, src, len)
}

void _sym_build_memset(char * ptr, bool input, int len, uint16_t symID){
    CO3_REPORT_2_OPS(ptr, len)
}

void _sym_build_memmove(char * dest, char * src, int len, uint16_t symID){
    CO3_REPORT_3_OPS(dest, src, len)
}

bool _sym_build_read_memory(char * ptr, int size, int8_t is_little, uint16_t symID){
    CO3_REPORT_1_OP(ptr)
    return true;
}

void _sym_build_write_memory(char * ptr, int size, bool input, int8_t is_little, uint16_t symID){
    CO3_REPORT_1_OP(ptr)
}

bool _sym_build_zero_bytes(int size, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_notify_select(bool cond, bool if_true, bool if_false, char * base_addr, int8_t offset, uint16_t symID){
    CO3_REPORT_1_OP(cond)
    return true;
}

bool _sym_build_insert(bool aggr_operand, bool inserted_operand, int offset, int8_t is_little, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

bool _sym_build_extract(bool input, int32_t offset, int32_t store_size, int8_t is_little, uint16_t symID){
    CO3_REPORT_0_OP
    return true;
}

void _sym_try_alternative(bool input1, bool input2, uint16_t symID){
    CO3_REPORT_0_OP
}

void _sym_notify_phi(int8_t phi_value, uint16_t symID, bool input, char * ptr, int8_t offset){
    CO3_REPORT_1_OP(phi_value)
}


void _sym_symbolize_memory(char * addr, size_t len, bool DR){
    uint16_t symID = 1;
    CO3_REPORT_1_OP(addr)
}

void _sym_end(){
    uint16_t symID = 2;
    CO3_REPORT_0_OP
}
void _sym_notify_call(int8_t callee_id){
    uint16_t symID = 3;
    CO3_REPORT_1_OP(callee_id)
}

void _sym_notify_func(int8_t func_id){
    uint16_t symID = 4;
    CO3_REPORT_1_OP(func_id)
}

void _sym_notify_ret(int8_t callee_id){
    uint16_t symID = 5;
    CO3_REPORT_1_OP(callee_id)
}

void _sym_notify_basic_block(int16_t bb_id, bool input, char * base_addr, int8_t offset){
    //CO3_REPORT_1_OP(_sym_notify_basic_block, bb_id)
}


/*
void _sym_build_integer128(uint64_t left, uint64_t right){

}*/

