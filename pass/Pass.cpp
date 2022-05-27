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

#include "Pass.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>

#include "Runtime.h"
#include "Symbolizer.h"
#include "Lower.h"

using namespace llvm;

#ifndef NDEBUG
#define DEBUG(X)                                                               \
  do {                                                                         \
    X;                                                                         \
  } while (false)
#else
#define DEBUG(X) ((void)0)
#endif


#include <llvm/Support/CommandLine.h>
#include <boost/filesystem.hpp>

char SymbolizePass::ID = 0;

Runtime * r = nullptr;
cl::opt<std::string> outDir("out", cl::Required,cl::desc("output dir"));


bool SymbolizePass::doInitialization(Module &M) {
  DEBUG(errs() << "Symbolizer module init\n");

  // Redirect calls to external functions to the corresponding wrappers and
  // rename internal functions.
  for (auto &function : M.functions()) {
    auto name = function.getName();
    if (isInterceptedFunction(function)){
        auto newName = kInterceptedFunctionPrefix + name;
        function.setName(newName);
    }

  }
  r =  new Runtime(M);
  // Insert a constructor that initializes the runtime and any globals.
  Function *ctor;
  std::tie(ctor, std::ignore) = createSanitizerCtorAndInitFunctions(
      M, kSymCtorName, "_sym_initialize", {}, {});
  appendToGlobalCtors(M, ctor, 0);

  return true;
}

bool SymbolizePass::doFinalization(llvm::Module & m) {
    (void)m;
    delete r;
    return false;
}
bool SymbolizePass::runOnFunction(Function &F) {
    auto functionName = F.getName();
    if (functionName == kSymCtorName)
        return false;
    llvm::errs() << "Symbolizing function " << functionName << '\n';

    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

    PostDominatorTree& pdTree = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
    DominatorTree& dTree = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
    boost::filesystem::path dir (outDir.getValue());
    if(! boost::filesystem::exists(dir)){
        errs()<< outDir<<'\n';
        llvm_unreachable("output dir does not exist");
    }
    boost::filesystem::path ddgFile = dir / (F.getName() + "_dfg.dot").str();
    boost::filesystem::path cfgFile = dir / (F.getName() + "_cfg.dot").str();
    boost::filesystem::path postDomTreeFile = dir / (F.getName() + "_postDom.dot").str();
    boost::filesystem::path domTreeFile = dir / (F.getName() + "_dom.dot").str();
    boost::filesystem::path intermediateFile = dir / (F.getName() + "_intermediate.ll").str();



    SmallVector<Instruction *, 0> allInstructions;
    allInstructions.reserve(F.getInstructionCount());

    for (auto &I : instructions(F)){
        allInstructions.push_back(&I);
    }

    Symbolizer symbolizer(*F.getParent(),r, LI);
    symbolizer.initializeFunctions(F);

    for (auto &basicBlock : F){
        symbolizer.insertBasicBlockNotification(basicBlock);
    }
    symbolizer.outputCFG(F,dTree, pdTree, cfgFile.string(),domTreeFile.string(), postDomTreeFile.string());


    for (auto *instPtr : allInstructions){
        symbolizer.visit(instPtr);
    }
    symbolizer.finalizePHINodes();
    symbolizer.shortCircuitExpressionUses();
    // output some intermediate info for debugging purpose
    breakConstantExpr(F);
    std::error_code ec;
    raw_fd_ostream intermediate_file(StringRef(intermediateFile.string()),ec);
    symbolizer.DisplaySymbolicIDs(intermediate_file);
    intermediate_file << F<<'\n';

    // end of output intermediate info

    symbolizer.createDFGAndReplace(F,ddgFile.string());


    assert(!verifyFunction(F, &errs()) &&
         "SymbolizePass produced invalid bitcode");

    return true;
}
