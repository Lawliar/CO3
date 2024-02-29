#include <Orchestrator.h>
#include <getTimeStamp.h>

int Orchestrator::ProcessMessage(Message* msg) {
    int ret = 1;
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
        }
        else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(cnt_msg); func_msg != nullptr){
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
                // assume a Func Notification is always immdediately preceded by a CallInst
                // except the first instrumented function (i.e., the entry point)
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
            // so we just set this to false and let the NotifyFunc decide.
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
        }else if(auto select_msg = dynamic_cast<SelectMessage*>(cnt_msg); select_msg != nullptr){
#ifdef DEBUG_OUTPUT
            assert(indent == 0);
                cout<<select_msg->Str()<<'\n';
                cout.flush();
#endif
            auto cur_func = getCurFunc();
            cur_func->changed = true;
            auto* notifySelect = dynamic_cast<SymVal_sym_notify_select*>(cur_func->Nodes.at(cur_func->symID2offMap.at(select_msg->symid)));
            assert(notifySelect != nullptr);

            auto* cond_node = dynamic_cast<RuntimeIntVal*>(notifySelect->In_edges.at(0));
            assert(cond_node != nullptr);
            cond_node->Assign(select_msg->cond);

            BackwardExecution(notifySelect, notifySelect->ready + 1);
#ifdef DEBUG_OUTPUT
            cout<<"finish "<<select_msg->Str()<<"\n\n";
                cout.flush();
#endif
        }
        else if(auto end_msg = dynamic_cast<EndMessage*>(cnt_msg); end_msg != nullptr ){
            ret = 0;
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

#ifdef DEBUG_CHECKING
            SymVal * cond_var = dynamic_cast<SymVal*>(buildConstraintVal->In_edges.at(0));
                assert(cond_var != nullptr);
                SymExpr cond_symExpr = SymVal::extractSymExprFromSymVal(cond_var, cond_var->ready);
#if not defined CO3_NO_MCU_SHADOW
                assert(cond_symExpr != nullptr);
#endif
#endif
#ifdef DEBUG_OUTPUT
            cout << "finish "<< sym_constraint_msg->Str()<<"\n\n";
                cout.flush();
#endif
        }else if(auto tryAltMsg = dynamic_cast<TryAltMessage*>(sym_sink_msg); tryAltMsg != nullptr){
#ifdef DEBUG_OUTPUT
            assert(false);// for now, this message should never be sent
                assert(indent == 0);
                cout << tryAltMsg->Str()<<'\n';
                cout.flush();
#endif
            auto tryAltVal = dynamic_cast<SymVal_sym_try_alternative*>(cur_val);
            assert(tryAltVal != nullptr);

            UpdateCallStackHashBB(tryAltVal->BBID);
            BackwardExecution(tryAltVal,tryAltVal->ready + 1 );
#ifdef DEBUG_OUTPUT
            cout << "finish "<<tryAltMsg->Str()<<"\n\n";
                cout.flush();
#endif
        }
        else if(auto memWriteMsg = dynamic_cast<WriteMemMessage*>(sym_sink_msg); memWriteMsg != nullptr){
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
    return ret;
}