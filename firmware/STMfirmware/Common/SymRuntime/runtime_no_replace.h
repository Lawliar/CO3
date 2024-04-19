// This file is part of the CO3 runtime.
//
// The CO3 runtime is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The CO3 runtime is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CO3. If not, see <https://www.gnu.org/licenses/>.


#ifndef RUNTIME_H_
#define RUNTIME_H_

//includes
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#define DEBUGPRINT 0

typedef enum{
    SymOp_sym_build_integer,
    SymOp_sym_build_integer128,
#if defined(SUPPORT_FLOAT)
    SymOp_sym_build_float,
#endif
    SymOp_sym_build_null_pointer,
    SymOp_sym_build_true,
    SymOp_sym_build_false,
    SymOp_sym_build_bool,
    SymOp_sym_build_sext,
    SymOp_sym_build_zext,
    SymOp_sym_build_trunc,
    SymOp_sym_build_bswap,
#if defined (SUPPORT_FLOAT)
    SymOp_sym_build_int_to_float,
    SymOp_sym_build_float_to_float,
    SymOp_sym_build_bits_to_float,
    SymOp_sym_build_float_to_bits,
    SymOp_sym_build_float_to_signed_integer,
    SymOp_sym_build_float_to_unsigned_integer,
    SymOp_sym_build_fp_abs,
#endif
    SymOp_sym_build_bool_and,
    SymOp_sym_build_bool_or,
    SymOp_sym_build_bool_xor,
    SymOp_sym_build_bool_to_bits,
    SymOp_sym_build_path_constraint,


    SymOp_sym_set_parameter_expression,
    SymOp_sym_get_parameter_expression,
    SymOp_sym_set_return_expression,
    SymOp_sym_get_return_expression,


    SymOp_sym_build_memcpy,
    SymOp_sym_build_memset,
    SymOp_sym_build_memmove,
    SymOp_sym_build_read_memory,
    SymOp_sym_build_write_memory,
    SymOp_sym_build_zero_bytes,
    SymOp_sym_notify_select,
    SymOp_sym_build_insert,
    SymOp_sym_build_extract,

    SymOp_sym_try_alternative,
    SymOp_sym_notify_phi,
    SymOp_sym_notify_call,
    SymOp_sym_notify_ret,
    SymOp_sym_notify_func,
    SymOp_sym_notify_basic_block,

    SymOp_sym_build_add,
    SymOp_sym_build_sub,
    SymOp_sym_build_mul,
    SymOp_sym_build_unsigned_div,
    SymOp_sym_build_signed_div,
    SymOp_sym_build_unsigned_rem,
    SymOp_sym_build_signed_rem,
    SymOp_sym_build_shift_left,
    SymOp_sym_build_logical_shift_right,
    SymOp_sym_build_arithmetic_shift_right,
    SymOp_sym_build_and,
    SymOp_sym_build_or,
    SymOp_sym_build_xor,

#if defined (SUPPORT_FLOAT)
    SymOp_sym_build_fp_add,
    SymOp_sym_build_fp_sub,
    SymOp_sym_build_fp_mul,
    SymOp_sym_build_fp_div,
    SymOp_sym_build_fp_rem,
#endif

    SymOp_sym_build_equal,
    SymOp_sym_build_not_equal,
    SymOp_sym_build_unsigned_greater_than,
    SymOp_sym_build_unsigned_greater_equal,
    SymOp_sym_build_unsigned_less_than,
    SymOp_sym_build_unsigned_less_equal,
    SymOp_sym_build_signed_greater_than,
    SymOp_sym_build_signed_greater_equal,
    SymOp_sym_build_signed_less_than,
    SymOp_sym_build_signed_less_equal,
} SymOpEnums;

#define NUMBER_SYM_PERI           10
#define NUMBER_PARAMETER_EXP      64


extern uint32_t *sym_peripherals[NUMBER_SYM_PERI];
extern uint8_t total_sym_peripherals;
extern uint32_t *shadowram;

extern bool parameter_exp[NUMBER_PARAMETER_EXP];
extern bool return_exp;

bool _sym_peripheral_symb(uint32_t *addr);   // call this function for each peripheral register that will be symbolized, addr: peripheral address register
//void _sym_peripheral_conc(uint32_t *addr);


/*
 *
 * https://github.com/RiS3-Lab/spear-writeup/blob/master/log.md?plain=1#L170
 *
 * | arg_idx | byte width | real value ... \
   | 1 byte  |  1 byte    |  x bytes
 * */

#endif /* RUNTIME_H_ */
