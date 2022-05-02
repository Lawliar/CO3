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
  /* 00 */ SYM_INIT,
  /* 01 */ SPEAR_RPT1,
  /* 02 */ SPEAR_RPT2,
  /* 03 */ SPEAR_RPT3,
  /* 04 */ SPEAR_RPT4,
  /* 05 */ SPEAR_SYM,
  /* 06 */ SYM_N_CALL,
  /* 07 */ SYM_N_RET,
  /* 08 */ SYM_N_BB,

};




//Function prototypes
void _sym_initialize();
void _spear_report1(uint32_t userID, char * arg1);
void _spear_report2(uint32_t userID, char * arg1,char * arg2);
void _spear_report3(uint32_t userID, char * arg1,char * arg2,char * arg3);
void _spear_report4(uint32_t userID, char * arg1,char * arg2,char * arg3,char * arg4);
void _spear_symbolize(char * address, uint32_t len);
void _sym_notify_call(uint32_t id);
void _sym_notify_ret(uint32_t id);
void _sym_notify_basic_block(uint32_t id);

/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes


 * */



#endif /* RUNTIME_H_ */
