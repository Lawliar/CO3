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
        BoolRuntimeValMsg,
        IntRuntimeValMsg,
        FloatRuntimeValMsg,
        DoubleRuntimeValMsg,
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
    ControlMessgaes(MessageType t, uint32_t id, uint16_t symid);
    uint32_t id;
    uint16_t symid;//needed only for phi
};

class RuntimeBoolValueMessage: public Message{
public:
    RuntimeBoolValueMessage(uint16_t symid,bool value):Message(BoolRuntimeValMsg), symid(symid), value(value){};
    uint16_t symid;
    bool value;
};

class RuntimePhiValueMessage: public Message{
public:
    RuntimePhiValueMessage(uint16_t symid,uint8_t value):Message(PhiMsg), symid(symid), value(value){};
    uint16_t symid;
    uint8_t value;
};

class RuntimeIntValueMessage: public Message{
public:
    RuntimeIntValueMessage(uint16_t symid, uint8_t width, int32_t value):Message(IntRuntimeValMsg), symid(symid),width(width), value(value){};
    uint16_t symid;
    uint8_t width;
    int32_t value;
};

class RuntimeFloatValueMessage: public Message{
public:
    RuntimeFloatValueMessage(uint16_t symid,float value):Message(FloatRuntimeValMsg), symid(symid), value(value){};
    uint16_t symid;
    float value;
};

class RuntimeDoubleValueMessage: public Message{
public:
    RuntimeDoubleValueMessage(uint16_t symid,double value):Message(DoubleRuntimeValMsg), symid(symid), value(value){};
    uint16_t symid;
    double value;
};

class PushConstraintMessage: public Message{
public:
    PushConstraintMessage(uint16_t symID, bool runtimeVal):Message(ConstraintRuntimeValMsg), symID(symID), runtimeVal(runtimeVal){};
    uint16_t symID;
    bool runtimeVal;
};

class MemCpyMessage: public Message{
public:
    MemCpyMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): Message(MemCpyRuntimeMsg),symID(symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint16_t symID;
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
};

class MemSetMessage: public Message{
public:
    MemSetMessage(uint16_t symID, uint32_t ptr, uint32_t length): Message(MemsetRuntimeMsg),symID(symID),  ptr(ptr),length(length){};
    uint16_t symID;
    uint32_t ptr;
    uint16_t length;
};

class MemMoveMessage: public Message{
public:
    MemMoveMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): Message(MemmoveRuntimeMsg),symID(symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint16_t symID;
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
};

class ReadMemMessage: public Message{
public:
    ReadMemMessage(uint16_t symID, uint32_t ptr, uint32_t length): Message(ReadMemRuntimeMsg),symID(symID), ptr(ptr),length(length){}
    uint16_t symID;
    uint32_t ptr;
    uint16_t length;
};

class WriteMemMessage: public Message{
public:
    WriteMemMessage(uint16_t symID, uint32_t ptr, uint32_t length): Message(WriteMemRuntimeMsg),symID(symID), ptr(ptr),length(length){}
    uint16_t symID;
    uint32_t ptr;
    uint16_t length;
};

class NotifyCallMessage: public Message{
public:
    NotifyCallMessage(uint8_t id): Message(CallMsg), id(id){};
    uint8_t id;
};

class NotifyRetMessage: public Message{
public:
    NotifyRetMessage(uint8_t id): Message(RetMsg), id(id){};
    uint8_t id;
};

class NotifyBasicBlockMessage: public Message{
public:
    NotifyBasicBlockMessage(uint16_t id): Message(BasicBlockMsg), id(id){};
    uint16_t id;
};

#endif //SYMBACKEND_MESSAGE_H
