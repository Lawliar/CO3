//
// Created by charl on 6/14/2022.
//

#ifndef SYMBACKEND_VAL_H
#define SYMBACKEND_VAL_H

#include <set>
#include <mutex>
#include <map>
#include <assert.h>
#include <sstream>
#include "Runtime.h"
#include "DataFlowGraph.h"

class Val{
public:
    typedef unsigned short      ArgIndexType;
    typedef unsigned short       ValVertexType;// even phi node (99.9%) won't have incoming edges exceed 255
    typedef unsigned short      BasicBlockIdType;
    typedef unsigned short      SymIDType;
    typedef unsigned int        ReadyType;
    typedef unsigned int        IntValType;
    typedef unsigned char       ByteWidthType;
    typedef enum _ValType{
        ConstantIntValTy,
        ConstantFloatValTy,
        ConstantDoubleValTy,
        RuntimeIntValTy,
        RuntimeFloatValTy,
        RuntimeDoubleValTy,
        RuntimePtrValTy,
        SymValTy,
    } ValType;

    static string valTypeString(ValType vT) {
        if(vT == ConstantIntValTy) {
            return "ConstInt";
        }else if(vT == ConstantFloatValTy){
            return "ConstantFloat";
        }else if(vT == ConstantDoubleValTy){
            return  "ConstantDouble";
        }else if(vT == RuntimeIntValTy){
            return "RuntimeInt";
        }else if(vT == RuntimeFloatValTy){
            return  "runtimeFloat";
        }else if(vT == RuntimeDoubleValTy) {
            return "runtimeDouble";
        }else if(vT == RuntimePtrValTy) {
            return "runtimePtr";
        }else if(vT == SymValTy){
            return "SymVal";
        }else {
            return "seriously?";
        }
    }
    vector<Val*> realChildren();
    Val(ValType t, BasicBlockIdType bid): type(t), BBID(bid), ready(0){}
    bool isThisNodeReady(Val*, Val::ReadyType);
    virtual ~Val(){};
    string Str() {
        return valTypeString(type);
    }
    Val(const Val& other):type(other.type),BBID(other.BBID),ready(0),inLoop(other.inLoop){}
    void FinishCopyConstructing(std::map<ArgIndexType, Val*>& old_In_edges,std::set<Val*>& old_UsedBy,std::map<Val*,Val*> &old2new){
        for(auto each_old_in_edge : old_In_edges){
            In_edges.insert(make_pair(each_old_in_edge.first, old2new.at(each_old_in_edge.second)));
        }
        for(auto each_used_by : old_UsedBy){
            UsedBy.insert(old2new.at(each_used_by));
        }
    }
    //std::mutex mutex;
    ValType type;
    BasicBlockIdType BBID;
    ReadyType ready = 0;
    bool inLoop;
    std::map<ArgIndexType, ValVertexType> tmpIn_edges;
    std::map<ArgIndexType, Val*> In_edges;
    std::set<Val*> UsedBy;

};

class ConstantVal: public Val{
public:
    ByteWidthType ByteWidth;
    ConstantVal(ValType t, BasicBlockIdType bid, ByteWidthType byteWidth): Val(t, bid), ByteWidth(byteWidth){
        assert(bid == 0);
        assert(t >= ConstantIntValTy && t <= ConstantDoubleValTy);
    }
    ConstantVal(const ConstantVal& other): Val(other), ByteWidth(other.ByteWidth){};
};

class ConstantIntVal: public ConstantVal{
public:
    IntValType Value;

    ConstantIntVal(BasicBlockIdType bid, IntValType value, ByteWidthType byte_width):ConstantVal(ConstantIntValTy, bid, byte_width), Value(value){}
    ConstantIntVal(const ConstantIntVal& other): ConstantVal(other), Value(other.Value){}
};

class ConstantFloatVal: public ConstantVal{
public:
    float Value;
    ConstantFloatVal(BasicBlockIdType bid, float value):ConstantVal(ConstantFloatValTy, bid, 4), Value(value){}
    ConstantFloatVal(const ConstantFloatVal&other): ConstantVal(other),Value(other.Value){}
};

