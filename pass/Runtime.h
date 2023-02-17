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

#ifndef RUNTIME_H
#define RUNTIME_H

#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Module.h>
#include <set>


#if LLVM_VERSION_MAJOR >= 9 && LLVM_VERSION_MAJOR < 11
  using SymFnT = llvm::FunctionCallee;
#else
  using SymFnT = llvm::FunctionCallee;
#endif

/// Runtime functions
struct Runtime {
    Runtime(llvm::Module &M);
    std::vector<SymFnT*> SymOperators;
    std::map<std::string, std::vector<unsigned> > isSymArgNo;
    std::map<std::string, std::vector<unsigned> > runtimeArgNo;
    std::map<std::string, std::vector<unsigned> > constArgNo;
    std::map<std::string, std::vector<unsigned> > symIdArgNo;
    std::map<std::string, std::vector<unsigned> > skipArgNo;
    std::vector<std::string> replaceToNone;
    std::vector<std::string> replaceToFalse;
    std::vector<std::string> replaceToTrue;
    std::vector<std::string> replaceToInput;
    std::vector<std::string> replaceToLogicOr;

    SymFnT buildInteger{};
    SymFnT buildInteger128{};
    SymFnT buildFloat{};
    SymFnT buildNullPointer{};
    SymFnT buildTrue{};
    SymFnT buildFalse{};
    SymFnT buildBool{};
    SymFnT buildSExt{};
    SymFnT buildZExt{};
    SymFnT buildTrunc{};
    SymFnT buildBswap{};
    SymFnT buildIntToFloat{};
    SymFnT buildFloatToFloat{};
    SymFnT buildBitsToFloat{};
    SymFnT buildFloatToBits{};
    SymFnT buildFloatToSignedInt{};
    SymFnT buildFloatToUnsignedInt{};
    SymFnT buildFloatAbs{};
    SymFnT buildBoolAnd{};
    SymFnT buildBoolOr{};
    SymFnT buildBoolXor{};
    SymFnT buildBoolToBits{};
    SymFnT buildBitsToBool{};
    SymFnT pushPathConstraint{};
    SymFnT getParameterExpression{};
    SymFnT setParameterExpression{};
    SymFnT setReturnExpression{};
    SymFnT getReturnExpression{};
    SymFnT memcpy{};
    SymFnT memset{};
    SymFnT memmove{};
    SymFnT readMemory{};
    SymFnT writeMemory{};
    SymFnT buildZeroBytes{};
    SymFnT buildSelect{};
    SymFnT buildInsert{};
    SymFnT buildExtract{};
    SymFnT notifyCall{};
    SymFnT notifyFunc{};
    SymFnT notifyRet{};
    SymFnT notifyPhi{};
    SymFnT notifyBasicBlock{};
    SymFnT tryAlternative{};

    /// Mapping from icmp predicates to the functions that build the corresponding
    /// symbolic expressions.
    std::array<SymFnT, llvm::CmpInst::BAD_ICMP_PREDICATE>
            comparisonHandlers{};

    /// Mapping from binary operators to the functions that build the
    /// corresponding symbolic expressions.
    std::array<SymFnT, llvm::Instruction::BinaryOpsEnd>
            binaryOperatorHandlers{};

    SymFnT spearReport1{};
    SymFnT spearReport2{};
    SymFnT spearReport3{};
    SymFnT spearReport4{};
};
/*
const std::set<llvm::StringRef> kInterceptedFunctions = {
        "malloc",   "calloc",  "mmap",    "mmap64", "open",   "read",    "lseek",
        "lseek64",  "fopen",   "fopen64", "fread",  "fseek",  "fseeko",  "rewind",
        "fseeko64", "getc",    "ungetc",  "memcpy", "memset", "strncpy", "strchr",
        "memcmp",   "memmove", "ntohl",   "fgets",  "fgetc", "getchar"};
*/
const std::set<llvm::StringRef> kInterceptedFunctions = {};
 const llvm::StringRef kInterceptedFunctionPrefix("_sym_build_");

bool isInterceptedFunction(const llvm::Function &f);

#endif
