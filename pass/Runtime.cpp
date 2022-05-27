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

#include "Runtime.h"

#include <llvm/ADT/StringSet.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace {

template <typename... ArgsTy>
SymFnT import(llvm::Module &M, llvm::StringRef name, llvm::Type *ret,
              ArgsTy... args) {
#if LLVM_VERSION_MAJOR >= 9 && LLVM_VERSION_MAJOR < 11
  return M.getOrInsertFunction(name, ret, args...);
#else
  return M.getOrInsertFunction(name, ret, args...);
#endif
}

} // namespace

Runtime::Runtime(Module &M) {

    IRBuilder<> IRB(M.getContext());
    intPtrType = M.getDataLayout().getIntPtrType(M.getContext());
    int_type = nullptr;
    if(M.getDataLayout().isLegalInteger(64)){
        int_type = IRB.getInt64Ty();
    }else if(M.getDataLayout().isLegalInteger(32)){
        int_type = IRB.getInt32Ty();
    }else if(M.getDataLayout().isLegalInteger(16)){
        int_type = IRB.getInt16Ty();
    }else{
        llvm_unreachable("integer width less than 16 bit?");
    }
    auto *ptrT = IRB.getInt8PtrTy();
    auto *voidT = IRB.getVoidTy();
    int8T = IRB.getInt8Ty();
    int32T = IRB.getInt32Ty();
    symIntT = IRB.getInt16Ty();
    int16T = IRB.getInt16Ty();
    isSymT = IRB.getInt1Ty();
    symIDTyName = StringRef("SymIDTy");
    symIDT = llvm::StructType::create(M.getContext(),{symIntT},symIDTyName);

    // should always return true(when this function is called, it means another variable has been symbolized, and thus this variable needs to be symbolized)
    buildInteger = import(M, "_sym_build_integer", isSymT, int_type, int8T, symIntT);
    SymOperators.push_back(&buildInteger);
    runtimeArgNo["_sym_build_integer"] = {0};
    constArgNo["_sym_build_integer"] = {1};
    symIdArgNo["_sym_build_integer"] = {2};

    /*
    buildInteger1 = import(M, "_sym_build_integer1", isSymT, int_type, int8T, symIntT);
    SymOperators.push_back(&buildInteger);
    constArgNo["_sym_build_integer1"] = {0,1};
    symIdArgNo["_sym_build_integer1"] = {2};
    replaceToTrue.push_back("_sym_build_integer1");
    */
    // sshould always return true(not used for embedded application)
    buildInteger128 = import(M, "_sym_build_integer128", isSymT, IRB.getInt64Ty(), IRB.getInt64Ty());
    SymOperators.push_back(&buildInteger128);

    // should always return true
    buildFloat = import(M, "_sym_build_float", isSymT, IRB.getDoubleTy(), IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildFloat);
    runtimeArgNo["_sym_build_float"] = {0};
    constArgNo["_sym_build_float"] = {1};
    symIdArgNo["_sym_build_float"] = {2};

    /*
    buildFloat1 = import(M, "_sym_build_float1", isSymT, IRB.getDoubleTy(), IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildFloat);
    constArgNo["_sym_build_float1"] = {0,1};
    symIdArgNo["_sym_build_float1"] = {2};
    replaceToTrue.push_back("_sym_build_float1");
    */

    // should always return true
    buildNullPointer = import(M, "_sym_build_null_pointer",isSymT);
    SymOperators.push_back(&buildNullPointer);
    replaceToTrue.push_back("_sym_build_null_pointer");

    // should always return true
    buildTrue = import(M, "_sym_build_true", isSymT);
    SymOperators.push_back(&buildTrue);
    replaceToTrue.push_back("_sym_build_true");

    // should always return true
    buildFalse = import(M, "_sym_build_false", isSymT);
    SymOperators.push_back(&buildFalse);
    replaceToTrue.push_back("_sym_build_false");

    // should always return true
    buildBool = import(M, "_sym_build_bool", isSymT, IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildBool);
    runtimeArgNo["_sym_build_bool"] = {0};
    symIdArgNo["_sym_build_bool"] = {1};

    /*
    buildBool1 = import(M, "_sym_build_bool1", isSymT, IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildBool);
    constArgNo["_sym_build_bool1"] = {0};
    symIdArgNo["_sym_build_bool1"] = {1};
    replaceToTrue.push_back("_sym_build_bool1");
    */

    // functions that need a sym id
    // should always be the same with the input
    // int8T should be constant
    buildSExt = import(M, "_sym_build_sext", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildSExt);
    isSymArgNo["_sym_build_sext"] = {0};
    constArgNo["_sym_build_sext"] = {1};
    replaceToInput.push_back("_sym_build_sext");

    // should always be the same with the input
    buildZExt = import(M, "_sym_build_zext", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildZExt);
    isSymArgNo["_sym_build_zext"] = {0};
    constArgNo["_sym_build_zext"] = {1};
    replaceToInput.push_back("_sym_build_zext");

    // should always be the same with the input
    buildTrunc = import(M, "_sym_build_trunc", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildTrunc);
    isSymArgNo["_sym_build_trunc"] = {0};
    constArgNo["_sym_build_trunc"] = {1};
    replaceToInput.push_back("_sym_build_trunc");

    // should always be the same with the input
    buildBswap = import(M, "_sym_build_bswap", isSymT, isSymT);
    SymOperators.push_back(&buildBswap);
    isSymArgNo["_sym_build_bswap"] = {0};
    replaceToInput.push_back("_sym_build_bswap");

    // should always be the same with the input
    buildIntToFloat = import(M, "_sym_build_int_to_float", isSymT, isSymT, IRB.getInt1Ty(), IRB.getInt1Ty());
    SymOperators.push_back(&buildIntToFloat);
    isSymArgNo["_sym_build_int_to_float"] = {0};
    constArgNo["_sym_build_int_to_float"] = {1,2};
    replaceToInput.push_back("_sym_build_int_to_float");

    // should always be the same with the input
    buildFloatToFloat = import(M, "_sym_build_float_to_float", isSymT, isSymT, IRB.getInt1Ty());
    SymOperators.push_back(&buildFloatToFloat);
    isSymArgNo["_sym_build_float_to_float"] = {0};
    constArgNo["_sym_build_float_to_float"] = {1};
    replaceToInput.push_back("_sym_build_float_to_float");

    // should always be the same with the input
    buildBitsToFloat = import(M, "_sym_build_bits_to_float", isSymT, isSymT, IRB.getInt1Ty());
    SymOperators.push_back(&buildBitsToFloat);
    isSymArgNo["_sym_build_bits_to_float"] = {0};
    constArgNo["_sym_build_bits_to_float"] = {1};
    replaceToInput.push_back("_sym_build_bits_to_float");

    // should always be the same with the input
    buildFloatToBits = import(M, "_sym_build_float_to_bits", isSymT, isSymT);
    SymOperators.push_back(&buildFloatToBits);
    isSymArgNo["_sym_build_float_to_bits"] = {0};
    replaceToInput.push_back("_sym_build_float_to_bits");

    // should always be the same with the input
    buildFloatToSignedInt =
            import(M, "_sym_build_float_to_signed_integer", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildFloatToSignedInt);
    isSymArgNo["_sym_build_float_to_signed_integer"] = {0};
    constArgNo["_sym_build_float_to_signed_integer"] = {1};
    replaceToInput.push_back("_sym_build_float_to_signed_integer");

    // should always be the same with the input
    buildFloatToUnsignedInt =
      import(M, "_sym_build_float_to_unsigned_integer", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildFloatToUnsignedInt);
    isSymArgNo["_sym_build_float_to_unsigned_integer"] = {0};
    constArgNo["_sym_build_float_to_unsigned_integer"] = {1};
    replaceToInput.push_back("_sym_build_float_to_unsigned_integer");

    // should always be the same with the input
    buildFloatAbs = import(M, "_sym_build_fp_abs", isSymT, isSymT);
    SymOperators.push_back(&buildFloatAbs);
    isSymArgNo["_sym_build_fp_abs"] = {0};
    replaceToInput.push_back("_sym_build_fp_abs");

    // logic OR
    buildBoolAnd = import(M, "_sym_build_bool_and", isSymT, isSymT, isSymT);
    SymOperators.push_back(&buildBoolAnd);
    isSymArgNo["_sym_build_bool_and"] = {0,1};
    replaceToLogicOr.push_back("_sym_build_bool_and");

    // logic OR
    buildBoolOr = import(M, "_sym_build_bool_or", isSymT, isSymT, isSymT);
    SymOperators.push_back(&buildBoolOr);
    isSymArgNo["_sym_build_bool_or"] = {0,1};
    replaceToLogicOr.push_back("_sym_build_bool_or");

    // logic OR
    buildBoolXor = import(M, "_sym_build_bool_xor", isSymT, isSymT, isSymT);
    SymOperators.push_back(&buildBoolXor);
    isSymArgNo["_sym_build_bool_xor"] = {0,1};
    replaceToLogicOr.push_back("_sym_build_bool_xor");

    // should always be the same with the input
    buildBoolToBits = import(M, "_sym_build_bool_to_bits", isSymT, isSymT, int8T);
    SymOperators.push_back(&buildBoolToBits);
    isSymArgNo["_sym_build_bool_to_bits"] = {0};
    constArgNo["_sym_build_bool_to_bits"] = {1};
    replaceToInput.push_back("_sym_build_bool_to_bits");

    // no ret
    pushPathConstraint = import(M, "_sym_build_path_constraint", voidT, isSymT, IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&pushPathConstraint);
    isSymArgNo["_sym_build_path_constraint"] = {0};
    runtimeArgNo["_sym_build_path_constraint"] = {1};
    //constArgNo["_sym_build_path_constraint"] = {2};
    symIdArgNo["_sym_build_path_constraint"] = {2};


    // no ret
    setParameterExpression = import(M, "_sym_set_parameter_expression", voidT, int8T, isSymT);
    SymOperators.push_back(&setParameterExpression);
    constArgNo["_sym_set_parameter_expression"] = {0};
    isSymArgNo["_sym_set_parameter_expression"] = {1};

    //see if the para is symbolic or not
    getParameterExpression = import(M, "_sym_get_parameter_expression", isSymT, int8T);
    SymOperators.push_back(&getParameterExpression);
    constArgNo["_sym_get_parameter_expression"] = {0};

    // not ret
    setReturnExpression = import(M, "_sym_set_return_expression", voidT, isSymT);
    SymOperators.push_back(&setReturnExpression);
    isSymArgNo["_sym_set_return_expression"] = {0};

    //see if the return val is symbolic or not
    getReturnExpression = import(M, "_sym_get_return_expression", isSymT);
    SymOperators.push_back(&getReturnExpression);


    // given the address, we need to check the src is symbolic or not inside this fuction
    memcpy = import(M, "_sym_build_memcpy", voidT, ptrT, ptrT, intPtrType, symIntT);
    SymOperators.push_back(&memcpy);
    runtimeArgNo["_sym_build_memcpy"] = {0,1};
    constArgNo["_sym_build_memcpy"] = {2};
    symIdArgNo["_sym_build_memcpy"] = {3};

    // the 1st para is a physical address
    // the 2nd para is actually a sym var which needs to be assigned with an ID
    memset = import(M, "_sym_build_memset", voidT, ptrT, isSymT, intPtrType, symIntT);
    SymOperators.push_back(&memset);
    runtimeArgNo["_sym_build_memset"] = {0};
    isSymArgNo["_sym_build_memset"] = {1};
    constArgNo["_sym_build_memset"] = {2};
    symIdArgNo["_sym_build_memset"] = {3};

    // the first 2 parameters are all physical address
    memmove = import(M, "_sym_build_memmove", voidT, ptrT, ptrT, intPtrType, symIntT);
    SymOperators.push_back(&memmove);
    runtimeArgNo["_sym_build_memmove"] = {0,1};
    constArgNo["_sym_build_memmove"] = {2};
    symIdArgNo["_sym_build_memmove"] = {3};

    // inside the function, it needs to check if the given memory area is symbolic or not
    readMemory = import(M, "_sym_build_read_memory", isSymT, intPtrType, intPtrType, int8T, symIntT);
    SymOperators.push_back(&readMemory);
    runtimeArgNo["_sym_build_read_memory"] = {0};
    constArgNo["_sym_build_read_memory"] = {1,2};
    symIdArgNo["_sym_build_read_memory"] = {3};

    // need to check if the given memory area as well as the written val is symbolic or not
    writeMemory = import(M, "_sym_build_write_memory", voidT, intPtrType, intPtrType, isSymT, int8T, symIntT);
    SymOperators.push_back(&writeMemory);
    runtimeArgNo["_sym_build_write_memory"] = {0};
    constArgNo["_sym_build_write_memory"] = {1,3};
    isSymArgNo["_sym_build_write_memory"] = {2};
    symIdArgNo["_sym_build_write_memory"] = {4};

    // logic or ..?
    buildInsert = import(M, "_sym_build_insert", isSymT, isSymT, isSymT, int_type, int8T);
    SymOperators.push_back(&buildInsert);
    isSymArgNo["_sym_build_insert"] = {0,1};
    constArgNo["_sym_build_insert"] = {2,3};
    replaceToLogicOr.push_back("_sym_build_insert");

    // same with input..?
    buildExtract = import(M, "_sym_build_extract", isSymT, isSymT, int_type, int_type, int8T);
    SymOperators.push_back(&buildExtract);
    isSymArgNo["_sym_build_extract"] = {0};
    constArgNo["_sym_build_extract"] = {1,2,3};
    replaceToInput.push_back("_sym_build_extract");

    // just a place-holder, will be removed before lowered to machine code
    tryAlternative = import(M,"_sym_try_alternative",voidT, isSymT, intPtrType );
    SymOperators.push_back(&tryAlternative);
    isSymArgNo["_sym_try_alternative"] = {0};
    runtimeArgNo["_sym_try_alternative"] = {1};
    replaceToNone.push_back("_sym_try_alternative");

    notifyPhi = import(M, "_sym_notify_phi", voidT, int8T, symIntT);
    SymOperators.push_back(&notifyPhi);
    runtimeArgNo["_sym_notify_phi"] = {0};
    symIdArgNo["_sym_notify_phi"] = {1};
    // control-flow related


    notifyCall = import(M, "_sym_notify_call", voidT, int8T);
    SymOperators.push_back(&notifyCall);
    runtimeArgNo["_sym_notify_call"] = {0};

    notifyFunc = import(M,"_sym_notify_func",voidT,int8T);


    notifyRet = import(M, "_sym_notify_ret", voidT, int8T);
    SymOperators.push_back(&notifyRet);
    runtimeArgNo["_sym_notify_ret"] = {0};

    notifyBasicBlock = import(M, "_sym_notify_basic_block", voidT, int8T);
    SymOperators.push_back(&notifyBasicBlock);
    runtimeArgNo["_sym_notify_basic_block"] = {0};

    notifyBasicBlock1 = import(M, "_sym_notify_basic_block1", voidT, int16T);
    SymOperators.push_back(&notifyBasicBlock1);
    runtimeArgNo["_sym_notify_basic_block1"] = {0};

#define LOAD_BINARY_OPERATOR_HANDLER(constant, name)                           \
  binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
       replaceToLogicOr.push_back("_sym_build_" #name);

  LOAD_BINARY_OPERATOR_HANDLER(Add, add)
  LOAD_BINARY_OPERATOR_HANDLER(Sub, sub)
  LOAD_BINARY_OPERATOR_HANDLER(Mul, mul)
  LOAD_BINARY_OPERATOR_HANDLER(UDiv, unsigned_div)
  LOAD_BINARY_OPERATOR_HANDLER(SDiv, signed_div)
  LOAD_BINARY_OPERATOR_HANDLER(URem, unsigned_rem)
  LOAD_BINARY_OPERATOR_HANDLER(SRem, signed_rem)
  LOAD_BINARY_OPERATOR_HANDLER(Shl, shift_left)
  LOAD_BINARY_OPERATOR_HANDLER(LShr, logical_shift_right)
  LOAD_BINARY_OPERATOR_HANDLER(AShr, arithmetic_shift_right)
  LOAD_BINARY_OPERATOR_HANDLER(And, and)
  LOAD_BINARY_OPERATOR_HANDLER(Or, or)
  LOAD_BINARY_OPERATOR_HANDLER(Xor, xor)

  // Floating-point arithmetic
  LOAD_BINARY_OPERATOR_HANDLER(FAdd, fp_add)
  LOAD_BINARY_OPERATOR_HANDLER(FSub, fp_sub)
  LOAD_BINARY_OPERATOR_HANDLER(FMul, fp_mul)
  LOAD_BINARY_OPERATOR_HANDLER(FDiv, fp_div)
  LOAD_BINARY_OPERATOR_HANDLER(FRem, fp_rem)

#undef LOAD_BINARY_OPERATOR_HANDLER

#define LOAD_COMPARISON_HANDLER(constant, name)                                \
  comparisonHandlers[CmpInst::constant] =                                      \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
       replaceToLogicOr.push_back("_sym_build_" #name);

  LOAD_COMPARISON_HANDLER(ICMP_EQ, equal)
  LOAD_COMPARISON_HANDLER(ICMP_NE, not_equal)
  LOAD_COMPARISON_HANDLER(ICMP_UGT, unsigned_greater_than)
  LOAD_COMPARISON_HANDLER(ICMP_UGE, unsigned_greater_equal)
  LOAD_COMPARISON_HANDLER(ICMP_ULT, unsigned_less_than)
  LOAD_COMPARISON_HANDLER(ICMP_ULE, unsigned_less_equal)
  LOAD_COMPARISON_HANDLER(ICMP_SGT, signed_greater_than)
  LOAD_COMPARISON_HANDLER(ICMP_SGE, signed_greater_equal)
  LOAD_COMPARISON_HANDLER(ICMP_SLT, signed_less_than)
  LOAD_COMPARISON_HANDLER(ICMP_SLE, signed_less_equal)

  // Floating-point comparisons
  LOAD_COMPARISON_HANDLER(FCMP_OGT, float_ordered_greater_than)
  LOAD_COMPARISON_HANDLER(FCMP_OGE, float_ordered_greater_equal)
  LOAD_COMPARISON_HANDLER(FCMP_OLT, float_ordered_less_than)
  LOAD_COMPARISON_HANDLER(FCMP_OLE, float_ordered_less_equal)
  LOAD_COMPARISON_HANDLER(FCMP_OEQ, float_ordered_equal)
  LOAD_COMPARISON_HANDLER(FCMP_ONE, float_ordered_not_equal)
  LOAD_COMPARISON_HANDLER(FCMP_ORD, float_ordered)
  LOAD_COMPARISON_HANDLER(FCMP_UNO, float_unordered)
  LOAD_COMPARISON_HANDLER(FCMP_UGT, float_unordered_greater_than)
  LOAD_COMPARISON_HANDLER(FCMP_UGE, float_unordered_greater_equal)
  LOAD_COMPARISON_HANDLER(FCMP_ULT, float_unordered_less_than)
  LOAD_COMPARISON_HANDLER(FCMP_ULE, float_unordered_less_equal)
  LOAD_COMPARISON_HANDLER(FCMP_UEQ, float_unordered_equal)
  LOAD_COMPARISON_HANDLER(FCMP_UNE, float_unordered_not_equal)

#undef LOAD_COMPARISON_HANDLER

    spearReport1 = import(M, "_spear_report1", voidT,symIntT, ptrT);
    spearReport2 = import(M, "_spear_report2", voidT,symIntT, ptrT,ptrT);
    spearReport3 = import(M, "_spear_report3", voidT,symIntT, ptrT,ptrT,ptrT);
    spearReport4 = import(M, "_spear_report4", voidT,symIntT, ptrT,ptrT,ptrT,ptrT);
}




/// Decide whether a function is called symbolically.
bool isInterceptedFunction(const Function &f) {
  return (kInterceptedFunctions.count(f.getName()) > 0);
}
