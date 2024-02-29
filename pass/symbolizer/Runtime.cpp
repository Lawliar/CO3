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

//#define SUPPORT_FLOAT

#define ARG(...) { __VA_ARGS__ }

#define CO3_SYMVAL_ARGS(ARGS) \
    isSymArgNo[OP_NAME] = ARG ARGS;


#define CO3_RUNTIME_ARGS(ARGS) \
    runtimeArgNo[OP_NAME] = ARG ARGS;

#define CO3_CONST_ARGS(ARGS) \
    constArgNo[OP_NAME] = ARG ARGS;

#define CO3_SYMID_ARGS(ARGS) \
    symIdArgNo[OP_NAME] = ARG ARGS;

#define CO3_SKIP_ARGS(ARGS) \
    skipArgNo[OP_NAME] = ARG ARGS;


#define CO3_TO_NONE  replaceToNone.push_back(OP_NAME);
#define CO3_TO_FALSE replaceToFalse.push_back(OP_NAME);
#define CO3_TO_TRUE  replaceToTrue.push_back(OP_NAME);
#define CO3_TO_INPUT replaceToInput.push_back(OP_NAME);
#define CO3_TO_OR    replaceToLogicOr.push_back(OP_NAME);

Runtime::Runtime(Module &M) {

    IRBuilder<> IRB(M.getContext());
    llvm::IntegerType * int_type = nullptr;
    llvm::IntegerType* intPtrType = M.getDataLayout().getIntPtrType(M.getContext());
    if(M.getDataLayout().isLegalInteger(64)){
        //int_type = IRB.getInt64Ty();
        llvm_unreachable("MCU with 64-bit data layout?");
    }else if(M.getDataLayout().isLegalInteger(32)){
        int_type = IRB.getInt32Ty();
    }else if(M.getDataLayout().isLegalInteger(16)){
        int_type = IRB.getInt16Ty();
    }else{
        llvm_unreachable("integer width less than 16 bit?");
    }
    Type *ptrT = IRB.getInt8PtrTy();
    Type *voidT = IRB.getVoidTy();
    Type *int8T = IRB.getInt8Ty();
    Type *symIntT = IRB.getInt16Ty();
    Type *int16T = IRB.getInt16Ty();
    Type *isSymT = IRB.getInt1Ty();

#define OP_NAME "_sym_build_integer"
    // should always return true(when this function is called, it means another variable has been symbolized, and thus this variable needs to be symbolized)
    buildInteger = import(M, OP_NAME, isSymT, int_type, int8T, symIntT);
    SymOperators.push_back(&buildInteger);
    CO3_RUNTIME_ARGS((0));
    CO3_CONST_ARGS((1));
    CO3_SYMID_ARGS((2));
#undef OP_NAME

    /*
    buildInteger1 = import(M, "_sym_build_integer1", isSymT, int_type, int8T, symIntT);
    SymOperators.push_back(&buildInteger);
    constArgNo["_sym_build_integer1"] = {0,1};
    symIdArgNo["_sym_build_integer1"] = {2};
    replaceToTrue.push_back("_sym_build_integer1");
    */
    // sshould always return true(not used for embedded application)
#define OP_NAME "_sym_build_integer128"
#if defined(CO3_REPLACE)
    buildInteger128 = import(M, OP_NAME , isSymT, IRB.getInt64Ty(), IRB.getInt64Ty());
#else
    buildInteger128 = import(M, OP_NAME, isSymT, IRB.getInt64Ty(), IRB.getInt64Ty(),symIntT);
    CO3_SYMID_ARGS((3))
#endif
    SymOperators.push_back(&buildInteger128);
#undef OP_NAME



#define OP_NAME "_sym_build_float"
    // should always return true
    buildFloat = import(M, OP_NAME, isSymT, IRB.getDoubleTy(), IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildFloat);
    CO3_RUNTIME_ARGS((0))
    CO3_CONST_ARGS((1))
    CO3_SYMID_ARGS((2))
#ifdef SUPPORT_FLOAT
#else
    CO3_TO_FALSE
#endif
#undef OP_NAME
    /*
    buildFloat1 = import(M, "_sym_build_float1", isSymT, IRB.getDoubleTy(), IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildFloat);
    constArgNo["_sym_build_float1"] = {0,1};
    symIdArgNo["_sym_build_float1"] = {2};
    replaceToTrue.push_back("_sym_build_float1");
    */


#define OP_NAME "_sym_build_null_pointer"
#if defined(CO3_REPLACE)
    buildNullPointer = import(M,OP_NAME ,isSymT);
    CO3_TO_TRUE
#else
    buildNullPointer = import(M, OP_NAME,isSymT, symIntT);
    CO3_SYMID_ARGS((0));
#endif
    SymOperators.push_back(&buildNullPointer);
#undef OP_NAME



#define OP_NAME "_sym_build_true"
#if defined(CO3_REPLACE)
    buildTrue = import(M,OP_NAME , isSymT);
    CO3_TO_TRUE
#else
    buildTrue = import(M,OP_NAME , isSymT,symIntT);
    CO3_SYMID_ARGS((0))
#endif
    SymOperators.push_back(&buildTrue);
#undef OP_NAME



#define OP_NAME "_sym_build_false"
#if defined(CO3_REPLACE)
    buildFalse = import(M,OP_NAME , isSymT);
    CO3_TO_TRUE
#else
    buildFalse = import(M, OP_NAME, isSymT, symIntT);
    CO3_SYMID_ARGS((0))
#endif
    SymOperators.push_back(&buildFalse);
#undef OP_NAME


#define OP_NAME "_sym_build_bool"
    buildBool = import(M, OP_NAME, isSymT, IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&buildBool);
    CO3_RUNTIME_ARGS((0))
    CO3_SYMID_ARGS((1))
#undef OP_NAME

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
#define OP_NAME "_sym_build_sext"
#if defined(CO3_REPLACE)
    buildSExt = import(M, OP_NAME, isSymT, isSymT, int8T);
    CO3_TO_INPUT
#else
    buildSExt = import(M, OP_NAME, isSymT, isSymT, int8T, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildSExt);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


    // should always be the same with the input
#define OP_NAME "_sym_build_zext"
#if defined(CO3_REPLACE)
    buildZExt = import(M, OP_NAME, isSymT, isSymT, int8T);
    CO3_TO_INPUT
#else
    buildZExt = import(M, OP_NAME, isSymT, isSymT, int8T,symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildZExt);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_build_trunc"
#if defined(CO3_REPLACE)
    buildTrunc = import(M, OP_NAME, isSymT, isSymT, int8T);
    CO3_TO_INPUT
#else
    buildTrunc = import(M, OP_NAME, isSymT, isSymT, int8T,symIntT);
    CO3_SYMID_ARGS((2));
#endif
    SymOperators.push_back(&buildTrunc);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME



#define OP_NAME "_sym_build_bswap"
#if defined(CO3_REPLACE)
    buildBswap = import(M, OP_NAME, isSymT, isSymT);
    CO3_TO_INPUT
#else
    buildBswap = import(M, OP_NAME, isSymT, isSymT,symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&buildBswap);
    CO3_SYMVAL_ARGS((0))

#undef OP_NAME


#define OP_NAME "_sym_build_int_to_float"
#if defined(CO3_REPLACE)
    buildIntToFloat = import(M,OP_NAME , isSymT, isSymT, IRB.getInt1Ty(), IRB.getInt1Ty());
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUI
    #else
        CO3_TO_FALSE
    #endif
#else
    buildIntToFloat = import(M,OP_NAME , isSymT, isSymT, IRB.getInt1Ty(), IRB.getInt1Ty(), symIntT);
    CO3_SYMID_ARGS((3))
#endif
    SymOperators.push_back(&buildIntToFloat);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1,2))
