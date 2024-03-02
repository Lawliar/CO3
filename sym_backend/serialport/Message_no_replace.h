#ifndef SYMBACKEND_MESSAGE_NO_REPLACE_H
#define SYMBACKEND_MESSAGE_NO_REPLACE_H

#include "stdint.h"
#include <sstream>

class Message{
public:
    typedef enum _MessageType{
        SymSymbolizeMsg = 1,
        SymEndMsg,
        NotifyCallMsg,
        NotifyFuncMsg,
        NotifyRetMsg,
        NotifyBBMsg,
        NormalMsg,
        Normal_0opMsg,
        Normal_1op8Msg,
        Normal_1op16Msg,
        Normal_1op32Msg,
        Normal_1op32_2op32Msg,
        Normal_1op32_2op32_3op32Msg,
    } MessageType;
    MessageType type;
    Message(MessageType t): type(t){}
    virtual ~Message() {}
};

#define NOTIFYCLASSTEMPLATE(Name) \
    class Notify##Name##Message: public Message{ \
    public:                       \
         uint8_t id;             \
         Notify##Name##Message(uint8_t id): Message(Notify##Name##Msg), id(id){}\
    };


class InitMessage: public Message{
public:
    char* addr;
    bool DR = false;
    InitMessage(char* addr, bool DR): Message(SymSymbolizeMsg), addr(addr), DR(DR) {}
};

class EndMessage: public Message{
public:
    EndMessage(): Message(SymEndMsg){};
};

NOTIFYCLASSTEMPLATE(Call)
NOTIFYCLASSTEMPLATE(Func)
NOTIFYCLASSTEMPLATE(Ret)
NOTIFYCLASSTEMPLATE(BB)

class NormalMessage: public Message{
public:
    uint16_t symID;
    NormalMessage(uint16_t symID): Message(NormalMsg), symID(symID){}
};

class NormalMessage_0OP: public NormalMessage{
public:
    NormalMessage_0OP(uint16_t symID): NormalMessage(symID){}
};

class NormalMessage_1OP8: public NormalMessage{
public:
    uint8_t op1;
    NormalMessage_1OP8(uint16_t symID, uint8_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP16: public NormalMessage{
public:
    uint16_t op1;
    NormalMessage_1OP16(uint16_t symID, uint16_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP32: public NormalMessage{
public:
    uint32_t op1;
    NormalMessage_1OP32(uint16_t symID, uint32_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP32_2OP32: public NormalMessage{
public:
    uint32_t op1;
    uint32_t op2;
    NormalMessage_1OP32_2OP32(uint16_t symID, uint32_t op1,uint32_t op2 ): NormalMessage(symID), op1(op1), op2(op2){}
};
class NormalMessage_1OP32_2OP32_3OP32: public NormalMessage{
public:
    uint32_t op1;
    uint32_t op2;
    uint32_t op3;
    NormalMessage_1OP32_2OP32_3OP32(uint16_t symID, uint32_t op1,uint32_t op2, uint32_t op3 ): NormalMessage(symID), op1(op1), op2(op2), op3(op3){}
};

#endif