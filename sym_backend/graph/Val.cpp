//
// Created by charl on 6/14/2022.
//
#include "Val.h"

bool Val::isThisNodeReady(Val * nodeInQuestion, unsigned targetReady) {
    auto * nodeIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(nodeInQuestion);
    auto * rootIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(this);
    assert(nodeIsTruePhi == nullptr and rootIsTruePhi == nullptr);
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
        assert(tmpConst != nullptr || tmpConst != nullptr);
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

bool SymVal::directlyConstructable(){
    bool allReady = true;
    for(auto eachInEdge: In_edges){
        if( ! this->isThisNodeReady(eachInEdge.second, ready + 1)){
            allReady = false;
        }
    }
    return allReady;
}

void RuntimeIntVal::Assign(IntValType val) {
    Val=val;
    ready++;
}

void RuntimeFloatVal::Assign(float val) {
    Val=val;
    ready++;
}

void RuntimeDoubleVal::Assign(double val) {
    Val=val;
    ready++;
}

void RuntimePtrVal::Assign(void *val) {
    Val=val;
    ready++;
}


#define DEFINE_SYMVAL_CONSTRUCTION(OP) \
void SymVal##OP::Construct(){          \
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_notify_call)
DEFINE_SYMVAL_CONSTRUCTION(_sym_try_alternative)

DEFINE_SYMVAL_CONSTRUCTION(_NULL)
void SymVal_sym_build_integer::Construct() {
    // check op0
    Val* op0 = In_edges.at(0);
    uint32_t op0_val = 0;
    if(auto tmp_const = dynamic_cast<ConstantIntVal*>(op0)){
        op0_val = tmp_const->Val;
    }else if(auto tmp_runtime = dynamic_cast<RuntimeIntVal*>(op0)){
        assert(tmp_runtime->ready == (ready + 1) );
        op0_val = tmp_runtime->Val;
    }

    //check op1
    auto op1 = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(op1 != nullptr);

    // construct the symExpr
    symExpr = _sym_build_integer(op0_val, op1->Val);
    //ready ++
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_null_pointer)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_true)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_false)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_neg)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_add)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_sub)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_mul)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_div)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_div)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_rem)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_rem)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_shift_left)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_logical_shift_right)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_arithmetic_shift_right)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_add)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_sub)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_mul)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_div)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_rem)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_fp_abs)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_not)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_less_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_less_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_greater_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_signed_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_less_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_less_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_greater_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_unsigned_greater_equal)

void SymVal_sym_build_equal::Construct(){
    auto left_operand = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(left_operand != nullptr && left_operand->symExpr != nullptr);

    auto right_operand = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(right_operand != nullptr && right_operand->symExpr != nullptr);

    symExpr = _sym_build_equal(left_operand->symExpr, right_operand->symExpr);
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_not_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool_and)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_and)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool_or)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_or)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool_xor)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_xor)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_ordered)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_unordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_sext)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_zext)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_trunc)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bswap)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_int_to_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bits_to_float)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_bits)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_signed_integer)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_float_to_unsigned_integer)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_bool_to_bits)
DEFINE_SYMVAL_CONSTRUCTION(_sym_set_parameter_expression)
DEFINE_SYMVAL_CONSTRUCTION(_sym_get_parameter_expression)
DEFINE_SYMVAL_CONSTRUCTION(_sym_set_return_expression)
DEFINE_SYMVAL_CONSTRUCTION(_sym_get_return_expression)
void SymVal_sym_build_path_constraint::Construct() {
    // check symVal operand
    auto symVal = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symVal != nullptr && symVal->ready > ready);

    // check the boolean
    // check op0
    auto boolean_operand = dynamic_cast<RuntimeIntVal*>(In_edges.at(1));
    assert(boolean_operand != nullptr && boolean_operand->ready == (ready+1) );
    // We simply use the symid as the siteID
    _sym_build_path_constraint(symVal->symExpr, boolean_operand->Val, symID);
    symExpr = nullptr;
    ready++;
}
void SymVal_sym_build_read_memory::Construct() {
    // check op0
    auto ptrOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
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

    symExpr = _sym_build_read_memory((uint8_t*) ptr_val, len_val, endian_val);
    ready++;
}
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_write_memory)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memcpy)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memset)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_memmove)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_insert)
DEFINE_SYMVAL_CONSTRUCTION(_sym_build_extract)

