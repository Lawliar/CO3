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

#include "Symbolizer.h"
#include "Lower.h"

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

llvm::cl::opt<std::string> outDir("out", cl::Required,cl::desc("output dir"));
//llvm::cl::opt<std::string> channel("channel", cl::Required,cl::desc("which input channel to use"));
std::string functionName;

namespace {
    static constexpr char kSymCtorName[] = "__sym_ctor";

    bool instrumentModule(Module &M) {
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

        // Insert a constructor that initializes the runtime and any globals.
        Function *ctor;
        std::tie(ctor, std::ignore) = createSanitizerCtorAndInitFunctions(
                M, 
                kSymCtorName, 
#if defined(CO3_MCUS)
                "_sym_initialize", 
#else
                "_sym_sanity_check", 
#endif
                {}, {});
        appendToGlobalCtors(M, ctor, 0);

        return true;
    }
}

bool instrumentFunction(Function &F, LoopInfo &LI, PostDominatorTree& pdTree,DominatorTree& dTree) {
    functionName = F.getName();
    if (functionName == kSymCtorName)
        return false;
    llvm::errs() << "Symbolizing function " << functionName << '\n';


    boost::filesystem::path dir (outDir.getValue());
    if(! boost::filesystem::exists(dir)){
        boost::filesystem::create_directory(dir);
        //errs()<< outDir<<'\n';
        //llvm_unreachable("output dir does not exist");
    }
    boost::filesystem::path dfgFile = dir / (F.getName() + "_dfg.dot").str();
    boost::filesystem::path cfgFile = dir / (F.getName() + "_cfg.dot").str();
    boost::filesystem::path postDomTreeFile = dir / (F.getName() + "_postDom.dot").str();
    boost::filesystem::path domTreeFile = dir / (F.getName() + "_dom.dot").str();
    boost::filesystem::path intermediateFile = dir / (F.getName() + "_intermediate.ll").str();
    boost::filesystem::path funcIDFile = dir / "spear_func_id.txt";
    if(!boost::filesystem::exists(funcIDFile)){
        std::ofstream touchfile(funcIDFile.string());
        touchfile.close();
    }

    SmallVector<Instruction *, 0> allInstructions;
    allInstructions.reserve(F.getInstructionCount());

    for (auto &I : instructions(F)){
        allInstructions.push_back(&I);
    }

    Symbolizer symbolizer(*F.getParent(), LI);
    symbolizer.initializeFunctions(F);

    for (auto &basicBlock : F){
        symbolizer.recordBasicBlockMapping(basicBlock);
    }
    symbolizer.OutputCFG(F,dTree, pdTree, cfgFile.string(),domTreeFile.string(), postDomTreeFile.string());


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

    symbolizer.createDFGAndReplace(F,dfgFile.string());


    symbolizer.insertNotifyFunc(F, funcIDFile.string());
    symbolizer.insertNotifyBasicBlock(F);
#if defined(CO3_MCU)
#else
    if(F.getName() == "main"){
        symbolizer.ProEpiLogue(F);
    }
#endif

    if(verifyFunction(F, &errs())){
        errs()<<F<<'\n';
        llvm_unreachable("SymbolizePass produced invalid bitcode");
    }
    return true;
}

bool SymbolizeLegacyPass::doInitialization(Module &M) {
    return instrumentModule(M);
}


bool SymbolizeLegacyPass::runOnFunction(Function &F) {
    //if(r == nullptr){
    //    llvm_unreachable("Runtime is not initialized.");
    //}
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

    PostDominatorTree& pdTree = getAnalysis<PostDominatorTreeWrapperPass>().getPostDomTree();
    DominatorTree& dTree = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
    return instrumentFunction(F,LI, pdTree, dTree);
}
/*
void SymbolizeLegacyPass::releaseMemory() {
    delete r;
    r = nullptr;
}*/
#if LLVM_VERSION_MAJOR >= 13

PreservedAnalyses SymbolizePass::run(Function &F, FunctionAnalysisManager & FAM) {
    //if(r == nullptr){
    //    llvm_unreachable("Runtime is not initialized.");
    //}
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    PostDominatorTree& pdTree = FAM.getResult<PostDominatorTreeAnalysis>(F);
    DominatorTree& dTree = FAM.getResult<DominatorTreeAnalysis>(F);
    return instrumentFunction(F, LI, pdTree, dTree) ? PreservedAnalyses::none()
                                 : PreservedAnalyses::all();
}

PreservedAnalyses SymbolizePass::run(Module &M, ModuleAnalysisManager &) {
    return instrumentModule(M) ? PreservedAnalyses::none()
                               : PreservedAnalyses::all();
}

/*
 * void SymbolizePass::releaseMemory(){
    delete r;
    r = nullptr;
}*/

#endif