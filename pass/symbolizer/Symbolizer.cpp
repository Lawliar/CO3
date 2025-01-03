// This file is part of SymCC.
//
// SymCC is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// SymCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// SymCC. If not, see <https://www.gnu.org/licenses/>.

#include "Symbolizer.h"

#include <cstdint>
#include <fstream>
#include <queue>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GetElementPtrTypeIterator.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include "Runtime.h"

using namespace llvm;


Symbolizer::Symbolizer(llvm::Module &M, llvm::LoopInfo& LI)
: runtime(M), loopinfo(LI), dataLayout(M.getDataLayout()),
ptrBits(M.getDataLayout().getPointerSizeInBits()),
ptrBytes(M.getDataLayout().getPointerSize()),
#if defined(CO3_REPLACE)
maxNumSymVars((1 << (llvm::IntegerType::getInt16Ty(M.getContext())->getBitWidth()) ) - 1),
#else
        // we reserve the first 3 bit to represent the message type at the MCU side. this is because
        // 1. 8192 should be already plenty 2. we don't want another 1 byte to tell the workstation how to parse messages.
maxNumSymVars((1 << (llvm::IntegerType::getInt16Ty(M.getContext())->getBitWidth() - 3) ) - 1),

#endif
g(true)
{
    intPtrType = M.getDataLayout().getIntPtrType(M.getContext());
    voidType = llvm::Type::getVoidTy(M.getContext());
    int8Type = llvm::Type::getInt8Ty(M.getContext());
    isSymType = llvm::Type::getInt1Ty(M.getContext());
    symIntType = llvm::Type::getInt16Ty(M.getContext());
    constFalse = llvm::ConstantInt::getFalse(M.getContext());

    if(M.getDataLayout().isLegalInteger(64)){
#if defined(CO3_MCUS)
        llvm_unreachable("64 bit integer should not be legal on MCUs");
#else
        archIntType = llvm::Type::getInt64Ty(M.getContext());
#endif
    }else if(M.getDataLayout().isLegalInteger(32)){
        archIntType = llvm::Type::getInt32Ty(M.getContext());
    }else if(M.getDataLayout().isLegalInteger(16)){
        archIntType = llvm::Type::getInt16Ty(M.getContext());
    }else{
        llvm_unreachable("integer width less than 16 bit?");
    }

#if defined(CO3_REPLACE)
    availableSymID = 1;
#else
    availableSymID = 7; // first 6 are reserved for symbolize_memory, sym_end ,notifyCall, notifyFunc, notifyRet, notifyBasicBlock
#endif
    assert(ptrBytes <= 8 );
    for(auto eachIntFunction : kInterceptedFunctions){
        std::string newFuncName = kInterceptedFunctionPrefix.str() + eachIntFunction.str() ;
        size_t len = newFuncName.size();
        // I know, I know..
        char * buf = (char *)malloc(len+ 1);
        snprintf(buf, len + 1, "%s%s",kInterceptedFunctionPrefix.str().c_str(),eachIntFunction.str().c_str());
        //buf[len] = '\0';
        interpretedFunctionNames.insert(buf);
    }

}

Symbolizer::~Symbolizer(){
    symbolicExpressions.clear();
    symbolicIDs.clear();
    for(auto eachSymPhi: phiSymbolicIDs){
        delete eachSymPhi.second;
    }
    symIdRedirect.clear();
    phiSymbolicIDs.clear();
    for(auto eachCallToSetPara: callToSetParaMap){
        eachCallToSetPara.second.clear();
    }
    callToCallId.clear();
    callToSetParaMap.clear();
    phiNodes.clear();
    splited2OriginalBB.clear();
    originalBB2ID.clear();
    stageSettingOperations.clear();
    perBBConcretenessChecking.clear();

    loopBB2Offset.clear();
    truePhi2Offset.clear();
    for(auto eachTryAltUnit : tryAlternatives){
        delete eachTryAltUnit;
    }
    tryAlternatives.clear();
}

unsigned Symbolizer::numBits2NumBytes(unsigned numBits){
    unsigned ret = numBits / 8;
    if(numBits % 8 != 0){
        ret += 1;
    }
    return ret;
}

void Symbolizer::deleteSymPhi(llvm::PHINode* symPhi){
    bool found = false;
    for(auto eachSymExpr: symbolicExpressions){
        if(eachSymExpr.second == symPhi){
            symbolicExpressions.erase(eachSymExpr.first);
            found = true;
            break;
        }
    }
    assert(found);
    auto it = phiSymbolicIDs.find(symPhi);
    assert(it != phiSymbolicIDs.end());
    delete it->second;
    phiSymbolicIDs.erase(it);
    return;
}

void Symbolizer::assignSymIDPhi(llvm::PHINode* symPhi, PhiStatus* phiStatus){
    auto exprIt = phiSymbolicIDs.find(symPhi);
    if(exprIt != phiSymbolicIDs.end()){
        llvm_unreachable("assigning phi sym id to a already existing sym phi");
    }
    phiSymbolicIDs[symPhi] = phiStatus;
}

