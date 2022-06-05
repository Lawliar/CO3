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


    std::mutex mutex;
    ValType type;
    BasicBlockIdType BBID;
    ReadyType ready;
    std::map<ArgIndexType, ValVertexType> In_edges;
    std::set<ValVertexType> UsedBy;

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
    SymIDType symID;
    SymVal(SymIDType symid, std::string op, BasicBlockIdType bid):Val( SymValTy,  bid), Op(op), symID(symid){}
};

#define DECLARE_SYMVAL_TYPE0(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 0;                                                       \
    SymVal##OP(SymIDType symid, BasicBlockIdType bid): SymVal(symid, #OP, bid){}                         \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE1(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 1;                                                       \
    SymVal##OP(SymIDType symid, BasicBlockIdType bid, ValVertexType dep): SymVal(symid, #OP,bid){ \
               In_edges[0] = dep;}                         \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE2(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 2;                    \
    SymVal##OP(SymIDType symid, BasicBlockIdType bid,     \
                 ValVertexType dep1, ValVertexType dep2 ): \
                 SymVal(symid, #OP, bid){                          \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE3(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 3;                   \
    SymVal##OP(SymIDType symid, BasicBlockIdType bid,     \
                  ValVertexType dep1, ValVertexType dep2,  \
                  ValVertexType dep3 ): SymVal(symid,#OP, bid){    \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;                         \
               In_edges[2] = dep3;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};

#define DECLARE_SYMVAL_TYPE4(OP)                           \
class SymVal##OP : public SymVal{                      \
public:                                                    \
    static const unsigned numOps = 4;                 \
    SymVal##OP(SymIDType symid, BasicBlockIdType bid,    \
          ValVertexType dep1, ValVertexType dep2,         \
          ValVertexType dep3, ValVertexType dep4):        \
          SymVal(symid, #OP,bid){                                 \
               In_edges[0] = dep1;                          \
               In_edges[1] = dep2;                         \
               In_edges[2] = dep3;                         \
               In_edges[3] = dep4;}                        \
    ~SymVal##OP(){In_edges.clear();}                        \
};
DECLARE_SYMVAL_TYPE0(_NULL) // NULL valued symval found from compile time

// try_alternative is different.
class SymVal_sym_try_alternative: public SymVal{
public:
    static const unsigned numOps = 2;
    SymVal_sym_try_alternative(SymIDType symid, BasicBlockIdType bid,
                 ValVertexType dep1, ValVertexType dep2 ):
                 SymVal(symid,"_sym_try_alternative",bid){
               In_edges[0] = dep1;
               In_edges[1] = dep2;}
    ~SymVal_sym_try_alternative(){In_edges.clear();}
};

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
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_bits)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_signed_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_float_to_unsigned_integer)
DECLARE_SYMVAL_TYPE2(_sym_build_bool_to_bits)


DECLARE_SYMVAL_TYPE2(_sym_set_parameter_expression)
DECLARE_SYMVAL_TYPE1(_sym_get_parameter_expression)
DECLARE_SYMVAL_TYPE1(_sym_set_return_expression)
DECLARE_SYMVAL_TYPE0(_sym_get_return_expression)

DECLARE_SYMVAL_TYPE2(_sym_build_path_constraint)//sym id will be added later

DECLARE_SYMVAL_TYPE3(_sym_build_read_memory)
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
    SymVal_sym_TruePhi(SymIDType symid, BasicBlockIdType bid, map<ArgIndexType , ValVertexType> PhiEdges, map<ArgIndexType , BasicBlockIdType> ArgNo2BBMap):
            SymVal(symid, "_sym_TruePhi", bid), ArgNo2BBMap(ArgNo2BBMap){
        numOps = PhiEdges.size();
        for(auto eachPhiEdge : PhiEdges){
            In_edges[eachPhiEdge.first] = eachPhiEdge.second;
        }
    }
    ~SymVal_sym_TruePhi(){In_edges.clear();}
};