#undef OP_NAME

#define OP_NAME "_sym_build_float_to_float"
#if defined(CO3_REPLACE)
    buildFloatToFloat = import(M, OP_NAME, isSymT, isSymT, IRB.getInt1Ty());
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUT
    #else
        CO3_TO_FALSE
    #endif
#else
    buildFloatToFloat = import(M, OP_NAME, isSymT, isSymT, IRB.getInt1Ty(), symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildFloatToFloat);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_build_bits_to_float"
#if defined(CO3_REPLACE)
    buildBitsToFloat = import(M, OP_NAME, isSymT, isSymT, IRB.getInt1Ty());
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUT
    #else
        CO3_TO_FALSE
    #endif
#else
    buildBitsToFloat = import(M, OP_NAME, isSymT, isSymT, IRB.getInt1Ty(), symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildBitsToFloat);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_build_float_to_bits"
#if defined(CO3_REPLACE)
    buildFloatToBits = import(M, OP_NAME, isSymT, isSymT);
#else
    buildFloatToBits = import(M, OP_NAME, isSymT, isSymT, symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&buildFloatToBits);
    CO3_SYMVAL_ARGS((0))
#ifdef SUPPORT_FLOAT
    CO3_TO_INPUT
#else
    CO3_TO_FALSE
