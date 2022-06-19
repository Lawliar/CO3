//
// Created by charl on 6/14/2022.
//
#include "Val.h"

bool Val::isThisNodeReady(Val * nodeInQuestion, unsigned targetReady) {
    auto * nodeIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(nodeInQuestion);
    auto * rootIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(this);
    assert(rootIsTruePhi == nullptr);
    if(nodeIsTruePhi != nullptr){
        // we execute truePhi pro-actively
        return true;
    }
    if(nodeInQuestion == this){
        if(this->ready == targetReady){
            return true;
        }else{
            return false;
        }
    }
    if(nodeInQuestion->BBID == 0){
        // if it's a constant or symNULL, then of course it's ready.
        //for debugging only
        auto tmpSymNull = dynamic_cast<SymVal_NULL*>(nodeInQuestion);
        auto tmpConst = dynamic_cast<ConstantVal*>(nodeInQuestion);
        assert(tmpSymNull != nullptr || tmpConst != nullptr);
        //end of debug
        return true;
    }
    if(!nodeInQuestion->inLoop){
        if(nodeInQuestion->ready == 1){
            return true;
        }else if (nodeInQuestion->ready == 0){
            return false;
        }else{
            cerr<<"Nodes in the non-loop BB should at executed at most once.";
            assert(false);
        }
    }else{
        // now nodeInQuestion is in loop, and root is in or out of a loop
        if(nodeInQuestion->BBID == this->BBID){
            // if they are in the same BB, then it's just a comparison of ready value
            if(nodeInQuestion->ready == this->ready){
                //nodeInQuestion has the same readyValue as this Node
                if(this->ready == targetReady){
                    return true;
                }else if(this->ready < targetReady){
                    return false;
                }else{
                    std::cerr<<"target Ready is less than this node's ready?";
                    std::cerr.flush();
                    abort();
                }
            }else if (nodeInQuestion->ready == (this->ready + 1)){
                return true;
            }else{
                cerr<<"the dep can only be one more or equal to the root in terms of ready value when they are in the same loop BB";
                assert(false);
            }
        }else{
            // for BB in the loop we execute in the per-BB level
            // given the root is under construction, no matter it's in the loop or not
            // the nodeInQuestion is inside a loop, and they are in different BB
            // then it must mean the construction for the nodeInQuestion must have been finished.
            assert(nodeInQuestion->ready >= 1);
            return true;
        }
    }
}

bool SymVal::directlyConstructable(Val::ReadyType targetReady){
    bool allReady = true;
    for(auto eachInEdge: In_edges){
        if( ! this->isThisNodeReady(eachInEdge.second, targetReady)){
            allReady = false;
        }
    }
    return allReady;
}

void RuntimeIntVal::Assign(IntValType val) {
    Val=val;
    Unassigned = false;
    ready++;
}

void RuntimeFloatVal::Assign(float val) {
    Val=val;
    Unassigned = false;
    ready++;
}

void RuntimeDoubleVal::Assign(double val) {
    Val=val;
    Unassigned = false;
    ready++;
}

void RuntimePtrVal::Assign(void *val) {
    Val=val;
    Unassigned = false;
    ready++;
}

inline SymExpr SymVal::extractSymExprFromSymVal(SymVal * op, ReadyType targetReady) {
    SymExpr ret = nullptr;
    if(auto tmpTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(op);tmpTruePhi != nullptr){
        assert(tmpTruePhi->ready >= targetReady);
        ret = tmpTruePhi->historyValues.back().second;
    } else{
        // for debugging only, since we already ensured that this op is ready before calling construct
        assert(this->isThisNodeReady(op, targetReady));
        //debugging end
        ret = op->symExpr;
    }
    return ret;
}

#define DEFINE_SYMVAL_CONSTRUCTION(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    assert(targetReady == (ready + 1));                                       \
}


#define DEFINE_SYMVAL_CONSTRUCTION1(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){ \
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp != nullptr && symOp->symExpr != nullptr); \
    symExpr =  OP(extractSymExprFromSymVal(symOp, targetReady));       \
    ready++;                          \
}


#define DEFINE_SYMVAL_CONSTRUCTION2(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    auto symOp1 = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp1 != nullptr && symOp1->symExpr != nullptr); \
    auto symOp2 = dynamic_cast<SymVal*>(In_edges.at(1));    \
    assert(symOp2 != nullptr && symOp2->symExpr != nullptr);           \
    symExpr =  OP(extractSymExprFromSymVal(symOp1, targetReady),extractSymExprFromSymVal(symOp2, targetReady));       \
    ready++;                                             \
}

