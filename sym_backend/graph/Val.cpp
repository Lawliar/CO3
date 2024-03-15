//
// Created by charl on 6/14/2022.
//
#include "Val.h"
#include "Shadow.h"

extern WriteShadowIteratorDR * DR_INPUT;

bool Val::isThisNodeReady(Val * nodeInQuestion, unsigned targetReady) {
    auto * nodeIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(nodeInQuestion);
    //auto * rootIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(this);
    //assert(rootIsTruePhi == nullptr);
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
        //todo: for debugging only
#ifdef DEBUG_CHECKING
        auto tmpSymNull = dynamic_cast<SymVal_NULL*>(nodeInQuestion);
        auto tmpConst = dynamic_cast<ConstantVal*>(nodeInQuestion);
        assert(tmpSymNull != nullptr || tmpConst != nullptr);
        //end of debug
#endif
        return true;
    }
    if(!nodeInQuestion->inLoop){
        if(nodeInQuestion->ready == 1){
            return true;
        }else if (nodeInQuestion->ready == 0){
            return false;
        }else{
            cerr<<"Nodes in the non-loop BB should at executed at most once.";
            abort();
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
                    std::cerr<<"this node's ready is even more than target Ready ?";
                    std::cerr.flush();
                    abort();
                }
            }else if (nodeInQuestion->ready > (this->ready )){
                // nodeInQuestion is more ready than this current node
                return true;
            }else{
                // this is the case where (nodeInquestion->ready < this->ready)
                // nodeInQuestion can still be ready, if it matches targetReady
                if(nodeInQuestion->ready == targetReady) {
                    return true;
                }else if(nodeInQuestion->ready < targetReady){
                    return false;
                }else{
                    // nodeInQuestion is larger than targetReady? targetReady value must be wrong
                    std::cerr << "TargetReady might be wrong\n";
                    std::cerr.flush();
                    exit(1);
                }
            }
        }else{
            // for BB in the loop we execute in the per-BB level
            // given the root is under construction, no matter it's in the loop or not
            // the nodeInQuestion is inside a loop, and they are in different BB
            // then it must has been executed on the MCU side
            // however on the PC side there are different cases:
            //     1. the nodeInQuestion's BB has been reported
            //     2. the nodeInquestion's BB has not been reported
            // if it's been reported, then this BB is updated(i.e., executed on the PC proactively)
            // if it's not reported, then it means, this nodeInQuestion is concrete, and we just leave it as it is(since the symExpr for every symVal is initialized to nullptr)
            //assert(nodeInQuestion->ready >= 1);
            return true;
        }
    }
}


Val::ReadyType SymVal_sym_TruePhi::getDepTargetReady(Val * nodeInQuestion) {
    // how TruePhi's dep work is different from other nodes
    Val::ReadyType current_ready = ready;
    if(nodeInQuestion->BBID == BBID) {
        // TruePhi is depending on a node within the same BB
        // given TruePhi is always the first instruction in the BB
        // the targetReady for the nodeInQuestion should just be ready, instead of ready + 1 like other nodes
        return current_ready;
    }else {
        if(nodeInQuestion->inLoop) {
            // not in the same BB, and the NodeInQuestion is inside a loop
            // then it's of course ready, just return its return value
            return nodeInQuestion->ready;
        }else {
            // not in the same BB, and nodeInquestion is not inside a loop,
            // then it must be executed once.
            return 1;
        }
    }
}

Val::ReadyType SymVal_sym_notify_select::getDepTargetReady(Val * nodeInQuestion) {
    Val::ReadyType current_ready = ready;
    if(nodeInQuestion->BBID == BBID){
        // if the select instruction and the selected instruction is inside the same BB
        // the select instruction must come after the selected instruction (right?)
        // otherwise the LLVM IR would be invalid
        return nodeInQuestion->ready + 1;
    }else {
        if(nodeInQuestion->inLoop){
            return nodeInQuestion->ready;
        }else {
            return 1;
        }
    }
}