#endif
#undef OP_NAME



#define OP_NAME "_sym_build_float_to_signed_integer"
#if defined(CO3_REPLACE)
    buildFloatToSignedInt = import(M,OP_NAME , isSymT, isSymT, int8T);
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUT
    #else
        CO3_TO_FALSE
    #endif
#else
    buildFloatToSignedInt = import(M,OP_NAME , isSymT, isSymT, int8T, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildFloatToSignedInt);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME



#define OP_NAME "_sym_build_float_to_unsigned_integer"
#if defined(CO3_REPLACE)
    buildFloatToUnsignedInt = import(M,OP_NAME , isSymT, isSymT, int8T);
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUT
    #else
        CO3_TO_FALSE
    #endif
#else
    buildFloatToUnsignedInt = import(M,OP_NAME , isSymT, isSymT, int8T, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildFloatToUnsignedInt);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_build_fp_abs"
#if defined(CO3_REPLACE)
    buildFloatAbs = import(M,OP_NAME , isSymT, isSymT);
    #ifdef SUPPORT_FLOAT
        CO3_TO_INPUT
    #else
        CO3_TO_FALSE
    #endif
#else
    buildFloatAbs = import(M,OP_NAME , isSymT, isSymT,symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&buildFloatAbs);
    CO3_SYMVAL_ARGS((0))

#undef OP_NAME


#define OP_NAME "_sym_build_bool_and"
#if defined(CO3_REPLACE)
    buildBoolAnd = import(M,OP_NAME , isSymT, isSymT, isSymT);
    CO3_TO_OR
#else
    buildBoolAnd = import(M,OP_NAME , isSymT, isSymT, isSymT, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildBoolAnd);
    CO3_SYMVAL_ARGS((0,1))
#undef OP_NAME


#define OP_NAME "_sym_build_bool_or"
#if defined(CO3_REPLACE)
    buildBoolOr = import(M,OP_NAME , isSymT, isSymT, isSymT);
    CO3_TO_OR
#else
    buildBoolOr = import(M,OP_NAME , isSymT, isSymT, isSymT, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildBoolOr);
    CO3_SYMVAL_ARGS((0,1))
#undef OP_NAME



#define OP_NAME "_sym_build_bool_xor"
#if defined(CO3_REPLACE)
    buildBoolXor = import(M, OP_NAME, isSymT, isSymT, isSymT);
    CO3_TO_OR
#else
    buildBoolXor = import(M, OP_NAME, isSymT, isSymT, isSymT, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildBoolXor);
    CO3_SYMVAL_ARGS((0,1))
#undef OP_NAME


    // should always be the same with the input
#define OP_NAME "_sym_build_bool_to_bits"
#if defined(CO3_REPLACE)
    buildBoolToBits = import(M, OP_NAME, isSymT, isSymT, int8T);
    CO3_TO_INPUT
#else
    buildBoolToBits = import(M, OP_NAME, isSymT, isSymT, int8T, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&buildBoolToBits);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_build_path_constraint"
    pushPathConstraint = import(M, OP_NAME, voidT, isSymT, IRB.getInt1Ty(), symIntT);
    SymOperators.push_back(&pushPathConstraint);
    CO3_SYMVAL_ARGS((0))
    CO3_RUNTIME_ARGS((1))
    CO3_SYMID_ARGS((2))
