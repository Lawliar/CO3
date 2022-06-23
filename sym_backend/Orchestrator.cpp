//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"
#include <iostream>
#include <fstream>


#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

std::string dbgInputFileName;

#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
int indent = 0;
int indentNum = 4;
#endif
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
#ifdef DEBUG_OUTPUT
        cout << "processing func:"<< cur_funcname<<'\n';
        cout.flush();
#endif
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

#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ')<<"Executing BB:"<<bbid<<'\n';
    cout.flush();
#endif
    auto bbTask = cur_func->bbTasks.at(bbid);
    bbTask->Refresh( bbTask->ready + 1 );
    for(auto eachNonReadyRoot : bbTask->nonReadyRoots){
        if(auto tmpSymVal = dynamic_cast<SymVal*>(eachNonReadyRoot); tmpSymVal != nullptr){
            BackwardExecution(tmpSymVal, bbTask->ready + 1);
        }else if(auto tmpRuntime = dynamic_cast<RuntimeVal*>(eachNonReadyRoot);  tmpRuntime != nullptr){
            tmpRuntime->Unassign();
        }else{
            cerr<<"Const should not be here";
            assert(false);
        }
    }


    assert(bbTask->isBBReady());
    bbTask->ready ++;

#ifdef DEBUG_OUTPUT
    indent -= indentNum;
    cout << string(indent, ' ') << "finish Executing BB:"<<bbid<<'\n';
    cout.flush();
#endif
}

void Orchestrator::ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot *falsePhiRoot) {
    if (falsePhiRoot->falsePhiLeaves.size() == 0){
        falsePhiRoot->symExpr = nullptr;
        falsePhiRoot->ready++;
    }else if(falsePhiRoot->falsePhiLeaves.size() == 1){
        auto leaf = dynamic_cast<SymVal*>(*falsePhiRoot->falsePhiLeaves.begin());
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
            auto eachFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(eachLeaf);
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

bool Orchestrator::ExecuteSpecialNode(SymVal *symval) {
    bool processed = false;
    auto notifyCall = dynamic_cast<SymVal_sym_notify_call*>(symval);
    auto tryAlternative = dynamic_cast<SymVal_sym_try_alternative*>(symval);
    auto truePhi = dynamic_cast<SymVal_sym_TruePhi*>(symval);
    if(notifyCall != nullptr || tryAlternative != nullptr || truePhi != nullptr ){
        // notifycall and true phi are handled else where
        // tryAlternative are simply ignored(for now)
        processed = true;
    }
    auto falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(symval);
    auto falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(symval);
    if(falsePhiLeaf != nullptr ){
        ExecuteFalsePhiLeaf(falsePhiLeaf);
        processed = true;
    }else if(falsePhiRoot != nullptr){
        ExecuteFalsePhiRoot(falsePhiRoot);
        processed = true;
    }
    return processed;
}
void Orchestrator::ForwardExecution(Val* source,bool force, bool crossBB, Val* target, unsigned targetReady) {
#ifdef DEBUG_OUTPUT
    indent += indentNum;
    cout << string(indent, ' ') << "Forwarding:"<<source->Str() <<"\n";
    std::cout.flush();
#endif
    SymGraph* cur_func = getCurFunc();
    SymVal * symVal = dynamic_cast<SymVal*>(source);
    RuntimeVal* runtimeVal = dynamic_cast<RuntimeVal*>(source);

    bool constructed = false;
    if(symVal != nullptr){
        // try to construct this symval
        // some specicial symVal
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "sym:"<<symVal->Str()<<'\n';
        std::cout.flush();
#endif
        if(source->ready < targetReady){
            auto isSpecial = ExecuteSpecialNode(symVal);
            if(isSpecial){
                constructed = true;
            }else{
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
                            ForwardExecution(eachInEdge,force, false, symVal,targetReady);
                        }
                    }
                }
            }
        }
    }else if(runtimeVal != nullptr){
        // when not forcing the runtimeValue, this source should not be runtimeVal, as this has been checked by code above
        assert(force == true);
        // this runtimeVal should not be ready, otherwise, this should not be called.
        assert(targetReady == (runtimeVal->ready + 1));
        runtimeVal->Unassign();
        constructed = true;
    }
    if(constructed){
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "constructed\n";
#endif
        if(target != nullptr && target == source ){
            //source reached target, no matter the source got constructed or not, time to exit
#ifdef DEBUG_OUTPUT
            std::cout << string(indent, ' ') << "finish Forwarding:"<<source->Str() <<'\n';
            std::cout.flush();
            indent -= indentNum;
#endif
            return;
        }else{
            for(auto eachUser : source->UsedBy){
                auto& userPostDominance = cur_func->bbTasks.at(eachUser->BBID)->post_dominance;
                if(eachUser->BBID == source->BBID || (eachUser->BBID != source->BBID && crossBB && userPostDominance.find(source->BBID) != userPostDominance.end() ) ){
                    ForwardExecution(eachUser, force, crossBB, target, targetReady);
                }
            }
        }
    }
