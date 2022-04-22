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
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GetElementPtrTypeIterator.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include "Runtime.h"

using namespace llvm;

void Symbolizer::initializeFunctions(Function &F) {

    IRBuilder<> IRB(F.getEntryBlock().getFirstNonPHI());
    if (F.getName() != "main"){
        // The main function doesn't receive symbolic arguments.
        for (auto &arg : F.args()) {
            if (!arg.user_empty()){
                auto * symcall  = IRB.CreateCall(runtime.getParameterExpression,
                                                                IRB.getInt8(arg.getArgNo()));
                symbolicExpressions[&arg] = symcall;
                assignSymID(symcall,getNextID());// the id of this sym call is not known until run-time.
            }
        }
    }
    for(unsigned i = 0 ; i < maxNumOperands ; i++){
        allocaBuffers.push_back(IRB.CreateAlloca(IRB.getInt8Ty(), 0,ConstantInt::get(IRB.getInt8Ty(),perBufferSize)));
    }
}

void Symbolizer::insertBasicBlockNotification(llvm::BasicBlock &B) {
    IRBuilder<> IRB(&*B.getFirstInsertionPt());
    llvm::ConstantInt * valueToInsert = getTargetPreferredInt(&B);
    IRB.CreateCall(runtime.notifyBasicBlock,valueToInsert);
}