#undef OP_NAME


#define OP_NAME "_sym_set_parameter_expression"
#if defined(CO3_REPLACE)
    setParameterExpression = import(M, OP_NAME, voidT, int8T, isSymT);
#else
    setParameterExpression = import(M, OP_NAME, voidT, int8T, isSymT, symIntT);
    CO3_SYMID_ARGS((2))
#endif
    SymOperators.push_back(&setParameterExpression);
    CO3_CONST_ARGS((0))
    CO3_SYMVAL_ARGS((1))
#undef OP_NAME


#define OP_NAME "_sym_get_parameter_expression"
    //see if the para is symbolic or not
#if defined(CO3_REPLACE)
    getParameterExpression = import(M, OP_NAME , isSymT, int8T);
#else
    getParameterExpression = import(M, OP_NAME , isSymT, int8T,symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&getParameterExpression);
    CO3_CONST_ARGS((0))
#undef OP_NAME


#define OP_NAME "_sym_set_return_expression"
#if defined(CO3_REPLACE)
    setReturnExpression = import(M, OP_NAME , voidT, isSymT);
#else
    setReturnExpression = import(M, OP_NAME , voidT, isSymT, symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&setReturnExpression);
    CO3_SYMVAL_ARGS((0))
#undef OP_NAME



#define OP_NAME "_sym_get_return_expression"
#if defined(CO3_REPLACE)
    getReturnExpression = import(M, OP_NAME, isSymT);
#else
    getReturnExpression = import(M, OP_NAME, isSymT, symIntT);
    CO3_SYMID_ARGS((0))
#endif
    SymOperators.push_back(&getReturnExpression);
#undef OP_NAME


#define OP_NAME "_sym_build_memcpy"
    memcpy = import(M, OP_NAME, voidT, ptrT, ptrT, intPtrType, symIntT);
    SymOperators.push_back(&memcpy);
    CO3_RUNTIME_ARGS((0,1,2))
    CO3_SYMID_ARGS((3))
#undef OP_NAME



#define OP_NAME "_sym_build_memset"
    // the 1st para is a physical address
    // the 2nd para is actually a sym var which needs to be assigned with an ID
    memset = import(M, OP_NAME, voidT, ptrT, isSymT, intPtrType, symIntT);
    SymOperators.push_back(&memset);
    CO3_RUNTIME_ARGS((0,2))
    CO3_SYMVAL_ARGS((1))
    CO3_SYMID_ARGS((3))
#undef OP_NAME



#define OP_NAME "_sym_build_memmove"
    // the first 2 parameters are all physical address
    memmove = import(M,OP_NAME , voidT, ptrT, ptrT, intPtrType, symIntT);
    SymOperators.push_back(&memmove);
    CO3_RUNTIME_ARGS((0,1))
    CO3_CONST_ARGS((2))
    CO3_SYMID_ARGS((3))
#undef OP_NAME


#define OP_NAME "_sym_build_read_memory"
    // inside the function, it needs to check if the given memory area is symbolic or not
    readMemory = import(M, OP_NAME, isSymT, intPtrType, intPtrType, int8T, symIntT);
    SymOperators.push_back(&readMemory);
    CO3_RUNTIME_ARGS((0))
    CO3_CONST_ARGS((1,2))
    CO3_SYMID_ARGS((3))
#undef OP_NAME


#define OP_NAME "_sym_build_write_memory"
    // need to check if the given memory area as well as the written val is symbolic or not
    writeMemory = import(M, OP_NAME, voidT, intPtrType, intPtrType, isSymT, int8T, symIntT);
    SymOperators.push_back(&writeMemory);
    CO3_RUNTIME_ARGS((0))
    CO3_CONST_ARGS((1,3))
    CO3_SYMVAL_ARGS((2))
    CO3_SYMID_ARGS((4))
#undef OP_NAME


#define OP_NAME "_sym_build_zero_bytes"
#if defined(CO3_REPLACE)
    buildZeroBytes = import(M, OP_NAME , isSymT, intPtrType);
    CO3_TO_TRUE
