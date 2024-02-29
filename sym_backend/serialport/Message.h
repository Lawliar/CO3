//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MESSAGE_H
#define SYMBACKEND_MESSAGE_H

#include "stdint.h"
#include <sstream>

enum {
    SYM_BLD_INT_1 =      1,
    SYM_BLD_INT_1_1       ,
    SYM_BLD_INT_2         ,
    SYM_BLD_INT_2_1       ,
    SYM_BLD_INT_4         ,
    SYM_BLD_INT_4_1       ,
    SYM_BLD_FLOAT         ,
    SYM_BLD_FLOAT_1       ,
    SYM_BLD_FLOAT_DBL     ,
    SYM_BLD_FLOAT_DBL_1   ,
    SYM_BLD_BOOL          ,
    SYM_BLD_BOOL_1        ,
    SYM_BLD_PATH_CNSTR    ,
    SYM_BLD_PATH_CNSTR_1  ,
    SYM_BLD_MEMCPY        ,
    SYM_BLD_MEMCPY_1      ,
    SYM_BLD_MEMSET        ,
    SYM_BLD_MEMSET_1      ,
    SYM_BLD_MEMMOVE       ,
    SYM_BLD_MEMMOVE_1     ,
    SYM_BLD_READ_MEM      ,
    SYM_BLD_READ_MEM_1    ,
    SYM_BLD_READ_MEM_HW   ,
    SYM_BLD_READ_MEM_HW_1 ,
    SYM_BLD_READ_MEM_W    ,
    SYM_BLD_READ_MEM_W_1  ,

    SYM_BLD_WRITE_MEM     ,
    SYM_BLD_WRITE_MEM_1   ,

// SYM_SET_PAR_EXP       ,  // these are not sent to the PC
// SYM_GET_PAR_EXP       ,
// SYM_SET_RET_EXP       ,
// SYM_GET_RET_EXP       ,

    SYM_NTFY_PHI          ,
    SYM_NTFY_PHI_1        ,

    SYM_NTFY_CALL         ,
    SYM_NTFY_FUNC         ,
    SYM_NTFY_RET          ,
    SYM_NTFY_BBLK         ,
    SYM_NTFY_BBLK1        ,

    SYM_NTFY_SELECT       ,
    SYM_NTFY_SELECT_1     ,

    SYM_TRY_ALT           , // I wanted to implement this as physical messages, but it really does not have to be
    SYM_TRY_ALT_1         , // since all the materials are already on the workstation, messages from the MCU are just confirming things

    SYM_INIT              ,
    SYM_INIT_DR           ,
    SYM_END
};





//legend FIELDNAME (SIZE in bytes)
#define SIZE_SYM_BLD_INT_1          3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_INT_1_1        4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_INT_2          4      // | FCODE(1) | symID(1)         | val(2)      |
#define SIZE_SYM_BLD_INT_2_1        5      // | FCODE(1) | symID(2)         | val(2)      |

#define SIZE_SYM_BLD_INT_4          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_INT_4_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_FLOAT_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT_DBL      10     // | FCODE(1) | symID(1)         | val(8)      |
#define SIZE_SYM_BLD_FLOAT_DBL_1    11     // | FCODE(1) | symID(2)         | val(8)      |

#define SIZE_SYM_BLD_BOOL           3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_BOOL_1         4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_PATH_CNSTR     3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_PATH_CNSTR_1   4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_MEMCPY         12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMCPY_1       13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_MEMSET         8      // | FCODE(1) | symID(1)         | mem(4)      | length(2) |
#define SIZE_SYM_BLD_MEMSET_1       9      // | FCODE(1) | symID(2)         | mem(4)      | length(2) |

#define SIZE_SYM_BLD_MEMMOVE        12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMMOVE_1      13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_READ_MEM       6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_READ_MEM_1     7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_BLD_READ_MEM_HW    8      // | FCODE(1) | symID(1)         | addr(4)     | val(2)    |
#define SIZE_SYM_BLD_READ_MEM_HW_1  9      // | FCODE(1) | symID(2)         | addr(4)     | val(2)    |

#define SIZE_SYM_BLD_READ_MEM_W     10     // | FCODE(1) | symID(1)         | addr(4)     | val(4)    |
#define SIZE_SYM_BLD_READ_MEM_W_1   11     // | FCODE(1) | symID(2)         | addr(4)     | val(4)    |

#define SIZE_SYM_BLD_WRITE_MEM      6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_WRITE_MEM_1    7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_NTFY_PHI           3      // | FCODE(1) | symID(1)         | branchNo(1) |
#define SIZE_SYM_NTFY_PHI_1         4      // | FCODE(1) | symID(2)         | branchNo(1) |

#define SIZE_SYM_NTFY_CALL          2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_FUNC          2      // | FCODE(1) | func_id(1)       |
#define SIZE_SYM_NTFY_RET           2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_BBLK          2      // | FCODE(1) | bbid(1)          |
#define SIZE_SYM_NTFY_BBLK1         3      // | FCODE(1) | bbid(2)          |

#define SIZE_SYM_NTFY_SELECT        3      // | FCODE(1) | symID(1)         |bool val(1)  |
#define SIZE_SYM_NTFY_SELECT_1      4      // | FCODE(1) | symID(2)         |bool val(1)  |

#define SIZE_SYM_TRY_ALTERNATIVE    2      // | FCODE(1) | symID(1)         |
#define SIZE_SYM_TRY_ALTERNATIVE_1  3      // | FCODE(1) | symID(2)         |

#define SIZE_SYM_INIT               5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_INIT_DR            5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_END                1      // | FCODE(1) |

class Message {
public:
    typedef enum _MessageType{
        BasicBlockMsg,
        CallMsg,
        SelectMsg,
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

        TryAltMsg,
        InitMsg,
        EndMsg,
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
    bool hasConcrete = false;
    uint32_t  concreteValue;
    std::string Str(){
        std::ostringstream s;
        s<<"ReadMemMsg:symid:"<<static_cast<unsigned>(symid)<<",mem:"<<std::hex<<\
            ptr<<",hasConcrete:"<<hasConcrete<<",concreteValue:"<<std::hex<<concreteValue;
        return s.str();
    }
    void AddConcreteValue(uint32_t val){
        hasConcrete = true;
        concreteValue = val;
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

class TryAltMessage: public SymSinkMessage{
public:
    TryAltMessage(uint16_t symID): SymSinkMessage(TryAltMsg, symID){}
    std::string Str(){
        std::ostringstream s;
        s<<"TryAlt:symid:"<<static_cast<unsigned>(symid);
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

class SelectMessage : public ControlMessgaes{
public:
    SelectMessage(bool cond, uint16_t id): ControlMessgaes(SelectMsg), cond(cond), symid(id){};
    bool cond;
    uint16_t symid;
    std::string Str(){
        std::ostringstream s;
        s<<"SelectMsg: cond"<<cond<<",symid"<<static_cast<unsigned>(symid);
        return s.str();
    }
};
class InitMessage : public ControlMessgaes{
public:
    char * addr = nullptr;
    bool DR = false;
    InitMessage(char* addr, bool is_dr): ControlMessgaes(InitMsg),addr(addr), DR(is_dr){};
    std::string Str(){
        std::ostringstream s;
        s << "SymInit: Addr:"<<reinterpret_cast<std::uintptr_t>(addr)<<",for DR:"<< DR;
        return s.str();
    }
};
class EndMessage: public ControlMessgaes{
public:
    EndMessage(): ControlMessgaes(EndMsg){};
    std::string Str(){
        return "SymEnd";
    }
};

#endif //SYMBACKEND_MESSAGE_H
