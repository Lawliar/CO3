/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */
#ifndef RUNTIME_H_
#define RUNTIME_H_

//includes
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

//types and enums
enum {
  /* 01 */ SYM_BLD_INT  =       1,
  /* 02 */ SYM_BLD_FLOAT         ,
  /* 03 */ SYM_BLD_BOOL          ,
  /* 04 */ SYM_BLD_PATH_CNSTR    ,
  /* 05 */ SYM_BLD_MEMCPY        ,
  /* 06 */ SYM_BLD_MENSET        ,
  /* 07 */ SYM_BLD_MEMMOVE       ,
  /* 08 */ SYM_BLD_READ_MEM      ,
  /* 09 */ SYM_BLD_WRITE_MEM     ,

  /* 10 */ SYM_SET_PAR_EXP       ,
  /* 11 */ SYM_GET_PAR_EXP       ,
  /* 12 */ SYM_SET_RET_EXP       ,

  /* 13 */ SYM_NTFY_CALL         ,
  /* 14 */ SYM_NTFY_FUNC         ,
  /* 15 */ SYM_NTFY_RET          ,
  /* 16 */ SYM_NTFY_BBLK         ,
  /* 17 */ SYM_NTFY_BBLK1        ,

  /* 18 */ SYM_INIT
};
/*
#define SIZE_SYM_INIT   1
#define SIZE_SPEAR_RPT1 10
#define SIZE_SPEAR_RPT2 15
#define SIZE_SPEAR_RPT3 20
#define SIZE_SPEAR_RPT4 25
#define SIZE_SPEAR_SYM  9
#define SIZE_SYM_N_CALL 5
#define SIZE_SYM_N_RET  5
#define SIZE_SYM_N_BB   5
*/

#define SIZE_SYM_BLD_INT
#define SIZE_SYM_BLD_FLOAT
#define SIZE_SYM_BLD_BOOL
#define SIZE_SYM_BLD_PATH_CNSTR
#define SIZE_SYM_BLD_MEMCPY
#define SIZE_SYM_BLD_MENSET
#define SIZE_SYM_BLD_MEMMOVE
#define SIZE_SYM_BLD_READ_MEM
#define SIZE_SYM_BLD_WRITE_MEM
#define SIZE_SYM_SET_PAR_EXP
#define SIZE_SYM_GET_PAR_EXP
#define SIZE_SYM_SET_RET_EXP
#define SIZE_SYM_NTFY_CALL
#define SIZE_SYM_NTFY_FUNC
#define SIZE_SYM_NTFY_RET
#define SIZE_SYM_NTFY_BBLK
#define SIZE_SYM_NTFY_BBLK1
#define SIZE_SYM_INIT


// all the following functions return true
bool _sym_build_integer(uint16_t int_val, uint8_t numBits, uint16_t symID);
bool _sym_build_float(uint64_t double_val, bool is_double, uint16_t symID);
bool _sym_build_bool(bool bool_val, uint16_t symID);
void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID);
void _sym_build_memcpy(char * dest, char * src, size_t length, uint16_t symID);
void _sym_build_memset(char * mem, bool input, size_t length, uint16_t symID);
void _sym_build_memmove(char * dest, char * src, size_t length, uint16_t symID);
bool _sym_build_read_memory(char * addr, size_t length, bool is_little_edian, uint16_t symID);
void _sym_build_write_memory(char * addr, size_t length, bool input, uint16_t symID );

void _sym_set_parameter_expression(int8_t para_index, bool input);
bool _sym_get_parameter_expression(int8_t para_index);
void _sym_set_return_expression(bool input);
bool _sym_get_return_expression();

void _sym_notify_call(uint8_t call_inst_id);
void _sym_notify_func(uint8_t func_id);
void _sym_notify_ret(uint8_t call_inst_id);
void _sym_notify_basic_block(uint8_t bbid);
void _sym_notify_basic_block1(uint16_t bbid);


void _sym_initialize();// allocate the shadow memory, and the space for the return values, parameters


/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes
 * */

#endif /* RUNTIME_H_ */