#else
    buildZeroBytes = import(M, OP_NAME , isSymT, intPtrType, symIntT);
    CO3_SYMID_ARGS((1))
#endif
    SymOperators.push_back(&buildZeroBytes);
    CO3_CONST_ARGS((0))
#undef OP_NAME


#define OP_NAME "_sym_notify_select"
    notifySelect = import(M, OP_NAME, isSymT, IRB.getInt1Ty(), isSymT, isSymT,ptrT,int8T,symIntT);
    SymOperators.push_back(&notifySelect);
    CO3_RUNTIME_ARGS((0))
    CO3_SYMVAL_ARGS((1,2))
    CO3_SKIP_ARGS((3,4))
    CO3_SYMID_ARGS((5))
#undef OP_NAME


#define OP_NAME "_sym_build_insert"
#if defined(CO3_REPLACE)
    buildInsert = import(M, OP_NAME, isSymT, isSymT, isSymT, int_type, int8T);
    CO3_TO_OR
#else
    buildInsert = import(M, OP_NAME, isSymT, isSymT, isSymT, int_type, int8T, symIntT);
    CO3_SYMID_ARGS((4))
#endif
    SymOperators.push_back(&buildInsert);
    CO3_SYMVAL_ARGS((0,1))
    CO3_CONST_ARGS((2,3))
#undef OP_NAME


#define OP_NAME "_sym_build_extract"
#if defined(CO3_REPLACE)
    buildExtract = import(M, OP_NAME, isSymT, isSymT, int_type, int_type, int8T);
    CO3_TO_INPUT
#else
    buildExtract = import(M, OP_NAME, isSymT, isSymT, int_type, int_type, int8T, symIntT);
    CO3_SYMID_ARGS((4))
#endif
    SymOperators.push_back(&buildExtract);
    CO3_SYMVAL_ARGS((0))
    CO3_CONST_ARGS((1,2,3))
#undef OP_NAME


#define OP_NAME "_sym_try_alternative"
    tryAlternative = import(M, OP_NAME,voidT, isSymT, isSymT,symIntT);
    SymOperators.push_back(&tryAlternative);
    CO3_SYMVAL_ARGS((0,1))
    CO3_SYMID_ARGS((2))
#undef OP_NAME



#define OP_NAME "_sym_notify_phi"
    notifyPhi = import(M, OP_NAME, voidT, int8T, symIntT,isSymT,ptrT,int8T);
    SymOperators.push_back(&notifyPhi);
    CO3_RUNTIME_ARGS((0))
    CO3_SYMID_ARGS((1))
#undef OP_NAME

#define OP_NAME "_sym_notify_call"
    notifyCall = import(M,OP_NAME , voidT, int8T);
    SymOperators.push_back(&notifyCall);
    CO3_RUNTIME_ARGS((0))
#undef OP_NAME


#define OP_NAME "_sym_notify_func"
    notifyFunc = import(M,OP_NAME,voidT,int8T);
    SymOperators.push_back(&notifyFunc);
    CO3_RUNTIME_ARGS((0))
#undef OP_NAME


#define OP_NAME "_sym_notify_ret"
    notifyRet = import(M, OP_NAME, voidT, int8T);
    SymOperators.push_back(&notifyRet);
    CO3_RUNTIME_ARGS((0))
#undef OP_NAME


#define OP_NAME "_sym_notify_basic_block"
    notifyBasicBlock = import(M, OP_NAME, voidT, int16T, isSymT,ptrT,int8T );
    SymOperators.push_back(&notifyBasicBlock);
    CO3_RUNTIME_ARGS((0))
    CO3_SYMVAL_ARGS((1))
#undef OP_NAME



// OK, looks like we cannot have #define inside the #define macro