void Symbolizer::initializeFunctions(Function &F) {

    IRBuilder<> IRB(&*(F.getEntryBlock().getFirstInsertionPt()));
    if (F.getName() != "main"){
        // The main function doesn't receive symbolic arguments.
        for (auto &arg : F.args()) {
            if (!arg.user_empty()){
                auto paraGetSymID = getNextID();
#if defined(CO3_REPLACE)
                auto * symcall  = IRB.CreateCall(runtime.getParameterExpression,IRB.getInt8(arg.getArgNo()));
#else
                auto * symcall  = IRB.CreateCall(runtime.getParameterExpression,{IRB.getInt8(arg.getArgNo()),ConstantHelper(symIntType,paraGetSymID)});
#endif
                symbolicExpressions[&arg] = symcall;
                assignSymID(symcall,paraGetSymID);
                stageSettingOperations.push_back(paraGetSymID);
            }
        }
    }
    // allocate some space for the BBs and PhiNodes
    unsigned numOfLoopBBs = 0;
    unsigned numOfTruePhis = 0;
    unsigned numOfSelectInsts = 0;
    for(auto & eachBB : F){
        if(loopinfo.getLoopFor(&eachBB) != nullptr){
            loopBB2Offset[&eachBB] = numOfLoopBBs;
            numOfLoopBBs += 1;
        }
        for(auto & eachInst : eachBB){
            if(isa<PHINode>(&eachInst)){
                numOfTruePhis += 1;
            }
            else if(isa<SelectInst>(&eachInst)){
                // NOT all of the selectInst needs to instrumented, (because some of them can be determined as concrete at compile-time)
                // But there are so few of them, it's ok .
                numOfSelectInsts += 1;
                
            }else if(isa<ReturnInst>(&eachInst)){
                // it is a return inst
                if(return_inst != nullptr){
                    llvm_unreachable("multiple return insts");
                }
                return_inst = dyn_cast<ReturnInst>(&eachInst);
                assert(return_inst != nullptr);
                if(exit_bb == nullptr){
                    exit_bb = &eachBB;
                }
                else{
                    llvm_unreachable("multiple exit BBs");
                }
            }
        }
    }
    /*
     * This is not working, it appears LLVM just regard i1 as i8
    ArrayType* space4LoopTy = ArrayType::get(IRB.getInt1Ty(), numOfLoopBBs);
    ArrayType* space4TruePhiTy = ArrayType::get(IRB.getInt1Ty(), numOfTruePhis);
    loopBBBaseAddr = IRB.CreateAlloca(space4LoopTy);
    truePhiBaseAddr = IRB.CreateAlloca(space4TruePhiTy);
    */
    if(numOfLoopBBs > 255){
        errs()<<"Error Func:"<< F.getName()<<'\n';
        llvm_unreachable("Number of BBs in the loop of this function exceeds 255, might need to change the offset from uint8_t to uint16_t, I believe you can do it[wink].");
    }
    if(numOfTruePhis > 255){
        errs()<<"Error Func:"<< F.getName()<<'\n';
        llvm_unreachable("Number of TruePhis in this function exceeds 255, might need to change the offset from uint8_t to uint16_t, I believe you can do it[wink].");
    }
    if(numOfSelectInsts > 255){
        errs()<<"Error Func:"<< F.getName()<<'\n';
        llvm_unreachable("Number of SelectInst of this function exceeds 255, might need to change the offset from uint8_t to uint16_t, I believe you can do it[wink].");
    }
    unsigned numBytesForLoopBBs = numBits2NumBytes(numOfLoopBBs);
    unsigned numBytesForTruePhis = numBits2NumBytes(numOfTruePhis);
    unsigned numBytesForSelectInst = numBits2NumBytes(numOfSelectInsts);

    ArrayType* space4LoopTy = ArrayType::get(IRB.getInt8Ty(), numBytesForLoopBBs);
    ArrayType* space4TruePhiTy = ArrayType::get(IRB.getInt8Ty(), numBytesForTruePhis);
    ArrayType* space4SelectInst = ArrayType::get(IRB.getInt8Ty(), numBytesForSelectInst);

    loopBBBaseAddr = IRB.CreateAlloca(space4LoopTy);
    truePhiBaseAddr = IRB.CreateAlloca(space4TruePhiTy);
    selectBaseAddr = IRB.CreateAlloca(space4SelectInst);
     // initialize the created space to zero
    IRB.CreateStore( ConstantAggregateZero::get(space4LoopTy), loopBBBaseAddr);
    IRB.CreateStore( ConstantAggregateZero::get(space4TruePhiTy), truePhiBaseAddr);
    IRB.CreateStore( ConstantAggregateZero::get(space4SelectInst), selectBaseAddr);
#if LLVM_VERSION_MAJOR < 13
    loopBBBaseAddr = IRB.CreateGEP(loopBBBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
    truePhiBaseAddr = IRB.CreateGEP(truePhiBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
    selectBaseAddr = IRB.CreateGEP(selectBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
# else
    loopBBBaseAddr = IRB.CreateGEP(space4LoopTy,loopBBBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
    truePhiBaseAddr = IRB.CreateGEP(space4TruePhiTy, truePhiBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
    selectBaseAddr = IRB.CreateGEP(space4SelectInst, selectBaseAddr, {ConstantHelper(IRB.getInt8Ty(), 0), ConstantHelper(IRB.getInt8Ty(), 0)});
#endif
}

bool Symbolizer::isStaticallyConcrete(llvm::Value* input){
    auto symExpr = getSymbolicExpression(input);
    if(symExpr == constFalse){
        return true;
    }
    if(PHINode * phiNode = dyn_cast<PHINode>(input)){
        if (std::all_of(phiNode->op_begin(), phiNode->op_end(), [this](Value *input) {
            return (getSymbolicExpression(input) == constFalse);
        })) {
            return true;
        }
    }
    return false;
}
void Symbolizer::recordBasicBlockMapping(llvm::BasicBlock &B) {
    originalBB2ID[&B] = getNextBBID();
}

void Symbolizer::finalizePHINodes() {
    SmallPtrSet<PHINode *, 32> nodesToErase;

    for (auto *phi : phiNodes) {
        auto symPhi = getSymbolicExpression(phi);
        auto symbolicPHI = cast<PHINode>(symPhi);
        // A PHI node that receives only compile-time constants can be replaced by
        // a null expression.
        if (std::all_of(phi->op_begin(), phi->op_end(), [this](Value *input) {
            return (isStaticallyConcrete(input) == true);
        })) {
            nodesToErase.insert(symbolicPHI);
            continue;
        }
        for (unsigned incoming = 0, totalIncoming = phi->getNumIncomingValues();incoming < totalIncoming; incoming++) {
            symbolicPHI->setIncomingValue(
                    incoming,
                    getSymbolicExpression(phi->getIncomingValue(incoming)));
        }
    }


    for (auto *symbolicPHI : nodesToErase) {
        // this code might fail because isa<KeySansPointerT>(...) assertation is not true
        //symbolicPHI->replaceAllUsesWith(ConstantInt::get( isSymType, 0 ));
        deleteSymPhi(symbolicPHI);
        replaceAllUseWith(symbolicPHI, constFalse);
        symbolicPHI->eraseFromParent();
    }
}


void Symbolizer::handleIntrinsicCall(CallBase &I) {
    auto *callee = I.getCalledFunction();

    switch (callee->getIntrinsicID()) {
        case Intrinsic::lifetime_start:
        case Intrinsic::lifetime_end:
        case Intrinsic::dbg_declare:
        case Intrinsic::dbg_value:
        case Intrinsic::is_constant:
        case Intrinsic::trap:
        case Intrinsic::invariant_start:
        case Intrinsic::invariant_end:
        case Intrinsic::assume:
            // These are safe to ignore.
            break;
        case Intrinsic::memcpy: {
            IRBuilder<> IRB(&I);

            tryAlternative(IRB, I.getOperand(0));
            tryAlternative(IRB, I.getOperand(1));
            tryAlternative(IRB, I.getOperand(2));

            // The intrinsic allows both 32 and 64-bit integers to specify the length;
            // convert to the right type if necessary. This may truncate the value on
            // 32-bit architectures. However, what's the point of specifying a length to
            // memcpy that is larger than your address space?
            unsigned memCpyId = getNextID();
            auto memCpyCall = IRB.CreateCall(runtime.memcpy,
                   {I.getOperand(0), I.getOperand(1),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType), ConstantHelper(symIntType, memCpyId) });
            assignSymID(memCpyCall, memCpyId);
            break;
        }
        case Intrinsic::memset: {
            IRBuilder<> IRB(&I);

            tryAlternative(IRB, I.getOperand(0));
            tryAlternative(IRB, I.getOperand(2));

            unsigned memSetId = getNextID();
            auto memSetCall = IRB.CreateCall(runtime.memset,
                   {I.getOperand(0),
                    getSymbolicExpression(I.getOperand(1)),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType), ConstantHelper(symIntType, memSetId)});
            assignSymID(memSetCall, memSetId);
            break;
        }
        case Intrinsic::memmove: {
            IRBuilder<> IRB(&I);

            tryAlternative(IRB, I.getOperand(0));
            tryAlternative(IRB, I.getOperand(1));
            tryAlternative(IRB, I.getOperand(2));

            // The comment on memcpy's length parameter applies analogously.
            unsigned memMoveId = getNextID();
            auto memMoveCall = IRB.CreateCall(runtime.memmove,
                   {I.getOperand(0), I.getOperand(1),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType), ConstantHelper(symIntType, memMoveId)});
            assignSymID(memMoveCall, memMoveId);
            break;
        }
        case Intrinsic::stacksave: {
            // The intrinsic returns an opaque pointer that should only be passed to
            // the stackrestore intrinsic later. We treat the pointer as a constant.
            break;
        }
        case Intrinsic::stackrestore:
            // Ignored; see comment on stacksave above.
            break;
        case Intrinsic::expect:
            // Just a hint for the optimizer; the value is the first parameter.
            if (auto *expr = getSymbolicExpression(I.getArgOperand(0)))
                symbolicExpressions[&I] = expr;
            break;
        case Intrinsic::fabs: {
            // Floating-point absolute value; use the runtime to build the
            // corresponding symbolic expression.

            IRBuilder<> IRB(&I);
            auto abs = buildRuntimeCall(IRB, runtime.buildFloatAbs, I.getOperand(0));
            registerSymbolicComputation(abs, &I);
            break;
        }
        case Intrinsic::cttz:
        case Intrinsic::ctpop:
        case Intrinsic::ctlz: {
            // Various bit-count operations. Expressing these symbolically is
            // difficult, so for now we just concretize.

            errs() << "Warning: losing track of symbolic expressions at bit-count "
                 "operation "
                << I << "\n";
            break;
        }
        case Intrinsic::returnaddress: {
            // Obtain the return address of the current function or one of its parents
            // on the stack. We just concretize.

            errs() << "Warning: using concrete value for return address\n";
            break;
        }
        case Intrinsic::bswap: {
            // Bswap changes the endian-ness of integer values.

            IRBuilder<> IRB(&I);
            auto swapped = buildRuntimeCall(IRB, runtime.buildBswap, I.getOperand(0));
            registerSymbolicComputation(swapped, &I);
            break;
        }
        default:
            errs() << "Warning: unhandled LLVM intrinsic " << callee->getName()
                << "; the result will be concretized\n";
        break;
    }
}

void Symbolizer::handleInlineAssembly(CallInst &I) {
  if (I.getType()->isVoidTy()) {
    errs() << "Warning: skipping over inline assembly " << I << '\n';
    return;
  }

  errs() << "Warning: losing track of symbolic expressions at inline assembly "
         << I << '\n';
}
void Symbolizer::interpretedFuncSanityCheck(CallBase  & I){
    auto funcName = I.getCalledFunction()->getName();
    SymFnT* target = nullptr;
    for(auto eachSymbolizer: runtime.SymOperators){
        if(funcName == eachSymbolizer->getCallee()->getName()){
            target = eachSymbolizer;
        }
    }
    assert(target != nullptr);
    assert(target->getFunctionType()->getParamType(target->getFunctionType()->getNumParams() - 1) ==  symIntType);
}
void Symbolizer::addSymIDToCall(CallBase  & I){
    interpretedFuncSanityCheck(I);
    IRBuilder<> IRB(&I);
    auto symID = getNextID();
    std::vector<Value*> args(I.arg_begin(), I.arg_end());
    args.push_back(ConstantHelper(symIntType, symID));
    auto * symcall  = IRB.CreateCall(I.getCalledFunction(), args);
    assignSymID(symcall, symID);
    //I.replaceAllUsesWith(symcall);
    replaceAllUseWith(&I, symcall);
    I.eraseFromParent();
}
void Symbolizer::handleFunctionCall(CallBase &I, Instruction *returnPoint) {
    auto *callee = I.getCalledFunction();
    if (callee != nullptr && callee->isIntrinsic()) {
        handleIntrinsicCall(I);
        return;
    }
    IRBuilder<> IRB(returnPoint);
    unsigned int callInstID = getNextCallID();
    IRB.CreateCall(runtime.notifyRet, ConstantHelper(IRB.getInt8Ty(), callInstID));
    IRB.SetInsertPoint(&I);


    if (callee == nullptr){
        //tryAlternative(IRB, I.getCalledOperand());
    }
    else{
        auto calleeName = callee->getName();
        bool is_interpreted = false;
        for(auto eachInterpreted: interpretedFunctionNames){
            if(calleeName.equals(eachInterpreted)){
                is_interpreted = true;
            }
        }
        if(is_interpreted){
            // symID is inserted inside this function
            addSymIDToCall(I);
        }
    }
    SmallVector<CallInst*, 8> setParas;
    for (Use &arg : I.args()){
        auto argSymExpr = getSymbolicExpression(arg);
        auto argSymID = getNextID();
#if defined(CO3_REPLACE)
        CallInst * call_to_set_para = IRB.CreateCall(runtime.setParameterExpression,{ConstantInt::get(IRB.getInt8Ty(), arg.getOperandNo()), argSymExpr});
#else
        CallInst * call_to_set_para = IRB.CreateCall(runtime.setParameterExpression,{ConstantInt::get(IRB.getInt8Ty(), arg.getOperandNo()), argSymExpr,ConstantHelper(symIntType, argSymID)});
#endif
        assignSymID(call_to_set_para,argSymID);
        setParas.push_back(call_to_set_para);
    }
    auto callIdConstant = ConstantHelper(IRB.getInt8Ty(), callInstID);
    auto notifyCall = IRB.CreateCall(runtime.notifyCall, callIdConstant);
    setCallInstId(notifyCall, callIdConstant);
    assignSymID(notifyCall, getNextID());
    for(auto eachSetPara: setParas){
        addSetParaToNotifyCall(notifyCall, eachSetPara);
    }

    if (!I.user_empty()) {
        // The result of the function is used somewhere later on. Since we have no
        // way of knowing whether the function is instrumented (and thus sets a
        // proper return expression), we have to account for the possibility that
        // it's not: in that case, we'll have to treat the result as an opaque
        // concrete value. Therefore, we set the return expression to null here in
        // order to avoid accidentally using whatever is stored there from the
        // previous function call. (If the function is instrumented, it will just
        // override our null with the real expression.)

#if defined(CO3_REPLACE)
        auto setRetSymID = getNextID();
        auto callToSetReturn = IRB.CreateCall(runtime.setReturnExpression, IRB.getFalse());
        assignSymID(callToSetReturn,setRetSymID);
#else
        // since we do this automatically on the workstation, we don't need to set ret on the MCU.
        //auto callToSetReturn = IRB.CreateCall(runtime.setReturnExpression, {IRB.getFalse(), ConstantHelper(symIntType,setRetSymID)});
#endif
        IRB.SetInsertPoint(returnPoint);
        auto getRetSymID = getNextID();
#if defined(CO3_REPLACE)
        auto getReturnCall = IRB.CreateCall(runtime.getReturnExpression);
#else
        auto getReturnCall = IRB.CreateCall(runtime.getReturnExpression, ConstantHelper(symIntType,getRetSymID ));
#endif
        symbolicExpressions[&I] = getReturnCall;
        assignSymID(getReturnCall,getRetSymID);
    }
}

void Symbolizer::visitBinaryOperator(BinaryOperator &I) {
    // Binary operators propagate into the symbolic expression.

    IRBuilder<> IRB(&I);
    SymFnT handler = runtime.binaryOperatorHandlers.at(I.getOpcode());

    // Special case: the run-time library distinguishes between "and" and "or"
    // on Boolean values and bit vectors.
    if (I.getOperand(0)->getType() == IRB.getInt1Ty()) {
        switch (I.getOpcode()) {
            case Instruction::And:
                handler = runtime.buildBoolAnd;
                break;
            case Instruction::Or:
                handler = runtime.buildBoolOr;
                break;
            case Instruction::Xor:
                handler = runtime.buildBoolXor;
                break;
            default:
                errs() << "Can't handle Boolean operator " << I << '\n';
                llvm_unreachable("Unknown Boolean operator");
                break;
        }
    }

    assert(handler && "Unable to handle binary operator");
    auto runtimeCall = buildRuntimeCall(IRB, handler, {I.getOperand(0), I.getOperand(1)});
    registerSymbolicComputation(runtimeCall, &I);
}


void Symbolizer::visitSelectInst(SelectInst &I) {
  // Select is like the ternary operator ("?:") in C. We push the (potentially
  // negated) condition to the path constraints and copy the symbolic
  // expression over from the chosen argument.

    IRBuilder<> IRB(&I);
    auto runtimeCall = buildRuntimeCall(IRB, runtime.pushPathConstraint,
                                        {{I.getCondition(), true},
                                         {I.getCondition(), false}});
    registerSymbolicComputation(runtimeCall);
    Value * trueSymExpr = getSymbolicExpression(I.getTrueValue());
    Value * falseSymExpr = getSymbolicExpression(I.getFalseValue());
    if ( trueSymExpr != ConstantInt::getFalse(I.getContext()) ||
            falseSymExpr != ConstantInt::getFalse(I.getContext()) ) {
        unsigned symID = getNextID();
        CallInst* dataSymExpr = IRB.CreateCall(runtime.notifySelect, {I.getCondition(),
                                                                     trueSymExpr,
                                                                     falseSymExpr,
                                                                     selectBaseAddr,
                                                                     ConstantHelper(int8Type, selectInstOff),
                                                                     ConstantHelper(symIntType,symID)});
        selectInstOff++;
        symbolicExpressions[&I] = dataSymExpr;
        assignSymID(dataSymExpr, symID);
    }
}

void Symbolizer::visitCmpInst(CmpInst &I) {
  // ICmp is integer comparison, FCmp compares floating-point values; we
  // simply include either in the resulting expression.

  IRBuilder<> IRB(&I);
  SymFnT handler = runtime.comparisonHandlers.at(I.getPredicate());
  assert(handler && "Unable to handle icmp/fcmp variant");
  auto runtimeCall =
      buildRuntimeCall(IRB, handler, {I.getOperand(0), I.getOperand(1)});
  registerSymbolicComputation(runtimeCall, &I);
}

void Symbolizer::visitReturnInst(ReturnInst &I) {
    // Upon return, we just store the expression for the return value.

    if (I.getReturnValue() == nullptr)
        return;

    // We can't short-circuit this call because the return expression needs to
    // be set even if it's null; otherwise we break the caller. Therefore,
    // create the call directly without registering it for short-circuit
    // processing.
    IRBuilder<> IRB(&I);
    auto returnSymExpr = getSymbolicExpression(I.getReturnValue());
    auto retSymID = getNextID();
#if defined(CO3_REPLACE)
    CallInst * set_return_inst = IRB.CreateCall(runtime.setReturnExpression,returnSymExpr);
#else
    CallInst * set_return_inst = IRB.CreateCall(runtime.setReturnExpression,{returnSymExpr, ConstantHelper(symIntType, retSymID)});
#endif
    assignSymID(set_return_inst,retSymID);
    stageSettingOperations.push_back(retSymID);
}

void Symbolizer::visitBranchInst(BranchInst &I) {
  // Br can jump conditionally or unconditionally. We are only interested in
  // the former case, in which we push the branch condition or its negation to
  // the path constraints.

  if (I.isUnconditional())
    return;

  IRBuilder<> IRB(&I);
  auto runtimeCall = buildRuntimeCall(IRB, runtime.pushPathConstraint,
                                      {{I.getCondition(), true},
                                       {I.getCondition(), false}});
  registerSymbolicComputation(runtimeCall);
}

void Symbolizer::visitIndirectBrInst(IndirectBrInst &I) {
  IRBuilder<> IRB(&I);
  tryAlternative(IRB, I.getAddress());
}

void Symbolizer::visitCallInst(CallInst &I) {
  if (I.isInlineAsm())
    handleInlineAssembly(I);
  else
    handleFunctionCall(I, I.getNextNode());
}

void Symbolizer::visitInvokeInst(InvokeInst &I) {
    // Invoke is like a call but additionally establishes an exception handler. We
    // can obtain the return expression only in the success case, but the target
    // block may have multiple incoming edges (i.e., our edge may be critical). In
    // this case, we split the edge and query the return expression in the new
    // block that is specific to our edge.
    llvm_unreachable("invoke inst called, need to handle the splitted edge");
    auto *newBlock = SplitCriticalEdge(I.getParent(), I.getNormalDest());
    handleFunctionCall(I, newBlock != nullptr ? newBlock->getFirstNonPHI()
                                              : I.getNormalDest()->getFirstNonPHI());
}

void Symbolizer::visitAllocaInst(AllocaInst & /*unused*/) {
  // Nothing to do: the shadow for the newly allocated memory region will be
  // created on first write; until then, the memory contents are concrete.
}

void Symbolizer::visitLoadInst(LoadInst &I) {
    IRBuilder<> IRB(&I);

    auto *addr = I.getPointerOperand();
    tryAlternative(IRB, addr);

    auto *dataType = I.getType();
    auto readMemSymID = getNextID();
    auto intByteSize = dataLayout.getTypeStoreSize(dataType);
    if(intByteSize == 8 && I.getType()->isDoubleTy()){
        symbolicExpressions[&I] = IRB.getFalse();
        return;
    }
    if(!(intByteSize == 1 || intByteSize == 2 || intByteSize == 4)){
        assert(intByteSize == 8);
        // TODO: even in MCU, this is still 32-bit, as, llvm assumes all functions pointers are 32-bit.
        // TODO: instead of assuming this is not symbolic, support this.
#if defined(CO3_MCUS)
        llvm_unreachable("loading 8-byte data");
        symbolicExpressions[&I] = IRB.getFalse();
        return;
#endif
    }
    Value* ptrOperand  = IRB.CreatePtrToInt(addr, intPtrType);;

    //}
    auto *data = IRB.CreateCall(
      runtime.readMemory,
      {ptrOperand,
       ConstantInt::get(intPtrType, intByteSize),
       ConstantInt::get(IRB.getInt8Ty(), isLittleEndian(dataType) ? 1 : 0),
       ConstantHelper(symIntType,readMemSymID)});
    assignSymID(data,readMemSymID);
    if (dataType->isFloatingPointTy()) {
        auto bitsToFloatSymID = getNextID();
#if defined(CO3_REPLACE)
        data = IRB.CreateCall(runtime.buildBitsToFloat,{data, IRB.getInt1(dataType->isDoubleTy())});
#else
        data = IRB.CreateCall(runtime.buildBitsToFloat,{data, IRB.getInt1(dataType->isDoubleTy()), ConstantHelper(symIntType, bitsToFloatSymID)});
#endif
        assignSymID(data,bitsToFloatSymID);
    }
    symbolicExpressions[&I] = data;
}
//TODO
void Symbolizer::visitStoreInst(StoreInst &I) {
    IRBuilder<> IRB(&I);

    tryAlternative(IRB, I.getPointerOperand());

    Value * dataSymExpr = getSymbolicExpression(I.getValueOperand());
    auto *dataType = I.getValueOperand()->getType();
    if (dataType->isFloatingPointTy()) {
        auto dataSymID = getNextID();
#if defined(CO3_REPLACE)
        dataSymExpr = IRB.CreateCall(runtime.buildFloatToBits, {dataSymExpr});
#else
        dataSymExpr = IRB.CreateCall(runtime.buildFloatToBits, {dataSymExpr, ConstantHelper(symIntType, dataSymID)});
#endif
        assignSymID(cast<CallInst>(dataSymExpr), dataSymID);
    }
    auto writeMemSymID = getNextID();
    auto writeMemCall = IRB.CreateCall(
        runtime.writeMemory,
        {IRB.CreatePtrToInt(I.getPointerOperand(), intPtrType),
         ConstantInt::get(intPtrType, dataLayout.getTypeStoreSize(dataType)),
         dataSymExpr,
         ConstantInt::get(IRB.getInt8Ty(), dataLayout.isLittleEndian() ? 1 : 0),
         ConstantHelper(symIntType, writeMemSymID)});
    assignSymID(writeMemCall,writeMemSymID);
}

void Symbolizer::visitGetElementPtrInst(GetElementPtrInst &I) {
    // GEP performs address calculations but never actually accesses memory. In
    // order to represent the result of a GEP symbolically, we start from the
    // symbolic expression of the original pointer and duplicate its
    // computations at the symbolic level.

    // If everything is compile-time concrete, we don't need to emit code.
    if (isStaticallyConcrete(I.getPointerOperand()) == true &&
        std::all_of(I.idx_begin(), I.idx_end(), [this](Value *index) {
            return (isStaticallyConcrete(index) == true);
        })) {
        return;
    }

    // If there are no indices or if they are all zero we can return early as
    // well.
    if (std::all_of(I.idx_begin(), I.idx_end(), [](Value *index) {
        auto *ci = dyn_cast<ConstantInt>(index);
        return (ci != nullptr && ci->isZero());
    })) {
        symbolicExpressions[&I] = getSymbolicExpression(I.getPointerOperand());
        return;
    }

    IRBuilder<> IRB(&I);
    SymbolicComputation symbolicComputation;
    Value *currentAddress = I.getPointerOperand();

    for (auto type_it = gep_type_begin(I), type_end = gep_type_end(I);
         type_it != type_end; ++type_it) {
        auto *index = type_it.getOperand();
        std::pair<Value *, bool> addressContribution;

        // There are two cases for the calculation:
        // 1. If the indexed type is a struct, we need to add the offset of the
        //    desired member.
        // 2. If it is an array or a pointer, compute the offset of the desired
        //    element.
        if (auto *structType = type_it.getStructTypeOrNull()) {
            // Structs can only be indexed with constants
            // (https://llvm.org/docs/LangRef.html#getelementptr-instruction).

            unsigned memberIndex = cast<ConstantInt>(index)->getZExtValue();
            unsigned memberOffset =
                    dataLayout.getStructLayout(structType)->getElementOffset(memberIndex);
            addressContribution = {ConstantInt::get(intPtrType, memberOffset), true};
        } else {
            if (auto *ci = dyn_cast<ConstantInt>(index);
                    ci != nullptr && ci->isZero()) {
                // Fast path: an index of zero means that no calculations are
                // performed.
                continue;
            }

            // TODO optimize? If the index is constant, we can perform the
            // multiplication ourselves instead of having the solver do it. Also, if
            // the element size is 1, we can omit the multiplication.

            unsigned elementSize =
                    dataLayout.getTypeAllocSize(type_it.getIndexedType());
            if (auto indexWidth = index->getType()->getIntegerBitWidth();
                    indexWidth != ptrBits) {
                symbolicComputation.merge(forceBuildRuntimeCall(
                        IRB, runtime.buildZExt,
                        {{index, true},
                         {ConstantInt::get(IRB.getInt8Ty(), ptrBits - indexWidth), false}}));
                symbolicComputation.merge(forceBuildRuntimeCall(
                        IRB, runtime.binaryOperatorHandlers[Instruction::Mul],
                        {{symbolicComputation.lastInstruction, false},
                         {ConstantInt::get(intPtrType, elementSize), true}}));
            } else {
                symbolicComputation.merge(forceBuildRuntimeCall(
                        IRB, runtime.binaryOperatorHandlers[Instruction::Mul],
                        {{index, true},
                         {ConstantInt::get(intPtrType, elementSize), true}}));
            }

            addressContribution = {symbolicComputation.lastInstruction, false};
        }

        symbolicComputation.merge(forceBuildRuntimeCall(
                IRB, runtime.binaryOperatorHandlers[Instruction::Add],
                {addressContribution,
                 {currentAddress, (currentAddress == I.getPointerOperand())}}));
        currentAddress = symbolicComputation.lastInstruction;
    }

    registerSymbolicComputation(symbolicComputation, &I);
}


void Symbolizer::visitBitCastInst(BitCastInst &I) {
    if (I.getSrcTy()->isIntegerTy() && I.getDestTy()->isFloatingPointTy()) {
        IRBuilder<> IRB(&I);
        auto conversion =
                buildRuntimeCall(IRB, runtime.buildBitsToFloat,
                                 {{I.getOperand(0), true},
                                  {IRB.getInt1(I.getDestTy()->isDoubleTy()), false}});
        registerSymbolicComputation(conversion, &I);
        return;
    }

    if (I.getSrcTy()->isFloatingPointTy() && I.getDestTy()->isIntegerTy()) {
        IRBuilder<> IRB(&I);
        auto conversion = buildRuntimeCall(IRB, runtime.buildFloatToBits,
                                           {{I.getOperand(0), true}});
        registerSymbolicComputation(conversion);
        return;
    }

    assert(I.getSrcTy()->isPointerTy() && I.getDestTy()->isPointerTy() &&
           "Unhandled non-pointer bit cast");
    if (auto *expr = getSymbolicExpression(I.getOperand(0)))
        symbolicExpressions[&I] = expr;
}

void Symbolizer::visitTruncInst(TruncInst &I) {
    IRBuilder<> IRB(&I);
    auto trunc = buildRuntimeCall(
            IRB, runtime.buildTrunc,
            {{I.getOperand(0), true},
             {IRB.getInt8(I.getDestTy()->getIntegerBitWidth()), false}});
    registerSymbolicComputation(trunc, &I);
}

void Symbolizer::visitIntToPtrInst(IntToPtrInst &I) {
    if (auto *expr = getSymbolicExpression(I.getOperand(0)))
        symbolicExpressions[&I] = expr;
    // TODO handle truncation and zero extension
}

void Symbolizer::visitPtrToIntInst(PtrToIntInst &I) {
    if (auto *expr = getSymbolicExpression(I.getOperand(0)))
        symbolicExpressions[&I] = expr;
    // TODO handle truncation and zero extension
}

void Symbolizer::visitSIToFPInst(SIToFPInst &I) {
    IRBuilder<> IRB(&I);
    auto conversion =
            buildRuntimeCall(IRB, runtime.buildIntToFloat,
                             {{I.getOperand(0), true},
                              {IRB.getInt1(I.getDestTy()->isDoubleTy()), false},
                              {/* is_signed */ IRB.getInt1(true), false}});
    registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitUIToFPInst(UIToFPInst &I) {
    IRBuilder<> IRB(&I);
    auto conversion =
            buildRuntimeCall(IRB, runtime.buildIntToFloat,
                             {{I.getOperand(0), true},
                              {IRB.getInt1(I.getDestTy()->isDoubleTy()), false},
                              {/* is_signed */ IRB.getInt1(false), false}});
    registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitFPExtInst(FPExtInst &I) {
    IRBuilder<> IRB(&I);
    auto conversion =
            buildRuntimeCall(IRB, runtime.buildFloatToFloat,
                             {{I.getOperand(0), true},
                              {IRB.getInt1(I.getDestTy()->isDoubleTy()), false}});
    registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitFPTruncInst(FPTruncInst &I) {
    IRBuilder<> IRB(&I);
    auto conversion =
            buildRuntimeCall(IRB, runtime.buildFloatToFloat,
                             {{I.getOperand(0), true},
                              {IRB.getInt1(I.getDestTy()->isDoubleTy()), false}});
    registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitFPToSI(FPToSIInst &I) {
  IRBuilder<> IRB(&I);
  auto conversion = buildRuntimeCall(
      IRB, runtime.buildFloatToSignedInt,
      {{I.getOperand(0), true},
       {IRB.getInt8(I.getType()->getIntegerBitWidth()), false}});
  registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitFPToUI(FPToUIInst &I) {
    IRBuilder<> IRB(&I);
    auto conversion = buildRuntimeCall(
            IRB, runtime.buildFloatToSignedInt,
            {{I.getOperand(0), true},
             {IRB.getInt8(I.getType()->getIntegerBitWidth()), false}});
    registerSymbolicComputation(conversion, &I);
}

void Symbolizer::visitCastInst(CastInst &I) {
    auto opcode = I.getOpcode();
    if (opcode != Instruction::SExt && opcode != Instruction::ZExt) {
        errs() << "Warning: unhandled cast instruction " << I << '\n';
        return;
    }

    IRBuilder<> IRB(&I);

    // LLVM bitcode represents Boolean values as i1. In Z3, those are a not a
    // bit-vector sort, so trying to cast one into a bit vector of any length
    // raises an error. The run-time library provides a dedicated conversion
    // function for this case.
    if (I.getSrcTy()->getIntegerBitWidth() == 1) {
        auto boolToBitConversion = buildRuntimeCall(
                IRB, runtime.buildBoolToBits,
                {{I.getOperand(0), true},
                 {IRB.getInt8(I.getDestTy()->getIntegerBitWidth()), false}});
        registerSymbolicComputation(boolToBitConversion, &I);
    } else {
        SymFnT target;

        switch (I.getOpcode()) {
            case Instruction::SExt:
                target = runtime.buildSExt;
                break;
            case Instruction::ZExt:
                target = runtime.buildZExt;
                break;
            default:
                llvm_unreachable("Unknown cast opcode");
        }

        auto symbolicCast =
                buildRuntimeCall(IRB, target,
                                 {{I.getOperand(0), true},
                                  {IRB.getInt8(I.getDestTy()->getIntegerBitWidth() -
                                               I.getSrcTy()->getIntegerBitWidth()),
                                                    false}});
        registerSymbolicComputation(symbolicCast, &I);
    }
}

void Symbolizer::visitPHINode(PHINode &I) {
    // PHI nodes just assign values based on the origin of the last jump, so we
    // assign the corresponding symbolic expression the same way.
    phiNodes.push_back(&I); // to be finalized later, see finalizePHINodes

    IRBuilder<> IRB(&I);
    unsigned numIncomingValues = I.getNumIncomingValues();
    auto *exprPHI = IRB.CreatePHI(isSymType, numIncomingValues);
    auto truePhi = new TruePhi(getNextID());
    assignSymIDPhi(exprPHI, truePhi);
    truePhi2Offset[exprPHI] = truePhiOff++;
    for (unsigned incoming = 0; incoming < numIncomingValues; incoming++) {
        exprPHI->addIncoming(
                // The null pointer will be replaced in finalizePHINodes.
                IRB.getFalse(),
                I.getIncomingBlock(incoming));
    }

    symbolicExpressions[&I] = exprPHI;
}

void Symbolizer::visitInsertValueInst(InsertValueInst &I) {
    //llvm_unreachable("Insert Unsupported.");
    IRBuilder<> IRB(&I);
    auto insert = buildRuntimeCall(
            IRB, runtime.buildInsert,
            {{I.getAggregateOperand(), true},
             {I.getInsertedValueOperand(), true},
             {IRB.getInt64(aggregateMemberOffset(I.getAggregateOperand()->getType(),
                                                 I.getIndices())),
                     false},
             {IRB.getInt8(isLittleEndian(I.getInsertedValueOperand()->getType()) ? 1 : 0), false}});
    registerSymbolicComputation(insert, &I);
}

void Symbolizer::visitExtractValueInst(ExtractValueInst &I) {
    //llvm_unreachable("Extract Unsupported.");
    IRBuilder<> IRB(&I);
    auto extract = buildRuntimeCall(
            IRB, runtime.buildExtract,
            {{I.getAggregateOperand(), true},
             {IRB.getInt64(aggregateMemberOffset(I.getAggregateOperand()->getType(),
                                                 I.getIndices())),
                     false},
             {IRB.getInt64(dataLayout.getTypeStoreSize(I.getType())), false},
             {IRB.getInt8(isLittleEndian(I.getType()) ? 1 : 0), false}});
    registerSymbolicComputation(extract, &I);
}

void Symbolizer::visitSwitchInst(SwitchInst &I) {
    // Switch compares a value against a set of integer constants; duplicate
    // constants are not allowed
    // (https://llvm.org/docs/LangRef.html#switch-instruction).

    IRBuilder<> IRB(&I);
    auto *condition = I.getCondition();
    if(isStaticallyConcrete(condition))
        return;
    auto *conditionExpr = getSymbolicExpression(condition);

    auto conditionExprSymId = getSymIDFromSym(conditionExpr);
    assert(conditionExprSymId > 0);

    BasicBlock* head = I.getParent();
    // Build a check whether we have a symbolic condition, to be used later.
    auto *haveSymbolicCondition = IRB.CreateICmpNE(
            conditionExpr, IRB.getFalse());
    auto *constraintBlock = SplitBlockAndInsertIfThen(haveSymbolicCondition, &I,
            /* unreachable */ false);
    // then -> head
    MapOriginalBlock(constraintBlock->getParent(), head);
    // tail -> head
    MapOriginalBlock(I.getParent(), head);
    // In the constraint block, we push one path constraint per case.

    Instruction* first_constraint = nullptr;
    Instruction* first_concrete_comp = nullptr;
    std::vector<unsigned int> groupedIds;
    for (auto &caseHandle : I.cases()) {
        IRB.SetInsertPoint(constraintBlock);

        auto caseConstraintSymID = getNextID();
#if defined(CO3_REPLACE)
        auto *caseConstraint = IRB.CreateCall(
                runtime.comparisonHandlers[CmpInst::ICMP_EQ],
                {conditionExpr, createValueExpression(caseHandle.getCaseValue(), IRB)});
#else
        auto *caseConstraint = IRB.CreateCall(
                runtime.comparisonHandlers[CmpInst::ICMP_EQ],
                {conditionExpr, createValueExpression(caseHandle.getCaseValue(), IRB), ConstantHelper(symIntType, caseConstraintSymID)});
#endif
        assignSymID(caseConstraint, caseConstraintSymID);

        if(first_concrete_comp == nullptr){
            IRB.SetInsertPoint(&I);
        }else{
            IRB.SetInsertPoint(first_concrete_comp);
        }

        auto *finalExpression = IRB.CreatePHI(isSymType, 2);

        auto falsePhiRoot = new FalsePhiRoot(getNextID(), {conditionExprSymId});
        assignSymIDPhi(finalExpression,falsePhiRoot);

        //symbolicComputation.lastInstruction->replaceAllUsesWith(finalExpression);
        replaceAllUseWith(caseConstraint, finalExpression);

        finalExpression->addIncoming(IRB.getFalse(),head);
        finalExpression->addIncoming(caseConstraint, caseConstraint->getParent());

        if(first_constraint == nullptr){
            IRB.SetInsertPoint(&I);
        }else{
            IRB.SetInsertPoint(first_constraint);
        }

        auto *caseTaken = IRB.CreateICmpEQ(condition, caseHandle.getCaseValue());
        if(first_concrete_comp == nullptr){
            first_concrete_comp = cast<Instruction>(caseTaken);
        }

        IRB.SetInsertPoint(&I);
        auto pushConstraintId = getNextID();
        auto callToPushConstraint = IRB.CreateCall(runtime.pushPathConstraint,
                       {finalExpression, caseTaken, ConstantHelper(symIntType, pushConstraintId)});
        assignSymID(callToPushConstraint, pushConstraintId);
        groupedIds.push_back(pushConstraintId);
        if(first_constraint == nullptr){
            first_constraint = callToPushConstraint;
        }
    }
    addRedirect(groupedIds);
}

void Symbolizer::visitUnreachableInst(UnreachableInst & /*unused*/) {
  // Nothing to do here...
}

void Symbolizer::visitInstruction(Instruction &I) {
  // Some instructions are only used in the context of exception handling, which
  // we ignore for now.
  if (isa<LandingPadInst>(I) || isa<ResumeInst>(I))
    return;

  errs() << "Warning: unknown instruction " << I
         << "; the result will be concretized\n";
}

CallInst *Symbolizer::createValueExpression(Value *V, IRBuilder<> &IRB) {
    auto *valueType = V->getType();
    CallInst* ret = nullptr;
    if (isa<ConstantPointerNull>(V)) {
        auto buildNullPtrSymID = getNextID();
#if defined(CO3_REPLACE)
        ret = IRB.CreateCall(runtime.buildNullPointer, {});
#else
        ret = IRB.CreateCall(runtime.buildNullPointer, {ConstantHelper(symIntType, buildNullPtrSymID)});
#endif
        assignSymID(ret,buildNullPtrSymID);
        return ret;
    }

    if (valueType->isIntegerTy()) {
        auto bits = valueType->getPrimitiveSizeInBits();
        if (bits == 1) {
            // Special case: LLVM uses the type i1 to represent Boolean values, but
            // for Z3 we have to create expressions of a separate sort.
            auto symid = getNextID();
            ret = IRB.CreateCall(runtime.buildBool, {V, ConstantHelper(symIntType, symid)});
            assignSymID(ret,symid);
            return ret;
        } else if (bits <= 64) {
            auto symid = getNextID();
            auto bytes = dataLayout.getTypeAllocSize(valueType);
            ret = IRB.CreateCall(runtime.buildInteger,
                                 {IRB.CreateZExtOrBitCast(V, archIntType),
                                  IRB.getInt8(bytes),
                                  ConstantHelper(symIntType,symid)});
            assignSymID(ret,symid);
            return ret;
        } else {
            // Anything up to the maximum supported 128 bits. Those integers are a bit
            // tricky because the symbolic backends don't support them per se. We have
            // a special function in the run-time library that handles them, usually
            // by assembling expressions from smaller chunks.
            llvm_unreachable("128 bit integer in MCU?");
            auto symid = getNextID();
            ret = IRB.CreateCall(
                    runtime.buildInteger128,
                    {
                            IRB.CreateTrunc(IRB.CreateLShr(V, ConstantInt::get(valueType, 64)),IRB.getInt64Ty()),
                            IRB.CreateTrunc(V, IRB.getInt64Ty())});
            assignSymID(ret,symid);
            return ret;
        }
    }

    if (valueType->isFloatingPointTy()) {
        auto symid = getNextID();
        ret = IRB.CreateCall(runtime.buildFloat,
                             {IRB.CreateFPCast(V, IRB.getDoubleTy()),
                              IRB.getInt1(valueType->isDoubleTy()),
                              ConstantHelper(symIntType,symid)});
        assignSymID(ret, symid);
        return ret;
    }

    if (valueType->isPointerTy()) {
        auto symid = getNextID();
        ret = IRB.CreateCall(
                runtime.buildInteger,
                {IRB.CreatePtrToInt(V, intPtrType), IRB.getInt8(ptrBytes), ConstantHelper(symIntType, symid)});
        assignSymID(ret,symid);
        return ret;
    }

    if (valueType->isStructTy()) {
        // In unoptimized code we may see structures in SSA registers. What we
        // want is a single bit-vector expression describing their contents, but
        // unfortunately we can't take the address of a register. We fix the
        // problem with a hack: we write the register to memory and initialize the
        // expression from there.
        //
        // An alternative would be to change the representation of structures in
        // SSA registers to "shadow structures" that contain one expression per
        // member. However, this would put an additional burden on the handling of
        // cast instructions, because expressions would have to be converted
        // between different representations according to the type.
        if(isa<UndefValue>(V)){
            auto symid = getNextID();
#if defined(CO3_REPLACE)
            ret = IRB.CreateCall(runtime.buildZeroBytes,{ConstantInt::get(intPtrType,dataLayout.getTypeStoreSize(valueType))});
#else
            ret = IRB.CreateCall(runtime.buildZeroBytes,{ConstantInt::get(intPtrType,dataLayout.getTypeStoreSize(valueType)),
                                                         ConstantHelper(symIntType, symid)});
#endif
            assignSymID(ret,symid);
            return ret;
        }else{
            auto *memory = IRB.CreateAlloca(V->getType());
            IRB.CreateStore(V, memory);
            auto symid = getNextID();
            auto intByteSize = dataLayout.getTypeStoreSize(V->getType());
            if(!(intByteSize == 1 || intByteSize == 2 || intByteSize == 4)){
                errs()<<V<<'\n';
                llvm_unreachable("loading values not 1, or 2, or 4 bytes");
            }
            ret = IRB.CreateCall(runtime.readMemory,
                                 {IRB.CreatePtrToInt(memory, intPtrType),
                                  ConstantInt::get(intPtrType,intByteSize),
                                  IRB.getInt8(0),
                                  ConstantHelper(symIntType, symid)});
            assignSymID(ret,symid);
            return ret;
        }

    }
    if(isa<UndefValue>(V)){
        auto undefValSymID = getNextID();
#if defined(CO3_REPLACE)
        ret = IRB.CreateCall(runtime.buildNullPointer, {});
#else
        ret = IRB.CreateCall(runtime.buildNullPointer, {ConstantHelper(symIntType, undefValSymID)});
#endif
        assignSymID(ret,undefValSymID);
        return ret;
        /*
        llvm_unreachable("Undef Value not support, not because itself is hard to support, but the composite value after it. \
        Plus this is most likely caused by some unintialized struct or integer, just give it some initialized value and get rid off this undef");
        */
    }
    errs()<<*V<<'\n';
    llvm_unreachable("Unhandled type for constant expression");
}

Symbolizer::SymbolicComputation
Symbolizer::forceBuildRuntimeCall(IRBuilder<> &IRB, SymFnT function,
                                  ArrayRef<std::pair<Value *, bool>> args) {
    std::vector<Value *> functionArgs;
    for (const auto &[arg, symbolic] : args) {
        functionArgs.push_back(symbolic ? getSymbolicExpression(arg) : arg);
    }
    auto symID = getNextID();


    // some sanity check
    /// check if this function needs report
    auto numArgs = function.getFunctionType()->getNumParams();
    if(isSymIdType(numArgs - 1, function.getCallee()->getName())){
        functionArgs.push_back(ConstantHelper(symIntType,symID));
    }
    auto *call = IRB.CreateCall(function, functionArgs);
    assignSymID(call,symID);
    std::vector<Input> inputs;
    for (unsigned i = 0; i < args.size(); i++) {
        const auto &[arg, symbolic] = args[i];
        if (symbolic){
            inputs.push_back({arg, i, call});
        }
    }

    return SymbolicComputation(call, call, inputs);
}

void Symbolizer::tryAlternative(IRBuilder<> &IRB, Value *V) {
    auto *destSymExpr = getSymbolicExpression(V);
    if (auto tmpExpr = dyn_cast<llvm::ConstantInt>(destSymExpr)) {
        assert(tmpExpr != nullptr );
        if(tmpExpr->isZero() ){
            return;
        }
    }
    // if this V is symbolized
    // create a symbolic constant for this V

    // runtime check for the value

    //auto *nullExpr = IRB.getFalse();
    //auto* symCheck = cast<Instruction>(IRB.CreateICmpNE(nullExpr, destSymExpr));// not null
    auto *symExpr = IRB.getTrue();
    auto* symCheck = cast<Instruction>(IRB.CreateICmpEQ(symExpr, destSymExpr));

    auto * head = symCheck->getParent();

    Instruction* splitBefore = symCheck->getNextNode();
    auto *argExpressionBlock = SplitBlockAndInsertIfThen(
            symCheck,splitBefore,false);

    MapOriginalBlock(argExpressionBlock->getParent(), head );
    MapOriginalBlock(splitBefore->getParent(), head );

    //save the insert point
    auto savedInsertPoint = IRB.GetInsertPoint();
    IRB.SetInsertPoint(argExpressionBlock);

    auto *deskConcExpr = createValueExpression(V, IRB);
    unsigned tryAlternativeSymID = getNextID();
    auto *destAssertion = IRB.CreateCall(runtime.tryAlternative,{destSymExpr,deskConcExpr ,ConstantHelper(symIntType, tryAlternativeSymID)});
    assignSymID(destAssertion,tryAlternativeSymID);
    unsigned tryAlternativeBBID = GetBBID(IRB.GetInsertBlock());
    // push the info
    tryAlternatives.insert(new TryAlternativeUnit(tryAlternativeSymID, tryAlternativeBBID, destSymExpr, V));

    //restore the insert point
    IRB.SetInsertPoint(&*savedInsertPoint);
}

uint64_t Symbolizer::aggregateMemberOffset(Type *aggregateType,
                                           ArrayRef<unsigned> indices) const {
    uint64_t offset = 0;
    auto *indexedType = aggregateType;
    for (auto index : indices) {
        // All indices in an extractvalue instruction are constant:
        // https://llvm.org/docs/LangRef.html#extractvalue-instruction

        if (auto *structType = dyn_cast<StructType>(indexedType)) {
            offset += dataLayout.getStructLayout(structType)->getElementOffset(index);
            indexedType = structType->getElementType(index);
        } else {
            auto *arrayType = cast<ArrayType>(indexedType);
            unsigned elementSize =
                    dataLayout.getTypeAllocSize(arrayType->getArrayElementType());
            offset += elementSize * index;
            indexedType = arrayType->getArrayElementType();
        }
    }
    return offset;
}
unsigned int getTypeWidth(Type* ty, const DataLayout& dataLayout){
    unsigned int width = 0;
    if(PointerType * ptrType = dyn_cast<PointerType>(ty)){
        width = dataLayout.getPointerTypeSize(ptrType);
    }
    else{
        width =  dataLayout.getTypeAllocSize(ty);
    }
    return width;
}

void Symbolizer::insertPerBBConcretenessCheck(unsigned BBID, llvm::Value * checking){
    if(perBBConcretenessChecking.find(BBID) != perBBConcretenessChecking.end()){
        perBBConcretenessChecking.at(BBID).push_back(checking);
    }else{
        perBBConcretenessChecking[BBID] = std::vector<Value*> {checking};
    }
}
void Symbolizer::shortCircuitExpressionUses() {
    for (auto &symbolicComputation : expressionUses) {
        assert(!symbolicComputation.inputs.empty() &&
               "Symbolic computation has no inputs");

        IRBuilder<> IRB(symbolicComputation.firstInstruction);
        // Build the check whether any input expression is non-null (i.e., there
        // is a symbolic input).
        auto *nullExpression = IRB.getFalse();
        std::vector<Value *> nullChecks;
        for (const auto &input : symbolicComputation.inputs) {
            nullChecks.push_back(IRB.CreateICmpEQ(nullExpression, input.getSymbolicOperand()));
        }
        auto *allConcrete = nullChecks[0];
        for (unsigned argIndex = 1; argIndex < nullChecks.size(); argIndex++) {
            allConcrete = IRB.CreateAnd(allConcrete, nullChecks[argIndex]);
        }


        // The main branch: if we don't enter here, we can short-circuit the
        // symbolic computation. Otherwise, we need to check all input expressions
        // and create an output expression.
        auto *head = symbolicComputation.firstInstruction->getParent();


        unsigned headBBID = GetBBID(head);
        insertPerBBConcretenessCheck(headBBID, allConcrete);

        auto *slowPath = SplitBlock(head, symbolicComputation.firstInstruction);
        MapOriginalBlock(slowPath, head);
        auto *tail = SplitBlock(slowPath,
                                symbolicComputation.lastInstruction->getNextNode());
        MapOriginalBlock(tail, head);
        ReplaceInstWithInst(head->getTerminator(),
                            BranchInst::Create(tail, slowPath, allConcrete));

        // In the slow case, we need to check each input expression for null
        // (i.e., the input is concrete) and create an expression from the
        // concrete value if necessary.
        auto numUnknownConcreteness = std::count_if(
                symbolicComputation.inputs.begin(), symbolicComputation.inputs.end(),
                [&](const Input &input) {
                    return (input.getSymbolicOperand() != nullExpression);
                });
        std::set<unsigned> falsePhiLeavesSymIDs;
        std::set<unsigned> peerOriginalSymIDs;
        std::set<FalsePhiLeaf*> falsePhiPeers;


        for (unsigned argIndex = 0; argIndex < symbolicComputation.inputs.size();argIndex++) {
            auto &argument = symbolicComputation.inputs[argIndex];
            auto *originalArgExpression = argument.getSymbolicOperand();

            auto *argCheckBlock = symbolicComputation.firstInstruction->getParent();

            // We only need a run-time check for concreteness if the argument isn't
            // known to be concrete at compile time already. However, there is one
            // exception: if the computation only has a single argument of unknown
            // concreteness, then we know that it must be symbolic since we ended up
            // in the slow path. Therefore, we can skip expression generation in
            // that case.
            bool needRuntimeCheck = originalArgExpression != nullExpression;
            auto originalArgSymID = getSymIDFromSym(originalArgExpression);
            if (needRuntimeCheck && (numUnknownConcreteness == 1)){
                if(originalArgSymID == 0){
                    errs() <<"concrete:"<< *argument.concreteValue<<'\n';
                    errs()<<"sym:"<<*originalArgExpression <<'\n';
                    llvm_unreachable("some symexpr needs runtime checking and its symid is 0");
                }
                falsePhiLeavesSymIDs.insert(originalArgSymID);
                continue;
            }

            if (needRuntimeCheck) {
                auto *argExpressionBlock = SplitBlockAndInsertIfThen(
                        nullChecks[argIndex], symbolicComputation.firstInstruction,
                        /* unreachable */ false);
                MapOriginalBlock(argExpressionBlock->getParent(), head );
                MapOriginalBlock(symbolicComputation.firstInstruction->getParent(), head );
                IRB.SetInsertPoint(argExpressionBlock);
            } else {
                IRB.SetInsertPoint(symbolicComputation.firstInstruction);
            }

            auto *newArgExpression =
                    createValueExpression(argument.concreteValue, IRB);

            Value *finalArgExpression;
            if (needRuntimeCheck) {
                IRB.SetInsertPoint(symbolicComputation.firstInstruction);
                auto *argPHI = IRB.CreatePHI(isSymType, 2);
                unsigned cur_phi_symid =  getNextID();

                falsePhiLeavesSymIDs.insert(cur_phi_symid);
                auto cur_falsePhiLeaf = new FalsePhiLeaf(cur_phi_symid, originalArgSymID);
                falsePhiPeers.insert(cur_falsePhiLeaf);
                peerOriginalSymIDs.insert(originalArgSymID);
                assignSymIDPhi(argPHI, cur_falsePhiLeaf);

                argPHI->addIncoming(originalArgExpression, argCheckBlock);
                argPHI->addIncoming(newArgExpression, newArgExpression->getParent());
                finalArgExpression = argPHI;
            } else {
                finalArgExpression = newArgExpression;
            }

            argument.replaceOperand(finalArgExpression);
        }

        // Finally, the overall result (if the computation produces one) is null
        // if we've taken the fast path and the symbolic expression computed above
        // if short-circuiting wasn't possible.

        if(falsePhiLeavesSymIDs.size() == 0 || falsePhiLeavesSymIDs.size() == 1){
            assert(peerOriginalSymIDs.size() == 0);
        }else{
            // there can be 1 function accepting 2 parameters that are the same original value
            // in this case, there is only one original value, but 2 different faslePhiLeaf will be created
            if(falsePhiLeavesSymIDs.size() != peerOriginalSymIDs.size()){
                errs()<<symbolicComputation<<'\n';
                errs()<<" same value feeding into different parameters\n";
            }
            for(auto eachPeer1 : falsePhiPeers){
                for(auto orig : peerOriginalSymIDs){
                    eachPeer1->InsertPeer(orig);
                }
            }
        }


        if (dyn_cast<CallInst>(symbolicComputation.lastInstruction)->getCalledFunction()->getReturnType() != voidType ) {
            IRB.SetInsertPoint(&tail->front());
            //errs()<<"before creating:"<<*tail<<'\n';
            auto *finalExpression = IRB.CreatePHI(isSymType, 2);
            //errs()<<"after creating:"<<*tail<<'\n';
            if(falsePhiLeavesSymIDs.size() == 0){
                //this simply means this is based on all constants, and this phiRootNode functionally equals false
                assert(numUnknownConcreteness == 0);
            }
            //symbolicComputation.lastInstruction->replaceAllUsesWith(finalExpression);
            replaceAllUseWith(symbolicComputation.lastInstruction, finalExpression);
            finalExpression->addIncoming(IRB.getFalse(),
                                         head);
            finalExpression->addIncoming(
                    symbolicComputation.lastInstruction,
                    symbolicComputation.lastInstruction->getParent());
            auto falsePhiRoot = new FalsePhiRoot(getNextID(), falsePhiLeavesSymIDs);
            assignSymIDPhi(finalExpression,falsePhiRoot);
        }

    }
}

SymDepGraph::vertex_t Symbolizer::addConstantIntVertice(ConstantInt* cont_int){
    unsigned int conWidth = dataLayout.getTypeAllocSize(cont_int->getType());
    int64_t contValue = cont_int->getSExtValue();
    // constant's BBID is the same with its user(maybe we can merge?)
    auto conVert = g.AddConstVertice(NodeConstInt, contValue, conWidth);
    return conVert;
}

SymDepGraph::vertex_t Symbolizer::addConstantFloatVertice(llvm::ConstantFP * const_fp) {
    unsigned int conWidth = dataLayout.getTypeAllocSize(const_fp->getType());
    Type * arg_type = const_fp->getType();
    double contValue = const_fp->getValueAPF().convertToDouble();
    bool isDouble = true;
    if(arg_type->isDoubleTy()){
        isDouble = true;
    }else if(arg_type->isFloatTy()){
        isDouble = false;
    }else{
        errs()<<*const_fp << "\n" << * arg_type<<'\n';
        llvm_unreachable("constant float is neither float ty nor double ty");
    }
    auto conVert = g.AddConstVertice( isDouble ? NodeConstDouble : NodeConstFloat ,contValue, conWidth);
    return conVert;
}

SymDepGraph::vertex_t Symbolizer::addRuntimeVertice(llvm::Value * v, unsigned bid) {
    Type* ty = v->getType();
    unsigned int width = getTypeWidth(ty,dataLayout);
    std::string vertType;
    if(ty->isIntegerTy()){
        vertType = NodeRuntimeInt;
    }else if(ty->isFloatTy()){
        vertType = NodeRuntimeFloat;
    }else if(ty->isDoubleTy()){
        vertType = NodeRuntimeDouble;
    }else if(ty->isPointerTy()){
        vertType = NodeRuntimePtr;
    }else{
        errs()<< * v<<'\n';
        errs() << * ty<<'\n';
        llvm_unreachable("unhandled runtime val type");
    }
    auto vert = g.AddRuntimeVertice(vertType, width, bid);
    return vert;
}
/*
void Symbolizer::addTryAlternativeToTheGraph(){
    for(auto eachTryAlternative: tryAlternatives){
        unsigned tryAlterntiveSymID = eachTryAlternative.first.first;
        unsigned tryAlternativeBBID = eachTryAlternative.first.second;
        auto tryAlternativeVertice = g.AddSymVertice(tryAlterntiveSymID,"_sym_try_alternative", tryAlternativeBBID);

        unsigned operandSymID = getSymIDFromSym(eachTryAlternative.second.first);
        assert(operandSymID != 0);// we've already passed this
        auto symOperandVertice = g.GetVerticeBySymID(operandSymID);
        g.AddEdge(*symOperandVertice,tryAlternativeVertice, 0);
        Value* concreteVal = eachTryAlternative.second.second;
        if(isa<Constant>(concreteVal)){
            if(ConstantInt * cont_int = dyn_cast<ConstantInt>(concreteVal)){
                auto conVert = addConstantIntVertice(cont_int);
                g.AddEdge(conVert,tryAlternativeVertice, 1);
            }else if(ConstantFP* const_fp = dyn_cast<ConstantFP>(concreteVal)){
                auto conVert = addConstantFloatVertice(const_fp);
                g.AddEdge(conVert,tryAlternativeVertice, 1);
            }else{
                errs()<<*concreteVal<<'\n';
                llvm_unreachable("unhandled constant");
            }
        }else{
            unsigned runtimeValueBBID = 0;
            if(isa<Argument>(concreteVal)){
                runtimeValueBBID = initialBBID;
            }else if(isa<Instruction>(concreteVal)){
                runtimeValueBBID = GetBBID(cast<Instruction>(concreteVal)->getParent());
            }else{
                errs()<<"argid:"<<1<<'\n';
                errs()<<"arg:"<<*concreteVal<<'\n';
                llvm_unreachable("runtime value is neither a parameter nor an instruction. \n");
            }
            auto runtimeVert = addRuntimeVertice(concreteVal,runtimeValueBBID);
            g.AddEdge(runtimeVert,tryAlternativeVertice,1);
        }
    }
}*/

void Symbolizer::createDFGAndReplace(llvm::Function& F, std::string filename){
    std::set<StringRef> symOperators;
    for(auto eachSymOperation: runtime.SymOperators){
        symOperators.insert(eachSymOperation->getCallee()->getName());
    }
    for(auto eachSymOperation:runtime.comparisonHandlers){
        if(eachSymOperation.getCallee() == nullptr)
            continue;
        symOperators.insert(eachSymOperation.getCallee()->getName());
    }
    for(auto eachSymOperation:runtime.binaryOperatorHandlers){
        if(eachSymOperation.getCallee() == nullptr)
            continue;
        symOperators.insert(eachSymOperation.getCallee()->getName());
    }
    for(auto eachInterpretedFunction: interpretedFunctionNames){
        symOperators.insert(eachInterpretedFunction);
    }

    std::vector<CallInst*> toReplaceToNone;
    std::vector<CallInst*> toReplaceToFalse;
    std::vector<CallInst*> toReplaceToTrue;
    std::vector<CallInst*> toReplaceToInput;
    std::vector<CallInst*> toReplaceToOr;
    for (auto &basicBlock : F){
        unsigned blockID = GetBBID(&basicBlock);
        for(auto & eachInst : basicBlock){
            if (PHINode *symPhi = dyn_cast<PHINode>(&eachInst); symPhi != nullptr && phiSymbolicIDs.find(symPhi) != phiSymbolicIDs.end()) {
                unsigned userSymID = getSymIDFromSym(symPhi);
                PhiStatus* cur_phi_status = phiSymbolicIDs.find(symPhi)->second;
                if(isa<TruePhi>(cur_phi_status)){
                    g.AddPhiVertice(NodeTruePhi,userSymID, blockID );
                }else if(isa<FalsePhiRoot>(cur_phi_status)){
                    g.AddPhiVertice(NodeFalseRootPhi,userSymID, blockID );
                }else{
                    g.AddPhiVertice(NodeFalseLeafPhi, userSymID, blockID);
                }
            }
            if(!isa<CallInst>(&eachInst)){
                continue;
            }
            auto callInst = cast<CallInst>(&eachInst);
            auto callee = callInst->getCalledFunction();
            // check if indirect call
            if(callee == nullptr)
                continue;
            auto calleeName = callee->getName();
            IRBuilder<> IRB(callInst);
            if(symOperators.find(calleeName) != symOperators.end()){
                // some special case
                if(calleeName.equals("_sym_notify_basic_block") || calleeName.equals("_sym_notify_ret") ){
                    continue;
                }
                unsigned userSymID = getSymIDFromSym(callInst);
                SymDepGraph::vertex_t userNode;
                unsigned stageSetting;
                unsigned reditect;
                if(std::find(stageSettingOperations.begin(), stageSettingOperations.end(),userSymID) != stageSettingOperations.end()){
                    stageSetting = 1;
                }else{
                    stageSetting = 0;
                }
                if(symIdRedirect.find(userSymID) != symIdRedirect.end()){
                    reditect = symIdRedirect.at(userSymID);
                }else{
                    reditect = 0;
                }
                userNode = g.AddSymVertice(userSymID, calleeName.str(),blockID, stageSetting,reditect);
                if(calleeName.equals("_sym_notify_call")){
                    auto conVert = addConstantIntVertice(callToCallId.at(callInst));
                    g.AddEdge(conVert,userNode, 0);
                    unsigned counter = 1;
                    if(callToSetParaMap.find(callInst) != callToSetParaMap.end()){
                        for(auto eachSetPara : callToSetParaMap.at(callInst)){
                            unsigned arg_symid = getSymIDFromSym(eachSetPara);
                            g.AddEdge(arg_symid,userSymID, counter++);//this is just some random counter, which won't be used anyway
                        }
                    }
                    continue;
                }

                for(auto arg_it = callInst->arg_begin() ; arg_it != callInst->arg_end() ; arg_it++){
                    Value * arg = *arg_it ;
                    unsigned arg_idx = callInst->getArgOperandNo(arg_it);
                    if(isSymStatusType(arg_idx, calleeName)){
                        unsigned arg_symid = getSymIDFromSym(arg);
                        g.AddEdge(arg_symid,userSymID, arg_idx);
                    }else if(isConstantType(arg_idx, calleeName)){
                        if(!isa<Constant>(arg)){
                            errs()<<"callinst:"<<*callInst<<'\n';
                            errs()<<"argid:"<<arg_idx<<'\n';
                            errs()<<"arg:"<<*arg<<'\n';
                            llvm_unreachable("annotated constant is not constant\n");
                        }
                        if(calleeName.equals("_sym_build_read_memory") && arg_idx == 1){
                            auto byteLen = dyn_cast<ConstantInt>(arg)->getZExtValue();
                            if(!(byteLen == 1 || byteLen == 2 || byteLen == 4)){
                                assert(byteLen == 8);
#if defined (CO3_MCUS)
                                errs()<< *callInst<<'\n';
                                llvm_unreachable("loading value not 1, 2 or 4 bytes?");
#endif
                            }
                        }
                        if(ConstantInt * cont_int = dyn_cast<ConstantInt>(arg)){
                            auto conVert = addConstantIntVertice(cont_int);
                            g.AddEdge(conVert,userNode, arg_idx);
                        }else if(ConstantFP* const_fp = dyn_cast<ConstantFP>(arg)){
                            auto conVert = addConstantFloatVertice(const_fp);
                            g.AddEdge(conVert,userNode, arg_idx);
                        }else{
                            errs()<<*arg<<'\n';
                            llvm_unreachable("unhandled constant");
                        }
                    }else if(isRuntimeType(arg_idx, calleeName)){
                        if(isa<Constant>(arg)){
                            // even if it's annotated as runtime val, it still can be constant.
                            if (calleeName.equals("_sym_build_integer") && arg_idx == 0 ){
                                toReplaceToTrue.push_back(callInst);
                            }
                            else if( calleeName.equals("_sym_build_float") && arg_idx == 0 ){
                                //toReplaceToFalse.push_back(callInst);
                            }
                            else if(calleeName.equals("_sym_build_bool") && arg_idx == 0){
                                toReplaceToTrue.push_back(callInst);
                            }else if(calleeName.equals("_sym_build_memset") && arg_idx == 2){
                                // this is also allowed, but since the mem ptr is not a constant, this is not to be replaced
                            }else if(calleeName.equals("_sym_build_memcpy") && arg_idx == 2){

                            }
                            else{
                                errs()<<"callinst:"<<*callInst<<'\n';
                                errs()<<"argid:"<<arg_idx<<'\n';
                                errs()<<"arg:"<<*arg<<'\n';
                                llvm_unreachable("annotated runtime is unexpectedly a constant. \n");
                            }
                            if(ConstantInt * cont_int = dyn_cast<ConstantInt>(arg)){
                                auto conVert = addConstantIntVertice(cont_int);
                                g.AddEdge(conVert,userNode, arg_idx);
                            }else if(ConstantFP* const_fp = dyn_cast<ConstantFP>(arg)){
                                auto conVert = addConstantFloatVertice(const_fp);
                                g.AddEdge(conVert,userNode, arg_idx);
                            }else{
                                errs()<<*arg<<'\n';
                                errs()<<*arg->getType()<<'\n';
                                llvm_unreachable("unhandled constant");
                            }

                        }else{
                            Type* val_type = arg->getType();
                            unsigned runtimeValueBBID = 0;
                            if(isa<Argument>(arg)){
                                runtimeValueBBID = initialBBID;
                            }else if(isa<Instruction>(arg)){
                                runtimeValueBBID = GetBBID(cast<Instruction>(arg)->getParent());
                            }else{
                                errs()<<"callinst:"<<*callInst<<'\n';
                                errs()<<"argid:"<<arg_idx<<'\n';
                                errs()<<"arg:"<<*arg<<'\n';
                                llvm_unreachable("runtime value is neither a parameter nor an instruction. \n");
                            }
                            if( !val_type->isIntegerTy() && !val_type->isDoubleTy() && !val_type->isFloatTy() && !val_type->isPointerTy()){
                                errs()<<"callInst"<< *callInst<<'\n';
                                errs()<<arg_idx <<"th parameter\n";
                                errs()<< * arg <<'\n' << *val_type<<'\n';
                                llvm_unreachable("type unhandled");
                            }
                            auto runtimeVert = addRuntimeVertice(arg,runtimeValueBBID);
                            g.AddEdge(runtimeVert,userNode,arg_idx);
                            //sanity check
                            if(find(runtime.replaceToNone.begin(), runtime.replaceToNone.end(), calleeName) == runtime.replaceToNone.end()){
                                if(callInst->getOperand(callInst->arg_size() - 1)->getType() != symIntType){
                                    errs()<<"callInst"<< *callInst<<'\n';
                                    errs()<<arg_idx <<"th parameter\n";
                                    llvm_unreachable("last para is not symInt");
                                }
                            }
                        }
                    }
                    else if(isSymIdType(arg_idx, calleeName)){
                        // no nothing
                    }else if(isSkippedArg(arg_idx, calleeName)){
                        // do nothing
                    }
                    else{
                        errs()<<"callinst:"<<*callInst<<'\n';
                        errs()<<"argid:"<<arg_idx<<'\n';
                        errs()<<"arg:"<<*arg<<'\n';
                        llvm_unreachable("un-annoated arg");
                    }
                }
                if(find(runtime.replaceToNone.begin(), runtime.replaceToNone.end(), calleeName) != runtime.replaceToNone.end()){
                    toReplaceToNone.push_back(callInst);
                }else if(find(runtime.replaceToFalse.begin(), runtime.replaceToFalse.end(), calleeName) != runtime.replaceToFalse.end()){
                    toReplaceToFalse.push_back(callInst);
                }else if(find(runtime.replaceToTrue.begin(), runtime.replaceToTrue.end(), calleeName) != runtime.replaceToTrue.end()){
                    toReplaceToTrue.push_back(callInst);
                }else if(find(runtime.replaceToInput.begin(), runtime.replaceToInput.end(), calleeName) != runtime.replaceToInput.end()){
                    toReplaceToInput.push_back(callInst);
                }else if(find(runtime.replaceToLogicOr.begin(), runtime.replaceToLogicOr.end(), calleeName) != runtime.replaceToLogicOr.end()){
                    toReplaceToOr.push_back(callInst);
                }
            }
        }
    }
    //addTryAlternativeToTheGraph();
    //finish phi nodes
    for (auto &basicBlock : F){
        for(auto & eachInst : basicBlock) {
            if (PHINode *symPhi = dyn_cast<PHINode>(&eachInst); symPhi != nullptr &&
                                                                phiSymbolicIDs.find(symPhi) != phiSymbolicIDs.end()) {

                PHINode* reportingPhi = nullptr;
                unsigned numIncomingValue = symPhi->getNumIncomingValues();
                unsigned userSymID = getSymIDFromSymPhi(symPhi);
                auto phi_status = phiSymbolicIDs.find(symPhi)->second;
                if(isa<TruePhi>(phi_status)){
                    IRBuilder<> IRB(&*symPhi->getParent()->getFirstInsertionPt());
                    reportingPhi = IRB.CreatePHI(IRB.getInt8Ty(), numIncomingValue);
                    auto truePhiOffset = truePhi2Offset.at(symPhi);
                    IRB.CreateCall(runtime.notifyPhi,
                                   {reportingPhi, ConstantHelper(symIntType, userSymID),symPhi, truePhiBaseAddr,ConstantHelper(IRB.getInt8Ty(), truePhiOffset) });
                }else if(auto falsePhiRoot = dyn_cast<FalsePhiRoot>(phi_status)){
                    for(auto eachFalseLeave: falsePhiRoot->leaves){
                        g.AddPhiEdge(eachFalseLeave, userSymID, 2 , 0, 1);
                        assert(numIncomingValue == 2);
                    }
                }else{
                    auto falsePhiLeaf = dyn_cast<FalsePhiLeaf>(phi_status);
                    assert(falsePhiLeaf != nullptr);
                    assert(numIncomingValue == 2);
                    for(auto eachPeer : falsePhiLeaf->peersOriginal){
                        g.AddPhiEdge(eachPeer, userSymID, 2 , 0, 1);
                    }
                }
                std::map<BasicBlock*, unsigned> valuesToSet;
                unsigned counter = 0;
                for (unsigned incoming = 0, totalIncoming =numIncomingValue;incoming < totalIncoming; incoming++) {
                    BasicBlock *incomingBB = symPhi->getIncomingBlock(incoming);
                    unsigned incomingBBID = GetBBID(incomingBB);
                    Value *incomingValue = symPhi->getIncomingValue(incoming);
                    if(valuesToSet.find(incomingBB) == valuesToSet.end()){
                        valuesToSet[incomingBB] = counter++;
                    }
                    unsigned valueToSet = valuesToSet.at(incomingBB);
                    if(isa<TruePhi>(phi_status)){
                        reportingPhi->addIncoming(
                                ConstantHelper(int8Type, valueToSet),
                                incomingBB);
                    }

                    unsigned incomingValueSymID = getSymIDFromSym(incomingValue);
                    //assert(incomingValueSymID != 0); // incoming symid could be zero
                    // add incoming BBID as edge property just to double check
                    g.AddPhiEdge(incomingValueSymID, userSymID,valueToSet, incomingBBID, 0);
                }
            }
        }
    }
#if defined(CO3_REPLACE)
    for(auto eachToBeRemoved: toReplaceToNone){
        eachToBeRemoved->eraseFromParent();
    }
    for(auto eachToReplaceToFalse : toReplaceToFalse){

        replaceAllUseWith(eachToReplaceToFalse, ConstantInt::getFalse(eachToReplaceToFalse->getContext()));
        eachToReplaceToFalse->eraseFromParent();
    }
    for(auto eachToReplaceToTrue : toReplaceToTrue){
        replaceAllUseWith(eachToReplaceToTrue, ConstantInt::getTrue(eachToReplaceToTrue->getContext()));
        eachToReplaceToTrue->eraseFromParent();
    }
    for(auto eachToReplaceToInput : toReplaceToInput){
        // although different functions in this category has different number of parameters
        // but the input is always the first parameter, and that is the only thing that matters
        assert(isSymStatusType(0,eachToReplaceToInput->getCalledFunction()->getName()));
        Value * input = eachToReplaceToInput->getArgOperand(0);
        replaceAllUseWith(eachToReplaceToInput, input);
        eachToReplaceToInput->eraseFromParent();
    }
    for(auto eachToReplaceWithOr: toReplaceToOr){
        // although different functions in this category has different number of parameters
        // but the inputs are always the first and the 2nd parameter, and that is the only thing that matters
        Value * input1 = eachToReplaceWithOr->getArgOperand(0);
        assert(isSymStatusType(0,eachToReplaceWithOr->getCalledFunction()->getName()));
        Value * input2 = eachToReplaceWithOr->getArgOperand(1);
        assert(isSymStatusType(1,eachToReplaceWithOr->getCalledFunction()->getName()));
        IRBuilder<> IRB(eachToReplaceWithOr);
        Value * orExpression = IRB.CreateOr(input1, input2);
        replaceAllUseWith(eachToReplaceWithOr, orExpression);
        eachToReplaceWithOr->eraseFromParent();
    }
#endif

    g.writeToFile(filename);
    // Replacing all uses has fixed uses of the symbolic PHI nodes in existing
    // code, but the nodes may still be referenced via symbolicExpressions. We
    // therefore invalidate symbolicExpressions, meaning that it cannot be used
    // after this point.
    symbolicExpressions.clear();
}
/*
void JustCuriousNoBackEdge(std::set<BasicBlock*> & bbs){

}*/

BasicBlock* Symbolizer::findExistingBB(unsigned bbid, std::set<BasicBlock*> & bbs){
    //errs() << "start\n";
    // I'm just curious if there will be any back edge
    std::set<BasicBlock*> existingBBs;
    for(auto curBB : bbs){
        std::queue<BasicBlock*> workQ;
        workQ.push(curBB);
        std::set<BasicBlock*>visitedNode;
        //errs()<<"working on:"<<curBB->getName()<<'\n';
        while(!workQ.empty()){
            BasicBlock* work_bb = workQ.front();
            workQ.pop();
            if (succ_empty(work_bb)){
                existingBBs.insert(work_bb);
            }
            for (BasicBlock *SuccBB : successors(work_bb)){
                unsigned succ_id = GetBBID(SuccBB);
                if(succ_id != bbid){
                    // this work bb is the point where these whole set of BBs exist to other group of BB
                    existingBBs.insert(work_bb);
                }else{
                    //errs() << work_bb->getName()<<"->"<<SuccBB->getName()<<'\n';
                    if(visitedNode.find(SuccBB) == visitedNode.end()){
                        workQ.push(SuccBB);
                    }
                }
            }
            visitedNode.insert(work_bb);
        }
    }
    assert(existingBBs.size() == 1);
    return *existingBBs.begin();
}

void Symbolizer::outputDebugCFG(llvm::Function * F) {
    std::string buffer_str;
    std::error_code error;
    raw_string_ostream rso(buffer_str);
    StringRef name("debugcfg.dot");
    raw_fd_ostream file(name, error);

    file << "digraph \"CFG for'" + F->getName() + "\' function\" {\n";

    for(auto & cur_bb : *F){
        BasicBlock* curBB = &cur_bb;
        unsigned cur_bbid = GetBBID(curBB);
        uintptr_t from_num = reinterpret_cast<std::uintptr_t>(curBB);
        file << "\t" << from_num << " [shape=record, label=\"";
        file  << curBB->getName()<<"|"<< cur_bbid << "\"]\n";
        for (BasicBlock *SuccBB : successors(curBB)){
            uintptr_t to_num = reinterpret_cast<std::uintptr_t>(SuccBB);
            file << "\t" << from_num<< "-> " << to_num << ";\n";
        }
    }
    file<<"}\n";
}
void Symbolizer::insertNotifyBasicBlock(Function& F) {
    std::map<unsigned, std::set<BasicBlock*> > splittedBBs;
    // group these BBs according to their ID
    for(auto& eachBB : F){
        BasicBlock* cur_bb = &eachBB;
        int bbid = GetBBID(cur_bb);
        if(splittedBBs.find(bbid) == splittedBBs.end()){
            splittedBBs[bbid] = std::set<BasicBlock*>{cur_bb};
        }else{
            splittedBBs[bbid].insert(cur_bb);
        }
    }

    // for each group of BBs, if the original BB is in the loop, add notify BB to the loop
    for(auto eachGroupOfBBs : splittedBBs){
        unsigned original_bbid = eachGroupOfBBs.first;
        BasicBlock* original_bb = nullptr;
        // find the original BasicBlock
        for(auto eachOriginalBB : originalBB2ID){
            if(eachOriginalBB.second == original_bbid){
                original_bb = eachOriginalBB.first;
                break;
            }
        }
        assert(original_bb != nullptr);
        if(loopinfo.getLoopFor(original_bb) != nullptr){

            if(perBBConcretenessChecking.find(original_bbid) == perBBConcretenessChecking.end()){
                // there is no registered Symbolic Computation in this loop BB, we don't have to insert the BB.
                // the unregister read_mem, write_mem e.t.c., will report themselves
                continue;
            }

            BasicBlock* existing_bb = findExistingBB(original_bbid, eachGroupOfBBs.second);
            Instruction* terminator = existing_bb->getTerminator();
            IRBuilder<> irb(terminator);

            auto& checkings = perBBConcretenessChecking.find(original_bbid)->second;
            Value* allAllConcrete = checkings[0];
            unsigned index = 1;
            for(; index < checkings.size() ; index++){
                allAllConcrete = irb.CreateAnd(allAllConcrete, checkings[index]);
            }
            Value * anySym = irb.CreateNot(allAllConcrete);
            llvm::ConstantInt * bbid = ConstantInt::get(irb.getInt16Ty(), original_bbid);

            auto offset = loopBB2Offset.at(original_bb);

            irb.CreateCall(runtime.notifyBasicBlock,{bbid,anySym,loopBBBaseAddr,ConstantHelper(irb.getInt8Ty(), offset) } );
        }
    }
}
void Symbolizer::insertNotifyFunc(llvm::Function& F, std::string file_name){

    std::string func_name = F.getName().str();
    std::string line;
    std::ifstream funcIDFileR (file_name);
    unsigned id = 0;
    assert(funcIDFileR.is_open());
    bool dup = false;
    while ( getline (funcIDFileR,line) )
    {
        size_t tab_pos = line.find('\t');
        assert(tab_pos != std::string::npos);
        std::string cur_funcname =  line.substr(0, tab_pos);
        if(cur_funcname == func_name){
            dup = true;
        }
        unsigned cur_id = stoi(line.substr(tab_pos + 1));
        if(cur_id >= id){
            id = cur_id + 1;
        }
    }
    funcIDFileR.close();

    std::ofstream funcIDFileW;
    if(dup){
        funcIDFileW.open(file_name,std::ios_base::out);
        id = 0;
    }else{
        funcIDFileW.open(file_name,std::ios_base::app);
    }

    IRBuilder<> IRB(&*(F.getEntryBlock()).getFirstInsertionPt());
    if(id > 255){
        llvm_unreachable("total number of functions exceeds 255");
    }
    IRB.CreateCall(runtime.notifyFunc, ConstantHelper(IRB.getInt8Ty(),id));

    assert(funcIDFileW.is_open());
    funcIDFileW << func_name <<'\t' << id<<'\n';
    funcIDFileW.close();
}

#if defined(CO3_MCU)
#else
void Symbolizer::ProEpiLogue(llvm::Function & F){
    assert(return_inst != nullptr);
    IRBuilder<> IRB(&*(F.getEntryBlock().getFirstInsertionPt()));
    IRB.CreateCall(runtime.symInit);
    IRB.SetInsertPoint(return_inst);
    IRB.CreateCall(runtime.symEnd);
}
#endif

void Symbolizer::RecursivePrintEdges(std::map<BasicBlock*, unsigned long>& basicBlockMap, raw_fd_ostream & O, DomTreeNodeBase<BasicBlock> * root, unsigned level){
    unsigned cur_bbid = basicBlockMap.at(root->getBlock());
    bool isLoop = loopinfo.getLoopFor(root->getBlock()) != nullptr ? true : false;
    O << std::string(level, '\t') << cur_bbid << " [shape=record, label=\"";
    O  << cur_bbid << "\",id=" << cur_bbid;
    O <<",level="<< level;

    if(isLoop){
        O <<",color=red";
    }
    O << "];\n";
    for (auto I = root->begin(),E = root->end(); I != E; ++I){
        unsigned child_bbid = basicBlockMap.at((*I)->getBlock());
        O << std::string(level, '\t') << cur_bbid <<" -> " << child_bbid<<";\n";
        RecursivePrintEdges(basicBlockMap, O, *I, level + 1);
    }
}

void Symbolizer::OutputCFG(llvm::Function & F, DominatorTree& dTree, PostDominatorTree& pdTree, std::string cfg_file,std::string dom_file, std::string postDom_file) {
    std::string buffer_str;
    std::error_code error;
    raw_string_ostream rso(buffer_str);
    StringRef name(cfg_file);
    raw_fd_ostream file(name, error);

    std::string d_buffer_str;
    raw_string_ostream d_rso(d_buffer_str);
    StringRef d_name(dom_file);
    raw_fd_ostream d_file(d_name, error);

    std::string pd_buffer_str;
    raw_string_ostream pd_rso(pd_buffer_str);
    StringRef pd_name(postDom_file);
    raw_fd_ostream pd_file(pd_name, error);



    std::map<BasicBlock*, unsigned long> basicBlockMap;
    for (Function::iterator B_iter = F.begin(); B_iter != F.end(); ++B_iter){
        unsigned long blockID = GetBBID(&*B_iter);
        basicBlockMap.insert(std::make_pair(&*B_iter,blockID));
    }
    file << "digraph \"CFG for'" + F.getName() + "\' function\" {\n";

    //get entry bb
    BasicBlock * entry_bb = &F.getEntryBlock();

    for (Function::iterator B_iter = F.begin(); B_iter != F.end(); ++B_iter){
        BasicBlock* curBB = &*B_iter;
        unsigned long from_num = basicBlockMap.find(curBB)->second;

        // check loop
        bool isLoop = loopinfo.getLoopFor(curBB) != nullptr ? true : false;
        file << "\t" << from_num << " [shape=record, label=\"";
        file  << from_num << "\",id=" << from_num;
        if(isLoop){
            file <<",loop=1,color=red";
        }else{
            file<<",loop=0";
        }

        //check entry point
        if(curBB == entry_bb){
            file << ",entry=1";
        }else{
            file << ",entry=0";
        }
        //check exit
        if(curBB == exit_bb){
            file << ",exit=1";
        }else{
            file << ",exit=0";
        }
        file << "];\n";

        for (BasicBlock *SuccBB : successors(curBB)){
            unsigned long to_num = basicBlockMap.find(SuccBB)->second;
            file << "\t" << from_num<< "-> " << to_num << ";\n";
        }
    }
    file << "}\n";
    file.close();

    //output d_file
    d_file << "digraph \"dom tree for'" + F.getName() + "\' function\" {\n";

    auto dtree_root = dTree.getRootNode();
    BasicBlock * d_root = dtree_root->getBlock();
    if(d_root != entry_bb){
        llvm_unreachable("entry block does not match");
    }
    RecursivePrintEdges(basicBlockMap,d_file,dtree_root, 1);
    d_file<<"}\n";
    d_file.close();

    //output pd_file
    pd_file << "digraph \"post dom tree for'" + F.getName() + "\' function\" {\n";

    auto tree_root = pdTree.getRootNode();
    BasicBlock * pd_root = tree_root->getBlock();
    if(pd_root != nullptr){
        llvm_unreachable("virtual exit should not have a corresponding BB");
    }
    if(tree_root->getNumChildren() != 1){
        llvm_unreachable("virtual exit should only have one child which is the real exit, most likely there is the deadloop in the code.");
    }
    BasicBlock* real_exit = tree_root->back()->getBlock();
    if(real_exit != exit_bb){
        llvm_unreachable("real exit does not match, maybe there is a dead loop");
    }

    RecursivePrintEdges(basicBlockMap,pd_file,tree_root->back(), 1);
    pd_file<<"}\n";
    pd_file.close();

}
