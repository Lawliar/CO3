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

#ifndef SYMBOLIZE_H
#define SYMBOLIZE_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Casting.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/PostDominators.h>

#include <optional>
#include <stdio.h>
#include <stdlib.h>

#include "dfg/DataFlowGraph.h"
#include "Runtime.h"


extern std::string functionName;

class Symbolizer : public llvm::InstVisitor<Symbolizer> {
public:
  explicit Symbolizer(llvm::Module &M, llvm::LoopInfo& LI);
  ~Symbolizer();

  /// Insert code to obtain the symbolic expressions for the function arguments.
  void initializeFunctions(llvm::Function &F);

  /// Insert a call to the run-time library to notify it of the basic block
  /// entry.
  void recordBasicBlockMapping(llvm::BasicBlock &B);

  /// Finish the processing of PHI nodes.
  ///
  /// This assumes that there is a dummy PHI node for each such instruction in
  /// the function, and that we have recorded all PHI nodes in the member
  /// phiNodes. In other words, the function has to be called after all
  /// instructions have been processed in order to fix up PHI nodes. See the
  /// documentation of member phiNodes for why we process PHI nodes in two
  /// steps.
  ///
  /// Important! Calling this function invalidates symbolicExpressions.
  void finalizePHINodes();

  /// Rewrite symbolic computation to only occur if some operand is symbolic.
  ///
  /// We don't want to build up formulas for symbolic computation if all
  /// operands are concrete. Therefore, this function rewrites all places that
  /// build up formulas (as recorded during the main pass) to skip formula
  /// construction if all operands are concrete. Additionally, it inserts code
  /// that constructs formulas for concrete operands if necessary.
  ///
  /// The basic idea is to transform code like this...
  ///
  ///   res_expr = call _sym_some_computation(expr1, expr2, ...)
  ///   res      = some_computation(val1, val2, ...)
  ///
  /// ...into this:
  ///
  ///   start:
  ///   expr1_symbolic = icmp ne 0, expr1
  ///   ...
  ///   some_symbolic = or expr1_symbolic, ...
  ///   br some_symbolic, check_arg1, end
  ///
  ///   check_arg1:
  ///   need_expr1 = icmp eq 0, expr1
  ///   br need_expr1, create_expr1, check_arg2
  ///
  ///   create_expr1:
  ///   new_expr1 = ... (based on val1)
  ///   br check_arg2
  ///
  ///   check_arg2:
  ///   good_expr1 = phi [expr1, check_arg1], [new_expr1, create_expr1]
  ///   need_expr2 = ...
  ///   ...
  ///
  ///   sym_computation:
  ///   sym_expr = call _sym_some_computation(good_expr1, good_expr2, ...)
  ///   br end
  ///
  ///   end:
  ///   final_expr = phi [null, start], [sym_expr, sym_computation]
  ///
  /// The resulting code is much longer but avoids solver calls for all
  /// operations without symbolic data.
  void shortCircuitExpressionUses();

  void handleIntrinsicCall(llvm::CallBase &I);
  void handleInlineAssembly(llvm::CallInst &I);
  void handleFunctionCall(llvm::CallBase &I, llvm::Instruction *returnPoint);