class ConstantDoubleVal: public ConstantVal{
public:
    double Value;
    ConstantDoubleVal(BasicBlockIdType bid, double val):ConstantVal(ConstantFloatValTy, bid, 8), Value(val){}
    ConstantDoubleVal(const ConstantDoubleVal&other): ConstantVal(other),Value(other.Value){}
};

class RuntimeVal: public Val{
public:
    // when this is true, it means, this value is not reported from the MCU
    // thus, whatever SymVal that is using this should be concrete.
    bool Unassigned = false;
    ByteWidthType ByteWidth;
    void Unassign(){ Unassigned = true; ready++;};
    RuntimeVal(ValType t, BasicBlockIdType bid, ByteWidthType byteWidth): Val(t, bid), ByteWidth(byteWidth){
        assert(t >= RuntimeIntValTy && t <= RuntimePtrValTy);
    }
    RuntimeVal(const RuntimeVal& other): Val(other), Unassigned(false), ByteWidth(other.ByteWidth){}
};
class RuntimeIntVal: public RuntimeVal{
public:
    IntValType Val;// whether this is set is checked by the Ready Field
    void Assign(IntValType val);
    RuntimeIntVal(BasicBlockIdType bid, ByteWidthType byte_width):RuntimeVal(RuntimeIntValTy, bid, byte_width){}
    RuntimeIntVal(const RuntimeIntVal& other): RuntimeVal(other), Val(other.Val){}
};

class RuntimeFloatVal: public RuntimeVal{
public:
    float Val;
    void Assign(float val);
    RuntimeFloatVal(BasicBlockIdType bid): RuntimeVal(RuntimeFloatValTy, bid, 4){};
    RuntimeFloatVal(const RuntimeFloatVal& other): RuntimeVal(other), Val(other.Val){}
};

class RuntimeDoubleVal: public RuntimeVal{
public:
    double Val;
    void Assign(double val);
    RuntimeDoubleVal(BasicBlockIdType bid): RuntimeVal(RuntimeDoubleValTy, bid, 8){};
    RuntimeDoubleVal(const RuntimeDoubleVal& other): RuntimeVal(other), Val(other.Val){}
};

class RuntimePtrVal: public RuntimeVal{
public:
    void* Val = nullptr;
    void Assign(void * val);
    RuntimePtrVal(BasicBlockIdType bid): RuntimeVal(RuntimePtrValTy, bid, 4){};
    RuntimePtrVal(const RuntimePtrVal& other): RuntimeVal(other), Val(other.Val){}
};

class SymVal: public Val{
public:
    std::string Op;
    SymIDType symID = 0;
    SymIDType symIDR = 0;
    SymExpr symExpr = nullptr;

    // todo: remove the targetReady parameter
    virtual void Construct(Val::ReadyType targetReady) {};
    bool directlyConstructable(Val::ReadyType targetReady);
    static SymExpr extractSymExprFromSymVal(SymVal*, ReadyType);
    string Str() {
        std::ostringstream ss;
        ss << "symID:"<<symID << ",op:"<<Op<<",BBID:"<<BBID<<",ready:"<<ready;
        return ss.str();
    }
    SymVal(SymIDType symid,SymIDType  symIDR, std::string op, BasicBlockIdType bid):Val( SymValTy,  bid), Op(op), symID(symid),symIDR(symIDR){}
    SymVal(const SymVal& other): Val(other),Op(other.Op), symID(other.symID),symIDR(other.symIDR), symExpr(nullptr){};

};


