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
        assert( leaf != nullptr && falsePhiRoot->isThisNodeReady(leaf, (falsePhiRoot->ready + 1)  ));
        if(leaf->symExpr ==  nullptr){
            // the only leaf is concrete, thus, this root is also concrete
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            SymVal* symVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(symVal != nullptr);
            BackwardExecution(symVal);
            assert(falsePhiRoot->isThisNodeReady(symVal, (falsePhiRoot->ready + 1) ));
            falsePhiRoot->symExpr = symVal->symExpr;
            falsePhiRoot->ready ++;
        }
    }else{
        bool anySym = false;
        for(auto eachLeaf : falsePhiRoot->falsePhiLeaves){
            auto eachFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(getCurFunc()->Nodes.at(eachLeaf));
            assert(eachFalsePhiLeaf != nullptr);
            auto eachFalsePhiLeafOriginalValue = dynamic_cast<SymVal*>(eachFalsePhiLeaf->In_edges.at(0));
            assert(eachFalsePhiLeafOriginalValue != nullptr);
            BackwardExecution(eachFalsePhiLeafOriginalValue);
            assert(falsePhiRoot->isThisNodeReady(eachFalsePhiLeafOriginalValue, (falsePhiRoot->ready + 1)  ));
            if(eachFalsePhiLeafOriginalValue->symExpr != nullptr){
                anySym = true;
            }
        }
        if(! anySym){
            falsePhiRoot->symExpr = nullptr;
            falsePhiRoot->ready ++;
        }else{
            SymVal* symVal = dynamic_cast<SymVal*>(falsePhiRoot->In_edges.at(1));
            assert(symVal != nullptr);
            BackwardExecution(symVal);
            assert(falsePhiRoot->isThisNodeReady(symVal, (falsePhiRoot->ready + 1) ));
            falsePhiRoot->symExpr = symVal->symExpr;
            falsePhiRoot->ready ++;
        }
    }
}

void Orchestrator::ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf * falsePhiLeaf) {
    auto falsePhiLeafOriginalVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(0));
    assert(falsePhiLeafOriginalVal != nullptr);
    BackwardExecution(falsePhiLeafOriginalVal);
    assert(falsePhiLeaf->isThisNodeReady(falsePhiLeafOriginalVal, (falsePhiLeaf->ready + 1) ));
    if(falsePhiLeaf->symExpr == nullptr){
        //the original value is nullptr
        if(falsePhiLeaf->isThisNodeReady(falsePhiLeaf->falsePhiRoot, (falsePhiLeaf->ready + 1) )){
            if(falsePhiLeaf->falsePhiRoot->symExpr == nullptr){
                // the root is also concrete, which means, this falsePhiLeaf is not executed at the MCU at all
                //falsePhiLeaf->symExpr = nullptr;
                //falsePhiLeaf->ready ++;
            }else{
                // the root is not concrete, which means,
                // this falsePhiLeaf need to take the newVal
                auto newVal = dynamic_cast<SymVal*>(falsePhiLeaf->In_edges.at(1));
                BackwardExecution(newVal);
                assert(falsePhiLeaf->isThisNodeReady(newVal, (falsePhiLeaf->ready + 1) ));
                falsePhiLeaf->symExpr = newVal->symExpr;
                falsePhiLeaf->ready++;
            }
        }else{
            // the root is not ready yet, try another time
        }
    }else{
        //the original value is not concrete
        // which mean, the root is not concrete either
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
            if(symVal->directlyConstructable()){
                // if we can construct right now, just do it
                symVal->Construct();
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


void Orchestrator::BackwardExecution(SymVal* sink) {
    std::cout << "backwarding:"<<sink->Print()<<'\n';
    std::cout.flush();
    //assuming all its dependents are ready
    SymGraph* cur_func = getCurFunc();
    if(sink->directlyConstructable()){
        // if we can construct right now, just do it
        sink->Construct();
        cout << "constructed\n";
        cout.flush();
    }else{
        cout << "not constructed\n";
        cout.flush();
        auto rootTask = cur_func->GetRootTask(sink);
        for(auto eachInBBLeaf : rootTask->inBBNonReadyLeafDeps){
            ForwardExecution(eachInBBLeaf,false,sink,(sink->ready + 1));
        }
    }

}

void Orchestrator::PreparingCalling(NotifyCallMessage* callMsg){

}

int Orchestrator::Run() {

    pool.enqueue(&MsgQueue::Listen,&(this->msgQueue));
    while(true){
        Message* msg = msgQueue.Pop();
        if(auto cnt_msg = dynamic_cast<ControlMessgaes*>(msg); cnt_msg != nullptr){
            if(auto bb_msg = dynamic_cast<NotifyBasicBlockMessage*>(msg) ; bb_msg != nullptr){

            }else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(msg); func_msg != nullptr){
                callStack.push(symGraphs.at(func_msg->id));
            }else if(auto call_msg = dynamic_cast<NotifyCallMessage*>(msg) ; call_msg != nullptr){

            }else if(auto ret_msg = dynamic_cast<NotifyRetMessage*>(msg) ; ret_msg != nullptr){

            }else{
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
                BackwardExecution(constraintVal);
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