#include "stdint.h"
#include <sstream>
#include <map>


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
    } MessageType;
    MessageType type;
    Message(MessageType t): type(t){}
    virtual ~Message() {}
};

#define NOTIFYCLASSTEMPLATE(Name) \
    class Notify##Name##Message: public Message{ \
    public:                       \
         uint16_t id;             \
         Notify##Name##Message(uint16_t id): Message(Notify##Name##Msg), id(id){}\
    };


class InitMessage: public Message{
public:
    char* addr;
    bool DR = false;
    InitMessage(char* addr): Message(SymSymbolizeMsg), addr(addr) {}
};

class SymEndMessage: public Message{
public:
    SymEndMessage(): Message(SymEndMsg){};
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
    NormalMessage_0OP(uint16_t symID): NormalMessage(symID){}
};

class NormalMessage_1OP8: public NormalMessage{
    uint8_t op1;
    NormalMessage_1OP8(uint16_t symID, uint8_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP16: public NormalMessage{
    uint16_t op1;
    NormalMessage_1OP16(uint16_t symID, uint16_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP32: public NormalMessage{
    uint32_t op1;
    NormalMessage_1OP32(uint16_t symID, uint32_t op1 ): NormalMessage(symID), op1(op1){}
};

class NormalMessage_1OP32_2OP32: public NormalMessage{
    uint32_t op1;
    uint32_t op2;
    NormalMessage_1OP32_2OP32(uint16_t symID, uint32_t op1,uint32_t op2 ): NormalMessage(symID), op1(op1), op2(op2){}
};
class NormalMessage_1OP32_2OP32_3OP32: public NormalMessage{
    uint32_t op1;
    uint32_t op2;
    uint32_t op3;
    NormalMessage_1OP32_2OP32_3OP32(uint16_t symID, uint32_t op1,uint32_t op2, uint32_t op3 ): NormalMessage(symID), op1(op1), op2(op2), op3(op3){}
};