void SymVal_sym_build_integer::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    // check op0
    Val* op0 = In_edges.at(0);
    uint32_t op0_val = 0;
    if(auto tmp_const = dynamic_cast<ConstantIntVal*>(op0)){
        op0_val = tmp_const->Val;
    }else if(auto tmp_runtime = dynamic_cast<RuntimeIntVal*>(op0)){
        // when this is called, we assume the runtime value will already be there
        // otherwise, it means, the FalsePhiLeaf concreteness checking is wrong
        assert( ! tmp_runtime->Unassigned);
        assert(tmp_runtime->ready == (ready + 1) );
        op0_val = tmp_runtime->Val;
    }else{
        std::cerr << "unhandled build integer type;";
        abort();
    }

    //check op1
    auto op1 = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(op1 != nullptr);

    // construct the symExpr
    symExpr = _sym_build_integer(op0_val, op1->Val);
    //ready ++
    ready++;
}
void SymVal_sym_build_float::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto val_op = In_edges.at(0);
    double val;
    if(auto runtimeFloat = dynamic_cast<RuntimeFloatVal*>(val_op)){
        assert(!runtimeFloat->Unassigned);
        val = static_cast<double>(runtimeFloat->Val);
    }else if(auto runtimeDouble = dynamic_cast<RuntimeDoubleVal*>(val_op)){
        assert(!runtimeDouble->Unassigned);
        val = runtimeDouble->Val;
    }else if(auto constFloat = dynamic_cast<ConstantFloatVal*>(val_op)){
        val = static_cast<double>(constFloat->Val);
    }else if(auto constDouble = dynamic_cast<ConstantDoubleVal*>(val_op)){
        val = constDouble->Val;
    }

    auto whateverthisis = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(whateverthisis != nullptr);
    bool isDouble = whateverthisis->Val;
    symExpr = _sym_build_float(val, isDouble);
    ready++;
}
void SymVal_sym_build_null_pointer::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    symExpr = _sym_build_null_pointer();
    ready++;
}
void SymVal_sym_build_true::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    symExpr = _sym_build_true();
    ready++;
}

void SymVal_sym_build_false::Construct(Val::ReadyType targetReady){
    assert(targetReady == (ready + 1));
    symExpr = _sym_build_false();
    ready ++ ;
}
void SymVal_sym_build_bool::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto val_op = In_edges.at(0);
    bool val;
    if(auto const_int = dynamic_cast<ConstantIntVal*>(val_op)){
        val = static_cast<bool>(const_int->Val);
    }else{
        auto runtime_int = dynamic_cast<RuntimeIntVal*>(val_op);
        assert(runtime_int != nullptr);
        assert(!runtime_int->Unassigned);
        val = static_cast<bool>(runtime_int->Val);
    }

    symExpr = _sym_build_bool(val);
    ready++;
}


DEFINE_SYMVAL_CONSTRUCTION(_sym_notify_call)
DEFINE_SYMVAL_CONSTRUCTION(_sym_try_alternative)
DEFINE_SYMVAL_CONSTRUCTION(_NULL)

DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_neg)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_add)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_sub)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_mul)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_div)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_div)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_rem)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_rem)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_shift_left)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_logical_shift_right)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_arithmetic_shift_right)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_fp_add)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_fp_sub)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_fp_mul)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_fp_div)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_fp_rem)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_fp_abs)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_not)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_less_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_less_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_greater_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_signed_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_less_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_less_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_greater_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_unsigned_greater_equal)

void SymVal_sym_build_equal::Construct(Val::ReadyType targetReady){
    auto left_operand = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(left_operand != nullptr && left_operand->symExpr != nullptr);

    auto right_operand = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(right_operand != nullptr && right_operand->symExpr != nullptr);

    symExpr = _sym_build_equal(extractSymExprFromSymVal(left_operand,targetReady), extractSymExprFromSymVal(right_operand,targetReady));
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_not_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_bool_and)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_and)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_bool_or)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_or)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_bool_xor)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_xor)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_ordered)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_equal)
DEFINE_SYMVAL_CONSTRUCTION2(_sym_build_float_unordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_sext)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_zext)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_trunc)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_bswap)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_int_to_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bits_to_float)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_float_to_bits)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_signed_integer)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_unsigned_integer)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool_to_bits)
DEFINE_SYMVAL_CONSTRUCTION(_sym_set_parameter_expression) // handled elsewhere
void SymVal_sym_get_parameter_expression::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    auto const_op = dynamic_cast<ConstantIntVal*>(In_edges.at(0));
    assert(const_op != nullptr);
    symExpr = _sym_get_parameter_expression(const_op->Val);
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_set_return_expression)
void SymVal_sym_get_return_expression::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    symExpr = _sym_get_return_expression();
    ready++;
}
void SymVal_sym_build_path_constraint::Construct(Val::ReadyType targetReady) {
    // check symVal operand
    assert(targetReady == (ready + 1) );
    auto symVal = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symVal != nullptr && symVal->ready > ready);

    // check the boolean
    // check op0
    auto boolean_operand = dynamic_cast<RuntimeIntVal*>(In_edges.at(1));
    assert(boolean_operand != nullptr && boolean_operand->ready == (ready+1) );
    // We simply use the symid as the siteID
    _sym_build_path_constraint(extractSymExprFromSymVal(symVal, targetReady), boolean_operand->Val, symID);
    symExpr = nullptr;
    ready++;
}
void SymVal_sym_build_read_memory::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    // check op0
    auto ptrOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    if(ptrOperand->Unassigned){
        // we are reading from addr that has not been reported from the MCU,
        // this only means that the address we are reading are all concrete
        symExpr = nullptr;
        ready++;
    }
    uint32_t ptr_val = 0;
    assert(ptrOperand != nullptr && ptrOperand->ready == (ready+1) );
    ptr_val = ptrOperand->Val;

    // length
    auto lengthOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    uint32_t len_val = 0;
    assert(lengthOperand != nullptr);
    len_val = lengthOperand->Val;

    // little endian
    auto endianOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(2));
    uint32_t endian_val = 0;
    assert(endianOperand != nullptr);
    endian_val = endianOperand->Val;

    symExpr = _sym_build_read_memory(reinterpret_cast<uint8_t*>(ptr_val), len_val, endian_val);
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_write_memory)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memcpy)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memset)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memmove)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_insert)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_extract)

