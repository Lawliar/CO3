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

#ifndef PASS_H
#define PASS_H

#include "Runtime.h"
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils/UnifyFunctionExitNodes.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Dominators.h>
#include <llvm/Analysis/PostDominators.h>
//#include <llvm/Transforms/Scalar/LoopUnrollAndJamPass.h>
//#include <llvm/Transforms/Scalar/Reg2Mem.h>
#include <llvm/Support/CommandLine.h>

#if LLVM_VERSION_MAJOR >= 13
#include <llvm/IR/PassManager.h>
#endif

class SymbolizeLegacyPass : public llvm::FunctionPass {
public:
  static char ID;

    SymbolizeLegacyPass() : FunctionPass(ID) {}
    //Runtime * r = nullptr;

  bool doInitialization(llvm::Module &M) override;
  bool runOnFunction(llvm::Function &F) override;
  //bool doFinalization(llvm::Module &M) override;

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override  {

      //AU.addRequired<llvm::LoopUnrollAndJamWrapperPass>();
      //AU.addRequired<llvm::RegToMemPass>();
      AU.addRequired<llvm::UnifyFunctionExitNodesLegacyPass>();
      AU.addRequired<llvm::DominatorTreeWrapperPass>();
      AU.addRequired<llvm::PostDominatorTreeWrapperPass>();
      AU.addRequired<llvm::LoopInfoWrapperPass>();

  }
    //void releaseMemory() override;
private:

  /// Mapping from global variables to their corresponding symbolic expressions.
  llvm::ValueMap<llvm::GlobalVariable *, llvm::GlobalVariable *>
      globalExpressions;
};

#if LLVM_VERSION_MAJOR >= 13

class SymbolizePass : public llvm::PassInfoMixin<SymbolizePass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &);

  //Runtime * r = nullptr;
  //void releaseMemory();
  static bool isRequired() { return true; }
};

#endif





#endif
