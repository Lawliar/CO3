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

#define SYM_RAM_ADDR_START           0x24000000
#define SYM_RAM_SIZE                 1024*512
#define SYM_SHADOW_RAM_START         0x30000000
#define SYM_SHADOW_RAM_LENGTH        1024*64
#define SYM_SHADOW_RAM_OFFSET        0x2B800000

#define SYM_FLASH_ADDR_START         0x08000000
#define SYM_FLASH_SIZE               1024*2048

#define SYM_PERIPHERAL_ADDR_START    0x40000000
#define SYM_PERIPHERAL_SIZE          0x1FFFFFFF

#define DEBUGPRINT 0


//types and enums
enum {
 SYM_BLD_INT_1 =      1,
 SYM_BLD_INT_1_1       ,
 SYM_BLD_INT_2         ,
 SYM_BLD_INT_2_1       ,
 SYM_BLD_INT_4         ,
 SYM_BLD_INT_4_1       ,
 SYM_BLD_FLOAT         ,
 SYM_BLD_FLOAT_1       ,
 SYM_BLD_FLOAT_DBL     ,
 SYM_BLD_FLOAT_DBL_1   ,
 SYM_BLD_BOOL          ,
 SYM_BLD_BOOL_1        ,
 SYM_BLD_PATH_CNSTR    ,
 SYM_BLD_PATH_CNSTR_1  ,
 SYM_BLD_MEMCPY        ,
 SYM_BLD_MEMCPY_1      ,
 SYM_BLD_MEMSET        ,
 SYM_BLD_MEMSET_1      ,
 SYM_BLD_MEMMOVE       ,
 SYM_BLD_MEMMOVE_1     ,
 SYM_BLD_READ_MEM      ,
 SYM_BLD_READ_MEM_1    ,
 SYM_BLD_READ_MEM_HW   ,
 SYM_BLD_READ_MEM_HW_1 ,
 SYM_BLD_READ_MEM_W    ,
 SYM_BLD_READ_MEM_W_1  ,

 SYM_BLD_WRITE_MEM     ,
 SYM_BLD_WRITE_MEM_1   ,

// SYM_SET_PAR_EXP       ,  // these are not sent to the PC
// SYM_GET_PAR_EXP       ,
// SYM_SET_RET_EXP       ,
// SYM_GET_RET_EXP       ,

 SYM_NTFY_PHI          ,
 SYM_NTFY_PHI_1        ,

 SYM_NTFY_CALL         ,
 SYM_NTFY_FUNC         ,
 SYM_NTFY_RET          ,
 SYM_NTFY_BBLK         ,
 SYM_NTFY_BBLK1        ,

 SYM_INIT              ,
 SYM_INIT_DR           ,
 SYM_END
};





//legend FIELDNAME (SIZE in bytes)
#define SIZE_SYM_BLD_INT_1          3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_INT_1_1        4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_INT_2          4      // | FCODE(1) | symID(1)         | val(2)      |
#define SIZE_SYM_BLD_INT_2_1        5      // | FCODE(1) | symID(2)         | val(2)      |

#define SIZE_SYM_BLD_INT_4          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_INT_4_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_FLOAT_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT_DBL      10     // | FCODE(1) | symID(1)         | val(8)      |
#define SIZE_SYM_BLD_FLOAT_DBL_1    11     // | FCODE(1) | symID(2)         | val(8)      |

#define SIZE_SYM_BLD_BOOL           3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_BOOL_1         4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_PATH_CNSTR     3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_PATH_CNSTR_1   4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_MEMCPY         12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMCPY_1       13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_MEMSET         8      // | FCODE(1) | symID(1)         | mem(4)      | length(2) |
#define SIZE_SYM_BLD_MEMSET_1       9      // | FCODE(1) | symID(2)         | mem(4)      | length(2) |

#define SIZE_SYM_BLD_MEMMOVE        12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMMOVE_1      13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_READ_MEM       6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_READ_MEM_1     7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_BLD_READ_MEM_HW    8      // | FCODE(1) | symID(1)         | addr(4)     | val(2)    |
#define SIZE_SYM_BLD_READ_MEM_HW_1  9      // | FCODE(1) | symID(2)         | addr(4)     | val(2)    |

#define SIZE_SYM_BLD_READ_MEM_W     10     // | FCODE(1) | symID(1)         | addr(4)     | val(4)    |
#define SIZE_SYM_BLD_READ_MEM_W_1   11     // | FCODE(1) | symID(2)         | addr(4)     | val(4)    |

#define SIZE_SYM_BLD_WRITE_MEM      6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_WRITE_MEM_1    7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_NTFY_PHI           3      // | FCODE(1) | symID(1)         | branchNo(1) |
#define SIZE_SYM_NTFY_PHI_1         4      // | FCODE(1) | symID(2)         | branchNo(1) |

#define SIZE_SYM_NTFY_CALL          2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_FUNC          2      // | FCODE(1) | func_id(1)       |
#define SIZE_SYM_NTFY_RET           2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_BBLK          2      // | FCODE(1) | bbid(1)          |
#define SIZE_SYM_NTFY_BBLK1         3      // | FCODE(1) | bbid(2)          |
#define SIZE_SYM_INIT               5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_INIT_DR            5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_END                1      // | FCODE(1) |

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


//symbolize buffer
void _sym_symbolize_memory(char * addr, size_t length, bool DR);

void _sym_initialize();// allocate the shadow memory, and the space for the return values, parameters
void _sym_end();

/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes
 * */

#endif /* RUNTIME_H_ */