vector<Val*> Val::realChildren() {
    vector<Val*> realChildren;
    auto false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(this);
    auto false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(this);

    if(false_phi_root != nullptr) {
        for (auto eachLeaf: false_phi_root->falsePhiLeaves) {
            realChildren.push_back(eachLeaf);
        }
    }else if(false_phi_leaf  != nullptr){
        realChildren.push_back(false_phi_leaf->In_edges.at(0));
        for(auto eachPeerOrig: false_phi_leaf->peerOriginals) {
            realChildren.push_back(eachPeerOrig);
        }
    }
    else{
        //normal nodes
        for(auto eachDep : In_edges){
            realChildren.push_back(eachDep.second);
        }
    }


    return realChildren;
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

SymExpr SymVal::extractSymExprFromSymVal(SymVal * op, ReadyType targetReady) {
    SymExpr ret = nullptr;
    if(auto tmpTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(op);tmpTruePhi != nullptr){
        //assert(tmpTruePhi->ready >= targetReady);
        if(tmpTruePhi->historyValues.size() == 0) {
            ret = nullptr;
        }else {
            ret = tmpTruePhi->historyValues.back().second;
        }
    }else if(auto tmpSelect = dynamic_cast<SymVal_sym_notify_select*>(op);tmpSelect!= nullptr){
        if(tmpSelect->historyValues.size() == 0){
            ret = nullptr;
        }else {
            ret = tmpSelect->historyValues.back().second;
        }
    }
    else if(auto tmpNull = dynamic_cast<SymVal_NULL*>(op);tmpNull != nullptr) {
        ret = nullptr;
    }
    else{
        // for debugging only, since we already ensured that this op is ready before calling construct
        //assert(this->isThisNodeReady(op, targetReady));
        //debugging end
        ret = op->symExpr;
    }
    return ret;
}

#define DEFINE_SYMVAL_CONSTRUCTION_DUMMY(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    assert(targetReady == (ready + 1));                                       \
}

#define DEFINE_SYMVAL_CONSTRUCTION_OP1_CONST1(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){ \
    auto intOp = dynamic_cast<ConstantIntVal*>(In_edges.at(0));    \
    assert(intOp != nullptr);           \
    symExpr =  OP(intOp->Value); \
    ready++;                          \
}



#define DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){ \
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp != nullptr);           \
    auto symInput =   extractSymExprFromSymVal(symOp, targetReady); \
    if(symInput != nullptr){      \
        symExpr =  OP(symInput); \
    }else{                              \
        symExpr = nullptr;                                    \
    }                                    \
    ready++;                          \
}

#if defined CO3_NO_MCU_SHADOW
#define DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){           \
    assert(targetReady == (ready + 1));                           \
    auto symOp1 = dynamic_cast<SymVal*>(In_edges.at(0));           \
    assert(symOp1 != nullptr);                                    \
    auto symOp2 = dynamic_cast<SymVal*>(In_edges.at(1));            \
    assert(symOp2 != nullptr);                                       \
    auto symInput1 = extractSymExprFromSymVal(symOp1, targetReady);  \
    auto symInput2 = extractSymExprFromSymVal(symOp2, targetReady);  \
    if(symInput1 == nullptr || symInput2 == nullptr){                \
        if(symInput1 == nullptr && symInput2 == nullptr){           \
            symExpr = nullptr;                                      \
        }else{                                                      \
            if(symInput1 == nullptr){                               \
                symInput1 = _sym_build_integer(0,symInput2->bits()); \
            }else if(symInput2 == nullptr){                           \
                symInput2 = _sym_build_integer(0,symInput1->bits());   \
            }                                                          \
            symExpr =  OP(symInput1,symInput2);                         \
        }                                                              \
    }else{                                                             \
        symExpr =  OP(symInput1,symInput2);                            \
    }                                                                 \
    ready++;                                                         \
}
#else
#define DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){           \
    assert(targetReady == (ready + 1));                           \
    auto symOp1 = dynamic_cast<SymVal*>(In_edges.at(0));           \
    assert(symOp1 != nullptr);                                    \
    auto symOp2 = dynamic_cast<SymVal*>(In_edges.at(1));            \
    assert(symOp2 != nullptr);                                       \
    auto symInput1 = extractSymExprFromSymVal(symOp1, targetReady);  \
    auto symInput2 = extractSymExprFromSymVal(symOp2, targetReady);  \
    if(symInput1 == nullptr || symInput2 == nullptr){                \
        assert(symInput1 == nullptr && symInput2 ==nullptr);              \
        symExpr = nullptr;                                              \
    }else{                                                             \
        symExpr =  OP(symInput1,symInput2);                            \
    }                                                                 \
    ready++;                                                         \
}
#endif

