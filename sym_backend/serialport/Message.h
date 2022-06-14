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
        FuncMsg,
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
    ControlMessgaes(MessageType t): Message(t){};
};

class SymSinkMessage : public Message{
public:
    uint16_t symid;
    SymSinkMessage(MessageType t, uint16_t symid): Message(t), symid(symid){};
};

class SymSourceMessage : public Message{
public:
    uint16_t symid;
    SymSourceMessage(MessageType t,uint16_t symid): Message(t), symid(symid){};
};

class RuntimeBoolValueMessage: public SymSourceMessage{
public:
    RuntimeBoolValueMessage(uint16_t symid,bool value):SymSourceMessage(BoolRuntimeValMsg, symid), value(value){};
    bool value;
};

class RuntimePhiValueMessage: public Message{
public:
    RuntimePhiValueMessage(uint16_t symid,uint8_t value):Message(PhiMsg), symid(symid), value(value){};
    uint16_t symid;
    uint8_t value;
};

class RuntimeIntValueMessage: public SymSourceMessage{
public:
    RuntimeIntValueMessage(uint16_t symid, uint8_t width, int32_t value):SymSourceMessage(IntRuntimeValMsg, symid),width(width), value(value){};
    uint8_t width;
    int32_t value;
};

class RuntimeFloatValueMessage: public SymSourceMessage{
public:
    RuntimeFloatValueMessage(uint16_t symid,float value):SymSourceMessage(FloatRuntimeValMsg, symid), value(value){};
    float value;
};

class RuntimeDoubleValueMessage: public SymSourceMessage{
public:
    RuntimeDoubleValueMessage(uint16_t symid,double value):SymSourceMessage(DoubleRuntimeValMsg, symid), value(value){};
    double value;
};

class PushConstraintMessage: public SymSinkMessage{
public:
    PushConstraintMessage(uint16_t symID, bool runtimeVal):SymSinkMessage(ConstraintRuntimeValMsg, symID), runtimeVal(runtimeVal){};
    bool runtimeVal;
};

class MemCpyMessage: public SymSinkMessage{
public:
    MemCpyMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): SymSinkMessage(MemCpyRuntimeMsg, symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
};

class MemSetMessage: public SymSinkMessage{
public:
    MemSetMessage(uint16_t symID, uint32_t ptr, uint32_t length): SymSinkMessage(MemsetRuntimeMsg,symID), ptr(ptr),length(length){};
    uint32_t ptr;
    uint16_t length;
};

class MemMoveMessage: public SymSinkMessage{
public:
    MemMoveMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): SymSinkMessage(MemmoveRuntimeMsg, symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
};

class ReadMemMessage: public SymSourceMessage{
public:
    ReadMemMessage(uint16_t symID, uint32_t ptr): SymSourceMessage(ReadMemRuntimeMsg, symID), ptr(ptr){}
    uint32_t ptr;
};

class WriteMemMessage: public SymSinkMessage{
public:
    WriteMemMessage(uint16_t symID, uint32_t ptr): SymSinkMessage(WriteMemRuntimeMsg, symID), ptr(ptr){}
    uint32_t ptr;
};

class NotifyCallMessage: public ControlMessgaes{
public:
    NotifyCallMessage(uint8_t id): ControlMessgaes(CallMsg), id(id){};
    uint8_t id;
};

class NotifyFuncMessage: public ControlMessgaes{
public:
    NotifyFuncMessage(uint8_t id): ControlMessgaes(CallMsg), id(id){};
    uint8_t id;
};

class NotifyRetMessage: public ControlMessgaes{
public:
    NotifyRetMessage(uint8_t id): ControlMessgaes(RetMsg), id(id){};
    uint8_t id;
};

class NotifyBasicBlockMessage: public ControlMessgaes{
public:
    NotifyBasicBlockMessage(uint16_t id): ControlMessgaes(BasicBlockMsg), id(id){};
    uint16_t id;
};

#endif //SYMBACKEND_MESSAGE_H