#ifdef DEBUG_OUTPUT
    else{
        cout << string(indent, ' ') << "not constructed\n";
    }
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
    if(sink->ready == targetReady || ExecuteSpecialNode(sink)){
        //sink is already constructed or it's special node
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ')<< "finish backwarding:"<<sink->Str()<<'\n';
        cout.flush();
        indent -= indentNum;
#endif
        return;
    }
    else if(sink->directlyConstructable(targetReady)){
        // if we can construct right now, just do it
        sink->Construct(targetReady);
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "constructed\n";
        cout.flush();
#endif
    }else{
#ifdef DEBUG_OUTPUT
        cout << string(indent, ' ') << "not constructed\n";
        cout.flush();
#endif
#ifdef DEBUG_CHECKING
        if(cur_func->funcname == "allocate_cgc" && sink->symID == 8){
            __asm__("nop");
        }
#endif
        auto rootTask = cur_func->GetRootTask(sink);
        for(auto eachBBDep : rootTask->depNonReadyNonLoopBB){
            ExecuteBasicBlock(eachBBDep->BBID);
        }

        for(auto eachInBBLeaf : rootTask->inBBNonReadyLeafDeps){
            // we are forcing the dependent unready runtime value to unassign themselves
            ForwardExecution(eachInBBLeaf,true, false,sink,targetReady);
        }
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
    unsigned index = 1;

    for(; index < notifyCallVal->In_edges.size() ; index ++){
        auto setPara = dynamic_cast<SymVal_sym_set_parameter_expression*>(notifyCallVal->In_edges.at(index));
        assert(setPara != nullptr);
        BackwardExecution(setPara, notifyCallVal->ready + 1 );
    }
    //initialize the ret value to null in case it's not instrumented.
    _sym_set_return_expression(nullptr);
    notifyCallVal->ready ++;
}