class SymVal_sym_FalsePhi: public SymVal{
public:
    unsigned numOps;
    map<ArgIndexType , BasicBlockIdType> ArgNo2BBMap;// not really used
    SymVal_sym_FalsePhi(SymIDType symid, BasicBlockIdType bid, map<ArgIndexType , ValVertexType> PhiEdges,map<ArgIndexType , BasicBlockIdType> ArgNo2BBMap):
            SymVal(symid, "_sym_FalsePhi", bid), ArgNo2BBMap(ArgNo2BBMap){
        numOps = PhiEdges.size();
        for(auto eachPhiEdge : PhiEdges){
            In_edges[eachPhiEdge.first] = eachPhiEdge.second;
        }
    }
    ~SymVal_sym_FalsePhi(){In_edges.clear();}
};

class DataDependents{
public:
    Val::ValVertexType root;
    vector<Val*>& allNodes;
    set<Val::ValVertexType> deps;
    map<Val::ValVertexType, map<Val::ArgIndexType, DataDependents* > > truePhiDependences;

    DataDependents(Val::ValVertexType root, vector<Val*>&  nodes): root(root), allNodes(nodes){}
    ~DataDependents(){
        deps.clear();
        for(auto eachTruePhi: truePhiDependences){
            for(auto eachTruePhiVal: eachTruePhi.second){
                delete eachTruePhiVal.second;
            }
            eachTruePhi.second.clear();
        }
        truePhiDependences.clear();
    }
    void nonTruePhiDataDependentsOf(set<Val::ValVertexType> ancesters);
    void allPossibleDataDependencies(set<Val::ValVertexType>, vector<set<Val::ValVertexType> >& );
};

class SymGraph {
private:
    std::set<Val::BasicBlockIdType> domChildrenOf(Val::BasicBlockIdType, map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t>, RuntimeCFG::DominanceTree&);
    void prepareBBTask();
    bool sortNonLoopBB(Val::BasicBlockIdType, Val::BasicBlockIdType);
    list<Val::BasicBlockIdType> sortNonLoopBBs(set<Val::BasicBlockIdType>);
public:

    SymGraph(std::string funcname, std::string cfg, std::string dt, std::string pdt, std::string dfg );
    ~SymGraph(){
        ver2offMap.clear();
        for(auto eachBBTask: bbTasks){
            delete eachBBTask.second;
            bbTasks[eachBBTask.first] = nullptr;
        }
        for(unsigned i = 0 ; i < Nodes.size(); i ++){
            delete Nodes[i];
            Nodes[i] = nullptr;
        }
    }

    class BasicBlockTask{
    public:

        BasicBlockTask(unsigned id, bool loop):BBID(id),inLoop(loop),ready(0){}
        ~BasicBlockTask(){
            dominance.clear();
            post_dominance.clear();
            leaves.clear();
            roots.clear();
            for(auto eachRootDep:nonLoopRootDependents){
                delete eachRootDep.second;
            }
            nonLoopRootDependents.clear();
        }
        Val::BasicBlockIdType BBID;
        bool inLoop;
        Val::ReadyType ready;
        std::set<Val::BasicBlockIdType> dominance;
        std::set<Val::BasicBlockIdType > post_dominance;// this BB post dominate these BBs
        // leaves are the "inputs" to this basic block
        std::set<Val::ValVertexType> leaves;
        // roots are the non-out vertices and the direct out vertices
        std::set<Val::ValVertexType> roots;

        // map the root to its Dependent BBs that are not in the loop(because we already make sure loop is properly executed)
        std::map<Val::ValVertexType, DataDependents* > nonLoopRootDependents;
    };

    std::string funcname;
    RuntimeCFG cfg;
    RuntimeSymFlowGraph dfg;

    map<RuntimeSymFlowGraph::vertex_t, Val::ValVertexType> ver2offMap;
    std::map<Val::BasicBlockIdType, BasicBlockTask*> bbTasks;
    vector<Val*> Nodes;
    vector<Val::ValVertexType> getParametersSym;
    Val::ValVertexType setRetSym;
};



#endif //SYMBACKEND_SYMGRAPH_H
