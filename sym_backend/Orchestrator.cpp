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

int msgCounter = 0;
Orchestrator::Orchestrator(std::string inputDir, std::string sp_port, int baud_rate): \
pool(2),sp(initSerialPort(sp_port.c_str(), baud_rate)), msgQueue(sp)
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
    freeSerialPort(sp);
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
        if(! falsePhiRoot->isThisNodeReady(leaf, targetReady )){
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
            falsePhiRoot->symExpr = slowBranchVal->symExpr;
            falsePhiRoot->ready ++;
            falsePhiRoot->slowPathChosen ++;
        }
    }else{
        // we have multiple leaves
        // and each leaf must be FalsePhiLeaf
        bool allConcrete = true;
        for(auto eachLeaf : falsePhiRoot->falsePhiLeaves){
            if( ! falsePhiRoot->isThisNodeReady(eachLeaf, targetReady)){
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
            falsePhiRoot->symExpr = slowBranchVal->symExpr;
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

SpecialNodeReturn Orchestrator::ExecuteSpecialNode(SymVal *symval, Val::ReadyType targetReady) {
    auto notifyCall = dynamic_cast<SymVal_sym_notify_call*>(symval);
    //auto tryAlternative = dynamic_cast<SymVal_sym_try_alternative*>(symval);
    auto truePhi = dynamic_cast<SymVal_sym_TruePhi*>(symval);
    auto symNull = dynamic_cast<SymVal_NULL*>(symval);
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
    return NotSpecial;
}

bool Orchestrator::ExecuteNode(Val* nodeToExecute, Val::ReadyType targetReady) {
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ') << "Trying to construct:"<<nodeToExecute->Str() <<"\n";
    std::cout.flush();
#endif
    if(auto runtimeVal = dynamic_cast<RuntimeVal*>(nodeToExecute); runtimeVal != nullptr){
#ifdef DEBUG_CHECKING
        assert(targetReady == (runtimeVal->ready + 1));
#endif
        runtimeVal->Unassign();
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "constructed:"<<nodeToExecute->Str() <<"\n";
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
            if(symVal->directlyConstructable(targetReady)) {
                // if we can construct right now, just do it
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
    if(cur_func->funcname == "receive_cgc_until" && tmpSymVal != nullptr && tmpSymVal->symID == 42 ){
        __asm__("nop");
    }
#endif

#ifdef DEBUG_CHECKING
    // shouldn't trying to construct a constant
    auto tmpConstant = dynamic_cast<ConstantVal*>(source);
    assert(tmpConstant == nullptr);
#endif

#ifdef  DEBUG_CHECKING
    if(target != nullptr){
        assert(!target->root->isThisNodeReady(source, targetReady));
    }
#endif
    bool shouldMoveForward = false;
    bool constructed = ExecuteNode(source, targetReady);
    if(! constructed && target == nullptr) {
        //only when no target is provided, we try to execute as much as we can find(without forcing the runtime val)
        // otherwise, we can just take care of this node itself, even if it's not ready to be constructed,
        // backward execution should already have take of that.
        auto symVal = dynamic_cast<SymVal*>(source);
#ifdef DEBUG_CHECKING
        assert(symVal != nullptr);
#endif
        auto rootTask = cur_func->GetRootTask(symVal);
        assert(rootTask != nullptr);
        Val::ReadyType old_ready = rootTask->root->ready;
        // finish the dependent BBs
        for(auto eachBBDep : rootTask->depNonReadyNonLoopBB){
            ExecuteBasicBlock(eachBBDep->BBID);
        }
        if(!rootTask->hasRuntimeDep()){
            // if there is not, then, we can also construct this
            for(auto eachInEdge : rootTask->inBBNonReadyLeafDeps){
                ForwardExecution(eachInEdge, rootTask,targetReady);
            }
            assert( rootTask->root->ready ==(old_ready + 1));
            shouldMoveForward = true;
        }
        rootTask->occupied=false;
        // if this root task is successfully executed, remove the task
        cur_func->ReleaseOrRemoveRootTask(symVal);

    }else if(constructed && target != nullptr && target->root == source){
        shouldMoveForward = false;
    }
    else if(constructed){
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
                if(target->inBBNonReadyDeps.find(eachUser) != target->inBBNonReadyDeps.end()){
                    if(target->root == eachUser){
                        ExecuteNode(target->root, targetReady);
                    }else{
                        ForwardExecution(eachUser,  target, targetReady);
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
#ifdef DEBUG_CHECKING
    if(cur_func->funcname == "receive_cgc_until" && sink->symID == 22){
        __asm__("nop");
    }
#endif
    if(sink->ready == targetReady || ExecuteNode(sink, targetReady)){
        //already constructed or can be constructed right away
    }else{
#ifdef DEBUG_CHECKING
        if(cur_func->funcname == "calcCRC" && sink->symID == 28 && targetReady == 3){
            __asm__("nop");
        }
#endif
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
    auto notifyCallVal = dynamic_cast<SymVal_sym_notify_call*>(cur_graph->callInsts.at(callMsg->id));

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
        auto targetReady = (setRetSym->ready + 1);
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

    sendDataSerialPort(sp.port, (uint8_t *)input, total_size);
}
int Orchestrator::Run() {
    auto listen_job = pool.enqueue(&MsgQueue::Listen,&(this->msgQueue));

    // get the initialization msg so that we can initialize on our side:
    while(true){

        Message* msg = msgQueue.Pop();
        if(msg == nullptr){
            usleep(100);
            continue;
        }

        auto init_msg = dynamic_cast<InitMessage*>(msg);
        msgCounter += 1;
#ifdef DEBUG_OUTPUT
        cout<<msgCounter<< "th msg,";
        cout.flush();
        assert(indent == 0);
        cout<<init_msg->Str()<<'\n';
        cout.flush();
#endif
        assert(init_msg != nullptr);
        _sym_initialize_mem(init_msg->addr);
        start_time = getTimeStamp();
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
        if(auto cnt_msg = dynamic_cast<ControlMessgaes*>(msg); cnt_msg != nullptr){

            if(auto bb_msg = dynamic_cast<NotifyBasicBlockMessage*>(cnt_msg) ; bb_msg != nullptr){
                // now a BB has been finished on the MCU side, we try to do the same here
#ifdef DEBUG_CHECKING
                auto cur_func = getCurFunc();
                assert(cur_func->bbTasks.at(bb_msg->id)->inLoop);
#endif
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout<<bb_msg->Str()<<'\n';
                cout.flush();
#endif
                getCurFunc()->changed = true;
                ExecuteBasicBlock(bb_msg->id);
#ifdef DEBUG_OUTPUT
                cout<<"finish "<<bb_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(cnt_msg); func_msg != nullptr){
                auto nextFunc = symGraphs.at(func_msg->id);
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout<<func_msg->Str()<< ':'<<nextFunc->funcname<<'\n';
                cout.flush();
#endif

                UpdateCallStackHashCall(func_msg->id);
                if(callStack.size() == 0){
                    callStack.push(nextFunc);
                }else{
                    assert(callStack.top() == nullptr);
                    callStack.top() = nextFunc;
                }
#ifdef DEBUG_OUTPUT
                cout<<"finish "<<func_msg->Str()<< ':'<<nextFunc->funcname<<"\n\n";
                cout.flush();
#endif
            }else if(auto call_msg = dynamic_cast<NotifyCallMessage*>(cnt_msg) ; call_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout <<call_msg->Str()<<'\n';
                cout.flush();
#endif
                PreparingCalling(call_msg);
                // not sure if the called function is instrumented or not
                // so we just set this to false and let the NotifyCall decide.
                callStack.push(nullptr);
#ifdef DEBUG_OUTPUT
                cout <<"finish "<<call_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto ret_msg = dynamic_cast<NotifyRetMessage*>(cnt_msg) ; ret_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout  <<ret_msg->Str()<<'\n';
                cout.flush();
#endif
                if(callStack.top() != nullptr){
                    // now the instrumented callee function is executed, and we are still in the callee function
#ifdef DEBUG_OUTPUT
                    cout <<"back from:"<<callStack.top()->funcname<<'\n';
                    cout.flush();
#endif
                    //get the funcID of the callee
                    auto funcId = INT32_MAX;
                    for(auto eachFunc = symGraphs.begin(); eachFunc != symGraphs.end(); eachFunc++){
                        if(eachFunc->second == callStack.top()){
                            funcId = eachFunc->first;
                            break;
                        }
                    }
                    assert(funcId != INT32_MAX);
                    UpdateCallStackRet(funcId);
                    SetRetAndRefreshGraph();
                }else{
#ifdef DEBUG_OUTPUT
                    cout <<"back from: Uninstrumented\n";
                    cout.flush();
#endif
                    _sym_set_return_expression(nullptr);
                }
                callStack.pop();
#ifdef DEBUG_OUTPUT
                cout <<"finish " <<ret_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto phi_msg = dynamic_cast<PhiValueMessage*>(cnt_msg); phi_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << phi_msg->Str()<<'\n';
                cout.flush();
#endif
                auto cur_func = getCurFunc();
                cur_func->changed = true;
                auto truePhi =  dynamic_cast<SymVal_sym_TruePhi*>(cur_func->Nodes.at(cur_func->symID2offMap.at(phi_msg->symid)));
                UpdateCallStackHashBB(truePhi->BBID);

                // get the chosen Symval
                auto val = dynamic_cast<SymVal*>(truePhi->In_edges.at(phi_msg->value));
                auto desiredReady = truePhi->getDepTargetReady(val);
                BackwardExecution(val,  desiredReady);
                assert(truePhi->isThisNodeReady(val, desiredReady));

                auto symExprToTake = SymVal::extractSymExprFromSymVal(val, desiredReady);
                truePhi->historyValues.push_back(make_pair(phi_msg->value, symExprToTake));
                truePhi->ready ++;

#ifdef DEBUG_OUTPUT
                cout<< "finish "<<phi_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto end_msg = dynamic_cast<EndMessage*>(cnt_msg); end_msg != nullptr ){
                uint64_t end_message_receive_time = listen_job.get();
                cout << "End message received time:"<< end_message_receive_time - start_time <<'\n';
                cout << "End message processed:"<< getTimeStamp() - start_time <<'\n';
                cout.flush();
#ifdef DEBUG_OUTPUT
                cout<< "finish "<<end_msg->Str()<<"\n\n";
                cout.flush();
#endif
                break;
            }
            else{
                std::cerr<<"seriously?";
                assert(false);
            }

        }else if (auto sym_source_msg = dynamic_cast<SymSourceMessage*>(msg) ; sym_source_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_source_msg->symid));
            cur_func->changed = true;
            if(auto sym_read_mem_msg = dynamic_cast<ReadMemMessage*>(msg) ; sym_read_mem_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << sym_read_mem_msg->Str()<<'\n';
                cout.flush();
#endif
                //sanity check
                auto readMemSymVal = dynamic_cast<SymVal_sym_build_read_memory*>(cur_val);
                assert(readMemSymVal != nullptr);
                // at compilation, we already ensure, operand 1, 2 are constant, and operand 0 is runtime, so we just
                // assign the Val from msg to the RuntimePtr's Val, and increase the ready value of that runtime operand by 1, and build this node
                // and then move on to this parents
                UpdateCallStackHashBB(readMemSymVal->BBID);

                auto runtime_operand = dynamic_cast<RuntimeIntVal*>(readMemSymVal->In_edges.at(0));
                assert(runtime_operand != nullptr);
                runtime_operand->Assign(sym_read_mem_msg->ptr);
                // allow this to cross BB, but not by force(i.e., halt when meeting other unknown runtime dependency)
                ForwardExecution(readMemSymVal,nullptr, readMemSymVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<sym_read_mem_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if (auto runtimeBoolMsg = dynamic_cast<BuildBoolValueMessage*>(msg); runtimeBoolMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeBoolMsg->Str()<<'\n';
                cout.flush();
#endif
                auto buildBoolSymVal = dynamic_cast<SymVal_sym_build_bool*>(cur_val);
                assert(buildBoolSymVal != nullptr);
                UpdateCallStackHashBB(buildBoolSymVal->BBID);
                auto runtimeBoolVal = dynamic_cast<RuntimeIntVal*>(buildBoolSymVal->In_edges.at(0));
                runtimeBoolVal->Assign(runtimeBoolMsg->value);
                ForwardExecution(buildBoolSymVal,nullptr, buildBoolSymVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeBoolMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeIntMsg = dynamic_cast<BuildIntValueMessage*>(msg); runtimeIntMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeIntMsg->Str()<<'\n';
                cout.flush();
#endif
                auto buildIntSymVal = dynamic_cast<SymVal_sym_build_integer*>(cur_val);
                assert(buildIntSymVal != nullptr);

                UpdateCallStackHashBB(buildIntSymVal->BBID);

                auto runtimeIntVal = dynamic_cast<RuntimeIntVal*>(buildIntSymVal->In_edges.at(0));
                assert(runtimeIntVal != nullptr);
                runtimeIntVal->Assign(runtimeIntMsg->value);
                ForwardExecution(buildIntSymVal,nullptr, buildIntSymVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeIntMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeFloatMsg = dynamic_cast<BuildFloatValueMessage*>(msg); runtimeFloatMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeFloatMsg->Str()<<'\n';
                cout.flush();
#endif
                auto buildFloatSymVal = dynamic_cast<SymVal_sym_build_float*>(cur_val);
                assert(buildFloatSymVal != nullptr);

                UpdateCallStackHashBB(buildFloatSymVal->BBID);

                auto runtimeFloatVal = dynamic_cast<RuntimeFloatVal*>(buildFloatSymVal->In_edges.at(0));
                assert(runtimeFloatVal != nullptr);
                runtimeFloatVal->Assign(runtimeFloatMsg->value);
                ForwardExecution(buildFloatSymVal, nullptr, buildFloatSymVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeFloatMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeDoubleMsg = dynamic_cast<BuildDoubleValueMessage*>(msg);runtimeDoubleMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeDoubleMsg->Str()<<'\n';
                cout.flush();
#endif
                auto buildDoubleSymVar = dynamic_cast<SymVal_sym_build_float*>(cur_val);
                assert(buildDoubleSymVar != nullptr);

                UpdateCallStackHashBB(buildDoubleSymVar->BBID);

                auto runtimeDoubleVar = dynamic_cast<RuntimeDoubleVal*>(buildDoubleSymVar->In_edges.at(0));
                assert(runtimeDoubleVar != nullptr);
                runtimeDoubleVar->Assign(runtimeDoubleMsg->value);
                ForwardExecution(buildDoubleSymVar, nullptr,buildDoubleSymVar->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeDoubleMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }
        }else if (auto sym_sink_msg = dynamic_cast<SymSinkMessage*>(msg) ; sym_sink_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            cur_func->changed = true;
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_sink_msg->symid));
            if(auto sym_constraint_msg = dynamic_cast<PushConstraintMessage*>(sym_sink_msg); sym_constraint_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout <<  sym_constraint_msg->Str()<<'\n';
                cout.flush();
#endif
                auto buildConstraintVal = dynamic_cast<SymVal_sym_build_path_constraint*>(cur_val);
                assert(buildConstraintVal != nullptr);

                UpdateCallStackHashBB(buildConstraintVal->BBID);

                auto runtime_value = dynamic_cast<RuntimeIntVal*>(buildConstraintVal->In_edges.at(1));
                assert(runtime_value != nullptr);
                runtime_value->Assign(sym_constraint_msg->runtimeVal);
                BackwardExecution(buildConstraintVal, (buildConstraintVal->ready + 1) );
                assert(dynamic_cast<SymVal*>(buildConstraintVal->In_edges.at(0))->symExpr != nullptr);
#ifdef DEBUG_OUTPUT
                cout << "finish "<< sym_constraint_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto memWriteMsg = dynamic_cast<WriteMemMessage*>(sym_sink_msg); memWriteMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << memWriteMsg->Str()<<'\n';
                cout.flush();
#endif

                auto memWriteVal = dynamic_cast<SymVal_sym_build_write_memory*>(cur_val);
                assert(memWriteVal != nullptr);

                UpdateCallStackHashBB(memWriteVal->BBID);

                auto addrOperand = dynamic_cast<RuntimeIntVal*>(memWriteVal->In_edges.at(0));
                assert(addrOperand != nullptr);
                addrOperand->Assign(memWriteMsg->ptr);
                BackwardExecution(memWriteVal,memWriteVal->ready + 1 );
#ifdef DEBUG_CHECKING

#endif
#ifdef DEBUG_OUTPUT
                cout << "finish "<<memWriteMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto memcpyMsg = dynamic_cast<MemCpyMessage*>(sym_sink_msg); memcpyMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << memcpyMsg->Str()<<'\n';
                cout.flush();
#endif
                auto memcpyVal = dynamic_cast<SymVal_sym_build_memcpy*>(cur_val);
                assert(memcpyVal != nullptr);

                UpdateCallStackHashBB(memcpyVal->BBID);


                auto destPtrVal = dynamic_cast<RuntimePtrVal*>(memcpyVal->In_edges.at(0));
                assert(destPtrVal != nullptr);
                destPtrVal->Assign(reinterpret_cast<void*>(memcpyMsg->dst_ptr));

                auto srcPtrVal = dynamic_cast<RuntimePtrVal*>(memcpyVal->In_edges.at(1));
                assert(srcPtrVal != nullptr);
                srcPtrVal->Assign(reinterpret_cast<void*>(memcpyMsg->src_ptr));

                auto lengthVal = dynamic_cast<RuntimeIntVal*>(memcpyVal->In_edges.at(2));
                assert(lengthVal != nullptr);
                lengthVal->Assign(memcpyMsg->length);

                BackwardExecution(memcpyVal, memcpyVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<memcpyMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto memsetMsg = dynamic_cast<MemSetMessage*>(sym_sink_msg); memsetMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << memsetMsg->Str()<<'\n';
                cout.flush();
#endif
                auto memsetVal = dynamic_cast<SymVal_sym_build_memset*>(cur_val);
                assert(memsetVal != nullptr);

                UpdateCallStackHashBB(memsetVal->BBID);


                auto ptrVal = dynamic_cast<RuntimePtrVal*>(memsetVal->In_edges.at(0));
                assert(ptrVal != nullptr);
                ptrVal->Assign(reinterpret_cast<void*>(memsetMsg->ptr));


                auto lengthVal = dynamic_cast<RuntimeIntVal*>(memsetVal->In_edges.at(2));
                assert(lengthVal != nullptr);
                lengthVal->Assign(memsetMsg->length);

                BackwardExecution(memsetVal, memsetVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<memsetMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto memmoveMsg = dynamic_cast<MemMoveMessage*>(sym_sink_msg); memmoveMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << memmoveMsg->Str()<<'\n';
                cout.flush();
#endif
                std::cerr<<"unsupported memmove\n";
                std::cerr.flush();
                abort();
            }
        }
        else{
            std::cerr<<"seriously?";
            std::cerr.flush();
            assert(false);
        }
        delete(msg);
    }
    return 0;
}
#pragma clang diagnostic pop