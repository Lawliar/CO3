/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */

#ifndef RUNTIME_H_
#define RUNTIME_H_

//includes
#include"stdint.h"


//types and enums

enum {
  /* 01 */ SYM_INIT =1,
  /* 02 */ SPEAR_RPT1,
  /* 03 */ SPEAR_RPT2,
  /* 04 */ SPEAR_RPT3,
  /* 05 */ SPEAR_RPT4,
  /* 06 */ SPEAR_SYM,
  /* 07 */ SYM_N_CALL,
  /* 08 */ SYM_N_RET,
  /* 09 */ SYM_N_BB,

};



// all the following functions return true
bool _sym_build_integer(uint16_t int_val, uint8_t numBits, uint16_t symID);
bool _sym_build_float(uint64_t double_val, bool is_double, uint16_t symID);
bool _sym_build_bool(bool bool_val, uint16_t symID);
//bool _sym_build_integer128(uint64_t higher, uint64_t lower); // leave empty, this function will not be called
//bool _sym_build_null_pointer();
//bool _sym_build_true();
//bool _sym_build_false();


// all the following functions return the same as "input"
//bool _sym_build_sext(bool input, uint8_t numBits);
//bool _sym_build_zext(bool input, uint8_t numBits);
//bool _sym_build_trunc(bool input, uint8_t numBits);
//bool _sym_build_bswap(bool input);
//bool _sym_build_int_to_float(bool input, bool is_double, bool is_signed);
//bool _sym_build_float_to_float(bool input, bool is_double);
//bool _sym_build_bits_to_float(bool input, bool is_double);
//bool _sym_build_float_to_bits(bool input);
//bool _sym_build_float_to_signed_integer(bool input, uint8_t numBits);
//bool _sym_build_float_to_unsigned_integer(bool input, uint8_t numBits);
//bool _sym_build_fp_abs（bool input);
//bool _sym_build_bool_and(bool input1, bool input2);
//bool _sym_build_bool_or(bool input1, bool input2);
//bool _sym_build_bool_xor(bool input1, bool input2);
//bool _sym_build_bool_to_bits(bool input1, uint8_t numBits);

void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID);


void _sym_set_parameter_expression(int8_t para_index, bool input);
bool _sym_get_parameter_expression(int8_t para_index);
void _sym_set_return_expression(bool input);
bool _sym_get_return_expression();


void _sym_build_memcpy(char * dest, char * src, size_t length, uint16_t symID);
void _sym_build_memset(char * mem, bool input, size_t length, uint16_t symID);
void _sym_build_memmove(char * dest, char * src, size_t length, uint16_t symID);

bool _sym_build_read_memory(char * addr, size_t length, bool is_little_edian, uint16_t symID);
void _sym_build_write_memory(char * addr, size_t length, bool input, uint16_t symID );


//bool _sym_build_insert(bool target, bool to_insert, uint32_t offset, bool is_little_edian);
//bool _sym_build_extract(bool expr, uint32_t offset uint32_t length, bool is_little_edian);

void _sym_notify_call(uint32_t call_inst_id);
void _sym_notify_ret(uint32_t call_inst_id);
void _sym_notify_basic_block(uint16_t bbid);
/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes


 * */



// some functions that are used internally(not called by the instrumentation)
void _sym_initialize();// allocate the shadow memory, and the space for the return values, parameters


#endif /* RUNTIME_H_ */
