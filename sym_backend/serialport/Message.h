//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MESSAGE_H
#define SYMBACKEND_MESSAGE_H

#include "stdint.h"

class Message {
public:
    typedef enum _MessageType{
        BasicBlockMsg,
        CallMsg,
        RetMsg,
        PhiMsg,
        IntRuntimeValMsg,
        FloatRuntimeValMsg,
        BoolRuntimeValMsg,
        ReadMemRuntimeMsg,
        ConstraintRuntimeValMsg,
        MemCpyRuntimeMsg,
        MemsetRuntimeMsg,
        MemmoveRuntimeMsg,
        WriteMemRuntimeMsg,
    } MessageType;
    MessageType type;
    Message(MessageType t): type(t){}
    virtual ~Message() {}
};

class ControlMessgaes : public Message{
public:
    ControlMessgaes(MessageType t, uint32_t id, uint32_t symid);
    uint32_t id;
    uint32_t symid;//needed only for phi
};

class RuntimeValueMessage: public Message{
public:
    RuntimeValueMessage(MessageType,uint32_t symid, uint8_t width, int64_t value);
    uint32_t symid;
    uint8_t width;
    int64_t value;
};

class CallToSolverMessage: public Message{
public:
    CallToSolverMessage(uint32_t symID, bool runtimeVal);
    uint32_t symID;
    bool runtimeVal;
};
class MemOpMessage: public Message{
public:
    MemOpMessage(MessageType , uint32_t src_ptr, uint32_t dst_ptr, uint32_t length, uint32_t symID);
    uint32_t src_ptr;
    uint32_t dst_ptr;
    uint32_t length;
    uint32_t symID;
};

#endif //SYMBACKEND_MESSAGE_H
