#include <Orchestrator.h>

#ifdef DEBUG_OUTPUT
extern int indent;
extern int indentNum;
#endif



int Orchestrator::ProcessMessage(Message* msg, int msgCounter) {
    int ret = 1;

    if(auto normal_msg = dynamic_cast<NormalMessage*>(msg); normal_msg != nullptr){
        uint16_t symID = normal_msg->symID;
        auto cur_func = getCurFunc();
        cur_func->changed = true;
        auto val = cur_func->Nodes.at(cur_func->symID2offMap.at(symID));
        auto symVal = dynamic_cast<SymVal*>(val);
        assert(symVal != nullptr);
        UpdateCallStackHashBB(symVal->BBID);
#if defined(DEBUG_OUTPUT)
        cout << "constructing sym val:"<<symVal->Op<<'\n';
#endif
        if(auto zero_op = dynamic_cast<NormalMessage_0OP*>(msg); zero_op != nullptr){
            symVal->Construct(symVal->ready + 1);
        }else if(auto one_8op = dynamic_cast<NormalMessage_1OP8*>(msg);one_8op!= nullptr){
            if (symVal->Op.compare("_sym_build_path_constraint") == 0){
                auto runtime_value = dynamic_cast<RuntimeIntVal*>(symVal->In_edges.at(1));
                assert(runtime_value != nullptr);
                runtime_value->Assign(one_8op->op1);
                bool executed = ExecuteNode(symVal,symVal->ready + 1);
#if defined(DEBUG_OUTPUT)
                cout << "push constraints:"<<  symVal->symExpr <<'\n';
#endif
                assert(executed);
            }else if(symVal->Op.compare("truePhi") == 0){
                auto truePhi =  dynamic_cast<SymVal_sym_TruePhi*>(val);
                assert(truePhi != nullptr);
                auto chosen_val = dynamic_cast<SymVal*>(truePhi->In_edges.at(one_8op->op1));
                auto desiredReady = truePhi->getDepTargetReady(chosen_val);
                bool executed = ExecuteNode(chosen_val, desiredReady);

                auto symExprToTake = SymVal::extractSymExprFromSymVal(chosen_val, desiredReady);
                truePhi->historyValues.push_back(make_pair(one_8op->op1, symExprToTake));
                truePhi->ready ++;
            }else if(symVal->Op.compare("_sym_notify_select") == 0){
                auto notifySelect = dynamic_cast<SymVal_sym_notify_select*>(symVal);
                assert(notifySelect != nullptr);
                auto cond_node = dynamic_cast<RuntimeIntVal*>(notifySelect->In_edges.at(0));
                assert(cond_node != nullptr);
                cond_node->Assign(one_8op->op1);
                SymVal * val = nullptr;
                if(static_cast<bool>(cond_node->Val)){
                    val = dynamic_cast<SymVal*>(notifySelect->In_edges.at(1));
                }else{
                    val = dynamic_cast<SymVal*>(notifySelect->In_edges.at(2));
                }
                auto desiredReady = notifySelect->getDepTargetReady(val);
                BackwardExecution(val, desiredReady);
                assert(notifySelect->isThisNodeReady(val, desiredReady));
                auto symExpr = SymVal::extractSymExprFromSymVal(val,desiredReady);
                notifySelect->historyValues.push_back(make_pair(one_8op->op1, symExpr));
                notifySelect->ready++;
            }else{
                std::cerr << "unhandled op one symval"<<'\n';
                assert(false);
            }
        }else if(auto one_32op = dynamic_cast<NormalMessage_1OP32*>(msg); one_32op != nullptr ){
            if(symVal->Op.compare("_sym_build_read_memory") == 0){
                auto readMemSymVal = dynamic_cast<SymVal_sym_build_read_memory*>(symVal);
                assert(readMemSymVal != nullptr);
                auto runtime_operand = dynamic_cast<RuntimeIntVal*>(readMemSymVal->In_edges.at(0));
                assert(runtime_operand != nullptr);
                runtime_operand->Assign(one_32op->op1);
                bool executed = ExecuteNode(readMemSymVal, readMemSymVal->ready + 1);
                assert(executed);
            }else if(symVal->Op.compare("_sym_build_write_memory") == 0){
                auto memWriteVal = dynamic_cast<SymVal_sym_build_write_memory*>(symVal);
                assert(memWriteVal != nullptr);
                auto addrOperand = dynamic_cast<RuntimeIntVal*>(memWriteVal->In_edges.at(0));
                assert(addrOperand != nullptr);
                addrOperand->Assign(one_32op->op1);
                bool executed = ExecuteNode(memWriteVal, memWriteVal->ready + 1);
                assert(executed);
            }
        }else if(auto two_32_32op = dynamic_cast<NormalMessage_1OP32_2OP32*>(msg); two_32_32op != nullptr){
            auto memsetVal = dynamic_cast<SymVal_sym_build_memset*>(symVal);
            assert(memsetVal != nullptr);
            auto ptrVal = dynamic_cast<RuntimePtrVal*>(memsetVal->In_edges.at(0));
            assert(ptrVal != nullptr);
            ptrVal->Assign(reinterpret_cast<void*>(two_32_32op->op1));

            auto lengthVal = dynamic_cast<RuntimeIntVal*>(memsetVal->In_edges.at(2));
            auto constLengthVal = dynamic_cast<ConstantIntVal*>(memsetVal->In_edges.at(2));
            if(lengthVal == nullptr){
                assert(constLengthVal != nullptr);
            }else{
                lengthVal->Assign(two_32_32op->op2);
            }

            bool executed = ExecuteNode(memsetVal, memsetVal->ready + 1);
            assert(executed);
        }else if(auto three_32_32_32op = dynamic_cast<NormalMessage_1OP32_2OP32_3OP32*>(msg); three_32_32_32op != nullptr){
            if(symVal->Op.compare("_sym_build_memcpy") == 0){
                auto memcpyVal = dynamic_cast<SymVal_sym_build_memcpy*>(symVal);
                assert(memcpyVal != nullptr);

                auto destPtrVal = dynamic_cast<RuntimePtrVal*>(memcpyVal->In_edges.at(0));
                assert(destPtrVal != nullptr);
                destPtrVal->Assign(reinterpret_cast<void*>(three_32_32_32op->op1));

                auto srcPtrVal = dynamic_cast<RuntimePtrVal*>(memcpyVal->In_edges.at(1));
                assert(srcPtrVal != nullptr);
                srcPtrVal->Assign(reinterpret_cast<void*>(three_32_32_32op->op2));

                auto lengthVal = dynamic_cast<RuntimeIntVal*>(memcpyVal->In_edges.at(2));
                assert(lengthVal != nullptr);
                lengthVal->Assign(three_32_32_32op->op3);

                bool executed = ExecuteNode(memcpyVal, memcpyVal->ready + 1);
                assert(executed);
            }else if(symVal->Op.compare("_sym_build_memmove") == 0){
                std::cerr<<"unsupported memmove\n";
                std::cerr.flush();
                abort();
            }else{
                std::cerr << "unsupported 3 op 32 32 32"<<'\n';
                std::cerr.flush();
                abort();
            }
        }
        else{
            cerr<<"unhandled symVal"<< symID<<'\n';
            abort();
        }
    }else if(auto call_msg = dynamic_cast<NotifyCallMessage*>(msg); call_msg != nullptr){
#ifdef DEBUG_OUTPUT
        assert(indent == 0);
        cout <<call_msg->Str()<<'\n';
        cout.flush();
#endif
        PreparingCalling(call_msg);
        // not sure if the called function is instrumented or not
        // so we just set this to false and let the NotifyFunc decide.
        callStack.push(nullptr);
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
            //get the funcID of the callee
            auto funcId = getCurFunc()->funcID;
            UpdateCallStackRet(funcId);
            SetRetAndRecycleGraph();
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
    }else if(auto func_msg = dynamic_cast<NotifyFuncMessage*>(msg); func_msg != nullptr){
        auto nextFunc = GetNextFunc(func_msg->id);
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
    }
    else if(auto end_msg = dynamic_cast<EndMessage*>(msg); end_msg != nullptr ){
        ret = 0;
    }else{
        cerr<<"unhandled message"<< msg->type<<'\n';
        abort();
    }
    return ret;
}