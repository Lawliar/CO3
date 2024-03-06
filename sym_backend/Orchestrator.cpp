//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"
#include <iostream>
#include <fstream>
#include "getTimeStamp.h"
#include "Config.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

std::string dbgUsbFileName;


#ifdef DEBUG_OUTPUT
int indent = 0;
int indentNum = 4;
#endif


extern WriteShadowIteratorDR * DR_INPUT;


#if defined(CO3_SER2NET)
Orchestrator::Orchestrator(std::string inputDir, std::string sp_port, int baud_rate):ser(initSer(sp_port.c_str())),pool(2),msgQueue(ser)
#else
Orchestrator::Orchestrator(std::string inputDir, std::string sp_port, int baud_rate):ser(initSer(sp_port.c_str(), baud_rate)),pool(2),msgQueue(ser)
#endif

{
    _sym_initialize_config(inputDir);
    boost::filesystem::path dir (inputDir);
    boost::filesystem::path funcIDFilePath = dir / "spear_func_id.txt";

    boost::filesystem::path usbFilePath = dir / "fileUSB.bin";


    boost::filesystem::path inputFile = g_config.inputFile;
    if(! boost::filesystem::exists(usbFilePath)){
        cerr<<" fileUSB.bin file does not exist, even if you don't use file for USB, please create an empty file there.";
        assert(false);
    }
    if(!boost::filesystem::exists(inputFile)){
        cerr<<"input file does not exist";
        assert(false);
    }
    symInputFile = inputFile.string();

    dbgUsbFileName.append(usbFilePath.string());
    /*
    ofstream ofs;
    ofs.open("tmpOut.txt");
    ofs << "g_config.inputFile:"<<g_config.inputFile<<'\n';
    ofs << "g_config.outputDir:"<<g_config.outputDir<<'\n';
    ofs <<"symInputFile"<<symInputFile<<'\n';
    ofs.close();
    */

    if(!boost::filesystem::exists(funcIDFilePath)){
        cerr<<"func id file does not exist";
        assert(false);
    }

    std::ifstream funcIDFile (funcIDFilePath);
    assert(funcIDFile.is_open());
    string line;
    while ( getline (funcIDFile,line) )
    {
        size_t tab_pos = line.find('\t');
        assert(tab_pos != std::string::npos);
        std::string cur_funcname =  line.substr(0, tab_pos);
        unsigned cur_id = stoi(line.substr(tab_pos + 1));

        boost::filesystem::path cfg_file = dir / (cur_funcname + "_cfg.dot");
        if(!boost::filesystem::exists(cfg_file)){
            cerr << cfg_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path dom_file = dir / (cur_funcname + "_dom.dot");
        if(!boost::filesystem::exists(dom_file)){
            cerr << dom_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path postDom_file = dir / (cur_funcname + "_postDom.dot");
        if(!boost::filesystem::exists(postDom_file)){
            cerr << postDom_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path dfg_file = dir / (cur_funcname + "_dfg.dot");
        if(!boost::filesystem::exists(dfg_file)){
            cerr << dfg_file.string() <<" does not exist";
            assert(false);
        }
#ifdef DEBUG_OUTPUT
        cout << "processing func:"<< cur_funcname<<'\n';
        cout.flush();
#endif
        SymGraph* cur_symgraph = new SymGraph(cur_funcname, cfg_file.string(),\
                                        dom_file.string(),postDom_file.string(),\
                                        dfg_file.string());
        symGraphs[cur_id] = cur_symgraph;
        vanillaSymGraphs[cur_id] = new SymGraph(*cur_symgraph);
    }
}
Orchestrator::~Orchestrator() {
    releaseSer(ser);
    for(auto eachFunc :symGraphs){
        delete eachFunc.second;
    }
    symGraphs.clear();
    for(auto eachFunc :vanillaSymGraphs){
        delete eachFunc.second;
    }
    vanillaSymGraphs.clear();
}

SymGraph* Orchestrator::getCurFunc() {
    return callStack.top();
}

void Orchestrator::UpdateCallStackHashBB(Val::BasicBlockIdType cur_bbid) {
    if(cur_bbid != lastBBID){
        _sym_notify_basic_block(cur_bbid);
        lastBBID = cur_bbid;
    }
}


void Orchestrator::UpdateCallStackHashCall(unsigned funcID) {
    _sym_notify_call(funcID);
}

void Orchestrator::UpdateCallStackRet(unsigned funcID) {
    _sym_notify_ret(funcID);
}
void Orchestrator::ExecuteBasicBlock(Val::BasicBlockIdType bbid) {
    SymGraph* cur_func = getCurFunc();
    UpdateCallStackHashBB(bbid);
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ')<<"Executing BB:"<<bbid<<'\n';
    cout.flush();
#endif
#ifdef DEBUG_CHECKING
    if(cur_func->funcname == "process_FC1" and bbid == 1){
        __asm__("nop");
    }
#endif
    cur_func->RefreshBBTask(bbid, cur_func->bbTasks.at(bbid)->ready + 1);
    auto bbTask = cur_func->bbTasks.at(bbid);

/*
    for(auto eachNonReadyPostDomBB : bbTask->nonReadyPostDominance){
        if(eachNonReadyPostDomBB == bbTask->BBID){
            continue;
        }
        ExecuteBasicBlock(eachNonReadyPostDomBB);
    }
*/
    for(auto eachNonReadyRoot : bbTask->nonReadyRoots){
        if(auto tmpSymVal = dynamic_cast<SymVal*>(eachNonReadyRoot); tmpSymVal != nullptr){
            if(!tmpSymVal->isThisNodeReady(tmpSymVal, bbTask->ready + 1)){
                BackwardExecution(tmpSymVal, bbTask->ready + 1);
            }
        }else if(auto tmpRuntime = dynamic_cast<RuntimeVal*>(eachNonReadyRoot);  tmpRuntime != nullptr){
            // these runtimeVal is used by instructions located in other basicBlocks
            // those instructions will take care of these by themselves
            //tmpRuntime->Unassign();
        }else{
            cerr<<"Const should not be here";
            assert(false);
        }
    }


    assert(bbTask->isBBReady());
    bbTask->ready ++;

#ifdef DEBUG_OUTPUT
    cout << string(indent, ' ') << "finish Executing BB:"<<bbid<<'\n';
    cout.flush();
    indent -= indentNum;
#endif
}

bool Orchestrator::ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot *falsePhiRoot, Val::ReadyType targetReady) {
    if (falsePhiRoot->falsePhiLeaves.size() == 0){
        falsePhiRoot->symExpr = nullptr;
        falsePhiRoot->ready++;
    }else if(falsePhiRoot->falsePhiLeaves.size() == 1){
        auto leaf = dynamic_cast<SymVal*>(*falsePhiRoot->falsePhiLeaves.begin());
        assert( leaf != nullptr);
        // make sure the only one leaf is executed.
#if defined(CO3_REPLACE)
        if(! falsePhiRoot->isThisNodeReady(leaf, targetReady )){
#else
        if(! ExecuteNode(leaf, targetReady )){
#endif
            return false;
        }
        if(SymVal::extractSymExprFromSymVal(leaf, targetReady) ==  nullptr){
            // the only leaf is concrete, thus, so this root is also concrete
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            // the only leaf is not concrete, then we try to execute the slow branch
            SymVal* slowBranchVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(slowBranchVal != nullptr);
            Val::ReadyType newTargetReady = falsePhiRoot->slowPathChosen + 1;
            BackwardExecution(slowBranchVal, newTargetReady);
            assert(falsePhiRoot->isThisNodeReady(slowBranchVal, newTargetReady ));
            falsePhiRoot->symExpr = SymVal::extractSymExprFromSymVal(slowBranchVal, newTargetReady);
            falsePhiRoot->ready ++;
            falsePhiRoot->slowPathChosen ++;
        }
    }else{
        // we have multiple leaves
        // and each leaf must be FalsePhiLeaf
        bool allConcrete = true;
        for(auto eachLeaf : falsePhiRoot->falsePhiLeaves){
#if defined(CO3_REPLACE)
            if( ! falsePhiRoot->isThisNodeReady(eachLeaf, targetReady)){
#else
            if( ! ExecuteNode(eachLeaf, targetReady)){
#endif
                return false;
            }
            auto eachFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(eachLeaf);
            assert(eachFalsePhiLeaf != nullptr);
            SymExpr leafSymExpr = SymVal::extractSymExprFromSymVal(eachFalsePhiLeaf, targetReady);
            if(leafSymExpr != nullptr){
                allConcrete = false;
            }
        }
        if(allConcrete){
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            SymVal* slowBranchVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(slowBranchVal != nullptr);
            Val::ReadyType newTargetReady = falsePhiRoot->slowPathChosen + 1;
            BackwardExecution(slowBranchVal, newTargetReady );
            assert(falsePhiRoot->isThisNodeReady(slowBranchVal, newTargetReady ));
            falsePhiRoot->symExpr = SymVal::extractSymExprFromSymVal(slowBranchVal, newTargetReady);
            falsePhiRoot->ready ++;
            falsePhiRoot->slowPathChosen ++;
        }
    }
    return true;
}


bool Orchestrator::ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf * falsePhiLeaf, Val::ReadyType targetReady) {
    auto falsePhiLeafOriginalVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(0));
    assert(falsePhiLeafOriginalVal != nullptr);

    SymExpr originalSymExpr = nullptr;
    bool allConcrete = true;
    if(!falsePhiLeaf->isThisNodeReady(falsePhiLeafOriginalVal, targetReady)){
        return false;
    }else{
        originalSymExpr = SymVal::extractSymExprFromSymVal(falsePhiLeafOriginalVal, targetReady);
        if(originalSymExpr != nullptr){
            allConcrete = false;
            //falsePhiLeaf->fastPathChosen += 1;
        }
    }
    for(auto eachPeerOrig: falsePhiLeaf->peerOriginals){
        if(!falsePhiLeaf->isThisNodeReady(eachPeerOrig, targetReady)){
            return false;
        }else{
            if(SymVal::extractSymExprFromSymVal(eachPeerOrig, targetReady) != nullptr){
                allConcrete = false;
            }
        }
    }
    if(originalSymExpr == nullptr && allConcrete){
        // both the root and the this leaf's orig are, which means,
        // this falsePhiLeaf is not executed at the MCU at all
        // although this FalsePhiLeaf is never executed at the MCU,
        // still we need to set this to null and say it's ready
        falsePhiLeaf->symExpr = nullptr;
        falsePhiLeaf->ready ++;
    }else if(originalSymExpr == nullptr && (! allConcrete)){
        // the original is concrete, but the peers are not, we need to construct the newExpr
        auto newVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(1));
        // this is very important:  if this falsePhiLeaf were to choose this newVal branch
        // this new Val branch is not necessarily executed the same numer of times as the falsePhiLeaf itself
        // it should be:
        //      <the number of time it chose the original value branch> +  <the number of times it chose the new branch> = targetReady
        auto newValTargetReady = falsePhiLeaf->slowPathChosen + 1;
        if(! falsePhiLeaf->isThisNodeReady(newVal, newValTargetReady)){
            BackwardExecution(newVal, newValTargetReady );
        }
        assert(falsePhiLeaf->isThisNodeReady(newVal, newValTargetReady ));
        falsePhiLeaf->symExpr = SymVal::extractSymExprFromSymVal(newVal, newValTargetReady);
        falsePhiLeaf->ready++;
        falsePhiLeaf->slowPathChosen++;
    }else if(originalSymExpr != nullptr){
        falsePhiLeaf->symExpr = originalSymExpr;
        falsePhiLeaf->ready++;
    }
    return true;
}
bool Orchestrator::ExecuteSelect(SymVal_sym_notify_select* ntfSelect, Val::ReadyType targetReady){
    // this seems overlapping a lot with the construct function, there might be something we can do about this.
    if(ntfSelect->isThisNodeReady(ntfSelect->In_edges.at(0),targetReady)){
        auto condOperand = dynamic_cast<RuntimeIntVal*>(ntfSelect->In_edges.at(0));
        assert(condOperand != nullptr);
        if(condOperand->Unassigned){
            ntfSelect->Construct(targetReady);
            return true;
        }
        if(static_cast<bool>(condOperand->Val)){
            return ExecuteNode(ntfSelect->In_edges.at(1), targetReady);
        }else{
            return ExecuteNode(ntfSelect->In_edges.at(2), targetReady);
        }
    }else{
        return false;
    }
}

SpecialNodeReturn Orchestrator::ExecuteSpecialNode(SymVal *symval, Val::ReadyType targetReady) {
    auto notifyCall = dynamic_cast<SymVal_sym_notify_call*>(symval);
    //auto tryAlternative = dynamic_cast<SymVal_sym_try_alternative*>(symval);
    auto truePhi = dynamic_cast<SymVal_sym_TruePhi*>(symval);
    auto symNull = dynamic_cast<SymVal_NULL*>(symval);
    auto notifySelect = dynamic_cast<SymVal_sym_notify_select*>(symval);
    if(notifyCall != nullptr  || truePhi != nullptr || symNull != nullptr){
        // notifycall and true phi are handled else where
        // tryAlternative are simply ignored(for now)
        return SpecialConstructed;
    }

    auto falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(symval);
    auto falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(symval);
    if(falsePhiLeaf != nullptr ){
        bool ret = ExecuteFalsePhiLeaf(falsePhiLeaf, targetReady);
        if(ret){
            return SpecialConstructed;
        }else{
            return SpecialNotReady;
        }
    }
    if(falsePhiRoot != nullptr){
        bool ret = ExecuteFalsePhiRoot(falsePhiRoot, targetReady);
        if(ret){
            return SpecialConstructed;
        }else{
            return SpecialNotReady;
        }
    }
    if(notifySelect != nullptr){
        if(ExecuteSelect(notifySelect, targetReady)){
            return SpecialConstructed;
        }else{
            return SpecialNotReady;
        }
    }
    return NotSpecial;
}

bool Orchestrator::ExecuteNode(Val* nodeToExecute, Val::ReadyType targetReady) {
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ') << "Trying to construct:"<<nodeToExecute->Str() <<"\n";
    std::cout.flush();
#endif
    if(nodeToExecute->ready == targetReady){
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') <<nodeToExecute->Str() <<" already constructed\n";
        std::cout.flush();
        indent -= indentNum;
#endif
        return true;
    }
    if(auto runtimeVal = dynamic_cast<RuntimeVal*>(nodeToExecute); runtimeVal != nullptr){
#ifdef DEBUG_CHECKING
        assert(targetReady == (runtimeVal->ready + 1));
#endif
        runtimeVal->Unassign();
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "unassign constructed:"<<nodeToExecute->Str() <<"\n";
        std::cout.flush();
        indent -= indentNum;
#endif
        return true;
    }else if(auto constantVal = dynamic_cast<ConstantVal*>(nodeToExecute); constantVal != nullptr){
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "constant already constructed "<<nodeToExecute->Str() <<"\n";
        std::cout.flush();
        indent -= indentNum;
#endif
        return true;
    }else{
        auto symVal = dynamic_cast<SymVal*>(nodeToExecute);
#ifdef DEBUG_CHECKING
        assert(symVal != nullptr);
#endif
        auto isSpecial = ExecuteSpecialNode(symVal, targetReady);
        if(isSpecial == SpecialConstructed){
#ifdef DEBUG_OUTPUT
            cout << string(indent, ' ') << "constructed:"<<symVal->Str() <<"\n";
            std::cout.flush();
            indent -= indentNum;
#endif
            getCurFunc()->ReleaseOrRemoveRootTask(symVal);
            return true;
        }else if(isSpecial == NotSpecial){
            bool node_ready = true;
            for(auto eachDep : symVal->realChildren()){
#if defined(CO3_REPLACE)
                if(!symVal->isThisNodeReady(eachDep, targetReady)){
#else
                if(!ExecuteNode(eachDep, targetReady)){
#endif
                    node_ready = false;
                    break;
                }
            }
            if(node_ready){
                symVal->Construct(targetReady);
#ifdef DEBUG_OUTPUT
                cout << string(indent, ' ') << "constructed:"<<symVal->Str() <<"\n";
                std::cout.flush();
                indent -= indentNum;
#endif
                getCurFunc()->ReleaseOrRemoveRootTask(symVal);
                return true;
            }
        }
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "not there yet:"<<symVal->Str() <<"\n";
        std::cout.flush();
        indent -= indentNum;
#endif
        return false;
    }
}
void Orchestrator::ForwardExecution(Val* source, SymGraph::RootTask* target, unsigned targetReady) {
#ifdef DEBUG_CHECKING
    auto tmpSymVal = dynamic_cast<SymVal*>(source);
#endif
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ') << "Forwarding:"<<source->Str();

    if( tmpSymVal != nullptr){
        cout<<',' << tmpSymVal->Str();
    }
    cout<<'\n';
    std::cout.flush();
#endif
    SymGraph* cur_func = getCurFunc();

#ifdef DEBUG_CHECKING
    // shouldn't trying to construct a constant
    auto tmpConstant = dynamic_cast<ConstantVal*>(source);
    assert(tmpConstant == nullptr);
#endif


#ifdef  DEBUG_CHECKING
    if(target != nullptr){
        assert( !target->root->isThisNodeReady(source, targetReady));
    }
#endif
    bool shouldMoveForward = false;
    bool constructed = ExecuteNode(source, targetReady);
    if( (! constructed) && target == nullptr) {
        //only when no target is provided, we try to execute as much as we can find(without forcing the runtime val)
        // otherwise, we can just take care of this node itself, even if it's not ready to be constructed,
        // backward execution should already have take of that.
        auto symVal = dynamic_cast<SymVal*>(source);
#ifdef DEBUG_CHECKING
        assert(symVal != nullptr);
#endif
        auto rootTask = cur_func->GetRootTask(symVal);
        assert(rootTask != nullptr);
        // finish the dependent BBs
        for(auto eachBBDep : rootTask->depNonReadyNonLoopBB){
            ExecuteBasicBlock(eachBBDep->BBID);
        }
        if(!rootTask->hasRuntimeDep()){
            // if there is not, then, we can also cons truct this
            for(auto eachInEdge : rootTask->inBBNonReadyLeafDeps){
                ForwardExecution(eachInEdge, rootTask,targetReady);
            }
            assert(rootTask->finished);
            shouldMoveForward = true;
        }
        rootTask->occupied=false;
        // if this root task is successfully executed, remove the task
        cur_func->ReleaseOrRemoveRootTask(symVal);

    }else if(constructed && target != nullptr && (target->root == source)){
        target->finished = true;
        shouldMoveForward = false;
    }
    else if(constructed && target != nullptr){
        shouldMoveForward = true;
    }


    if(shouldMoveForward){
        if(target != nullptr  ){
            // since we try to execute the target from its children,
            // this source should never be the target root itself
#ifdef DEBUG_CHECKING
            assert(source != target->root);
#endif
            bool foundNext = false;
            for(auto eachUser : source->UsedBy){
                if(target->finished){
                    break;
                }
                /*
                 * this is like a best-of-efforts moving upwards towards the root,
                 * in a depth-first manner.
                 * So there will be cases where, there are, say, 2 users,
                 * we depth-first constructing on the 1st user, after finished (recursively),
                 * the 2nd user has already been constructed in that process
                 */
                // make sure the forwarded node is on the path to the root
                if(target->inBBNonReadyDeps.find(eachUser) != target->inBBNonReadyDeps.end()){
                    if(target->root == eachUser){
                        // we've reached the root itself
                        // if it is the root itself, just execute it without forwarding
                        if (ExecuteNode(target->root, targetReady)){
                            target->finished = true;
                        }
                    }else{
                        // execute and forward
                        if( !target->root->isThisNodeReady(eachUser, targetReady)){
                            ForwardExecution(eachUser,  target, targetReady);
                        }
                    }
                    foundNext = true;
                }
            }
            if(foundNext == false){
                cerr << "hasn't reached the target, but found no way to reach it";
                assert(false);
            }
        }else{
            // no target is provided, free farm
            auto& sourceDominance = cur_func->bbTasks.at(source->BBID)->dominance;
            for(auto eachUser : source->UsedBy){
                if(eachUser->BBID == source->BBID || (eachUser->BBID != source->BBID  && sourceDominance.find(eachUser->BBID) != sourceDominance.end() ) ){
                    auto tmpFalsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(eachUser);
                    auto tmpFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiRoot*>(eachUser);
                    auto tmpTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(eachUser);
                    if(tmpFalsePhiLeaf != nullptr || tmpFalsePhiRoot != nullptr){
                        // since these 2 nodes involves backward execution, which might force runtimeInt to be Unassigned, we don't explore these 2, for now.
                        continue;
                    }else if(tmpTruePhi != nullptr){
                        continue;
                    }
                    ForwardExecution(eachUser,  target, targetReady);
                }
            }
        }
    }
#ifdef DEBUG_OUTPUT
    std::cout << string(indent, ' ') << "finish Forwarding:"<<source->Str() <<'\n';
    std::cout.flush();
    indent -= indentNum;
#endif
}


void Orchestrator::BackwardExecution(SymVal* sink, Val::ReadyType targetReady) {
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ') << "backwarding:"<<sink->Str()<<'\n';
    std::cout.flush();
#endif
    //assuming all its dependents are ready
    SymGraph* cur_func = getCurFunc();
    if(sink->ready >= targetReady || ExecuteNode(sink, targetReady)){
        //already constructed or can be constructed right away
    }else{
        auto rootTask = cur_func->GetRootTask(sink);
        assert(rootTask != nullptr);
        for(auto eachBBDep : rootTask->depNonReadyNonLoopBB){
            ExecuteBasicBlock(eachBBDep->BBID);
        }

        for(auto eachInBBLeaf : rootTask->inBBNonReadyLeafDeps){
            // we are forcing the dependent unready runtime value to unassign themselves
            ForwardExecution(eachInBBLeaf,rootTask,targetReady);
        }
        rootTask->occupied=false;
        assert(rootTask->finished);
        cur_func->ReleaseOrRemoveRootTask(sink);
        // we hope the execution above ful-filled its job properly
        assert(sink->ready == targetReady);
    }
#ifdef DEBUG_OUTPUT
    cout << string(indent, ' ')<< "finish backwarding:"<<sink->Str()<<'\n';
    cout.flush();
    indent -= indentNum;
#endif
}


void Orchestrator::PreparingCalling(NotifyCallMessage* callMsg){
    SymGraph* cur_graph = getCurFunc();
    auto* notifyCallVal = dynamic_cast<SymVal_sym_notify_call*>(cur_graph->callInsts.at(callMsg->id));
    assert(notifyCallVal != nullptr);

    for(unsigned index = 1; index < notifyCallVal->In_edges.size() ; index ++){
        auto setPara = dynamic_cast<SymVal_sym_set_parameter_expression*>(notifyCallVal->In_edges.at(index));
#ifdef DEBUG_CHECKING
        assert(setPara != nullptr);
#endif
        if(! notifyCallVal->isThisNodeReady(setPara, notifyCallVal->ready + 1)){
            BackwardExecution(setPara, notifyCallVal->ready + 1 );
        }
    }
    //initialize the ret value to null in case it's not instrumented.
    _sym_set_return_expression(nullptr);
    notifyCallVal->ready ++;
}

void Orchestrator::SetRetAndRefreshGraph() {
    auto cur_func = getCurFunc();
    auto cur_func_name = getCurFunc()->funcname;// copy construct
    auto setRetSym = cur_func->setRetSym;
    bool ret_sym_is_concrete = true;
    if(setRetSym != nullptr){
        Val::ReadyType targetReady = 0;
        if(setRetSym->inLoop){
            targetReady = (setRetSym->ready + 1);
        }else{
            targetReady = 1;
        }
        BackwardExecution(setRetSym,  targetReady);
        assert(setRetSym->ready == targetReady);
        if(SymVal::extractSymExprFromSymVal(setRetSym,targetReady) != nullptr){
            ret_sym_is_concrete = false;
        }
    }


    unsigned funcId = UINT_MAX;
    for(auto eachSymGraph : symGraphs){
        if(eachSymGraph.second == cur_func){
            funcId = eachSymGraph.first;
            break;
        }
    }
    assert(funcId != UINT_MAX);
    if( ret_sym_is_concrete == false || cur_func->changed == true){
        delete cur_func;
        symGraphs[funcId] = new SymGraph(*vanillaSymGraphs.at(funcId));
    }

    //symGraphs[funcId] = new SymGraph(cur_func_name, funcFiles.at(cur_func_name).cfg_file,funcFiles.at(cur_func_name).dom_file,\
    //                            funcFiles.at(cur_func_name).postDom_file, funcFiles.at(cur_func_name).dfg_file);
}

void Orchestrator::SendInput() {
    ifstream inputFile (symInputFile,ios::in | ios::binary);
    assert(inputFile.is_open());
    // get its size:
    inputFile.seekg(0, std::ios::end);
    std::streampos payload_size = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    unsigned total_size = 0;
    if(payload_size > 1024){
        total_size = 1024;
    }else{
        total_size = payload_size;
    }
    total_size += 4;
    char  input [1024 + 4];
    inputFile.read(input + 4, payload_size);

    input[0] = total_size & 0x000000ff;
    input[1] = (total_size & 0x0000ff00) >> 8;
    input[2] = (total_size & 0x00ff0000) >> 16;
    input[3] = (total_size & 0xff000000) >> 24;

    sendData(ser, (uint8_t *)input, total_size);
}
int Orchestrator::Run() {
    auto listen_job = pool.enqueue(&MsgQueue::Listen,&(this->msgQueue));
    int msgCounter = 0;
    uint64_t building_total_time = 0;
    uint64_t start_time = 0;
    uint64_t end_time = 0;
    // get the initialization msg so that we can initialize on our side:
    while(true){

        Message* msg = msgQueue.Pop();
        if(msg == nullptr){
            usleep(100);
            continue;
        }

        auto init_msg = dynamic_cast<InitMessage*>(msg);
        assert(init_msg != nullptr);
        msgCounter += 1;
#ifdef DEBUG_OUTPUT
        cout<<msgCounter<< "th msg,";
        cout.flush();
        assert(indent == 0);
        cout<<init_msg->Str()<<'\n';
        cout.flush();
#endif

        start_time = getTimeStamp();
        _sym_initialize_mem(init_msg->addr, init_msg->DR);
        if(init_msg->DR == true){
            // then the shadow mem is not set up in _sym_initialize_mem, we need to create a global iter for DR
            unsigned inputSize = boost::filesystem::file_size(g_config.inputFile);
            DR_INPUT = new WriteShadowIteratorDR(reinterpret_cast<uintptr_t>(init_msg->addr), inputSize);
        }
        end_time = getTimeStamp();
        building_total_time += (end_time - start_time);


        //now we know the MCU is properly initialized

#ifdef DEBUG_OUTPUT
        cout<<"finish "<<init_msg->Str()<<"\n\n";
        cout.flush();
#endif
        break;
    }
    while(true){
        Message* msg = msgQueue.Pop();
        if(msg == nullptr){
            usleep(100);
            continue;
        }
        msgCounter += 1;
#ifdef DEBUG_OUTPUT
        cout<<msgCounter<< "th msg,";
        cout.flush();
#endif
        start_time = getTimeStamp();
        int ret = ProcessMessage(msg,msgCounter);
        delete(msg);
        end_time = getTimeStamp();
        building_total_time += (end_time - start_time);
        if( ret == 0){
            // end message is received.
            listen_job.get();
            cout << "Orchestrator processing time:"<< building_total_time <<'\n';
            cout.flush();
            break;
        }
    }
    if(DR_INPUT != nullptr){
        delete(DR_INPUT);
        DR_INPUT = nullptr;
    }
    return 0;
}
#pragma clang diagnostic pop