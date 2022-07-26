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
  auto *voidT = IRB.getVoidTy();

  notifyBasicBlock = import(M, "_sym_notify_basic_block", voidT, intPtrType);
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