void Symbolizer::finalizePHINodes() {
    SmallPtrSet<PHINode *, 32> nodesToErase;

    for (auto *phi : phiNodes) {
        auto symPhi = getSymbolicExpression(phi);
        auto symbolicPHI = cast<PHINode>(symPhi);

        // A PHI node that receives only compile-time constants can be replaced by
        // a null expression.
        if (std::all_of(phi->op_begin(), phi->op_end(), [this](Value *input) {
            return (getSymbolicExpression(input) == nullptr);
        })) {
            nodesToErase.insert(symbolicPHI);
            continue;
        }
        IRBuilder<> IRB(phi);
        phiSymbolicIDs.insert(std::make_pair(symbolicPHI,getNextID()));
        for (unsigned incoming = 0, totalIncoming = phi->getNumIncomingValues();
            incoming < totalIncoming; incoming++) {
            Value * incomingValue = phi->getIncomingValue(incoming);
            //auto symExpr = getSymbolicExpressionOrNull(incomingValue);
            auto symID = getSymIDOrCreateFromConcreteExpr(incomingValue,IRB);
            symbolicPHI->setIncomingValue(incoming,symID);
        }
    }

    for (auto *symbolicPHI : nodesToErase) {
        symbolicPHI->replaceAllUsesWith(ConstantPointerNull::get(cast<PointerType>(symbolicPHI->getType())));
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

    IRB.CreateCall(runtime.memcpy,
                   {I.getOperand(0), I.getOperand(1),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType)});
    break;
  }
  case Intrinsic::memset: {
    IRBuilder<> IRB(&I);

    tryAlternative(IRB, I.getOperand(0));
    tryAlternative(IRB, I.getOperand(2));

    // The comment on memcpy's length parameter applies analogously.

    IRB.CreateCall(runtime.memset,
                   {I.getOperand(0),
                    getSymbolicExpressionOrNull(I.getOperand(1)),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType)});
    break;
  }
  case Intrinsic::memmove: {
    IRBuilder<> IRB(&I);

    tryAlternative(IRB, I.getOperand(0));
    tryAlternative(IRB, I.getOperand(1));
    tryAlternative(IRB, I.getOperand(2));

    // The comment on memcpy's length parameter applies analogously.

    IRB.CreateCall(runtime.memmove,
                   {I.getOperand(0), I.getOperand(1),
                    IRB.CreateZExtOrTrunc(I.getOperand(2), intPtrType)});
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

void Symbolizer::handleFunctionCall(CallBase &I, Instruction *returnPoint) {
    auto *callee = I.getCalledFunction();
    if (callee != nullptr && callee->isIntrinsic()) {
        handleIntrinsicCall(I);
        return;
    }
    IRBuilder<> IRB(returnPoint);
    IRB.CreateCall(runtime.notifyRet, getTargetPreferredInt(&I));
    IRB.SetInsertPoint(&I);
    IRB.CreateCall(runtime.notifyCall, getTargetPreferredInt(&I));

    if (callee == nullptr)
        tryAlternative(IRB, I.getCalledOperand());
    else{
        auto calleeName = callee->getName();
        bool is_interpreted = false;
        for(auto eachInterpreted: interpretedFunctionNames){
            if(calleeName.equals(eachInterpreted)){
                is_interpreted = true;
            }
        }
        if(is_interpreted){
            assignSymID(cast<CallInst>(&I),getNextID());
        }
    }
    for (Use &arg : I.args()){
        auto argSymExpr = getSymbolicExpressionOrNull(arg);
        auto argSymID = getSymIDOrZeroFromSymExpr(argSymExpr);
        CallInst * call_to_set_para = IRB.CreateCall(runtime.setParameterExpression,
                     {ConstantInt::get(IRB.getInt8Ty(), arg.getOperandNo()), argSymID});
        assignSymID(call_to_set_para,getNextID());
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
        auto callToSetReturn = IRB.CreateCall(runtime.setReturnExpression, symIDFromInt(0));
        assignSymID(callToSetReturn,getNextID());
        IRB.SetInsertPoint(returnPoint);
        auto getReturnCall = IRB.CreateCall(runtime.getReturnExpression);
        symbolicExpressions[&I] = getReturnCall;
        assignSymID(getReturnCall,getNextID());
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
  auto runtimeCall =
      buildRuntimeCall(IRB, handler, {I.getOperand(0), I.getOperand(1)});
  registerSymbolicComputation(runtimeCall, &I);
}

void Symbolizer::visitSelectInst(SelectInst &I) {
  // Select is like the ternary operator ("?:") in C. We push the (potentially
  // negated) condition to the path constraints and copy the symbolic
  // expression over from the chosen argument.

  IRBuilder<> IRB(&I);
  auto runtimeCall = buildRuntimeCall(IRB, runtime.pushPathConstraint,
                                      {{I.getCondition(), true},
                                       {I.getCondition(), false},
                                       {getTargetPreferredInt(&I), false}});
  registerSymbolicComputation(runtimeCall);
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
  auto returnSymExpr = getSymbolicExpressionOrNull(I.getReturnValue());
  Value* returnSymID = getSymIDOrZeroFromSymExpr(returnSymExpr);

  CallInst * set_return_inst = IRB.CreateCall(runtime.setReturnExpression,
                 returnSymID);
  assignSymID(set_return_inst,getNextID());
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
                                       {I.getCondition(), false},
                                       {getTargetPreferredInt(&I), false}});
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
    auto *data = IRB.CreateCall(
      runtime.readMemory,
      {IRB.CreatePtrToInt(addr, intPtrType),
       ConstantInt::get(intPtrType, dataLayout.getTypeStoreSize(dataType)),
       ConstantInt::get(IRB.getInt8Ty(), isLittleEndian(dataType) ? 1 : 0)});
    assignSymID(data,readMemSymID);

    if (dataType->isFloatingPointTy()) {
        data = IRB.CreateCall(runtime.buildBitsToFloat,
                          {readMemSymID, IRB.getInt1(dataType->isDoubleTy())});
        assignSymID(data,getNextID() );
    }

    symbolicExpressions[&I] = data;
}

void Symbolizer::visitStoreInst(StoreInst &I) {
    IRBuilder<> IRB(&I);

    tryAlternative(IRB, I.getPointerOperand());

    Value * dataSymID = getSymIDOrCreateFromConcreteExpr(I.getValueOperand(),IRB);
    auto *dataType = I.getValueOperand()->getType();
    if (dataType->isFloatingPointTy()) {
        auto newSymID = getNextID();
        assignSymID(cast<CallInst>(IRB.CreateCall(runtime.buildFloatToBits, {dataSymID})) , newSymID);
        dataSymID = newSymID;
    }
    auto writeMemCall = IRB.CreateCall(
        runtime.writeMemory,
        {IRB.CreatePtrToInt(I.getPointerOperand(), intPtrType),
         ConstantInt::get(intPtrType, dataLayout.getTypeStoreSize(dataType)),
         dataSymID,
         ConstantInt::get(IRB.getInt8Ty(), dataLayout.isLittleEndian() ? 1 : 0)});
    assignSymID(writeMemCall,getNextID());
}