#if defined(CO3_REPLACE)
#define LOAD_BINARY_OPERATOR_HANDLER(constant, name)                           \
  binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
      replaceToLogicOr.push_back("_sym_build_" #name);

#define LOAD_BINARY_OPERATOR_HANDLER_Unsupported(constant, name)                           \
  binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
      replaceToFalse.push_back("_sym_build_" #name);
#else
#define LOAD_BINARY_OPERATOR_HANDLER(constant, name)                           \
    binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT, symIntT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                 \
      symIdArgNo["_sym_build_" #name] = {2};

#define LOAD_BINARY_OPERATOR_HANDLER_Unsupported(constant, name)                           \
  binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT,symIntT);                      \
      isSymArgNo["_sym_build_" #name] = {0,1};                                             \
      symIdArgNo["_sym_build_" #name] = {2};
#endif




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
#ifdef SUPPORT_FLOAT
  LOAD_BINARY_OPERATOR_HANDLER(FAdd, fp_add)
  LOAD_BINARY_OPERATOR_HANDLER(FSub, fp_sub)
  LOAD_BINARY_OPERATOR_HANDLER(FMul, fp_mul)
  LOAD_BINARY_OPERATOR_HANDLER(FDiv, fp_div)
  LOAD_BINARY_OPERATOR_HANDLER(FRem, fp_rem)
#else
  LOAD_BINARY_OPERATOR_HANDLER_Unsupported(FAdd, fp_add)
  LOAD_BINARY_OPERATOR_HANDLER_Unsupported(FSub, fp_sub)
  LOAD_BINARY_OPERATOR_HANDLER_Unsupported(FMul, fp_mul)
  LOAD_BINARY_OPERATOR_HANDLER_Unsupported(FDiv, fp_div)
  LOAD_BINARY_OPERATOR_HANDLER_Unsupported(FRem, fp_rem)
#endif
#undef LOAD_BINARY_OPERATOR_HANDLER

#if defined(CO3_REPLACE)
#define LOAD_COMPARISON_HANDLER(constant, name)                                \
  comparisonHandlers[CmpInst::constant] =                                      \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
       replaceToLogicOr.push_back("_sym_build_" #name);

#define LOAD_COMPARISON_HANDLER_Unsupported(constant, name)                                \
  comparisonHandlers[CmpInst::constant] =                                      \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                \
       replaceToFalse.push_back("_sym_build_" #name);
#else
#define LOAD_COMPARISON_HANDLER(constant, name)                                \
  comparisonHandlers[CmpInst::constant] =                                      \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT,symIntT);           \
      isSymArgNo["_sym_build_" #name] = {0,1};                                  \
      symIdArgNo["_sym_build_" #name] = {2};

#define LOAD_COMPARISON_HANDLER_Unsupported(constant, name)                                \
  comparisonHandlers[CmpInst::constant] =                                      \
      import(M, "_sym_build_" #name, isSymT, isSymT, isSymT,symIntT);                  \
      isSymArgNo["_sym_build_" #name] = {0,1};                                             \
      symIdArgNo["_sym_build_" #name] = {2};
#endif


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

#ifdef SUPPORT_FLOAT
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
#else
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_OGT, float_ordered_greater_than)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_OGE, float_ordered_greater_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_OLT, float_ordered_less_than)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_OLE, float_ordered_less_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_OEQ, float_ordered_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_ONE, float_ordered_not_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_ORD, float_ordered)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_UNO, float_unordered)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_UGT, float_unordered_greater_than)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_UGE, float_unordered_greater_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_ULT, float_unordered_less_than)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_ULE, float_unordered_less_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_UEQ, float_unordered_equal)
    LOAD_COMPARISON_HANDLER_Unsupported(FCMP_UNE, float_unordered_not_equal)
#endif
#undef LOAD_COMPARISON_HANDLER

#if (false)
    spearReport1 = import(M, "_spear_report1", voidT,symIntT, ptrT);
    spearReport2 = import(M, "_spear_report2", voidT,symIntT, ptrT,ptrT);
    spearReport3 = import(M, "_spear_report3", voidT,symIntT, ptrT,ptrT,ptrT);
    spearReport4 = import(M, "_spear_report4", voidT,symIntT, ptrT,ptrT,ptrT,ptrT);
#endif
}




/// Decide whether a function is called symbolically.
bool isInterceptedFunction(const Function &f) {
  return (kInterceptedFunctions.count(f.getName()) > 0);
}