#define DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){ \
    assert(targetReady == (ready + 1));                     \
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp != nullptr);                              \
    auto constOp = dynamic_cast<ConstantIntVal*>(In_edges.at(1));    \
    assert(constOp != nullptr);         \
    auto symInput = extractSymExprFromSymVal(symOp, targetReady);  \
    if(symInput == nullptr){     \
        symExpr = nullptr;                                  \
    }else{                              \
        symExpr =  OP(symInput, constOp->Value); \
    }                                                     \
    ready++;                                             \
}

#define DEFINE_SYMVAL_CONSTRUCTION_OP3_SYM1_CONST2(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){ \
    assert(targetReady == (ready + 1));                     \
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp != nullptr);                              \
    auto constOp1 = dynamic_cast<ConstantIntVal*>(In_edges.at(1));    \
    assert(constOp1 != nullptr);                              \
    auto constOp2 = dynamic_cast<ConstantIntVal*>(In_edges.at(2));    \
    assert(constOp2 != nullptr);                              \
    auto symInput = extractSymExprFromSymVal(symOp, targetReady);\
    if(symInput == nullptr){     \
        symExpr = nullptr;                                  \
    }else{                              \
        symExpr =  OP(symInput, constOp1->Value,constOp2->Value); \
    }                                                     \
    ready++;                                             \
}

void SymVal_sym_build_integer::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    // check op0
    Val* op0 = In_edges.at(0);
    uint32_t op0_val = 0;
    if(auto tmp_const = dynamic_cast<ConstantIntVal*>(op0); tmp_const != nullptr){
        op0_val = tmp_const->Value;
    }else if(auto tmp_runtime = dynamic_cast<RuntimeIntVal*>(op0);tmp_runtime != nullptr){
        // build integer can be fed into various symVal(e.g., the bool value of the path contraint), not just FalsePhi
        if(tmp_runtime->Unassigned){
            symExpr = nullptr;
            ready++;
            return;
        }
        assert(tmp_runtime->ready == (ready + 1) );
        op0_val = tmp_runtime->Val;
    }else{
        std::cerr << "unhandled build integer type;";
        abort();
    }

    //check op1
    auto op1 = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(op1 != nullptr);

    assert(op1->Value % 8 != 0);// make sure it's byteLength, and then we convert it into bit length


    // construct the symExpr
    symExpr = _sym_build_integer(op0_val, op1->Value * 8);
    //ready ++
    ready++;
    return;
}
void SymVal_sym_build_float::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto val_op = In_edges.at(0);
    double val;
    if(auto runtimeFloat = dynamic_cast<RuntimeFloatVal*>(val_op); runtimeFloat != nullptr){
        if(runtimeFloat->Unassigned){
            symExpr = nullptr;
            ready++;
            return;
        }
        val = static_cast<double>(runtimeFloat->Val);
    }else if(auto runtimeDouble = dynamic_cast<RuntimeDoubleVal*>(val_op); runtimeDouble != nullptr){
        if(runtimeDouble->Unassigned){
            symExpr = nullptr;
            ready++;
            return;
        }
        val = runtimeDouble->Val;
    }else if(auto constFloat = dynamic_cast<ConstantFloatVal*>(val_op); constFloat != nullptr){
        val = static_cast<double>(constFloat->Value);
    }else if(auto constDouble = dynamic_cast<ConstantDoubleVal*>(val_op) ; constDouble != nullptr){
        val = constDouble->Value;
    }

    auto isDouble_Con = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(isDouble_Con != nullptr);
    bool isDouble = static_cast<bool>(isDouble_Con->Value);
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
        val = static_cast<bool>(const_int->Value);
    }else{
        auto runtime_int = dynamic_cast<RuntimeIntVal*>(val_op);
        assert(runtime_int != nullptr);
        if(runtime_int->Unassigned){
            symExpr =  nullptr;
            ready++;
            return;
        }
        val = static_cast<bool>(runtime_int->Val);
    }

    symExpr = _sym_build_bool(val);
    ready++;
    return;
}