#define DECLARE_SYMVAL_TYPE0(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 0;                      \
    void Construct(ReadyType) override;                                                       \
    SymVal##OP(SymIDType symid, SymIDType symidr,BasicBlockIdType bid): SymVal(symid, symidr,#OP, bid){}              \
    SymVal##OP(const SymVal##OP & other): SymVal(other){}                                      \
    ~SymVal##OP(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE1(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    void Construct(ReadyType) override;                                                       \
    static const unsigned numOps = 1;                                                       \
    SymVal##OP(SymIDType symid,SymIDType symidr, BasicBlockIdType bid, ValVertexType dep): SymVal(symid,symidr, #OP,bid){ tmpIn_edges[0] = dep;} \
    SymVal##OP(const SymVal##OP & other): SymVal(other){}                                      \
    ~SymVal##OP(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE2(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    void Construct(ReadyType) override;                                                       \
    static const unsigned numOps = 2;                    \
    SymVal##OP(SymIDType symid,SymIDType symidr, BasicBlockIdType bid,     \
                 ValVertexType dep1, ValVertexType dep2 ): \
                 SymVal(symid,symidr, #OP, bid){                          \
               tmpIn_edges[0] = dep1;                          \
               tmpIn_edges[1] = dep2;}                     \
    SymVal##OP(const SymVal##OP & other): SymVal(other){}                                      \
    ~SymVal##OP(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE3(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    void Construct(ReadyType) override;                \
    static const unsigned numOps = 3;                   \
    SymVal##OP(SymIDType symid,SymIDType symidr, BasicBlockIdType bid,     \
                  ValVertexType dep1, ValVertexType dep2,  \
                  ValVertexType dep3 ): SymVal(symid,symidr,#OP, bid){    \
               tmpIn_edges[0] = dep1;                          \
               tmpIn_edges[1] = dep2;                         \
               tmpIn_edges[2] = dep3;}                     \
    SymVal##OP(const SymVal##OP & other): SymVal(other){}                                      \
    ~SymVal##OP(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE4(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 4;                      \
    void Construct(ReadyType) override;                                                       \
    SymVal##OP(SymIDType symid,SymIDType symidr, BasicBlockIdType bid,    \
          ValVertexType dep1, ValVertexType dep2,         \
          ValVertexType dep3, ValVertexType dep4):        \
          SymVal(symid,symidr, #OP,bid){                  \
               tmpIn_edges[0] = dep1;                          \
               tmpIn_edges[1] = dep2;                         \
               tmpIn_edges[2] = dep3;                         \
               tmpIn_edges[3] = dep4;}                     \
    SymVal##OP(const SymVal##OP & other): SymVal(other){}                                      \
    ~SymVal##OP(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}                        \
};

class SymVal_sym_notify_call: public SymVal {
public:
    void Construct(ReadyType) override;
    SymVal_sym_notify_call(SymIDType symid,SymIDType symidr, BasicBlockIdType bid, std::map<unsigned , unsigned>& paras): SymVal(symid,symidr, "_sym_notify_call", bid) {
        for(auto eachPara: paras) {
            tmpIn_edges.insert(std::make_pair(eachPara.first, eachPara.second));
        }
    }
    SymVal_sym_notify_call(SymVal_sym_notify_call&other): SymVal(other){}
    ~SymVal_sym_notify_call(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}
};


class SymVal_sym_try_alternative: public SymVal{
public:
    static const unsigned numOps = 2;
    void Construct(ReadyType) override;
    SymVal_sym_try_alternative(SymIDType symid,SymIDType symidr, BasicBlockIdType bid,
                               ValVertexType dep1, ValVertexType dep2 ):
            SymVal(symid,symidr,"_sym_try_alternative",bid){
        tmpIn_edges[0] = dep1;
        tmpIn_edges[1] = dep2;}
    SymVal_sym_try_alternative(SymVal_sym_try_alternative&other): SymVal(other){}
    ~SymVal_sym_try_alternative(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}
};


DECLARE_SYMVAL_TYPE0(_NULL) // NULL valued symval found from compile time

DECLARE_SYMVAL_TYPE2(_sym_build_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_float)
DECLARE_SYMVAL_TYPE0(_sym_build_null_pointer)
DECLARE_SYMVAL_TYPE0(_sym_build_true)
DECLARE_SYMVAL_TYPE0(_sym_build_false)
DECLARE_SYMVAL_TYPE1(_sym_build_bool)

DECLARE_SYMVAL_TYPE1(_sym_build_neg)
DECLARE_SYMVAL_TYPE2(_sym_build_add)
DECLARE_SYMVAL_TYPE2(_sym_build_sub)
DECLARE_SYMVAL_TYPE2(_sym_build_mul)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_div)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_div)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_rem)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_rem)
DECLARE_SYMVAL_TYPE2(_sym_build_shift_left)
DECLARE_SYMVAL_TYPE2(_sym_build_logical_shift_right)
DECLARE_SYMVAL_TYPE2(_sym_build_arithmetic_shift_right)

