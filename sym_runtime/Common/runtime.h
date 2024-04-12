/*
 * runtime.h
 *
 *  Created on: Apr 29, 2022
 *      Author: alejandro
 */
#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "CO3CommonRuntime.h"

#define NUMBER_SYM_PERI           10
#define NUMBER_PARAMETER_EXP      64


extern uint32_t *sym_peripherals[NUMBER_SYM_PERI];
extern uint8_t total_sym_peripherals;
extern uint32_t *shadowram;

extern bool parameter_exp[NUMBER_PARAMETER_EXP];
extern bool return_exp;

bool _sym_peripheral_symb(uint32_t *addr);   // call this function for each peripheral register that will be symbolized, addr: peripheral address register
//void _sym_peripheral_conc(uint32_t *addr);

// all the following functions return true
bool _sym_build_integer(uint32_t int_val, uint8_t numBits, uint16_t symID);
bool _sym_build_float(double double_val, bool is_double, uint16_t symID);
bool _sym_build_bool(bool bool_val, uint16_t symID);
void _sym_build_path_constraint(bool input, bool runtimeVal, uint16_t symID);

// needs shadow and report
void _sym_build_memcpy(char * dest, char * src, size_t length, uint16_t symID);
void _sym_build_memset(char * mem, bool input, size_t length, uint16_t symID);
void _sym_build_memmove(char * dest, char * src, size_t length, uint16_t symID);
bool _sym_build_read_memory(char * addr, size_t length, bool is_little_edian, uint16_t symID);
void _sym_build_write_memory(char * addr, size_t length, bool input,bool is_little_edian, uint16_t symID );

//needs some RAM, no report
void _sym_set_parameter_expression(int8_t para_index, bool input);
bool _sym_get_parameter_expression(int8_t para_index);
void _sym_set_return_expression(bool input);
bool _sym_get_return_expression();

//needs report
void _sym_notify_phi(uint8_t branchNo, uint16_t symID, bool isSym, char * base_addr, uint8_t offset );
void _sym_notify_call(uint8_t call_inst_id);
void _sym_notify_func(uint8_t func_id);
void _sym_notify_ret(uint8_t call_inst_id);
void _sym_notify_basic_block(uint16_t bbid, bool isSym, char * base_addr, uint8_t offset );

void _sym_notify_select(bool cond, bool isSym1, bool isSym2, char * base_addr, uint8_t offset, uint16_t symID);

void _sym_try_alternative(bool destSym, bool concSym, uint16_t symID);
//symbolize buffer
void _sym_symbolize_memory(char * addr, size_t length, bool DR);

void _sym_initialize();// allocate the shadow memory, and the space for the return values, parameters
void _sym_end();


void sockRec();
/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes
 * */

#endif /* RUNTIME_H_ */
