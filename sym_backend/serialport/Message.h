//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MESSAGE_H
#define SYMBACKEND_MESSAGE_H

#include "stdint.h"
#include <sstream>

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

class BuildBoolValueMessage: public SymSourceMessage{
public:
    BuildBoolValueMessage(uint16_t symid,bool value):SymSourceMessage(BoolRuntimeValMsg, symid), value(value){};
    bool value;
    std::string Str() {
        std::ostringstream s;
        s<<"RuntimeBoolValue:symid:"<<static_cast<unsigned>(symid)<<",value:"<<value;
        return s.str();
    }
};

class PhiValueMessage: public ControlMessgaes{
public:
    PhiValueMessage(uint16_t symid,uint8_t value):ControlMessgaes(PhiMsg), symid(symid), value(value){};
    uint16_t symid;
    uint8_t value;
    std::string Str() {
        std::ostringstream s;
        s<<"PhiValue:symid:"<<static_cast<unsigned>(symid)<<",value:"<<static_cast<unsigned>(value);
        return s.str();
    }
};

class BuildIntValueMessage: public SymSourceMessage{
public:
    BuildIntValueMessage(uint16_t symid, uint8_t width, int32_t value):SymSourceMessage(IntRuntimeValMsg, symid),width(width), value(value){};
    uint8_t width;
    int32_t value;
    std::string Str() {
        std::ostringstream s;
        s<<"RuntimeIntMsg:symid:"<<static_cast<unsigned>(symid)<<",width:"<<static_cast<unsigned>(width)<<",value:"<<static_cast<unsigned>(value);
        return s.str();
    }
};

class BuildFloatValueMessage: public SymSourceMessage{
public:
    BuildFloatValueMessage(uint16_t symid,float value):SymSourceMessage(FloatRuntimeValMsg, symid), value(value){};
    float value;
    std::string Str() {
        std::ostringstream s;
        s<<"RuntimeFloatMsg:symid:"<<static_cast<unsigned>(symid)<<",value:"<<value;
        return s.str();
    }
};

class BuildDoubleValueMessage: public SymSourceMessage{
public:
    BuildDoubleValueMessage(uint16_t symid,double value):SymSourceMessage(DoubleRuntimeValMsg, symid), value(value){};
    double value;
    std::string Str() {
        std::ostringstream s;
        s<<"RuntimeDoubleMsg:symid:"<<static_cast<unsigned>(symid)<<",value:"<<value;
        return s.str();
    }
};

class PushConstraintMessage: public SymSinkMessage{
public:
    PushConstraintMessage(uint16_t symID, bool runtimeVal):SymSinkMessage(ConstraintRuntimeValMsg, symID), runtimeVal(runtimeVal){};
    bool runtimeVal;
    std::string Str(){
        std::ostringstream s;
        s<<"PushConstraint:symid:"<<static_cast<unsigned>(symid)<<",value:"<<runtimeVal;
        return s.str();
    }
};

class MemCpyMessage: public SymSinkMessage{
public:
    MemCpyMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): SymSinkMessage(MemCpyRuntimeMsg, symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
    std::string Str(){
        std::ostringstream s;
        s<<"MemCpyMsg:symid:"<<static_cast<unsigned>(symid)<<",dest:"<<std::hex<<dst_ptr<<",src:"<<std::hex<<src_ptr<<",len:"<<length;
        return s.str();
    }
};

class MemSetMessage: public SymSinkMessage{
public:
    MemSetMessage(uint16_t symID, uint32_t ptr, uint32_t length): SymSinkMessage(MemsetRuntimeMsg,symID), ptr(ptr),length(length){};
    uint32_t ptr;
    uint16_t length;
    std::string Str(){
        std::ostringstream s;
        s<<"MemSetMsg:symid:"<<static_cast<unsigned>(symid)<<",dest:"<<std::hex<<ptr<<",len:"<<length;
        return s.str();
    }
};

class MemMoveMessage: public SymSinkMessage{
public:
    MemMoveMessage(uint16_t symID, uint32_t dst_ptr, uint32_t src_ptr, uint32_t length): SymSinkMessage(MemmoveRuntimeMsg, symID), dst_ptr(dst_ptr), src_ptr(src_ptr),length(length){};
    uint32_t dst_ptr;
    uint32_t src_ptr;
    uint32_t length;
    std::string Str(){
        std::ostringstream s;
        s<<"MemMoveMsg:symid"<<static_cast<unsigned>(symid)<<",dest:"<<std::hex<<dst_ptr<<",src:"<<std::hex<<src_ptr<<",len:"<<length;
        return s.str();
    }
};

class ReadMemMessage: public SymSourceMessage{
public:
    ReadMemMessage(uint16_t symID, uint32_t ptr): SymSourceMessage(ReadMemRuntimeMsg, symID), ptr(ptr){}
    uint32_t ptr;
    std::string Str(){
        std::ostringstream s;
        s<<"ReadMemMsg:symid:"<<static_cast<unsigned>(symid)<<",mem:"<<std::hex<<ptr;
        return s.str();
    }
};

class WriteMemMessage: public SymSinkMessage{
public:
    WriteMemMessage(uint16_t symID, uint32_t ptr): SymSinkMessage(WriteMemRuntimeMsg, symID), ptr(ptr){}
    uint32_t ptr;
    std::string Str(){
        std::ostringstream s;
        s<<"WriteMemMsg:symid:"<<static_cast<unsigned>(symid)<<",mem:"<< std::hex <<ptr;
        return s.str();
    }
};

class NotifyCallMessage: public ControlMessgaes{
public:
    NotifyCallMessage(uint8_t id): ControlMessgaes(CallMsg), id(id){};
    uint8_t id;
    std::string Str(){
        std::ostringstream s;
        s<<"NotifyCallMsg:"<<static_cast<unsigned>(id);
        return s.str();
    }
};

class NotifyFuncMessage: public ControlMessgaes{
public:
    NotifyFuncMessage(uint8_t id): ControlMessgaes(CallMsg), id(id){};
    uint8_t id;
    std::string Str(){
        std::ostringstream s;
        s<<"NotifyFuncMsg:"<<static_cast<unsigned>(id);
        return s.str();
    }
};

class NotifyRetMessage: public ControlMessgaes{
public:
    NotifyRetMessage(uint8_t id): ControlMessgaes(RetMsg), id(id){};
    uint8_t id;
    std::string Str(){
        std::ostringstream s;
        s<<"NotifyRetMsg:"<<static_cast<unsigned>(id);
        return s.str();
    }
};

class NotifyBasicBlockMessage: public ControlMessgaes{
public:
    NotifyBasicBlockMessage(uint16_t id): ControlMessgaes(BasicBlockMsg), id(id){};
    uint16_t id;
    std::string Str(){
        std::ostringstream s;
        s<<"NotifyBBMsg:"<<static_cast<unsigned>(id);
        return s.str();
    }
};

#endif //SYMBACKEND_MESSAGE_H