DECLARE_SYMVAL_TYPE2(_sym_build_fp_add)
DECLARE_SYMVAL_TYPE2(_sym_build_fp_sub)
DECLARE_SYMVAL_TYPE2(_sym_build_fp_mul)
DECLARE_SYMVAL_TYPE2(_sym_build_fp_div)
DECLARE_SYMVAL_TYPE2(_sym_build_fp_rem)
DECLARE_SYMVAL_TYPE1(_sym_build_fp_abs)

DECLARE_SYMVAL_TYPE1(_sym_build_not)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_less_than)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_less_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_greater_than)
DECLARE_SYMVAL_TYPE2(_sym_build_signed_greater_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_less_than)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_less_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_greater_than)
DECLARE_SYMVAL_TYPE2(_sym_build_unsigned_greater_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_not_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_and)
DECLARE_SYMVAL_TYPE2(_sym_build_and)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_or)
DECLARE_SYMVAL_TYPE2(_sym_build_or)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_xor)
DECLARE_SYMVAL_TYPE2(_sym_build_xor)

DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_greater_than)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_greater_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_less_than)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_less_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered_not_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_ordered)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_greater_than)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_greater_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_less_than)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_less_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_equal)
DECLARE_SYMVAL_TYPE2(_sym_build_float_unordered_not_equal)

DECLARE_SYMVAL_TYPE2(_sym_build_sext)
DECLARE_SYMVAL_TYPE2(_sym_build_zext)
DECLARE_SYMVAL_TYPE2(_sym_build_trunc)
DECLARE_SYMVAL_TYPE1(_sym_build_bswap)
DECLARE_SYMVAL_TYPE3(_sym_build_int_to_float)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_float)
DECLARE_SYMVAL_TYPE2(_sym_build_bits_to_float)
DECLARE_SYMVAL_TYPE1(_sym_build_float_to_bits)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_signed_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_unsigned_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_to_bits)


DECLARE_SYMVAL_TYPE2(_sym_set_parameter_expression)
DECLARE_SYMVAL_TYPE1(_sym_get_parameter_expression)
DECLARE_SYMVAL_TYPE1(_sym_set_return_expression)
DECLARE_SYMVAL_TYPE0(_sym_get_return_expression)

DECLARE_SYMVAL_TYPE2(_sym_build_path_constraint)//sym id will be added later

class SymVal_sym_build_read_memory : public SymVal{
public:
    void Construct(ReadyType) override;
    static const unsigned numOps = 3;
    bool hasConcrete = false;
    uint32_t concreteValue = 0;

    SymVal_sym_build_read_memory(SymIDType symid,SymIDType symidr, BasicBlockIdType bid,
                  ValVertexType dep1, ValVertexType dep2,
                  ValVertexType dep3 ): SymVal(symid,symidr,"_sym_build_read_memory", bid){
               tmpIn_edges[0] = dep1;
               tmpIn_edges[1] = dep2;
               tmpIn_edges[2] = dep3;
    }
    SymVal_sym_build_read_memory(const SymVal_sym_build_read_memory&other): SymVal(other), hasConcrete(false),concreteValue(0) {}
    void AddConcreteValue(uint32_t val){
        hasConcrete = true;
        concreteValue = val;
    }
    ~SymVal_sym_build_read_memory(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}
};

DECLARE_SYMVAL_TYPE4(_sym_build_write_memory)
DECLARE_SYMVAL_TYPE3(_sym_build_memcpy)
DECLARE_SYMVAL_TYPE3(_sym_build_memset)
DECLARE_SYMVAL_TYPE3(_sym_build_memmove)

DECLARE_SYMVAL_TYPE4(_sym_build_insert)
DECLARE_SYMVAL_TYPE4(_sym_build_extract)

class SymVal_sym_TruePhi: public SymVal{
public:
    unsigned numOps;
    map<ArgIndexType , BasicBlockIdType> ArgNo2BBMap;// not really used
    // at given time of execution, which branch this true phi took and what symExpr it represents
    // todo: replace these historyValue to just the newest value
    vector<pair<Val::ArgIndexType, SymExpr> > historyValues;

