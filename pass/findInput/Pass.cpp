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

#include <fstream>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>



using namespace llvm;

#ifndef NDEBUG
#define DEBUG(X)                                                               \
  do {                                                                         \
    X;                                                                         \
  } while (false)
#else
#define DEBUG(X) ((void)0)
#endif


#include <boost/filesystem.hpp>
#include <iostream>

char SymbolizeLegacyPass::ID = 0;


struct APIInfo {
    llvm::StringRef s;
    int paraIdx;
};

llvm::cl::opt<std::string> outDir("out", cl::Required,cl::desc("output dir"));
std::string functionName;

namespace {
    static constexpr char kSymCtorName[] = "__sym_ctor";
}


std::map<llvm::StringRef, APIInfo> inputConfigurations = {
        {"HAL_UARTEx_ReceiveToIdle_DMA",{"DMA",1}}
};




bool instrumentFunction(Function &F) {
    functionName = F.getName();

    boost::filesystem::path dir (outDir.getValue());
    if(! boost::filesystem::exists(dir)){
        errs()<< outDir<<'\n';
        llvm_unreachable("output dir does not exist");
    }

    for (auto &I : instructions(F)){
        if(CallInst * callInst = dyn_cast<CallInst>(&I)){
            auto callee = callInst->getCalledFunction();
            if(callee != nullptr){
                continue;
            }
            auto calleeName = callee->getName();
            for(auto it = inputConfigurations.begin(); it != inputConfigurations.end(); it ++){
                if(calleeName.equals(it->first)){
                    errs() << "Found:"<<it->second.s<<','<<callee->getArg(it->second.paraIdx)<<'\n';
                }
            }
        }
    }
    if(verifyFunction(F, &errs())){
        errs()<<F<<'\n';
        llvm_unreachable("SymbolizePass produced invalid bitcode");
    }
    return true;
}

bool SymbolizeLegacyPass::doInitialization(Module &) {
    return false;
}


bool SymbolizeLegacyPass::runOnFunction(Function &F) {
    
    return instrumentFunction(F);
}
#if LLVM_VERSION_MAJOR >= 13

PreservedAnalyses SymbolizePass::run(Function &F, FunctionAnalysisManager &) {
    instrumentFunction(F);
    return PreservedAnalyses::all();
}

PreservedAnalyses SymbolizePass::run(Module &, ModuleAnalysisManager &) {
    return PreservedAnalyses::all();
}

/*
 * void SymbolizePass::releaseMemory(){
    delete r;
    r = nullptr;
}*/

#endif