    SymDepGraph::vertex_t addConstantIntVertice(llvm::ConstantInt*);
    SymDepGraph::vertex_t addConstantFloatVertice(llvm::ConstantFP*);
    SymDepGraph::vertex_t addRuntimeVertice(llvm::Value*, unsigned);
  void createDFGAndReplace(llvm::Function&,std::string);
  void insertNotifyBasicBlock(llvm::Function&);
    void RecursivePrintEdges(std::map<llvm::BasicBlock*, unsigned long>& basicBlockMap, llvm::raw_fd_ostream & O, llvm::DomTreeNodeBase<llvm::BasicBlock> * root, unsigned level);
  void OutputCFG(llvm::Function&,llvm::DominatorTree&, llvm::PostDominatorTree&,std::string,std::string,std::string);
  //
  // Implementation of InstVisitor
  //
  void visitBinaryOperator(llvm::BinaryOperator &I);
  void visitSelectInst(llvm::SelectInst &I);
  void visitCmpInst(llvm::CmpInst &I);
  void visitReturnInst(llvm::ReturnInst &I);
  void visitBranchInst(llvm::BranchInst &I);
  void visitIndirectBrInst(llvm::IndirectBrInst &I);
  void visitCallInst(llvm::CallInst &I);
  void visitInvokeInst(llvm::InvokeInst &I);
  void visitAllocaInst(llvm::AllocaInst &);
  void visitLoadInst(llvm::LoadInst &I);
  void visitStoreInst(llvm::StoreInst &I);
  void visitGetElementPtrInst(llvm::GetElementPtrInst &I);
  void visitBitCastInst(llvm::BitCastInst &I);
  void visitTruncInst(llvm::TruncInst &I);
  void visitIntToPtrInst(llvm::IntToPtrInst &I);
  void visitPtrToIntInst(llvm::PtrToIntInst &I);
  void visitSIToFPInst(llvm::SIToFPInst &I);
  void visitUIToFPInst(llvm::UIToFPInst &I);
  void visitFPExtInst(llvm::FPExtInst &I);
  void visitFPTruncInst(llvm::FPTruncInst &I);
  void visitFPToSI(llvm::FPToSIInst &I);
  void visitFPToUI(llvm::FPToUIInst &I);
  void visitCastInst(llvm::CastInst &I);
  void visitPHINode(llvm::PHINode &I);
  void visitInsertValueInst(llvm::InsertValueInst &I);
  void visitExtractValueInst(llvm::ExtractValueInst &I);
  void visitSwitchInst(llvm::SwitchInst &I);
  void visitUnreachableInst(llvm::UnreachableInst &);
  void visitInstruction(llvm::Instruction &I);

//private:
  static constexpr unsigned kExpectedMaxPHINodesPerFunction = 16;
  static constexpr unsigned kExpectedSymbolicArgumentsPerComputation = 2;
  /// A symbolic input.
  struct Input {
    llvm::Value *concreteValue;
    unsigned operandIndex;
    llvm::Instruction *user;

    llvm::Value *getSymbolicOperand() const {
      return user->getOperand(operandIndex);
    }

    void replaceOperand(llvm::Value *newOperand) {
      user->setOperand(operandIndex, newOperand);
    }
  };

  /// A symbolic computation with its inputs.
  struct SymbolicComputation {
    llvm::Instruction *firstInstruction = nullptr, *lastInstruction = nullptr;
    llvm::SmallVector<Input, kExpectedSymbolicArgumentsPerComputation> inputs;

    SymbolicComputation() = default;

    SymbolicComputation(llvm::Instruction *first, llvm::Instruction *last,
                        llvm::ArrayRef<Input> in)
        : firstInstruction(first), lastInstruction(last),
          inputs(in.begin(), in.end()) {}

    /// Append another symbolic computation to this one.
    ///
    /// The computation that is to be appended must occur after the one that
    /// this method is called on.
    void merge(const SymbolicComputation &other) {
      if (&other == this)
        return;

      if (firstInstruction == nullptr)
        firstInstruction = other.firstInstruction;
      lastInstruction = other.lastInstruction;

      for (const auto &input : other.inputs)
        inputs.push_back(input);
    }

    friend llvm::raw_ostream &
    operator<<(llvm::raw_ostream &out,
               const Symbolizer::SymbolicComputation &computation) {
      out << "\nComputation starting at " << *computation.firstInstruction
          << "\n...ending at " << *computation.lastInstruction
          << "\n...with inputs:\n";
      for (const auto &input : computation.inputs) {
        out << '\t' << input.operandIndex<<',' <<*input.concreteValue << '\n';
      }
      return out;
    }
  };

    /// Create an expression that represents the concrete value.
    llvm::CallInst *createValueExpression(llvm::Value *V, llvm::IRBuilder<> &IRB);

    /// Get the (already created) symbolic expression for a value.

