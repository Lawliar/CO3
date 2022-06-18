//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"
#include <iostream>
#include <fstream>


#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

std::string dbgInputFileName;
Orchestrator::Orchestrator(std::string inputDir, std::string sp_port, int baud_rate): \
pool(12),sp(initSerialPort(sp_port.c_str(), baud_rate)), msgQueue(sp)
{
    boost::filesystem::path dir (inputDir);
    boost::filesystem::path funcIDFilePath = dir / "spear_func_id.txt";

    boost::filesystem::path fileInputFilePath = dir / "fileUSB.bin";
    if(!boost::filesystem::exists(fileInputFilePath)){
        cerr<<"fileUSB.bin file does not exist, even if you don't use file for USB, please create an empty file there.";
        assert(false);
    }

    dbgInputFileName.append(fileInputFilePath.string());

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
        cout << "processing func:"<< cur_funcname<<'\n';
        cout.flush();
        SymGraph* cur_symgraph = new SymGraph(cur_funcname, cfg_file.string(),dom_file.string(),postDom_file.string(),dfg_file.string());
        symGraphs[cur_id] = cur_symgraph;
    }
    _sym_initialize(inputDir);
}
Orchestrator::~Orchestrator() {
    freeSerialPort(sp);
    for(auto eachFunc :symGraphs){
        delete eachFunc.second;
    }
    symGraphs.clear();
}

SymGraph* Orchestrator::getCurFunc() {
    return callStack.top();
}

void Orchestrator::ExecuteBasicBlock(Val::BasicBlockIdType bbid) {
    SymGraph* cur_func = getCurFunc();
    auto bbTask = cur_func->bbTasks.at(bbid);
    bbTask->Refresh();
    for(auto eachNonReadyLeaf : bbTask->nonReadyLeaves){
        ForwardExecution(eachNonReadyLeaf,false,nullptr,(bbTask->ready + 1) );
    }
    assert(bbTask->isBBReady());
}

void Orchestrator::ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot *falsePhiRoot) {
    if (falsePhiRoot->falsePhiLeaves.size() == 0){
        falsePhiRoot->symExpr = nullptr;
        falsePhiRoot->ready++;
    }else if(falsePhiRoot->falsePhiLeaves.size() == 1){
        auto leaf = dynamic_cast<SymVal*>(getCurFunc()->Nodes.at( *falsePhiRoot->falsePhiLeaves.begin() ));
        assert( leaf != nullptr);
        // make sure the only one leaf is executed.
        if(! falsePhiRoot->isThisNodeReady(leaf, (falsePhiRoot->ready + 1) )){
            BackwardExecution(leaf, falsePhiRoot->ready + 1);
        }
        if(leaf->symExpr ==  nullptr){
            // the only leaf is concrete, thus, this root is also concrete
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            // the only leaf is not concrete, then we try to execute the slow branch
            SymVal* slowBranchVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(slowBranchVal != nullptr);
            BackwardExecution(slowBranchVal, (falsePhiRoot->ready + 1));
            assert(falsePhiRoot->isThisNodeReady(slowBranchVal, (falsePhiRoot->ready + 1) ));
            falsePhiRoot->symExpr = slowBranchVal->symExpr;
            falsePhiRoot->ready ++;
        }
    }else{
        // we have multiple leaves
        // and each leaf must be symFlaseLeaf
        bool anySym = false;
        for(auto eachLeaf : falsePhiRoot->falsePhiLeaves){
            auto eachFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(getCurFunc()->Nodes.at(eachLeaf));
            assert(eachFalsePhiLeaf != nullptr);
            auto eachFalsePhiLeafOriginalValue = dynamic_cast<SymVal*>(eachFalsePhiLeaf->In_edges.at(0));
            assert(eachFalsePhiLeafOriginalValue != nullptr);
            // we only care about if the falsePhiRoot's original value is concrete or not
            BackwardExecution(eachFalsePhiLeafOriginalValue, (falsePhiRoot->ready + 1) );
            assert(falsePhiRoot->isThisNodeReady(eachFalsePhiLeafOriginalValue, (falsePhiRoot->ready + 1)  ));
            if(eachFalsePhiLeafOriginalValue->symExpr != nullptr){
                // this original value is not concrete
                anySym = true;
            }
        }
        if(! anySym){
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            SymVal* slowBranchVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(slowBranchVal != nullptr);
            BackwardExecution(slowBranchVal, (falsePhiRoot->ready + 1) );
            assert(falsePhiRoot->isThisNodeReady(slowBranchVal, (falsePhiRoot->ready + 1) ));
            falsePhiRoot->symExpr = slowBranchVal->symExpr;
            falsePhiRoot->ready ++;
        }
    }
}

