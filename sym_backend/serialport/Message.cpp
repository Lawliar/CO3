//
// Created by charl on 4/29/2022.
//

#include "Message.h"
#include <assert.h>

ControlMessgaes::ControlMessgaes(MessageType t, unsigned int id, uint32_t symid): Message(t),id(id), symid(symid){
    assert(t == BasicBlockMsg || t == CallMsg || t == RetMsg);
}

RuntimeValueMessage::RuntimeValueMessage(MessageType t , uint32_t symid, uint8_t width, int64_t value): \
Message(t),symid(symid),width(width),value(value){
    assert(t >= IntRuntimeValMsg && t <= ConstraintRuntimeValMsg);
}



MemOpMessage::MemOpMessage(MessageType t, uint32_t src_ptr, uint32_t dst_ptr, uint32_t length, uint32_t symID):
Message(t), src_ptr(src_ptr), dst_ptr(dst_ptr), length(length), symID(symID){
    assert(t >= MemCpyRuntimeMsg && t<= WriteMemRuntimeMsg);
}