//handled else where
DEFINE_SYMVAL_CONSTRUCTION_DUMMY(_sym_notify_call)

void SymVal_sym_try_alternative::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));

    auto symNode = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symNode != nullptr);
    auto symExpr = extractSymExprFromSymVal(symNode, targetReady);

    auto concNode = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(concNode != nullptr);
    auto concExpr = extractSymExprFromSymVal(concNode, targetReady);
    SymIDType redirectedSymID = symIDR != 0 ? symIDR : symID;
    if(symExpr != nullptr && concExpr !=  nullptr){
        _sym_build_path_constraint(
                _sym_build_equal(symExpr,
                                 concExpr),
                true, redirectedSymID);
    }
    ready++;
    return;
}

void SymVal_sym_notify_select::Construct(Val::ReadyType targetReady){
    assert(false);
    assert(targetReady == (ready + 1));

    auto condOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    assert(condOperand != nullptr);
    if(condOperand->Unassigned){
        // we are constructing a select, without the message from the MCU
        // this means this is concrete
        symExpr = nullptr;
        ready++;
        return;
    }
    if(static_cast<bool>(condOperand->Val)){
        auto trueSymOperand = dynamic_cast<SymVal*>(In_edges.at(1));
        symExpr = extractSymExprFromSymVal( trueSymOperand ,targetReady);
    }else{
        auto falseSymOperand = dynamic_cast<SymVal*>(In_edges.at(2));
        symExpr = extractSymExprFromSymVal( falseSymOperand ,targetReady);
    }
    ready++;
}


void SymVal_NULL::Construct(Val::ReadyType targetReady){
    // should not be called
    assert(false);
}



void SymVal_sym_set_parameter_expression::Construct(ReadyType targetReady) {
    auto paraIndex = dynamic_cast<ConstantIntVal*>(In_edges.at(0));
    auto symPara = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(paraIndex != nullptr && symPara != nullptr);
    unsigned paraOff = paraIndex->Value;

    _sym_set_parameter_expression(paraOff, extractSymExprFromSymVal(symPara, targetReady));
    ready++;
}
void SymVal_sym_set_return_expression::Construct(ReadyType targetReady) {
    auto symRet = dynamic_cast<SymVal*>(In_edges.at(0));
    assert( symRet != nullptr);
    auto retSymExpr = extractSymExprFromSymVal(symRet, targetReady);
    _sym_set_return_expression(retSymExpr);
    ready++;
}

DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(_sym_build_neg)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_add)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_sub)

DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_mul)

DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_div)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_div)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_rem)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_rem)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_shift_left)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_logical_shift_right)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_arithmetic_shift_right)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_fp_add)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_fp_sub)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_fp_mul)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_fp_div)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_fp_rem)
DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(_sym_build_fp_abs)
DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(_sym_build_not)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_less_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_less_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_greater_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_signed_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_less_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_less_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_greater_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_unsigned_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_not_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_bool_and)

DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_and)

DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_bool_or)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_or)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_bool_xor)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_xor)
DEFINE_SYMVAL_CONSTRUCTION_OP1_CONST1(_sym_build_zero_bytes)

DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_ordered)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_greater_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_greater_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_less_than)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_less_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM2(_sym_build_float_unordered_not_equal)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_sext)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_zext)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_trunc)
DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(_sym_build_bswap)
DEFINE_SYMVAL_CONSTRUCTION_OP3_SYM1_CONST2(_sym_build_int_to_float)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_float_to_float)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_bits_to_float)
DEFINE_SYMVAL_CONSTRUCTION_OP1_SYM1(_sym_build_float_to_bits)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_float_to_signed_integer)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_float_to_unsigned_integer)
DEFINE_SYMVAL_CONSTRUCTION_OP2_SYM1_CONST1(_sym_build_bool_to_bits)

void SymVal_sym_get_parameter_expression::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    auto const_op = dynamic_cast<ConstantIntVal*>(In_edges.at(0));
    assert(const_op != nullptr);
    symExpr = _sym_get_parameter_expression(const_op->Value);
    ready++;
}

void SymVal_sym_get_return_expression::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    symExpr = _sym_get_return_expression();
    ready++;
}
void SymVal_sym_build_path_constraint::Construct(Val::ReadyType targetReady) {
    // check symVal operand
    assert(targetReady == (ready + 1) );
    auto symVal = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symVal != nullptr);

    auto boolean_operand = dynamic_cast<RuntimeIntVal*>(In_edges.at(1));
    assert(boolean_operand != nullptr);

    auto symInput = extractSymExprFromSymVal(symVal, targetReady);
    if(symInput == nullptr){
        // the symExpr called to the solver is concrete.
#if not defined CO3_NO_MCU_SHADOW
        assert(boolean_operand->Unassigned);
#endif
    }else{
        // We simply use the symid as the siteID
        assert(!boolean_operand->Unassigned);
        SymIDType redirectedSymID = symIDR != 0 ? symIDR : symID;
        _sym_build_path_constraint(symInput, boolean_operand->Val, redirectedSymID);
    }
    ready++;
}
void SymVal_sym_build_read_memory::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    // check op0
    auto ptrOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    assert(ptrOperand != nullptr);
    if(ptrOperand->Unassigned){
        // we are reading from addr that has not been reported from the MCU,
        // this only means that the address we are reading are all concrete
        symExpr = nullptr;
    }else{
        // length
        auto lengthOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
        assert(lengthOperand != nullptr);

        // little endian
        auto endianOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(2));
        assert(endianOperand != nullptr);

        if(DR_INPUT != nullptr && ( ptrOperand->Val >= DR_INPUT->orig_addr && ptrOperand->Val < (DR_INPUT->orig_addr + DR_SIZE) ) ){
            // reading from DR
            symExpr = nullptr;//initialize
            for(int i =  0 ; i < lengthOperand->Value ; i ++){
                symExpr = endianOperand->Value
                          ? _sym_concat_helper( *(*DR_INPUT), symExpr)
                          : _sym_concat_helper(symExpr, *(*DR_INPUT));
                ++(*DR_INPUT);
            }
        }else{
            // if not reading from DR
            if(! hasConcrete){
                symExpr = _sym_build_read_memory(reinterpret_cast<uint8_t*>(ptrOperand->Val), lengthOperand->Value, endianOperand->Value);
            }else{
                symExpr = _sym_build_read_memory_concrete(reinterpret_cast<uint8_t*>(ptrOperand->Val), lengthOperand->Value, endianOperand->Value,concreteValue);
            }
        }
        // the MCU think this is not concrete.
#if not defined CO3_NO_MCU_SHADOW
        assert(symExpr != nullptr);