void Orchestrator::ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf * falsePhiLeaf) {
    auto falsePhiLeafOriginalVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(0));
    assert(falsePhiLeafOriginalVal != nullptr);
    BackwardExecution(falsePhiLeafOriginalVal, (falsePhiLeaf->ready + 1) );
    assert(falsePhiLeaf->isThisNodeReady(falsePhiLeafOriginalVal, (falsePhiLeaf->ready + 1) ));
    if(falsePhiLeaf->symExpr == nullptr){
        //the original value is nullptr
        if(!falsePhiLeaf->isThisNodeReady(falsePhiLeaf->falsePhiRoot, (falsePhiLeaf->ready + 1) )){
            BackwardExecution(falsePhiLeaf->falsePhiRoot, (falsePhiLeaf->ready + 1) );
        }
        assert(falsePhiLeaf->isThisNodeReady(falsePhiLeaf->falsePhiRoot, (falsePhiLeaf->ready + 1)));
        if(falsePhiLeaf->falsePhiRoot->symExpr == nullptr){
            // the root is also concrete, which means, this falsePhiLeaf is not executed at the MCU at all
            //falsePhiLeaf->symExpr = nullptr;
            //falsePhiLeaf->ready ++;
        }else{
            // the root is not concrete, which means,
            // this falsePhiLeaf need to take the newVal
            auto newVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(1));
            BackwardExecution(newVal, (falsePhiLeaf->ready + 1) );
            assert(falsePhiLeaf->isThisNodeReady(newVal, (falsePhiLeaf->ready + 1) ));
            falsePhiLeaf->symExpr = newVal->symExpr;
            falsePhiLeaf->ready++;
        }
    }else{
        //the original value is not concrete
        // which mean, the root must not be concrete either
        // we just take the original value then
        falsePhiLeaf->symExpr = falsePhiLeafOriginalVal->symExpr;
        falsePhiLeaf->ready++;
    }
}
void Orchestrator::ForwardExecution(Val* source, bool crossBB, Val* target, unsigned targetReady) {
    std::cout << "Forwarding:"<<source->Print() <<"\n";
    std::cout.flush();
    SymGraph* cur_func = getCurFunc();
    SymVal * symVal = dynamic_cast<SymVal*>(source);
    bool constructed = false;
    if(symVal != nullptr){
        // try to construct this symval
        // some specicial symVal
        std::cout << "sym:"<<symVal->Print()<<'\n';
        std::cout.flush();
        if(source->ready < targetReady){
            auto notifyCall = dynamic_cast<SymVal_sym_notify_call*>(symVal);
            auto tryAlternative = dynamic_cast<SymVal_sym_try_alternative*>(symVal);
            auto truePhi = dynamic_cast<SymVal_sym_TruePhi*>(symVal);
            if(notifyCall != nullptr || tryAlternative != nullptr || truePhi != nullptr ){
                // notifycall and true phi are handled else where
                // tryAlternative are simply ignored(for now)
                return;
            }
            auto falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(symVal);
            auto falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(symVal);
            if(falsePhiLeaf != nullptr ){
                ExecuteFalsePhiLeaf(falsePhiLeaf);
            }else if(falsePhiRoot != nullptr){
                ExecuteFalsePhiRoot(falsePhiRoot);
            }
            if(symVal->directlyConstructable(targetReady)){
                // if we can construct right now, just do it
                symVal->Construct(targetReady);
                constructed = true;
            }else{
                auto rootTask = cur_func->GetRootTask(symVal);
                // finish the dependent BBs
                for(auto eachBBDep : rootTask->depNonReadyNonLoopBB){
                    ExecuteBasicBlock(eachBBDep->BBID);
                }
                // see if inside the BB, there is runtime
                if(!rootTask->hasRuntimeDep()){
                    // if there is not, then, we can also construct this
                    for(auto eachInEdge : rootTask->inBBNonReadyLeafDeps){
                        ForwardExecution(eachInEdge, false, symVal,targetReady);
                    }
                }
            }
        }
    }
    if(constructed){
        cout << "constructed\n";
    }else{
        cout << "not constructed\n";
    }
    cout.flush();
    if(target != nullptr && target == source ){
        //source reached target, no matter the source got constructed or not, time to exit
        return;
    }else{
        for(auto eachUser : source->UsedBy){
            auto& userPostDominance = cur_func->bbTasks.at(eachUser->BBID)->post_dominance;
            if(eachUser->BBID == source->BBID || (eachUser->BBID != source->BBID && crossBB && userPostDominance.find(source->BBID) != userPostDominance.end() ) ){
                ForwardExecution(eachUser, crossBB, target, targetReady);
            }
        }
    }
}


