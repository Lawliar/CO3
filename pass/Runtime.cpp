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
  return M.getOrInsertFunction(name, ret, args...).getCallee();
#else
  return M.getOrInsertFunction(name, ret, args...);
#endif
}

} // namespace

Runtime::Runtime(Module &M) {

    IRBuilder<> IRB(M.getContext());
    auto *intPtrType = M.getDataLayout().getIntPtrType(M.getContext());
    auto *ptrT = IRB.getInt8PtrTy();
    auto *int8T = IRB.getInt8Ty();
    auto *voidT = IRB.getVoidTy();
    booleanT = IRB.getInt1Ty();
    symIntT = IRB.getInt32Ty();
    symIDTyName = StringRef("SymIDTy");
    symIDT = llvm::StructType::create(M.getContext(),{symIntT},symIDTyName);


    buildInteger = import(M, "_sym_build_integer", voidT, IRB.getInt64Ty(), int8T);
    SymOperators.push_back(&buildInteger);

    buildInteger128 = import(M, "_sym_build_integer128", voidT, IRB.getInt64Ty(), IRB.getInt64Ty());
    SymOperators.push_back(&buildInteger128);

    buildFloat = import(M, "_sym_build_float", voidT, IRB.getDoubleTy(), IRB.getInt1Ty());
    SymOperators.push_back(&buildFloat);

    buildNullPointer = import(M, "_sym_build_null_pointer", booleanT);
    SymOperators.push_back(&buildNullPointer);


    buildTrue = import(M, "_sym_build_true", voidT);
    SymOperators.push_back(&buildTrue);

    buildFalse = import(M, "_sym_build_false", voidT);
    SymOperators.push_back(&buildFalse);

    buildBool = import(M, "_sym_build_bool", voidT, IRB.getInt1Ty());
    SymOperators.push_back(&buildBool);

    buildSExt = import(M, "_sym_build_sext", voidT, symIDT, int8T);
    SymOperators.push_back(&buildSExt);

    buildZExt = import(M, "_sym_build_zext", voidT, symIDT, int8T);
    SymOperators.push_back(&buildZExt);

    buildTrunc = import(M, "_sym_build_trunc", voidT, symIDT, int8T);
    SymOperators.push_back(&buildTrunc);

    buildBswap = import(M, "_sym_build_bswap", voidT, symIDT);
    SymOperators.push_back(&buildBswap);

    buildIntToFloat = import(M, "_sym_build_int_to_float", voidT, symIDT, IRB.getInt1Ty(), IRB.getInt1Ty());
    SymOperators.push_back(&buildIntToFloat);

    buildFloatToFloat = import(M, "_sym_build_float_to_float", voidT, symIDT, IRB.getInt1Ty());
    SymOperators.push_back(&buildFloatToFloat);

    buildBitsToFloat = import(M, "_sym_build_bits_to_float", voidT, symIDT, IRB.getInt1Ty());
    SymOperators.push_back(&buildBitsToFloat);

    buildFloatToBits = import(M, "_sym_build_float_to_bits", voidT, symIDT);
    SymOperators.push_back(&buildFloatToBits);

    buildFloatToSignedInt =
      import(M, "_sym_build_float_to_signed_integer", voidT, symIDT, int8T);
    SymOperators.push_back(&buildFloatToSignedInt);

    buildFloatToUnsignedInt =
      import(M, "_sym_build_float_to_unsigned_integer", voidT, symIDT, int8T);
    SymOperators.push_back(&buildFloatToUnsignedInt);

    buildFloatAbs = import(M, "_sym_build_fp_abs", voidT, symIDT);
    SymOperators.push_back(&buildFloatAbs);

    buildBoolAnd = import(M, "_sym_build_bool_and", voidT, symIDT, symIDT);
    SymOperators.push_back(&buildBoolAnd);

    buildBoolOr = import(M, "_sym_build_bool_or", voidT, symIDT, symIDT);
    SymOperators.push_back(&buildBoolOr);

    buildBoolXor = import(M, "_sym_build_bool_xor", voidT, symIDT, symIDT);
    SymOperators.push_back(&buildBoolXor);

    buildBoolToBits = import(M, "_sym_build_bool_to_bits", voidT, symIDT, int8T);
    SymOperators.push_back(&buildBoolToBits);

    pushPathConstraint = import(M, "_sym_push_path_constraint", voidT, symIDT, IRB.getInt1Ty(), intPtrType);
    SymOperators.push_back(&pushPathConstraint);

    setParameterExpression = import(M, "_sym_set_parameter_expression", voidT, int8T, symIDT);
    SymOperators.push_back(&setParameterExpression);

    //get the symIDT from the global memory on the MCU.
    getParameterExpression = import(M, "_sym_get_parameter_expression", voidT, int8T);
    SymOperators.push_back(&getParameterExpression);

    setReturnExpression = import(M, "_sym_set_return_expression", voidT, symIDT);
    SymOperators.push_back(&setReturnExpression);

    getReturnExpression = import(M, "_sym_get_return_expression", symIDT);
    SymOperators.push_back(&getReturnExpression);




    memcpy = import(M, "_sym_memcpy", voidT, ptrT, ptrT, intPtrType);
    SymOperators.push_back(&memcpy);

    memset = import(M, "_sym_memset", voidT, ptrT, ptrT, intPtrType);
    SymOperators.push_back(&memset);

    memmove = import(M, "_sym_memmove", voidT, ptrT, ptrT, intPtrType);
    SymOperators.push_back(&memmove);

    readMemory = import(M, "_sym_read_memory", voidT, intPtrType, intPtrType, int8T);
    SymOperators.push_back(&readMemory);

    writeMemory = import(M, "_sym_write_memory", voidT, intPtrType, intPtrType, symIDT, int8T);
    SymOperators.push_back(&writeMemory);

    buildInsert = import(M, "_sym_build_insert", voidT, symIDT, symIDT, IRB.getInt64Ty(), int8T);
    SymOperators.push_back(&buildInsert);

    buildExtract = import(M, "_sym_build_extract", voidT, symIDT, IRB.getInt64Ty(), IRB.getInt64Ty(), int8T);
    SymOperators.push_back(&buildExtract);

    notifyCall = import(M, "_sym_notify_call", voidT, intPtrType);
    SymOperators.push_back(&notifyCall);

    notifyRet = import(M, "_sym_notify_ret", voidT, intPtrType);
    SymOperators.push_back(&notifyRet);

    notifyBasicBlock = import(M, "_sym_notify_basic_block", voidT, intPtrType);
    SymOperators.push_back(&notifyBasicBlock);

#define LOAD_BINARY_OPERATOR_HANDLER(constant, name)                           \
  binaryOperatorHandlers[Instruction::constant] =                              \
      import(M, "_sym_build_" #name, voidT, symIDT, symIDT);

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
      import(M, "_sym_build_" #name, voidT, symIDT, symIDT);

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


}


/// Decide whether a function is called symbolically.
bool isInterceptedFunction(const Function &f) {
  static const StringSet<> kInterceptedFunctions = {
      "malloc",   "calloc",  "mmap",    "mmap64", "open",   "read",    "lseek",
      "lseek64",  "fopen",   "fopen64", "fread",  "fseek",  "fseeko",  "rewind",
      "fseeko64", "getc",    "ungetc",  "memcpy", "memset", "strncpy", "strchr",
      "memcmp",   "memmove", "ntohl",   "fgets",  "fgetc", "getchar"};

  return (kInterceptedFunctions.count(f.getName()) > 0);
}