void Orchestrator::SetRet() {
    auto cur_func = getCurFunc();
    auto setRetSym = cur_func->setRetSym;
    assert(setRetSym != nullptr);
    if(auto symNull = dynamic_cast<SymVal_NULL*>(setRetSym); symNull != nullptr){
        _sym_set_return_expression(nullptr);
    }else{
        BackwardExecution(setRetSym, (setRetSym->ready + 1) );
        _sym_set_return_expression(setRetSym->symExpr);
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
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout<<bb_msg->Str()<<'\n';
                cout.flush();
#endif
                ExecuteBasicBlock(bb_msg->id);
#ifdef DEBUG_OUTPUT
                cout<<"finish "<<bb_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(msg); func_msg != nullptr){
                auto nextFunc = symGraphs.at(func_msg->id);
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout<<func_msg->Str()<< ':'<<nextFunc->funcname<<'\n';
                cout.flush();
#endif
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
            }else if(auto call_msg = dynamic_cast<NotifyCallMessage*>(msg) ; call_msg != nullptr){
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
            }else if(auto ret_msg = dynamic_cast<NotifyRetMessage*>(msg) ; ret_msg != nullptr){
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
                    SetRet();
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
            }else if(auto phi_msg = dynamic_cast<PhiValueMessage*>(msg); phi_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << phi_msg->Str()<<'\n';
                cout.flush();
#endif
                auto cur_func = getCurFunc();
                auto truePhi =  dynamic_cast<SymVal_sym_TruePhi*>(cur_func->Nodes.at(cur_func->symID2offMap.at(phi_msg->symid)));
                auto val = dynamic_cast<SymVal*>(truePhi->In_edges.at(phi_msg->value));
                assert(val != nullptr);
                BackwardExecution(val, (truePhi->ready + 1) );
                truePhi->historyValues.push_back(make_pair(phi_msg->value, val->symExpr));
                truePhi->ready ++;
#ifdef DEBUG_OUTPUT
                cout<< "finish "<<phi_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }
            else{
                std::cerr<<"seriously?";
                assert(false);
            }

        }else if (auto sym_source_msg = dynamic_cast<SymSourceMessage*>(msg) ; sym_source_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_source_msg->symid));
            if(auto sym_read_mem_msg = dynamic_cast<ReadMemMessage*>(msg) ; sym_read_mem_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << sym_read_mem_msg->Str()<<'\n';
                cout.flush();
#endif
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
                ForwardExecution(realMemSymVal,false, true,nullptr, realMemSymVal->ready + 1);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<sym_read_mem_msg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if (auto runtimeBoolMsg = dynamic_cast<RuntimeBoolValueMessage*>(msg); runtimeBoolMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeBoolMsg->Str()<<'\n';
                cout.flush();
#endif
                auto runtimeBoolVal = dynamic_cast<RuntimeIntVal*>(cur_val);
                assert(runtimeBoolVal != nullptr);
                runtimeBoolVal->Assign(runtimeBoolMsg->value);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeBoolMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeIntMsg = dynamic_cast<RuntimeIntValueMessage*>(msg); runtimeIntMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeIntMsg->Str()<<'\n';
                cout.flush();
#endif
                auto runtimeIntVal = dynamic_cast<RuntimeIntVal*>(cur_val);
                assert(runtimeIntVal != nullptr);
                runtimeIntVal->Assign(runtimeBoolMsg->value);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeIntMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeFloatMsg = dynamic_cast<RuntimeFloatValueMessage*>(msg); runtimeFloatMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeFloatMsg->Str()<<'\n';
                cout.flush();
#endif
                auto runtimeFloat = dynamic_cast<RuntimeFloatVal*>(cur_val);
                assert(runtimeFloat != nullptr);
                runtimeFloat->Assign(runtimeBoolMsg->value);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeFloatMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }else if(auto runtimeDoubleMsg = dynamic_cast<RuntimeDoubleValueMessage*>(msg);runtimeDoubleMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << runtimeDoubleMsg->Str()<<'\n';
                cout.flush();
#endif
                auto runtimeDouble = dynamic_cast<RuntimeDoubleVal*>(cur_val);
                assert(runtimeDouble != nullptr);
                runtimeDouble->Assign(runtimeBoolMsg->value);
#ifdef DEBUG_OUTPUT
                cout << "finish "<<runtimeDoubleMsg->Str()<<"\n\n";
                cout.flush();
#endif
            }
        }else if (auto sym_sink_msg = dynamic_cast<SymSinkMessage*>(msg) ; sym_sink_msg != nullptr){
            SymGraph * cur_func = callStack.top();
            Val * cur_val =  cur_func->Nodes.at(cur_func->symID2offMap.at(sym_sink_msg->symid));
            if(auto sym_constraint_msg = dynamic_cast<PushConstraintMessage*>(sym_sink_msg); sym_constraint_msg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout <<  sym_constraint_msg->Str()<<'\n';
                cout.flush();
#endif
                auto constraintVal = dynamic_cast<SymVal_sym_build_path_constraint*>(cur_val);
                assert(constraintVal != nullptr);
                auto runtime_value = dynamic_cast<RuntimeIntVal*>(constraintVal->In_edges.at(1));
                assert(runtime_value != nullptr);
                runtime_value->Assign(sym_constraint_msg->runtimeVal);
                BackwardExecution(constraintVal, (constraintVal->ready + 1) );
                assert(dynamic_cast<SymVal*>(constraintVal->In_edges.at(0))->symExpr != nullptr);
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
                auto addrOperand = dynamic_cast<RuntimeIntVal*>(memWriteVal->In_edges.at(0));
                assert(addrOperand != nullptr);
                addrOperand->Assign(memWriteMsg->ptr);
                BackwardExecution(memWriteVal,memWriteVal->ready + 1 );
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
                auto destPtrVal = dynamic_cast<RuntimeIntVal*>(memcpyVal->In_edges.at(0));
                assert(destPtrVal != nullptr);
                destPtrVal->Assign(memcpyMsg->dst_ptr);

                auto srcPtrVal = dynamic_cast<RuntimeIntVal*>(memcpyVal->In_edges.at(1));
                assert(srcPtrVal != nullptr);
                srcPtrVal->Assign(memcpyMsg->src_ptr);

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
                assert(false);
            }else if(auto memmoveMsg = dynamic_cast<MemMoveMessage*>(sym_sink_msg); memmoveMsg != nullptr){
#ifdef DEBUG_OUTPUT
                assert(indent == 0);
                cout << memmoveMsg->Str()<<'\n';
                cout.flush();
#endif
                assert(false);
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