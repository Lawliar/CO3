//
// Created by charl on 6/14/2022.
//
#include "Val.h"

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


inline vector<Val*> Val::realChildren() {
    vector<Val*> realChildren;
    SymVal_sym_FalsePhiRoot * false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(this);
    SymVal_sym_FalsePhiLeaf * false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(this);
    if(false_phi_root != nullptr) {
        for (auto eachLeaf: false_phi_root->falsePhiLeaves) {
            realChildren.push_back(eachLeaf);
        }
    }else if(false_phi_leaf  != nullptr){
        realChildren.push_back(false_phi_leaf->In_edges.at(0));
        for(auto eachPeerOrig: false_phi_leaf->peerOriginals) {
            realChildren.push_back(eachPeerOrig);
        }
    }else{
        //normal nodes
        for(auto eachDep : In_edges){
            realChildren.push_back(eachDep.second);
        }
    }
    return realChildren;
}
bool SymVal::directlyConstructable(Val::ReadyType targetReady){
    for(auto eachDep: realChildren()){
        if( ! this->isThisNodeReady(eachDep, targetReady)){
            return false;
        }
    }
    return true;
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
    }else if(auto tmpNull = dynamic_cast<SymVal_NULL*>(op);tmpNull != nullptr) {
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

#define DEFINE_SYMVAL_CONSTRUCTION(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    assert(targetReady == (ready + 1));                                       \
}


#define DEFINE_SYMVAL_CONSTRUCTION1(OP) \
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


#define DEFINE_SYMVAL_CONSTRUCTION2(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    auto symOp1 = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp1 != nullptr);                              \
    auto symOp2 = dynamic_cast<SymVal*>(In_edges.at(1));    \
    assert(symOp2 != nullptr);          \
    auto symInput1 = extractSymExprFromSymVal(symOp1, targetReady);  \
    auto symInput2 = extractSymExprFromSymVal(symOp2, targetReady);  \
    if(symInput1 == nullptr){     \
        assert(symInput2 ==nullptr);                  \
        symExpr = nullptr;                                  \
    }else{                                \
        symExpr =  OP(symInput1,symInput2);  \
    }                                                     \
    ready++;                                             \
}

#define DEFINE_SYMVAL_SYM_1CONSTINT(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
    auto symOp = dynamic_cast<SymVal*>(In_edges.at(0));    \
    assert(symOp != nullptr);                              \
    auto constOp = dynamic_cast<ConstantIntVal*>(In_edges.at(1));    \
    assert(constOp != nullptr);         \
    auto symInput = extractSymExprFromSymVal(symOp, targetReady);  \
    if(symInput == nullptr){     \
        symExpr = nullptr;                                  \
    }else{                              \
        symExpr =  OP(symInput, constOp->Val); \
    }                                                     \
    ready++;                                             \
}

#define DEFINE_SYMVAL_SYM_2CONSTINT(OP) \
void SymVal##OP::Construct(Val::ReadyType targetReady){          \
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
        symExpr =  OP(symInput, constOp1->Val,constOp2->Val); \
    }                                                     \
    ready++;                                             \
}

