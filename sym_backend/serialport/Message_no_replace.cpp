#include <Message_no_replace.h>
#include <map>

#define SYM_0OP(NAME) {NAME, Message::Normal_0opMsg}

#define SYM_1OP8(NAME) {NAME, Message::Normal_1op8Msg}

#define SYM_1OP16(NAME) {NAME, Message::Normal_1op16Msg}

#define SYM_1OP32(NAME) {NAME, Message::Normal_1op32Msg}

#define SYM_1OP32_2OP32(NAME) {NAME, Message::Normal_1op32_2op32Msg}

#define SYM_1OP32_2OP32_3OP32(NAME) {NAME, Message::Normal_1op32_2op32_3op32Msg}

std::map<std::string, Message::MessageType> SymOp2MsgType = {
        SYM_1OP32("_sym_build_integer"),
        SYM_0OP("_sym_build_null_pointer"),
        SYM_0OP("_sym_build_true"),
        SYM_0OP("_sym_build_false"),
        SYM_1OP8("_sym_build_bool"),
        SYM_0OP("_sym_build_sext"),
        SYM_0OP("_sym_build_zext"),
        SYM_0OP("_sym_build_trunc"),
        SYM_0OP("_sym_build_bswap"),
        SYM_0OP("_sym_build_bool_and"),
        SYM_0OP("_sym_build_bool_or"),
        SYM_0OP("_sym_build_bool_xor"),
        SYM_0OP("_sym_build_add"),
        SYM_0OP("_sym_build_sub"),
        SYM_0OP("_sym_build_mul"),
        SYM_0OP("_sym_build_unsigned_div"),
        SYM_0OP("_sym_build_signed_div"),
        SYM_0OP("_sym_build_unsigned_rem"),
        SYM_0OP("_sym_build_signed_rem"),
        SYM_0OP("_sym_build_shift_left"),
        SYM_0OP("_sym_build_logical_shift_right"),
        SYM_0OP("_sym_build_arithmetic_shift_right"),
        SYM_0OP("_sym_build_and"),
        SYM_0OP("_sym_build_or"),
        SYM_0OP("_sym_build_xor"),
        SYM_0OP("_sym_build_equal"),
        SYM_0OP("_sym_build_not_equal"),
        SYM_0OP("_sym_build_unsigned_greater_than"),
        SYM_0OP("_sym_build_unsigned_greater_equal"),
        SYM_0OP("_sym_build_unsigned_less_than"),
        SYM_0OP("_sym_build_unsigned_less_equal"),
        SYM_0OP("_sym_build_signed_greater_than"),
        SYM_0OP("_sym_build_signed_greater_equal"),
        SYM_0OP("_sym_build_signed_less_than"),
        SYM_0OP("_sym_build_less_equal"),

        SYM_0OP("_sym_build_bool_to_bits"),
        SYM_1OP8("_sym_build_path_constraint"),
        SYM_0OP("_sym_set_parameter_expression"),
        SYM_0OP("_sym_get_parameter_expression"),
        SYM_0OP("_sym_set_return_expression"),
        SYM_0OP("_sym_get_return_expression"),

        SYM_1OP32_2OP32_3OP32("_sym_build_memcpy"),
        SYM_1OP32_2OP32("_sym_build_memset"),
        SYM_1OP32_2OP32_3OP32("_sym_build_memmove"),

        SYM_1OP32("_sym_build_read_memory"),
        SYM_1OP32("_sym_build_write_memory"),

        SYM_0OP("_sym_build_zero_bytes"),
        SYM_1OP8("_sym_notify_select"),
        SYM_0OP("_sym_build_insert"),
        SYM_0OP("_sym_build_extract"),
        SYM_0OP("_sym_try_alternative"),
        SYM_1OP8("_sym_notify_phi"),

        SYM_1OP32_2OP32("_sym_symbolize_memory"),
        SYM_0OP("_sym_end"),
        SYM_1OP8("_sym_notify_call"),
        SYM_1OP8("_sym_notify_func"),
        SYM_1OP8("_sym_notify_ret"),
};