#endif
    }
    ready++;
}
void SymVal_sym_build_write_memory::Construct(ReadyType targetReady) {
    assert((ready + 1) == targetReady );
    auto symValOperand = dynamic_cast<SymVal*>(In_edges.at(2));

    auto symInput = extractSymExprFromSymVal(symValOperand, targetReady);
    auto addrOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    assert(addrOperand != nullptr);

    //assert(!addrOperand->Unassigned);
    if(addrOperand->Unassigned){
        //the execution of this function is not triggered by messages from MCU, we should do nothing
    }else{
        auto lengthOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
        assert(lengthOperand != nullptr);

        auto isLittleEndianOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(3));
        assert(isLittleEndianOperand != nullptr);
#ifdef DEBUG_CHECKING
        if(addrOperand->Val == 604002768){
            __asm__("nop");
        }
#endif
        _sym_build_write_memory(reinterpret_cast<uint8_t*>(addrOperand->Val), lengthOperand->Value, \
                                symInput, isLittleEndianOperand->Value );
    }
    ready++;
}

void SymVal_sym_build_memcpy::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto destOperand = dynamic_cast<RuntimePtrVal*>(In_edges.at(0));
    assert(destOperand != nullptr);
    auto srcOperand = dynamic_cast<RuntimePtrVal*>(In_edges.at(1));
    assert(srcOperand != nullptr);
    auto lengthOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(2));
    assert(lengthOperand != nullptr);

    if(destOperand->Unassigned){
        assert(srcOperand->Unassigned && lengthOperand->Unassigned);
    }else{
        _sym_build_memcpy(reinterpret_cast<uint8_t*>(destOperand->Val),reinterpret_cast<uint8_t*>(srcOperand->Val), lengthOperand->Val);
    }
    ready++;
}
void SymVal_sym_build_memset::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto memOperand = dynamic_cast<RuntimePtrVal*>(In_edges.at(0));
    assert(memOperand != nullptr);

    auto symVal = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(symVal != nullptr);

    auto lengthOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(2));
    assert(lengthOperand != nullptr);

    if(memOperand->Unassigned || lengthOperand->Unassigned){
        assert(symVal->symExpr == nullptr && memOperand->Unassigned && lengthOperand->Unassigned);
    }else{
        _sym_build_memset(reinterpret_cast<uint8_t*>(memOperand->Val), extractSymExprFromSymVal(symVal, targetReady), lengthOperand->Val);
    }
    ready++;
}
void SymVal_sym_build_memmove::Construct(ReadyType) {
    std::cerr <<"not supported yet\n";
    assert(false);
}
void SymVal_sym_build_insert::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto target_symOp = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(target_symOp != nullptr);
    auto toInsert_symOp = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(toInsert_symOp != nullptr);
    auto offset_const = dynamic_cast<ConstantIntVal*>(In_edges.at(2));
    assert(offset_const != nullptr);
    auto little_endian_const = dynamic_cast<ConstantIntVal*>(In_edges.at(3));
    assert(little_endian_const != nullptr);
    auto target_symExpr = extractSymExprFromSymVal(target_symOp, targetReady);
    auto toInsert_symExpr = extractSymExprFromSymVal(toInsert_symOp, targetReady);

    if(target_symExpr == nullptr || toInsert_symExpr == nullptr){
        assert(target_symExpr == nullptr && toInsert_symExpr == nullptr);
        symExpr = nullptr;
    }else{
        symExpr =  _sym_build_insert(target_symExpr, toInsert_symExpr, offset_const->Value,little_endian_const->Value);
    }
    ready++;
}
void SymVal_sym_build_extract::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symOp != nullptr);
    auto offset_const = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
    assert(offset_const != nullptr);
    auto length_const = dynamic_cast<ConstantIntVal*>(In_edges.at(2));
    assert(length_const != nullptr);
    auto little_endian_const = dynamic_cast<ConstantIntVal*>(In_edges.at(3));
    assert(little_endian_const != nullptr);
    auto input_symexpr = extractSymExprFromSymVal(symOp, targetReady);

    if(input_symexpr == nullptr){
        symExpr = nullptr;
    }else{
        symExpr =  _sym_build_extract(input_symexpr, offset_const->Value, length_const->Value,little_endian_const->Value);
    }
    ready++;
}