    // What is different from SymCC is that,
    //  "not having a symbolic value" is also gonna have a value and it will be a constant false
    llvm::Value *getSymbolicExpression(llvm::Value *V) {
        auto exprIt = symbolicExpressions.find(V);
        return (exprIt != symbolicExpressions.end()) ? exprIt->second : llvm::ConstantInt::getFalse(V->getContext());
    }



    unsigned getSymIDFromSym(llvm::Value *V) {
        unsigned retSymID = 0;
        if(auto nonPhiSymExpr = llvm::dyn_cast<llvm::CallInst>(V)){
            retSymID = getSymIDFromSymExpr(nonPhiSymExpr);
        }else if(auto phiSymExpr = llvm::dyn_cast<llvm::PHINode>(V)){
            retSymID = getSymIDFromSymPhi(phiSymExpr);
        }else if(auto symConst = llvm::dyn_cast<llvm::ConstantInt>(V); V->getType() == isSymType && symConst != nullptr && symConst->isZero() ){
            return 0;
        }else{
            llvm::errs()<<*V<<'\n';
            llvm::errs()<< * llvm::cast<llvm::Instruction>(V)->getFunction()<<'\n';
            llvm_unreachable("sym expr can only be of phiNode or call inst or constant false");
        }
        return retSymID;
    }
    unsigned int getSymIDFromSymExpr(llvm::CallInst *V) {
        auto exprIt = symbolicIDs.find(V);
        assert(exprIt != symbolicIDs.end());
        return exprIt->second;
    }
    unsigned int getSymIDFromSymPhi(llvm::PHINode * phi){
        auto it = phiSymbolicIDs.find(phi);
        if(it == phiSymbolicIDs.end()){
            llvm::errs()<<*phi <<"does not have a sym id\n";
            llvm_unreachable("");
        }
        return it->second->symid;
    }
    void replaceAllUseWith(llvm::Instruction * i, llvm::Value * v){
        unsigned numUser1 = 0;
        unsigned numUser2 = 0;
        while( ! i->use_empty()) {
            llvm::User* u = i->user_back();
            u->replaceUsesOfWith(i, v);
            numUser1++;
        }
        /*
        for(auto eachUser = i->user_begin(); eachUser != i->user_end(); eachUser++){
            eachUser->replaceUsesOfWith(i,v);
            numUser1++;
        }*/
        for(auto eachUser = i->user_begin(); eachUser != i->user_end(); eachUser++){
            numUser2++;
        }
        if( !i->user_empty()){
            llvm::errs()<<numUser1<<'|'<<numUser2<<'\n';
            llvm_unreachable("user not empty after replaced");
        }
    }
    bool isSymStatusType(unsigned arg_idx, llvm::StringRef calleename){
        if(runtime.isSymArgNo.find(calleename.str()) != runtime.isSymArgNo.end()){
            auto isSymArgs = runtime.isSymArgNo.at(calleename.str());
            if(std::find(isSymArgs.begin(), isSymArgs.end(), arg_idx) != isSymArgs.end()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    bool isSkippedArg(unsigned arg_idx, llvm::StringRef calleename){
        if(runtime.skipArgNo.find(calleename.str()) != runtime.skipArgNo.end()){
            auto skipArgs = runtime.skipArgNo.at(calleename.str());
            if(std::find(skipArgs.begin(), skipArgs.end(), arg_idx) != skipArgs.end()){
                return true;
            }else{
                return false;
            }
        } else{
            return false;
        }
    }
    bool isConstantType(unsigned arg_idx, llvm::StringRef calleename){
        if(runtime.constArgNo.find(calleename.str()) != runtime.constArgNo.end()){
            auto constant_args = runtime.constArgNo.at(calleename.str());
            if(std::find(constant_args.begin(), constant_args.end(), arg_idx) != constant_args.end()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    bool isRuntimeType(unsigned arg_idx, llvm::StringRef calleename){
        if(runtime.runtimeArgNo.find(calleename.str()) != runtime.runtimeArgNo.end()){
            auto runtime_args = runtime.runtimeArgNo.at(calleename.str());
            if(std::find(runtime_args.begin(), runtime_args.end(), arg_idx) != runtime_args.end()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    bool isSymIdType(unsigned arg_idx, llvm::StringRef calleename){
        if(runtime.symIdArgNo.find(calleename.str()) != runtime.symIdArgNo.end()){
            auto symid_args = runtime.symIdArgNo.at(calleename.str());
            if(std::find(symid_args.begin(), symid_args.end(), arg_idx) != symid_args.end()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    bool isInterpretedFunc(llvm::StringRef f){
        for(auto each_f : interpretedFunctionNames){
            if(each_f.equals(f))
                return true;
        }
        return false;
    }
    unsigned availableSymID = 1;

    unsigned int getNextID(){
        unsigned id;
        id = availableSymID;
        availableSymID++;
        if(availableSymID > maxNumSymVars){
            llvm_unreachable("current function has too many in memory sym variables");
        }
        //if(id == 29 && functionName == "modbusparsing"){
        //    __asm__("nop");
        //}
        return id;
    }
    void addRedirect(std::vector<unsigned int>& ids ){
        unsigned int symidR = ids.at(0);
        for(auto eachId : ids){
            symIdRedirect.insert(std::make_pair(eachId, symidR));
        }
    }
    unsigned int getNextBBID(){
        unsigned id;
        id = BBID;
        BBID++;
        if(BBID > 65535){
            llvm_unreachable("current function has too many basic blocks");
        }
        return id;
    }
    unsigned int getNextCallID(){
        unsigned id;
        id = callID;
        callID++;
        if(callID > 255){
            llvm_unreachable("current function has too many call inst");
        }
        return id;
    }
    void assignSymID(llvm::CallInst * symcall, unsigned int ID){
        auto exprIt = symbolicIDs.find(symcall);
        assert(exprIt == symbolicIDs.end());
        symbolicIDs[symcall] = ID;
    }

    class TryAlternativeUnit{
    public:
        int symID;
        unsigned BBID;
        llvm::Value* symExpr;
        llvm::Value* concreteExpr;
        TryAlternativeUnit(int symID, unsigned BBID, llvm::Value* symExpr,llvm::Value* concreteExpr): \
            symID(symID), BBID(BBID), symExpr(symExpr), concreteExpr(concreteExpr){}

    };

    class PhiStatus{
    public:
        enum PhiStatusKind{
            PS_TruePhi,
            PS_FalsePhiRoot,
            PS_FalsePhiLeaf
        };
    private:
        const PhiStatusKind Kind;
    public:
        PhiStatusKind getKind() const{return Kind;}
        unsigned symid;
        PhiStatus(PhiStatusKind K, unsigned symid): Kind(K), symid(symid){}
        virtual ~PhiStatus() {}
        friend llvm::raw_ostream & operator<<(llvm::raw_ostream &out, PhiStatus const& data) {
            out << data.symid<<",";
            switch (data.getKind()) {
                case PhiStatus::PS_TruePhi:
                    out << "TruePhi";
                    break;
                case PhiStatus::PS_FalsePhiRoot:
                    out << "FalsePhiRoot";
                    break;
                case PhiStatus::PS_FalsePhiLeaf:
                    out << "FalsePhiLeaf";
                    break;
                default:
                    break;
            }
            return out;
        }
    };
    class TruePhi: public PhiStatus{
    public:
        TruePhi(unsigned symid): PhiStatus(PS_TruePhi,symid){};
        static bool classof(const PhiStatus *p) {
            return p->getKind() == PS_TruePhi;
        }
    };
    class FalsePhiRoot: public PhiStatus{
    public:
        std::set<unsigned> leaves;
        FalsePhiRoot(unsigned symid, std::set<unsigned> leaves) : PhiStatus(PS_FalsePhiRoot, symid), leaves(leaves){};
        static bool classof(const PhiStatus *p) {
            return p->getKind() == PS_FalsePhiRoot;
        }
    };
    class FalsePhiLeaf: public PhiStatus{
    public:
        std::set<unsigned> peersOriginal;
        unsigned originalValueSymId;
        FalsePhiLeaf(unsigned symid, unsigned orig): PhiStatus(PS_FalsePhiLeaf,symid), originalValueSymId(orig){};
        static bool classof(const PhiStatus *p) {
            return p->getKind() == PS_FalsePhiLeaf;
        }
        void InsertPeer(unsigned p){
            if(p != originalValueSymId){
                peersOriginal.insert(p);
            }
        }
    };

    bool isStaticallyConcrete(llvm::Value*);
    void assignSymIDPhi(llvm::PHINode* symPhi, PhiStatus* phiStatus);
    void deleteSymPhi(llvm::PHINode* symPhi);
    void setCallInstId(llvm::CallInst* notifyCall, llvm::ConstantInt* con){
        assert(callToCallId.find(notifyCall) == callToCallId.end());
        callToCallId[notifyCall] = con;
    }
    void addSetParaToNotifyCall(llvm::CallInst* notifyCall, llvm::CallInst* setPara){
        if( callToSetParaMap.find(notifyCall) == callToSetParaMap.end() ){
            callToSetParaMap[notifyCall] = llvm::SmallVector<llvm::CallInst*, 8>();
            callToSetParaMap[notifyCall].push_back(setPara);
        }else{
            callToSetParaMap[notifyCall].push_back(setPara);
        }
    }
    bool isLittleEndian(llvm::Type *type) {
        return (!type->isAggregateType() && dataLayout.isLittleEndian());
    }
    llvm::BasicBlock* findExistingBB(unsigned,std::set<llvm::BasicBlock*>&);
    void outputDebugCFG(llvm::Function*);
    unsigned GetBBID(llvm::BasicBlock* BB){
        llvm::BasicBlock* realOriginal = BB;
        while(splited2OriginalBB.find(realOriginal) != splited2OriginalBB.end()){
            realOriginal = splited2OriginalBB.at(realOriginal);
        }
        return originalBB2ID.at(realOriginal);
    }

    void insertPerBBConcretenessCheck(unsigned BBID, llvm::Value * checking);
    void MapOriginalBlock(llvm::BasicBlock * splitted, llvm::BasicBlock* original){
        llvm::BasicBlock* realOriginal = original;
        while(splited2OriginalBB.find(realOriginal) != splited2OriginalBB.end()){
            realOriginal = splited2OriginalBB.at(realOriginal);
        }
        if( originalBB2ID.find(realOriginal) == originalBB2ID.end()){
            llvm::errs()<<*original<<'\n'<<*original->getParent()<<'\n';
            llvm_unreachable("original BB does not have a ID");
        }

        splited2OriginalBB[splitted] = original;
    }
  llvm::ConstantInt* ConstantHelper(llvm::IntegerType * ty, unsigned int id){
        return llvm::ConstantInt::get(ty,id);
    }
  /// Like buildRuntimeCall, but the call is always generated.
  SymbolicComputation
  forceBuildRuntimeCall(llvm::IRBuilder<> &IRB, SymFnT function,
                        llvm::ArrayRef<std::pair<llvm::Value *, bool>> args);

  /// Create a call to the specified function in the run-time library.
  ///
  /// Each argument is specified as a pair of Value and Boolean. The Boolean
  /// specifies whether the Value is a symbolic argument, in which case the
  /// corresponding symbolic expression will be passed to the run-time function.
  /// Moreover, the use of symbolic expressions will be recorded in the
  /// resulting SymbolicComputation. If all symbolic arguments are known to be
  /// concrete (e.g., be cause they are compile-time constants), no call
  /// instruction is emitted and the function returns null.
  std::optional<SymbolicComputation>
  buildRuntimeCall(llvm::IRBuilder<> &IRB, SymFnT function,
                   llvm::ArrayRef<std::pair<llvm::Value *, bool>> args) {
    if (std::all_of(args.begin(), args.end(),
                    [this](std::pair<llvm::Value *, bool> arg) {
                      return (getSymbolicExpression(arg.first) == constFalse );
                    })) {
      return {};
    }

    return forceBuildRuntimeCall(IRB, function, args);
  }

  /// Convenience overload that treats all arguments as symbolic.
  std::optional<SymbolicComputation>
  buildRuntimeCall(llvm::IRBuilder<> &IRB, SymFnT function,
                   llvm::ArrayRef<llvm::Value *> symbolicArgs) {
    std::vector<std::pair<llvm::Value *, bool>> args;
    for (const auto &arg : symbolicArgs) {
      args.emplace_back(arg, true);
    }

    return buildRuntimeCall(IRB, function, args);
  }

  /// Register the result of the computation as the symbolic expression
  /// corresponding to the concrete value and record the computation for
  /// short-circuiting.
  void registerSymbolicComputation(const SymbolicComputation &computation,
                                   llvm::Value *concrete = nullptr) {
    if (concrete != nullptr)
      symbolicExpressions[concrete] = computation.lastInstruction;
    expressionUses.push_back(computation);
  }

  /// Convenience overload for chaining with buildRuntimeCall.
  void registerSymbolicComputation(
      const std::optional<SymbolicComputation> &computation,
      llvm::Value *concrete = nullptr) {
    if (computation)
      registerSymbolicComputation(*computation, concrete);
  }

  /// Generate code that makes the solver try an alternative value for V.
  void tryAlternative(llvm::IRBuilder<> &IRB, llvm::Value *V);

  /// Helper to use a pointer to a host object as integer (truncating!).
  ///
  /// Note that the conversion will truncate the most significant bits of the
  /// pointer if the host uses larger addresses than the target. Therefore, use
  /// this function only when such loss is acceptable (e.g., when generating
  /// site identifiers to be passed to the backend, where collisions of the
  /// least significant bits are reasonably unlikely).
  ///
  /// Why not do a lossless conversion and make the backend accept 64-bit
  /// integers?
  ///
  /// 1. Performance: 32-bit architectures will process 32-bit values faster
  /// than 64-bit values.
  ///
  /// 2. Pragmatism: Changing the backend to accept and process 64-bit values
  /// would require modifying code that we don't control (in the case of Qsym).
  llvm::ConstantInt *getTargetPreferredInt(void *pointer) {
    return llvm::ConstantInt::get(intPtrType,
                                  reinterpret_cast<uint64_t>(pointer));
  }
  void DisplaySymbolicIDs(llvm::raw_fd_ostream& output){
      for(auto eachSymOp : symbolicIDs){
          output<<eachSymOp->second<<"|"<<*eachSymOp->first<<'\n';
      }
      for(auto eachSymOp : phiSymbolicIDs) {
          output << eachSymOp.second->symid << "|" << *eachSymOp.first << "|";
          if (llvm::isa<TruePhi>(eachSymOp.second)) {
              output << "true,";
          } else{
              output << "false,";
          }
          if(llvm::isa<FalsePhiRoot>(eachSymOp.second)){
              output << "root\n";
          }else{
              output << "leaf\n";
          }
      }
      for(auto it = splited2OriginalBB.begin(); it != splited2OriginalBB.end();it++){
          output<< "BB:"<<it->first->getName() <<"->BB"<<it->second->getName()<<'\n';
      }
    /*
      for(auto eachTryAlternative : tryAlternatives){
          auto tryAltSymId = eachTryAlternative->symID;
          auto tryAltBBid = eachTryAlternative->BBID;
          auto operandSym = eachTryAlternative->symExpr;
          auto operand = eachTryAlternative->concreteExpr;
          output<<"tryAlt:symid:"<<tryAltSymId<<",bbid:"<<tryAltBBid<<",symop:"<<*operandSym<<",op:"<<*operand<<'\n';
      }*/
  }
    unsigned numBits2NumBytes(unsigned numBits);

  /// Compute the offset of a member in a (possibly nested) aggregate.
  uint64_t aggregateMemberOffset(llvm::Type *aggregateType,
                                 llvm::ArrayRef<unsigned> indices) const;
  void addTryAlternativeToTheGraph();
  const Runtime runtime;
  const llvm::LoopInfo& loopinfo;
  /// The data layout of the currently processed module.
  const llvm::DataLayout &dataLayout;

  /// The width in bits of pointers in the module.
  unsigned ptrBits;
  unsigned ptrBytes;
  /// max number in-register symbolic variables
  const unsigned maxNumSymVars;
  /// An integer type at least as wide as a pointer.
  llvm::IntegerType *intPtrType = nullptr;
  llvm::IntegerType * archIntType = nullptr;
  llvm::Type * voidType = nullptr;
  llvm::IntegerType * int8Type = nullptr;
  llvm::IntegerType * isSymType = nullptr;
  llvm::IntegerType * symIntType = nullptr;
  llvm::ConstantInt * constFalse = nullptr;
  /// Mapping from SSA values to symbolic expressions.
  ///
  /// For pointer values, the stored value is an expression describing the value
  /// of the pointer itself (i.e., the address, not the referenced value). For
  /// structure values, the expression is a single large bit vector.
  ///
  /// TODO This member adds a lot of complexity: various methods rely on it, and
  /// finalizePHINodes invalidates it. We may want to pass the map around
  /// explicitly.
  llvm::ValueMap<llvm::Value *, llvm::Value *> symbolicExpressions;
  /// Maps symbolic value to its IDs
  llvm::ValueMap<llvm::CallInst *, unsigned int> symbolicIDs;
  /// Maps phi nodes to its IDs
  std::map<llvm::PHINode* , PhiStatus* > phiSymbolicIDs;
  // redirect the symID into a new one(as we are finer-grained by symcc(e.g, they didn't distinguish different branches for the SwitchInst, but we need to))
  std::map<unsigned int, unsigned int> symIdRedirect;
  //map call inst to its corresponding set paras
  std::map<llvm::CallInst*, llvm::ConstantInt* > callToCallId;
  std::map<llvm::CallInst*, llvm::SmallVector<llvm::CallInst*, 8> > callToSetParaMap;
  /// A record of all PHI nodes in this function.
  ///
  /// PHI nodes may refer to themselves, in which case we run into an infinite
  /// loop when trying to generate symbolic expressions recursively. Therefore,
  /// we only insert a dummy symbolic expression for each PHI node and fix it
  /// after all instructions have been processed.
  llvm::SmallVector<llvm::PHINode *, kExpectedMaxPHINodesPerFunction> phiNodes;

  /// A record of expression uses that can be short-circuited.
  ///
  /// Most values in a program are concrete, even if they're not constant (in
  /// which case we would know that they're concrete at compile time already).
  /// There is no point in building up formulas if all values involved in a
  /// computation are concrete, so we short-circuit those cases. Since this
  /// process requires splitting basic blocks, we can't do it during the main
  /// analysis phase (because InstVisitor gets out of step if we try).
  /// Therefore, we keep a record of all the places that construct expressions
  /// and insert the fast path later.
    std::vector<SymbolicComputation> expressionUses;
    std::map<llvm::BasicBlock*, llvm::BasicBlock*> splited2OriginalBB;
    std::map<llvm::BasicBlock*, unsigned> originalBB2ID;
    std::map<unsigned, std::vector<llvm::Value*> > perBBConcretenessChecking;
    std::vector<unsigned> stageSettingOperations;

    // map loopBB and phi to an bit
    llvm::Value* loopBBBaseAddr = nullptr;
    llvm::Value* truePhiBaseAddr = nullptr;
    llvm::Value* selectBaseAddr = nullptr;
    std::map<llvm::BasicBlock*, unsigned> loopBB2Offset;
    unsigned truePhiOff = 0;
    std::map<llvm::PHINode*, unsigned> truePhi2Offset;
    unsigned selectInstOff = 0;
    //std::map<llvm::CallInst*, unsigned> selectInst2Off;

    //std::map<std::pair<unsigned, unsigned>, std::pair<llvm::Value*, llvm::Value *> > tryAlternativePairs;
    std::set<TryAlternativeUnit*> tryAlternatives;

    SymDepGraph g;
    const unsigned initialBBID = 1;
    unsigned int BBID = initialBBID;
    unsigned int callID = 1;
    std::set<llvm::StringRef> interpretedFunctionNames;
    void addSymIDToCall(llvm::CallBase&);
    void interpretedFuncSanityCheck(llvm::CallBase&);
    void insertNotifyFunc(llvm::Function& func, std::string file_name);
};

#endif
