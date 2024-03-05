#include <Message_no_replace.h>
#include <map>
#include <MsgQueue.h>
#include <assert.h>
#include <iostream>

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

        SYM_1OP32("_sym_symbolize_memory"),
        SYM_0OP("_sym_end"),
        SYM_1OP8("_sym_notify_call"),
        SYM_1OP8("_sym_notify_func"),
        SYM_1OP8("_sym_notify_ret"),
};

bool MsgQueue::RenderAndPush(char * buf, char size){
    int cur = 0;
    bool end_received = false;
    while(cur < size){
        uint16_t symID = *(uint16_t *)(buf + cur);
        uint8_t t = (symID & 0xe000) >> 13;
        symID = symID & 0x1fff;

        cur += 2;
        if(symID == 1){
            char * addr = reinterpret_cast<char *>(*(uint32_t*)(buf + cur));
            Push(new InitMessage(addr, false));
            cur += sizeof(uint32_t);
        }else if(symID == 2){
            Push(new EndMessage());
            std::cout<<"end msg received\n";
            std::cout.flush();
            end_received = true;
        }else if(symID == 3){
            uint8_t callID = reinterpret_cast<uint8_t>(*(uint8_t *)(buf + cur));
            Push(new NotifyCallMessage(callID));
            cur += sizeof(uint8_t);
        }else if(symID == 4){
            uint8_t funcID = reinterpret_cast<uint8_t>(*(uint8_t *)(buf + cur));
            Push(new NotifyFuncMessage(funcID));
            cur += sizeof(uint8_t);
        }else if(symID == 5){
            uint8_t retID = reinterpret_cast<uint8_t>(*(uint8_t *)(buf + cur));
            Push(new NotifyRetMessage(retID));
            cur += sizeof(uint8_t);
        }else if(symID == 6){
            assert(false);
        }else{
            if(t == 0b000){
                Push(new NormalMessage_0OP(symID));
            }else if(t == 0b001){
                uint8_t op1 = reinterpret_cast<uint8_t>(*(uint8_t *)(buf + cur));
                Push(new NormalMessage_1OP8(symID, op1));
                cur += 1;
            }else if(t == 0b010){
                uint16_t op1 = reinterpret_cast<uint16_t>(*(uint16_t *)(buf + cur));
                Push(new NormalMessage_1OP16(symID, op1));
                cur += 2;
            }else if(t == 0b011){
                uint32_t op1 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur));
                Push(new NormalMessage_1OP32(symID, op1));
                cur += 4;
            }else if(t == 0b100){
                uint32_t op1 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur));
                uint32_t op2 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur + 4));
                Push(new NormalMessage_1OP32_2OP32(symID, op1,op2));
                cur += 8;
            }else if(t == 0b101){
                uint32_t op1 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur));
                uint32_t op2 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur + 4));
                uint32_t op3 = reinterpret_cast<uint32_t>(*(uint32_t *)(buf + cur + 8));
                Push(new NormalMessage_1OP32_2OP32_3OP32(symID, op1, op2, op3));
                cur += 12;
            }else{
                std::cerr <<"corrupt type:"<< t  <<", the connection is corrupted";
                abort();
            }
        }
    }
    assert(cur == size);
    return end_received;
}