void Orchestrator::BackwardExecution(SymVal* sink, Val::ReadyType targetReady) {
    std::cout << "backwarding:"<<sink->Print()<<'\n';
    std::cout.flush();
    //assuming all its dependents are ready
    SymGraph* cur_func = getCurFunc();
    if(sink->ready == targetReady){
        //sink is already constructed
        return;
    }
    else if(sink->directlyConstructable(targetReady)){
        // if we can construct right now, just do it
        sink->Construct(targetReady);
        cout << "constructed\n";
        cout.flush();
    }else{
        cout << "not constructed\n";
        cout.flush();
        auto rootTask = cur_func->GetRootTask(sink);
        for(auto eachInBBLeaf : rootTask->inBBNonReadyLeafDeps){
            ForwardExecution(eachInBBLeaf,false,sink,targetReady);
        }
    }
}


void Orchestrator::PreparingCalling(NotifyCallMessage* callMsg){
    SymGraph* cur_graph = getCurFunc();
    auto notifyCallVal = dynamic_cast<SymVal_sym_notify_call*>(cur_graph->callInsts.at(callMsg->id));
    unsigned index = 1;

    for(; index < notifyCallVal->In_edges.size() ; index ++){
        auto setPara = dynamic_cast<SymVal_sym_set_parameter_expression*>(notifyCallVal->In_edges.at(index));
        assert(setPara != nullptr);
        auto paraIndex = dynamic_cast<ConstantIntVal*>(setPara->In_edges.at(0));
        auto symPara = dynamic_cast<SymVal*>(setPara->In_edges.at(1));
        assert(paraIndex != nullptr && symPara != nullptr);
        unsigned paraOff = paraIndex->Val;
        if(auto symNull = dynamic_cast<SymVal_NULL*>(symPara); symNull != nullptr){
            _sym_set_parameter_expression(paraOff, nullptr);
        }else{
            BackwardExecution(symPara, notifyCallVal->ready + 1 );
            assert(notifyCallVal->isThisNodeReady(symPara, (notifyCallVal->ready + 1)));
            _sym_set_parameter_expression(paraOff, symPara->symExpr);
        }
    }
    notifyCallVal->ready ++;
}

void Orchestrator::SetRet() {
    auto cur_func = getCurFunc();
    auto setRetSym = dynamic_cast<SymVal*>(cur_func->setRetSym->In_edges.at(0));
    assert(setRetSym != nullptr);
    if(auto symNull = dynamic_cast<SymVal_NULL*>(setRetSym); symNull != nullptr){
        _sym_set_return_expression(nullptr);
    }else{
        BackwardExecution(setRetSym, (setRetSym->ready + 1) );
    }
    setRetSym->ready ++;

}

