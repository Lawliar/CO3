//
// Created by charl on 5/25/2022.
//

#ifndef SYMBACKEND_SYMGRAPH_H
#define SYMBACKEND_SYMGRAPH_H

#include <mutex>
#include <map>
#include <assert.h>
#include "RuntimeCFG.h"
#include "RuntimeDataFlowGraph.h"

class Val{
public:
    typedef unsigned short      ArgIndexType;
    typedef unsigned short      ValVertexType;
    typedef unsigned short      BasicBlockIdType;
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


    std::mutex mutex;
    ValType type;
    BasicBlockIdType BBID;
    ReadyType ready;
    std::map<ArgIndexType, ValVertexType> In_edges;

    Val(ValType t, BasicBlockIdType bid): type(t), BBID(bid), ready(0){}
    virtual ~Val(){};
};

class ConstantVal: public Val{
public:
    ByteWidthType ByteWidth;
    ConstantVal(ValType t, BasicBlockIdType bid, ByteWidthType byteWidth): Val(t, bid), ByteWidth(byteWidth){
        assert(t >= ConstantIntValTy && t <= ConstantDoubleValTy);
    }
};

class ConstantIntVal: public ConstantVal{
public:
    IntValType Val;

    ConstantIntVal(BasicBlockIdType bid, IntValType val, ByteWidthType byte_width):\
    ConstantVal(ConstantIntValTy, bid, byte_width), Val(val){}
};

class ConstantFloatVal: public ConstantVal{
public:
    float Val;
    ByteWidthType ByteWidth;
    ConstantFloatVal(BasicBlockIdType bid, float val):\
    ConstantVal(ConstantFloatValTy, bid, 4), Val(val){};
};

class ConstantDoubleVal: public ConstantVal{
public:
    double Val;
    ByteWidthType ByteWidth;
    ConstantDoubleVal(BasicBlockIdType bid, double val):\
    ConstantVal(ConstantFloatValTy, bid, 8), Val(val){};
};

class RuntimeVal: public Val{
public:
    ByteWidthType ByteWidth;
    RuntimeVal(ValType t, BasicBlockIdType bid, ByteWidthType byteWidth): Val(t, bid), ByteWidth(byteWidth){
            assert(t >= RuntimeIntValTy && t <= RuntimePtrValTy);
    }
};
class RuntimeIntVal: public RuntimeVal{
public:
    IntValType Val;// whether this is set is checked by the Ready Field
    RuntimeIntVal(BasicBlockIdType bid, ByteWidthType byte_width):\
    RuntimeVal(RuntimeIntValTy, bid, byte_width){}
};

class RuntimeFloatVal: public RuntimeVal{
public:
    float Val;
    RuntimeFloatVal(BasicBlockIdType bid): RuntimeVal(RuntimeFloatValTy, bid, 4){};
};

class RuntimeDoubleVal: public RuntimeVal{
public:
    double Val;
    RuntimeDoubleVal(BasicBlockIdType bid): RuntimeVal(RuntimeDoubleValTy, bid, 8){};
};

class RuntimePtrVal: public RuntimeVal{
public:
    void* Val = nullptr;
    RuntimePtrVal(BasicBlockIdType bid): RuntimeVal(RuntimePtrValTy, bid, 4){};
};

class SymVal: public Val{
public:
    std::string Op;
    SymVal(BasicBlockIdType bid, std::string op):Val( SymValTy,  bid), Op(op){}
};

#define DECLARE_SYMVAL_TYPE0(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 0;                                                       \
    SymVal##OP(BasicBlockIdType bid,std::string op): SymVal(bid, op){}                         \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE1(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 1;                                                       \
    SymVal##OP(BasicBlockIdType bid,std::string op, ValVertexType dep): SymVal(bid, op){ \
               In_edges[0] = dep;}                         \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE2(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 2;                    \
    SymVal##OP(BasicBlockIdType bid,std::string op,     \
                 ValVertexType dep1, ValVertexType dep2 ): \
                 SymVal(bid, op){                          \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE3(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 3;                   \
    SymVal##OP(BasicBlockIdType bid,std::string op,     \
                  ValVertexType dep1, ValVertexType dep2,  \
                  ValVertexType dep3 ): SymVal(bid, op){    \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;                         \
               In_edges[2] = dep3;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE4(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 4;                 \
    SymVal##OP(BasicBlockIdType bid,std::string op,    \
          ValVertexType dep1, ValVertexType dep2,         \
          ValVertexType dep3, ValVertexType dep4):        \
          SymVal(bid, op){                                 \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;                         \
               In_edges[2] = dep3;                         \
               In_edges[3] = dep4;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};

DECLARE_SYMVAL_TYPE1(_sym_build_integer)
DECLARE_SYMVAL_TYPE1(_sym_build_float)
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
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_bits)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_signed_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_unsigned_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_to_bits)




class SymGraph {
public:
    SymGraph(std::string cfg, std::string pdt, std::string dfg );

    vector<Val*> Nodes;
    RuntimeCFG cfg;
    RuntimeSymFlowGraph dfg;
};


#endif //SYMBACKEND_SYMGRAPH_H