void SymVal_sym_build_integer::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1));
    // check op0
    Val* op0 = In_edges.at(0);
    uint32_t op0_val = 0;
    if(auto tmp_const = dynamic_cast<ConstantIntVal*>(op0); tmp_const != nullptr){
        op0_val = tmp_const->Val;
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

    // construct the symExpr
    symExpr = _sym_build_integer(op0_val, op1->Val);
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
        val = static_cast<double>(constFloat->Val);
    }else if(auto constDouble = dynamic_cast<ConstantDoubleVal*>(val_op) ; constDouble != nullptr){
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
DEFINE_SYMVAL_CONSTRUCTION(_sym_notify_call)

void SymVal_sym_try_alternative::Construct(ReadyType targetReady) {
    assert(targetReady == (ready + 1));


    auto runtimeOperand = dynamic_cast<RuntimeVal*>(In_edges.at(0));
    assert(runtimeOperand != nullptr);

    auto symOperand = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(symOperand != nullptr);
    extractSymExprFromSymVal(symOperand, targetReady);


    //we are not really gonna do anything
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
    unsigned paraOff = paraIndex->Val;

    _sym_set_parameter_expression(paraOff, extractSymExprFromSymVal(symPara, targetReady));
    ready++;
}
void SymVal_sym_set_return_expression::Construct(ReadyType targetReady) {
    auto symRet = dynamic_cast<SymVal*>(In_edges.at(0));
    assert( symRet != nullptr);
    _sym_set_return_expression(extractSymExprFromSymVal(symRet, targetReady));
    ready++;
}

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
void SymVal_sym_build_equal::Construct(ReadyType targetReady) {
    auto symOp1 = dynamic_cast<SymVal*>(In_edges.at(0));
    assert(symOp1 != nullptr);
    auto symOp2 = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(symOp2 != nullptr);
    auto symInput1 = extractSymExprFromSymVal(symOp1, targetReady);
    auto symInput2 = extractSymExprFromSymVal(symOp2, targetReady);
    if(symInput1 == nullptr){
        assert(symInput2 ==nullptr);
        symExpr = nullptr;
    }else{
        symExpr =  _sym_build_equal(symInput1,symInput2);
    }
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
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_sext)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_zext)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_trunc)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_bswap)
DEFINE_SYMVAL_SYM_2CONSTINT(_sym_build_int_to_float)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_float_to_float)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_bits_to_float)
DEFINE_SYMVAL_CONSTRUCTION1(_sym_build_float_to_bits)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_float_to_signed_integer)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_float_to_unsigned_integer)
DEFINE_SYMVAL_SYM_1CONSTINT(_sym_build_bool_to_bits)

void SymVal_sym_get_parameter_expression::Construct(Val::ReadyType targetReady) {
    assert(targetReady == (ready + 1) );
    auto const_op = dynamic_cast<ConstantIntVal*>(In_edges.at(0));
    assert(const_op != nullptr);
    symExpr = _sym_get_parameter_expression(const_op->Val);
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
        assert(boolean_operand->Unassigned);
    }else{
        // We simply use the symid as the siteID
        _sym_build_path_constraint(symInput, boolean_operand->Val, symID);
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
        symExpr = _sym_build_read_memory(reinterpret_cast<uint8_t*>(ptrOperand->Val), lengthOperand->Val, endianOperand->Val);
    }
    ready++;
}
void SymVal_sym_build_write_memory::Construct(ReadyType targetReady) {
    assert((ready + 1) == targetReady );
    auto symValOperand = dynamic_cast<SymVal*>(In_edges.at(2));

    auto symInput = extractSymExprFromSymVal(symValOperand, targetReady);
    auto addrOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    assert(addrOperand != nullptr);

    if(symInput== nullptr){
        // ensure what I calculate on the PC side is consistent with the MCU side(i.e., it's not reporting this as well)
        // however, this consistency might break, since there might be some false positive from the MCU side.
        assert(addrOperand->Unassigned);
    }else{
        auto lengthOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(1));
        assert(lengthOperand != nullptr);

        auto isLittleEndianOperand = dynamic_cast<ConstantIntVal*>(In_edges.at(3));
        assert(isLittleEndianOperand != nullptr);

        _sym_build_write_memory(reinterpret_cast<uint8_t*>(addrOperand->Val), lengthOperand->Val, \
                                symInput, isLittleEndianOperand->Val );
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
    auto memOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(0));
    assert(memOperand != nullptr);

    auto symVal = dynamic_cast<SymVal*>(In_edges.at(1));
    assert(symVal != nullptr);

    auto lengthOperand = dynamic_cast<RuntimeIntVal*>(In_edges.at(2));
    assert(lengthOperand != nullptr);

    if(memOperand->Unassigned){
        assert(symVal->symExpr == nullptr && lengthOperand->Unassigned);
    }else{
        _sym_build_memset(reinterpret_cast<uint8_t*>(memOperand->Val), extractSymExprFromSymVal(symVal, targetReady), lengthOperand->Val);
    }
    ready++;
}
void SymVal_sym_build_memmove::Construct(ReadyType) {
    std::cerr <<"not supported yet\n";
    assert(false);
}
void SymVal_sym_build_insert::Construct(ReadyType) {
    std::cerr <<"not supported yet\n";
    assert(false);
}
void SymVal_sym_build_extract::Construct(ReadyType) {
    std::cerr <<"not supported yet\n";
    assert(false);
}