int Orchestrator::Run() {

    pool.enqueue(&MsgQueue::Listen,&(this->msgQueue));
    while(true){
        Message* msg = msgQueue.Pop();
        if(auto cnt_msg = dynamic_cast<ControlMessgaes*>(msg); cnt_msg != nullptr){
            if(auto bb_msg = dynamic_cast<NotifyBasicBlockMessage*>(msg) ; bb_msg != nullptr){
                // now a BB has been finished on the MCU side, we try to do the same here
                ExecuteBasicBlock(bb_msg->id);
            }else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(msg); func_msg != nullptr){
                callStack.push(symGraphs.at(func_msg->id));
                functionedPushed=true;
            }else if(auto call_msg = dynamic_cast<NotifyCallMessage*>(msg) ; call_msg != nullptr){
                // not sure if the called function is instrumented or not
                // so we just set this to false and let the NotifyCall decide.
                functionedPushed=false;
                PreparingCalling(call_msg);
            }else if(auto ret_msg = dynamic_cast<NotifyRetMessage*>(msg) ; ret_msg != nullptr){
                if(functionedPushed){
                    // now the instrumented callee function is executed, and we are still in the callee function
                    SetRet();// execute and set the ret val
                    callStack.pop();// pop the top off the callee func off the stack
                }else{
                    // we just go back from calling an uninstrumented function
                    // set the retExpress and move on
                    _sym_set_return_expression(nullptr);
                }
            }else if(auto phi_msg = dynamic_cast<PhiValueMessage*>(msg); ret_msg != nullptr){
                auto cur_func = getCurFunc();
                auto truePhi =  dynamic_cast<SymVal_sym_TruePhi*>(cur_func->Nodes.at(cur_func->symID2offMap.at(phi_msg->symid)));
                auto val = dynamic_cast<SymVal*>(truePhi->In_edges.at(phi_msg->value));
                assert(val != nullptr);
                BackwardExecution(val, (truePhi->ready + 1) );
                truePhi->historyValues.push_back(make_pair(phi_msg->value, val->symExpr));
                truePhi->ready ++;
            }
            else{
                std::cerr<<"seriously?";
                assert(false);
            }

        }else if (auto sym_source_msg = dynamic_cast<SymSourceMessage*>(msg) ; sym_source_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_source_msg->symid));
            if(auto sym_read_mem_msg = dynamic_cast<ReadMemMessage*>(msg) ; sym_read_mem_msg != nullptr){
                //sanity check
                auto realMemSymVal = dynamic_cast<SymVal_sym_build_read_memory*>(cur_val);
                assert(realMemSymVal != nullptr);
                // at compilation, we already ensure, operand 1, 2 are constant, and operand 0 is runtime, so we just
                // assign the Val from msg to the RuntimePtr's Val, and increase the ready value of that runtime operand by 1, and build this node
                // and then move on to this parents
                auto runtime_operand = dynamic_cast<RuntimeIntVal*>(realMemSymVal->In_edges.at(0));
                assert(runtime_operand != nullptr);
                runtime_operand->Assign(sym_read_mem_msg->ptr);
                // allow this to cross BB, but not by force(i.e., halt when meeting other unknown runtime dependency)
                ForwardExecution(realMemSymVal,true,nullptr, realMemSymVal->ready + 1);
            }
        }else if (auto sym_sink_msg = dynamic_cast<SymSinkMessage*>(msg) ; sym_sink_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_sink_msg->symid));
            if(auto sym_constraint_msg = dynamic_cast<PushConstraintMessage*>(sym_sink_msg); sym_constraint_msg != nullptr){
                auto constraintVal = dynamic_cast<SymVal_sym_build_path_constraint*>(cur_val);
                assert(constraintVal != nullptr);
                auto runtime_value = dynamic_cast<RuntimeIntVal*>(constraintVal->In_edges.at(1));
                assert(runtime_value != nullptr);
                runtime_value->Assign(sym_constraint_msg->runtimeVal);
                BackwardExecution(constraintVal, (constraintVal->ready + 1) );
            }
        }else if(msg == nullptr){
            usleep(100);
        }
        else{
            std::cerr<<"seriously?";
            assert(false);
        }
        delete(msg);
    }
    return 0;
}
#pragma clang diagnostic pop