void Symbolizer::visitGetElementPtrInst(GetElementPtrInst &I) {
  // GEP performs address calculations but never actually accesses memory. In
  // order to represent the result of a GEP symbolically, we start from the
  // symbolic expression of the original pointer and duplicate its
  // computations at the symbolic level.

  // If everything is compile-time concrete, we don't need to emit code.
  if (getSymbolicExpression(I.getPointerOperand()) == nullptr &&
      std::all_of(I.idx_begin(), I.idx_end(), [this](Value *index) {
        return (getSymbolicExpression(index) == nullptr);
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
             {ConstantInt::get(IRB.getInt8Ty(), ptrBits - indexWidth),
              false}}));
        symbolicComputation.merge(forceBuildRuntimeCall(
            IRB, runtime.binaryOperatorHandlers[Instruction::Mul],
            {{symbolicComputation.lastInstruction, false}, // the reason why this is false is probably because, the "index" a few lines above has already been true, so, no need to check this one i.e., if index is NULL equals "lastinstruction" being null, during short-circuiting no need to check
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
      IRB, runtime.buildFloatToUnsignedInt,
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
    auto *exprPHI = IRB.CreatePHI(IRB.getInt8PtrTy(), numIncomingValues);
    for (unsigned incoming = 0; incoming < numIncomingValues; incoming++) {
        exprPHI->addIncoming(
            // The null pointer will be replaced in finalizePHINodes.
                symIDFromInt(0),
            I.getIncomingBlock(incoming));
    }

    symbolicExpressions[&I] = exprPHI;
}

void Symbolizer::visitInsertValueInst(InsertValueInst &I) {
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
    auto *conditionSymExpr = getSymbolicExpression(condition);
    if (conditionSymExpr == nullptr)
        return;
    auto conditionSymID = getSymIDFromSymExpr(cast<CallInst>(conditionSymExpr));
    assert(conditionSymID != nullptr);

    for (auto &caseHandle : I.cases()) {
        auto *caseTaken = IRB.CreateICmpEQ(condition, caseHandle.getCaseValue());
        auto caseValueExpr = createValueExpression(caseHandle.getCaseValue(), IRB);
        auto caseValueSymID = getSymIDFromSymExpr(caseValueExpr);
        assert(caseValueSymID != nullptr);

        auto caseConstraintSymID = getNextID();
        auto *caseConstraint = IRB.CreateCall(
        runtime.comparisonHandlers[CmpInst::ICMP_EQ],
        {conditionSymID, caseValueSymID});
        assignSymID(caseConstraint,caseConstraintSymID);

        auto pushConstraintCall = IRB.CreateCall(runtime.pushPathConstraint,
                   {caseConstraintSymID, caseTaken, getTargetPreferredInt(&I)});
        assignSymID(pushConstraintCall,getNextID());
    }
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
        ret = IRB.CreateCall(runtime.buildNullPointer, {});
        assignSymID(ret,getNextID());
        return ret;
    }

    if (valueType->isIntegerTy()) {
        auto bits = valueType->getPrimitiveSizeInBits();
        if (bits == 1) {
            // Special case: LLVM uses the type i1 to represent Boolean values, but
            // for Z3 we have to create expressions of a separate sort.
            auto symid = getNextID();
            ret = IRB.CreateCall(runtime.buildBool, {V});
            assignSymID(ret,symid);
            return ret;
        } else if (bits <= 64) {
            auto symid = getNextID();
            ret = IRB.CreateCall(runtime.buildInteger,
                                 {IRB.CreateZExtOrBitCast(V, IRB.getInt64Ty()),
                                  IRB.getInt8(valueType->getPrimitiveSizeInBits())});
            assignSymID(ret,symid);
            return ret;
        } else {
            // Anything up to the maximum supported 128 bits. Those integers are a bit
            // tricky because the symbolic backends don't support them per se. We have
            // a special function in the run-time library that handles them, usually
            // by assembling expressions from smaller chunks.
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
                              IRB.getInt1(valueType->isDoubleTy())});
        assignSymID(ret, symid);
        return ret;
    }

    if (valueType->isPointerTy()) {
        auto symid = getNextID();
        ret = IRB.CreateCall(
                runtime.buildInteger,
                {IRB.CreatePtrToInt(V, IRB.getInt64Ty()), IRB.getInt8(ptrBits)});
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

        auto *memory = IRB.CreateAlloca(V->getType());
        IRB.CreateStore(V, memory);
        auto symid = getNextID();
        ret = IRB.CreateCall(runtime.readMemory,{IRB.CreatePtrToInt(memory, intPtrType), ConstantInt::get(intPtrType,dataLayout.getTypeStoreSize(V->getType())),IRB.getInt8(0)});
        assignSymID(ret,symid);
        return ret;
    }
    llvm_unreachable("Unhandled type for constant expression");
}

Symbolizer::SymbolicComputation
Symbolizer::forceBuildRuntimeCall(IRBuilder<> &IRB, SymFnT function,
                                  ArrayRef<std::pair<Value *, bool>> args) {
    std::vector<Value *> functionArgs;
    for (const auto &[arg, symbolic] : args) {
        Value * paraSymID = nullptr;
        CallInst* symExpr = dyn_cast<CallInst>(arg);
        if( symExpr != nullptr && getSymIDFromSymExpr(symExpr) != nullptr ){
            // if this is call inst and this call is a symbolic call
            paraSymID = getSymIDFromSymExpr(symExpr);
            unsigned symID_int = getIntFromSymID(paraSymID);
            assert(symID_int > 0);
        }else{
            // we are dealing with non-symbolic operations
            // if this non-symbolic operations has been associated with symExpr and symID before, return the id
            // if not, construct a symid for it
            paraSymID = getSymIDOrCreateFromConcreteExpr(arg,IRB);
        }
        functionArgs.push_back(paraSymID);
    }

    auto *call = IRB.CreateCall(function, functionArgs);
    assignSymID(call,getNextID());

    std::vector<Input> inputs;
    for (unsigned i = 0; i < args.size(); i++) {
        const auto &[arg, symbolic] = args[i];
        if (symbolic)
            inputs.push_back({arg, i, call});
    }

    return SymbolicComputation(call, call, inputs);
}

void Symbolizer::tryAlternative(IRBuilder<> &IRB, Value *V) {
    auto *destExpr = getSymbolicExpression(V);
    if (destExpr != nullptr) {
        auto destSymId = getSymIDFromSymExpr(cast<CallInst>(destExpr));
        assert(destSymId != nullptr);
        auto *concreteDestExpr = createValueExpression(V, IRB);
        auto concreteDestSymId = getSymIDFromSymExpr(concreteDestExpr);
        assert(concreteDestSymId != nullptr);

        auto destAssertSymID = getNextID();
        auto *destAssertion = IRB.CreateCall(runtime.comparisonHandlers[CmpInst::ICMP_EQ],
                                             {destSymId, concreteDestSymId});
        assignSymID(destAssertion,destAssertSymID);
        // no need to assign a symid for push constraint, as long as all its symbolic operands are there
        auto *pushAssertion = IRB.CreateCall(runtime.pushPathConstraint,
                                             {destAssertSymID, IRB.getInt1(true), getTargetPreferredInt(V)});
        assignSymID(pushAssertion,getNextID());
        registerSymbolicComputation(SymbolicComputation(
                concreteDestExpr, pushAssertion, {{V, 0, destAssertion}}));
    }
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
    if(IntegerType * intType = dyn_cast<IntegerType>(ty)){
        width =  dataLayout.getTypeAllocSize(intType);
    }else if(PointerType * ptrType = dyn_cast<PointerType>(ty)){
        width = dataLayout.getPointerTypeSize(ptrType);
    }
    else{
        errs()<<*ty<<'\n';
        llvm_unreachable("unhandled type");
    }
    return width;
}
void Symbolizer::createDDGAndReplace(llvm::Function& F, std::string filename){
    std::set<StringRef> toKeep{"_sym_notify_call", "_sym_notify_ret","_sym_notify_basic_block"};
    std::set<StringRef> toRemove{"_sym_set_parameter_expression", "_sym_get_parameter_expression",
                                 "_sym_set_return_expression","_sym_get_return_expression"};
    std::vector<CallInst*> toBeRemoved;
    std::set<StringRef> toExamine;

    for(auto eachSymOperation: runtime.SymOperators){
        if(eachSymOperation->getCallee()->getName().startswith("_sym_build")){
            toExamine.insert(eachSymOperation->getCallee()->getName());
        }
    }
    for(auto eachSymOperation:runtime.comparisonHandlers){
        if(eachSymOperation.getCallee() == nullptr)
            continue;
        toExamine.insert(eachSymOperation.getCallee()->getName());
    }
    for(auto eachSymOperation:runtime.binaryOperatorHandlers){
        if(eachSymOperation.getCallee() == nullptr)
            continue;
        toExamine.insert(eachSymOperation.getCallee()->getName());
    }
    for(auto eachInterpretedFunction: interpretedFunctionNames){
        toExamine.insert(eachInterpretedFunction);
    }
    for (auto &basicBlock : F){
        unsigned long blockID = cast<ConstantInt>(cast<CallInst>(basicBlock.getFirstNonPHI())->getOperand(0))->getZExtValue();
        for(auto & eachInst : basicBlock){
            if( PHINode* symPhi = dyn_cast<PHINode>(&eachInst); symPhi != nullptr && phiSymbolicIDs.find(symPhi) != phiSymbolicIDs.end()){
                PHINode *phi = phiSymbolicIDs.find(symPhi)->first;
                unsigned userSymID = getIntFromSymID(phiSymbolicIDs.find(symPhi)->second);
                g.AddPhiVertice(userSymID, blockID);
                for (unsigned incoming = 0, totalIncoming = phi->getNumIncomingValues();
                     incoming < totalIncoming; incoming++) {
                    BasicBlock* incomingBB = phi->getIncomingBlock(incoming);
                    // although we embed BBID into each node in the DDG, however,
                    // phi node can have incoming value that does not reside in the incoming BB e.g., a constant
                    // so instead of trusting the BBID field of the DDG node, we might as well just note it on the edge.
                    unsigned incomingBBID = cast<ConstantInt>(cast<CallInst>(incomingBB->getFirstNonPHI())->getOperand(0))->getZExtValue();
                    Value * incomingValue = phi->getIncomingValue(incoming);
                    unsigned incomingValueSymID = 0;
                    if(CallInst * symIncomingValue = dyn_cast<CallInst>(getSymbolicExpression(incomingValue))){
                        incomingValueSymID = getIntFromSymID(getSymIDFromSymExpr(symIncomingValue));
                    }else if(PHINode * anotherPhi = dyn_cast<PHINode>(getSymbolicExpression(incomingValue))){
                        // this one should have been constructed already.
                        auto anotherPhiIt = phiSymbolicIDs.find(anotherPhi);
                        incomingValueSymID = getIntFromSymID(anotherPhiIt->second);
                    }
                    assert(incomingValueSymID != 0);
                    g.AddEdge(incomingValueSymID,userSymID,incomingBBID);
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
            if(toKeep.find(calleeName) != toKeep.end()){
                continue;
            }else if(toRemove.find(calleeName) != toRemove.end()){
                toBeRemoved.push_back(callInst);
                if(calleeName.equals("_sym_get_parameter_expression") ){
                    g.AddSymVertice(getIntFromSymID(getSymIDFromSymExpr(callInst)), calleeName,blockID);
                }else if(calleeName.equals("_sym_get_return_expression")){
                    g.AddSymVertice(getIntFromSymID(getSymIDFromSymExpr(callInst)), calleeName,blockID);
                }else if(calleeName.equals("_sym_set_parameter_expression")){
                    g.AddSymVertice(getIntFromSymID(getSymIDFromSymExpr(callInst)), calleeName,blockID);
                    size_t arg_size = callInst->arg_size();
                    assert(arg_size == 2);
                    ConstantInt * idx_const = cast<ConstantInt>(callInst->getArgOperand(0));

                    Value * sym_arg = callInst->getArgOperand(1);
                    assert(isSymIDType(sym_arg));
                    g.AddEdge(getIntFromSymID(sym_arg),getIntFromSymID(getSymIDFromSymExpr(callInst)), idx_const->getZExtValue());
                }else if(calleeName.equals("_sym_set_return_expression")){
                    g.AddSymVertice(getIntFromSymID(getSymIDFromSymExpr(callInst)), calleeName,blockID);
                    size_t arg_size = callInst->arg_size();
                    assert(arg_size == 1);
                    Value * arg = callInst->getArgOperand(0);
                    assert(isSymIDType(arg));
                    g.AddEdge(getIntFromSymID(arg),getIntFromSymID(getSymIDFromSymExpr(callInst)), 0);
                }
            }else if(toExamine.find(calleeName) != toExamine.end()){
                unsigned userSymID = getIntFromSymID(getSymIDFromSymExpr(callInst));
                auto userNode = g.AddSymVertice(userSymID, calleeName,blockID);
                std::map<unsigned,std::pair<unsigned,Value*> > pushed_arg;
                for(auto arg_it = callInst->arg_begin() ; arg_it != callInst->arg_end() ; arg_it++){
                    Value * arg = *arg_it ;
                    unsigned arg_idx = callInst->getArgOperandNo(arg_it);
                    if(isSymIDType(arg)){
                        unsigned arg_symid = getIntFromSymID(arg);
                        g.AddEdge(arg_symid,userSymID, arg_idx);
                    }else if(isa<Constant>(arg)){
                        if(ConstantInt * cont_int = dyn_cast<ConstantInt>(arg)){
                            unsigned int conWidth = dataLayout.getTypeAllocSize(cont_int->getType());
                            int64_t contValue = cont_int->getSExtValue();
                            // constant's BBID is the same with its user(maybe we can merge?)
                            auto conVert = g.AddConstVertice(contValue, conWidth);
                            g.AddEdge(conVert,userNode, arg_idx);
                        }else if(ConstantExpr * const_expr = dyn_cast<ConstantExpr>(arg)){
                            Instruction * arg_inst = const_expr->getAsInstruction();
                            if(PtrToIntInst* ptrToInt = dyn_cast<PtrToIntInst>(arg_inst)){
                                // I don't know any pointer value, need runtime to tell me the value of the pointer
                                unsigned int width = getTypeWidth(ptrToInt->getType(),dataLayout);
                                assert(width > 0);
                                if(width + 2 > perBufferSize){
                                    errs()<< "Width:"<<width<<'\n';
                                    errs()<< "arg:"<<*arg<<'\n';
                                    llvm_unreachable("bitwidth of the runtime arg is too large");
                                }
                                auto runtimeVert = g.AddRuntimeVertice(width,blockID);
                                g.AddEdge(runtimeVert,userNode,arg_idx);
                                pushed_arg.insert(std::make_pair(arg_idx, std::make_pair(width,arg_inst)));
                            }else{
                                errs()<<*arg_inst->getType()<<'\n';
                                errs()<< *arg_inst<<'\n';
                                llvm_unreachable("unhandled constantExpr inst");
                            }
                        }else{
                            errs()<<*arg->getType()<<'\n';
                            errs()<< *arg<<'\n';
                            llvm_unreachable("unhandled constant");
                        }
                    }else{
                        Type* val_type = arg->getType();
                        unsigned int width = getTypeWidth(val_type,dataLayout);
                        assert(width > 0);
                        if(width + 2 > perBufferSize){
                            errs()<< "Width:"<<width<<'\n';
                            errs()<< "arg:"<<*arg<<'\n';
                            llvm_unreachable("bitwidth of the runtime arg is too large");
                        }
                        auto runtimeVert = g.AddRuntimeVertice(width,blockID);
                        g.AddEdge(runtimeVert,userNode,arg_idx);
                        pushed_arg.insert(std::make_pair(arg_idx, std::make_pair(width,arg)));
                    }
                }

                size_t args_to_report_size = pushed_arg.size();
                if(pushed_arg.size() > 0){
                    // need to report the runtime values
                    SymFnT reporter;
                    if(args_to_report_size == 1)
                        reporter = runtime.spearReport1;
                    else if(args_to_report_size == 2)
                        reporter = runtime.spearReport2;
                    else if(args_to_report_size == 3)
                        reporter = runtime.spearReport3;
                    else if(args_to_report_size == 4)
                        reporter = runtime.spearReport4;
                    else
                        llvm_unreachable("too many runtime values need to report");

                    std::vector<Value*> reporter_args;
                    // push the user's symid
                    reporter_args.push_back(ConstantInt::get(runtime.int32T,userSymID));
                    unsigned buffer_idx = 0;
                    for(auto each_arg : pushed_arg){
                        unsigned arg_idx = each_arg.first;
                        unsigned bitwidth = each_arg.second.first;
                        Value * runtime_val = each_arg.second.second;
                        IRB.CreateStore(ConstantInt::get(runtime.int8T,arg_idx),allocaBuffers[buffer_idx]);
                        Value * width_offset = IRB.CreateGEP(allocaBuffers[buffer_idx],ConstantInt::get(runtime.int8T,1));
                        IRB.CreateStore(ConstantInt::get(runtime.int8T,bitwidth),width_offset);
                        Value* value_offset = IRB.CreateGEP(allocaBuffers[buffer_idx],ConstantInt::get(runtime.int8T,2));
                        Value* bitcast_value_offset = IRB.CreateBitCast(value_offset, PointerType::getUnqual(runtime_val->getType()));
                        IRB.CreateStore(runtime_val,bitcast_value_offset);
                        reporter_args.push_back(allocaBuffers[buffer_idx]);
                        buffer_idx++;
                    }
                    CallInst* spear_call = IRB.CreateCall(reporter,reporter_args);
                    if(isInterpretedFunc(calleeName)){
                        callInst->replaceAllUsesWith(spear_call);
                    }
                }
                if(!isInterpretedFunc(calleeName))
                    toBeRemoved.push_back(callInst);
            }
        }
    }
    for(auto eachToBeRemoved: toBeRemoved){
        eachToBeRemoved->eraseFromParent();
    }
    g.writeToFile(filename);
    // Replacing all uses has fixed uses of the symbolic PHI nodes in existing
    // code, but the nodes may still be referenced via symbolicExpressions. We
    // therefore invalidate symbolicExpressions, meaning that it cannot be used
    // after this point.
    symbolicExpressions.clear();
}

void Symbolizer::outputCFG(llvm::Function & F, std::string filename) {
    std::string buffer_str;
    std::error_code error;
    raw_string_ostream rso(buffer_str);
    StringRef name(filename);
    std::map<BasicBlock*, unsigned long> basicBlockMap;

    raw_fd_ostream file(name, error);
    for (Function::iterator B_iter = F.begin(); B_iter != F.end(); ++B_iter){
        unsigned long blockID = cast<ConstantInt>(cast<CallInst>(B_iter->getFirstNonPHI())->getOperand(0))->getZExtValue();
        basicBlockMap.insert(std::make_pair(&*B_iter,blockID));
    }
    file << "digraph \"CFG for'" + F.getName() + "\' function\" {\n";
    for (Function::iterator B_iter = F.begin(); B_iter != F.end(); ++B_iter){
        BasicBlock* curBB = &*B_iter;
        unsigned long from_num = basicBlockMap.find(curBB)->second;
        file << "\tBB" << from_num << " [shape=record, label=\"{";
        file  << from_num << "}\"];\n";
        for (BasicBlock *SuccBB : successors(curBB)){
            unsigned long to_num = basicBlockMap.find(SuccBB)->second;
            file << "\tBB" << from_num<< "-> BB" << to_num << ";\n";
        }
    }
    file << "}\n";
    file.close();
}