    Val::ReadyType getDepTargetReady(Val*);
    SymVal_sym_TruePhi(SymIDType symid,SymIDType symidr, BasicBlockIdType bid, map<ArgIndexType , ValVertexType> PhiEdges, map<ArgIndexType , BasicBlockIdType> ArgNo2BBMap):
            SymVal(symid,symidr, NodeTruePhi, bid), ArgNo2BBMap(ArgNo2BBMap){
        numOps = PhiEdges.size();
        for(auto eachPhiEdge : PhiEdges){
            tmpIn_edges[eachPhiEdge.first] = eachPhiEdge.second;
        }
    }
    SymVal_sym_TruePhi(const SymVal_sym_TruePhi&other):SymVal(other),ArgNo2BBMap(other.ArgNo2BBMap){}
    ~SymVal_sym_TruePhi(){In_edges.clear();tmpIn_edges.clear(); UsedBy.clear();}
};




class SymVal_sym_FalsePhiRoot: public SymVal{
public:
    //unsigned numOps;
    set<ValVertexType> tmpfalsePhiLeaves;
    set<Val*> falsePhiLeaves;
    ReadyType slowPathChosen = 0;
    SymVal_sym_FalsePhiRoot(SymIDType symid,SymIDType symidr, BasicBlockIdType bid, map<ArgIndexType , ValVertexType> PhiEdges,set<ValVertexType> falsePhiLeaves):
            SymVal(symid,symidr, NodeFalseRootPhi, bid), tmpfalsePhiLeaves(falsePhiLeaves){
        //numOps = PhiEdges.size();
        for(auto eachPhiEdge : PhiEdges){
            tmpIn_edges[eachPhiEdge.first] = eachPhiEdge.second;
        }
    }
    SymVal_sym_FalsePhiRoot(const SymVal_sym_FalsePhiRoot&other):SymVal(other){}
    void FinishCopyConstructing(set<Val*>& old_falsePhiLeaves,std::map<Val*,Val*>& old2new){
        for(auto each_old_FalsePhiLeaf: old_falsePhiLeaves){
            falsePhiLeaves.insert(old2new.at(each_old_FalsePhiLeaf));
        }
    }
    ~SymVal_sym_FalsePhiRoot(){In_edges.clear();
        tmpIn_edges.clear(); UsedBy.clear();
        tmpfalsePhiLeaves.clear();falsePhiLeaves.clear();}
};

class SymVal_sym_FalsePhiLeaf: public SymVal{
public:
    unsigned numOps;
    set<ValVertexType> tmpPeerOriginals;
    set<SymVal*> peerOriginals;
    ReadyType slowPathChosen = 0;
    //SymVal_sym_FalsePhiRoot * root = nullptr;
    SymVal_sym_FalsePhiLeaf(SymIDType symid,SymIDType symidr, BasicBlockIdType bid, map<ArgIndexType , ValVertexType> PhiEdges,set<ValVertexType> tmpOriginals):
            SymVal(symid,symidr, NodeFalseLeafPhi, bid), tmpPeerOriginals(tmpOriginals){
        numOps = PhiEdges.size();
        for(auto eachPhiEdge : PhiEdges){
            tmpIn_edges[eachPhiEdge.first] = eachPhiEdge.second;
        }
    }
    SymVal_sym_FalsePhiLeaf(const SymVal_sym_FalsePhiLeaf&other):SymVal(other),slowPathChosen(0){}
    void FinishCopyConstructing(set<SymVal*>& old_peerOriginals,std::map<Val*,Val*> &old2new){
        //Val::FinishCopyConstructing(old2new);
        for(auto each_old_PeerOrig: old_peerOriginals){
            auto* new_peer_orig = dynamic_cast<SymVal*>(old2new.at(each_old_PeerOrig));
            peerOriginals.insert(new_peer_orig);
        }
    }
    ~SymVal_sym_FalsePhiLeaf(){In_edges.clear();
        tmpIn_edges.clear(); UsedBy.clear();
        peerOriginals.clear();tmpPeerOriginals.clear();}
};

#endif //SYMBACKEND_VAL_H
