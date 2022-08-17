//
// Created by charl on 5/25/2022.
//

#include "SymGraph.h"
#include <stack>
#include <queue>

#define CHECK_SYM_OP0(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 0);                           \
    cur_node = new SymVal##OP(symid,symidR, bbid);}

#define CHECK_SYM_OP1(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 1);             \
    cur_node = new SymVal##OP(symid,symidR, bbid, in_paras.at(0));}

#define CHECK_SYM_OP2(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 2);             \
    cur_node = new SymVal##OP(symid,symidR, bbid, in_paras.at(0),in_paras.at(1));}

#define CHECK_SYM_OP3(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 3);             \
    cur_node = new SymVal##OP(symid,symidR, bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2));}

#define CHECK_SYM_OP4(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 4);             \
    cur_node = new SymVal##OP(symid, symidR, bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2),in_paras.at(3));}

#ifdef DEBUG_OUTPUT
set<string> leaves;
set<string> nodesDepOnRuntime;
#endif

SymGraph::SymGraph(std::string funcname,std::string cfg_filename,std::string dt_filename, std::string pdt_filename, std::string dfg_filename) \
:funcname(funcname),changed(false) {
    cfg = new RuntimeCFG(cfg_filename,dt_filename, pdt_filename);
    dfg = new RuntimeSymFlowGraph(dfg_filename);
    unsigned numNodes = boost::num_vertices(dfg->graph);
    Nodes.assign(numNodes, nullptr);
    RuntimeSymFlowGraph::vertex_it dfg_vi, dfg_vi_end;
    unsigned cur = 0;// must start from 0
    for (boost::tie(dfg_vi, dfg_vi_end) = boost::vertices(dfg->graph); dfg_vi != dfg_vi_end; ++dfg_vi){
        // SINCE in vecS, Vertex_t is also unsigned int, so this part of code is just mapping N to N, but we do this just in case
        assert(ver2offMap.find(*dfg_vi) == ver2offMap.end());
        ver2offMap[*dfg_vi] = cur;
        cur ++;
    }
    unsigned NULL_sym =0; // just for sanity check
    for (boost::tie(dfg_vi, dfg_vi_end) = boost::vertices(dfg->graph); dfg_vi != dfg_vi_end; ++dfg_vi){
        RuntimeSymFlowGraph::vertex_t cur_ver = *dfg_vi;
        //unpack the info
        std::string nodeType = dfg->graph[cur_ver].nodeType;
        unsigned bbid = dfg->graph[cur_ver].BBID;
        int symid = dfg->graph[cur_ver].symID;
        unsigned width = dfg->graph[cur_ver].byteWidth;
        unsigned long const_val = dfg->graph[cur_ver].const_value;
        unsigned stageSetting = dfg->graph[cur_ver].stageSetting;
        unsigned symidR = dfg->graph[cur_ver].symIDReditect;
        std::string op = dfg->graph[cur_ver].op;
        // check in-edges
        RuntimeSymFlowGraph::in_edge_it in_eit, in_eit_end;
        boost::tie(in_eit, in_eit_end) = boost::in_edges(cur_ver, dfg->graph);
        unsigned in_degree = boost::in_degree(cur_ver, dfg->graph);

        if(symidR != 0){
            symIDReditectMap.insert(make_pair(symid, symidR));
        }
        Val* cur_node = nullptr;
        if(nodeType == NodeConstInt ||nodeType == NodeConstFloat || nodeType == NodeConstDouble){
            assert(symid == -1);
            assert(bbid == 0);
            assert(op == VoidStr);
            assert(in_degree == 0);
            if(nodeType == NodeConstInt){
                cur_node = new ConstantIntVal(bbid, const_val, width);
            }else if(nodeType == NodeConstFloat){
                cur_node = new ConstantFloatVal(bbid, static_cast< float >(const_val));
            }else if(nodeType == NodeConstDouble){
                cur_node = new ConstantDoubleVal(bbid, static_cast< double >(const_val));
            }
        }else if(nodeType == NodeRuntimeInt || nodeType == NodeRuntimeFloat || nodeType == NodeRuntimeDouble || nodeType == NodeRuntimePtr ){
            assert(symid == -1);
            assert(op == VoidStr);
            assert(in_degree == 0);
            if(nodeType == NodeRuntimeInt){
                cur_node = new RuntimeIntVal(bbid, width);
            }else if(nodeType == NodeRuntimeFloat){
                cur_node = new RuntimeFloatVal(bbid);
            }else if(nodeType == NodeRuntimeDouble){
                cur_node = new RuntimeDoubleVal(bbid);
            }else if(nodeType == NodeRuntimePtr){
                cur_node = new RuntimePtrVal(bbid);
            }
        }else if ( nodeType == NodeSym){
            assert(symid >= 0);
            map<unsigned, unsigned> in_paras;
            for(;in_eit != in_eit_end; in_eit++ ){
                unsigned arg_index = dfg->graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg->graph);
                assert(in_paras.find(arg_index) == in_paras.end());
                in_paras[arg_index] = ver2offMap.at(source);
            }
            if(op == VoidStr){
                assert(bbid == 0);
                cur_node = new SymVal_NULL(symid,symidR, bbid);
                dynamic_cast<SymVal_NULL*>(cur_node)->symExpr = nullptr;
                NULL_sym++;
                assert(NULL_sym == 1);
            } else if(op == "_sym_notify_call"){
                cur_node = new SymVal_sym_notify_call(symid,symidR, bbid, in_paras);
            }
            CHECK_SYM_OP2(_sym_try_alternative)
            CHECK_SYM_OP2(_sym_build_integer)
            CHECK_SYM_OP2(_sym_build_float)
            CHECK_SYM_OP0(_sym_build_null_pointer)
            CHECK_SYM_OP0(_sym_build_true)
            CHECK_SYM_OP0(_sym_build_false)
            CHECK_SYM_OP1(_sym_build_bool)
            CHECK_SYM_OP1(_sym_build_neg)
            CHECK_SYM_OP2(_sym_build_add)
            CHECK_SYM_OP2(_sym_build_sub)
            CHECK_SYM_OP2(_sym_build_mul)
            CHECK_SYM_OP2(_sym_build_unsigned_div)
            CHECK_SYM_OP2(_sym_build_signed_div)
            CHECK_SYM_OP2(_sym_build_unsigned_rem)
            CHECK_SYM_OP2(_sym_build_signed_rem)
            CHECK_SYM_OP2(_sym_build_shift_left)
            CHECK_SYM_OP2(_sym_build_logical_shift_right)
            CHECK_SYM_OP2(_sym_build_arithmetic_shift_right)
            CHECK_SYM_OP2(_sym_build_fp_add)
            CHECK_SYM_OP2(_sym_build_fp_sub)
            CHECK_SYM_OP2(_sym_build_fp_mul)
            CHECK_SYM_OP2(_sym_build_fp_div)
            CHECK_SYM_OP2(_sym_build_fp_rem)
            CHECK_SYM_OP1(_sym_build_fp_abs)
            CHECK_SYM_OP1(_sym_build_not)
            CHECK_SYM_OP2(_sym_build_signed_less_than)
            CHECK_SYM_OP2(_sym_build_signed_less_equal)
            CHECK_SYM_OP2(_sym_build_signed_greater_than)
            CHECK_SYM_OP2(_sym_build_signed_greater_equal)
            CHECK_SYM_OP2(_sym_build_unsigned_less_than)
            CHECK_SYM_OP2(_sym_build_unsigned_less_equal)
            CHECK_SYM_OP2(_sym_build_unsigned_greater_than)
            CHECK_SYM_OP2(_sym_build_unsigned_greater_equal)
            CHECK_SYM_OP2(_sym_build_equal)
            CHECK_SYM_OP2(_sym_build_not_equal)
            CHECK_SYM_OP2(_sym_build_bool_and)
            CHECK_SYM_OP2(_sym_build_and)
            CHECK_SYM_OP2(_sym_build_bool_or)
            CHECK_SYM_OP2(_sym_build_or)
            CHECK_SYM_OP2(_sym_build_bool_xor)
            CHECK_SYM_OP2(_sym_build_xor)
            CHECK_SYM_OP2(_sym_build_float_ordered_greater_than)
            CHECK_SYM_OP2(_sym_build_float_ordered_greater_equal)
            CHECK_SYM_OP2(_sym_build_float_ordered_less_than)
            CHECK_SYM_OP2(_sym_build_float_ordered_less_equal)
            CHECK_SYM_OP2(_sym_build_float_ordered_equal)
            CHECK_SYM_OP2(_sym_build_float_ordered_not_equal)
            CHECK_SYM_OP2(_sym_build_float_ordered)
            CHECK_SYM_OP2(_sym_build_float_unordered)
            CHECK_SYM_OP2(_sym_build_float_unordered_greater_than)
            CHECK_SYM_OP2(_sym_build_float_unordered_greater_equal)
            CHECK_SYM_OP2(_sym_build_float_unordered_less_than)
            CHECK_SYM_OP2(_sym_build_float_unordered_less_equal)
            CHECK_SYM_OP2(_sym_build_float_unordered_equal)
            CHECK_SYM_OP2(_sym_build_float_unordered_not_equal)
            CHECK_SYM_OP2(_sym_build_sext)
            CHECK_SYM_OP2(_sym_build_zext)
            CHECK_SYM_OP2(_sym_build_trunc)
            CHECK_SYM_OP1(_sym_build_bswap)
            CHECK_SYM_OP3(_sym_build_int_to_float)
            CHECK_SYM_OP2(_sym_build_float_to_float)
            CHECK_SYM_OP2(_sym_build_bits_to_float)
            CHECK_SYM_OP1(_sym_build_float_to_bits)
            CHECK_SYM_OP2(_sym_build_float_to_signed_integer)
            CHECK_SYM_OP2(_sym_build_float_to_unsigned_integer)
            CHECK_SYM_OP2(_sym_build_bool_to_bits)
            CHECK_SYM_OP2(_sym_set_parameter_expression)
            CHECK_SYM_OP1(_sym_get_parameter_expression)
            CHECK_SYM_OP1(_sym_set_return_expression)
            CHECK_SYM_OP0(_sym_get_return_expression)
            CHECK_SYM_OP2(_sym_build_path_constraint)
            CHECK_SYM_OP3(_sym_build_read_memory)
            CHECK_SYM_OP4(_sym_build_write_memory)
            CHECK_SYM_OP3(_sym_build_memcpy)
            CHECK_SYM_OP3(_sym_build_memset)
            CHECK_SYM_OP3(_sym_build_memmove)
            CHECK_SYM_OP4(_sym_build_insert)
            CHECK_SYM_OP4(_sym_build_extract)
            else{
                cerr<<"unhandled op:"<<op<<'\n';
                assert(false);
            }
        }

        else if( nodeType == NodeTruePhi ){
            map<unsigned short, unsigned short> in_paras;
            map<unsigned short, unsigned short> argNo2BBMap;
            for(;in_eit != in_eit_end; in_eit++ ){
                unsigned arg_index = dfg->graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg->graph);
                assert(in_paras.find(arg_index) == in_paras.end());
                in_paras[arg_index] = ver2offMap.at(source);
                assert(dfg->graph[*in_eit].incomingBB > 0);
                argNo2BBMap[arg_index] = dfg->graph[*in_eit].incomingBB;
            }
            cur_node = new  SymVal_sym_TruePhi(symid,symidR, bbid, in_paras,argNo2BBMap);

        }else if(nodeType == NodeFalseRootPhi || nodeType == NodeFalseLeafPhi ){
            map<unsigned short, unsigned short> in_paras;
            set<Val::ValVertexType> leavesOrOriginal;// since both original to the falsePhiLeaf and falsePhiLeaf to root are all marked edged 2
            for(;in_eit != in_eit_end; in_eit++ ){
                unsigned arg_index = dfg->graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg->graph);
                if(arg_index == 0 || arg_index == 1){
                    assert(in_paras.find(arg_index) == in_paras.end());
                    in_paras[arg_index] = ver2offMap.at(source);
                    assert(dfg->graph[*in_eit].incomingBB > 0);
                }else{
                    assert(arg_index == 2);
                    leavesOrOriginal.insert(ver2offMap.at(source));
                }
            }
            assert(in_paras.size() == 2);
            if(nodeType == NodeFalseRootPhi){
                cur_node = new SymVal_sym_FalsePhiRoot(symid,symidR, bbid, in_paras, leavesOrOriginal);
            }else{
                cur_node = new SymVal_sym_FalsePhiLeaf(symid,symidR, bbid, in_paras, leavesOrOriginal);
            }
        }
        if(stageSetting == 1){
            if(auto tmpGetPara = dynamic_cast<SymVal_sym_get_parameter_expression*>(cur_node) ; tmpGetPara != nullptr){
                getParametersSym.push_back(tmpGetPara);
            }
            if(auto tmpSetRet = dynamic_cast<SymVal_sym_set_return_expression*>(cur_node); tmpSetRet != nullptr){
                setRetSym = tmpSetRet;
            }
        }
        Nodes[ver2offMap.at(cur_ver)] = cur_node;
    }
    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        // make sure every node is parsed.
        Val* cur_node = Nodes[index];
        assert(cur_node!= nullptr);
        //convert tmpInEdge to the real In_edges
        for(auto eachTmpInEdge : cur_node->tmpIn_edges){
            cur_node->In_edges.insert(make_pair(eachTmpInEdge.first, Nodes.at(eachTmpInEdge.second)));
        }
        cur_node->tmpIn_edges.clear(); // not gonna use it anymore
        // complete the falsePhiRoot For the falsePhiLeaves
        if(auto falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node); falsePhiRoot!= nullptr){
            for(auto eachFalsePhiLeaf: falsePhiRoot->tmpfalsePhiLeaves){
                auto leafNode = Nodes.at(eachFalsePhiLeaf);
                auto falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(leafNode);
                falsePhiRoot->falsePhiLeaves.insert(leafNode);
                if(falsePhiLeaf != nullptr){
#ifdef DEBUG_CHECKING
                    assert(falsePhiRoot->tmpfalsePhiLeaves.size() > 1);
                    //falsePhiLeaf->root = falsePhiRoot;
#endif
                }
            }
            falsePhiRoot->tmpfalsePhiLeaves.clear();// not gonna use it
        }else if(auto falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node); falsePhiLeaf != nullptr){
            for(auto eachPeerOri : falsePhiLeaf->tmpPeerOriginals){
                auto peerOriginal = dynamic_cast<SymVal*>(Nodes.at(eachPeerOri));
#ifdef DEBUG_CHECKING
                assert(peerOriginal != nullptr);
#endif
                falsePhiLeaf->peerOriginals.insert(peerOriginal);
            }
        }
    }
    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        // some sanity check for some special node
        if(SymVal_sym_FalsePhiRoot * falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(Nodes[index])){
            SymVal_NULL * op1 = dynamic_cast<SymVal_NULL*>(falsePhiRoot->In_edges.at(0));
            SymVal_NULL * op2 = dynamic_cast<SymVal_NULL*>(falsePhiRoot->In_edges.at(1));
            assert(op1 != nullptr && op2 == nullptr); // op1 is always pointing to false, op2 is always not pointing to false
        }
        if(SymVal_sym_FalsePhiLeaf * falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(Nodes[index])){
            SymVal_NULL * op1 = dynamic_cast<SymVal_NULL*>(falsePhiLeaf->In_edges.at(0));
            SymVal_NULL * op2 = dynamic_cast<SymVal_NULL*>(falsePhiLeaf->In_edges.at(1));
            assert(op1 == nullptr && op2 == nullptr); // both of the operands are not pointing to false
        }
        // end of the sanity check

        Val* cur_node = Nodes[index];
        // complete the use relation
        if(cur_node->BBID == 0){
            cur_node->inLoop = false;
        }else{
            cur_node->inLoop = cfg->bbid2loop.at(cur_node->BBID);
        }
        for(auto each_in_edge : cur_node->In_edges){
            each_in_edge.second->UsedBy.insert(cur_node);
        }
        // falsePhiLeaf is used by falsePhiRoot as well
        if(auto tmpFalsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node)){
            for(auto eachLeaf : tmpFalsePhiRoot->falsePhiLeaves){
                eachLeaf->UsedBy.insert(tmpFalsePhiRoot);
            }
        }
        // peer's Originals are used by every falseLeaf as well
        if(auto tmpFalsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node)){
            for(auto eachPeerOriginal : tmpFalsePhiLeaf->peerOriginals){
                eachPeerOriginal->UsedBy.insert(cur_node);
            }
        }

        //complete the symid to off map
        if(SymVal* symval = dynamic_cast<SymVal*>(cur_node) ; symval != nullptr){
            Val::SymIDType symid = symval->symID;
            assert(symID2offMap.find(symid) == symID2offMap.end());
            symID2offMap[symid] = static_cast<Val::ValVertexType>(index);
        }
    }
    /*
    // just to debugging and know more
    for(auto eachNode : Nodes){
        if(auto tmpRuntime = dynamic_cast<RuntimeVal*>(eachNode)){
            if(tmpRuntime->UsedBy.size() > 1){
                __asm__("nop");
            }
            for(auto eachUser : tmpRuntime->UsedBy){
                auto symVal = dynamic_cast<SymVal*>(eachUser);
                assert(symVal != nullptr);

                nodesDepOnRuntime.insert(symVal->Op);
                auto buildBool = dynamic_cast<SymVal_sym_build_bool*>(symVal);
                auto buildFloat = dynamic_cast<SymVal_sym_build_float*>(symVal);
                auto buildInt = dynamic_cast<SymVal_sym_build_integer*>(symVal);
                if(buildBool != nullptr || buildFloat != nullptr || buildInt != nullptr){
                    if(symVal->UsedBy.size() > 1){
                        __asm__("nop");
                    }
                    for(auto eachUserUser : symVal->UsedBy){
                        auto symValVal = dynamic_cast<SymVal*>(eachUserUser);
                        nodesDepOnRuntime.insert(symVal->Op);
                    }
                }
            }
        }
    }


    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        auto * node = Nodes.at(index);
        if(node->In_edges.size() == 0){
            if(auto symVal = dynamic_cast<SymVal*>(node); symVal != nullptr){
                leaves.insert(symVal->Op);
            }else{
                leaves.insert(node->Str());
            }
        }
    }*/
    //

    // get the getPara setRet, callInst for this function

    for(auto nodeIt = Nodes.begin(); nodeIt != Nodes.end(); nodeIt ++){
        if(auto notifyCall = dynamic_cast<SymVal_sym_notify_call*>(*nodeIt)){
            auto idOperand = dynamic_cast<ConstantIntVal*>(notifyCall->In_edges.at(0));
            assert(idOperand != nullptr);
            unsigned char callInstId = idOperand->Value;
            assert(callInsts.find(callInstId) == callInsts.end());
            callInsts[callInstId] = notifyCall;
        }
    }
    prepareBBTask();
    //assert(setRetSym != nullptr);
}

#define COPY_CONSTRUCT_VAL(OP)               \
    else if(auto old_Val = dynamic_cast<OP*>(old_one); old_Val != nullptr){                          \
        new_one = new OP(*old_Val);  \
    }
#define COPY_CONSTRUCT_SymVAL(SYMOP)               \
    else if(auto old_Val = dynamic_cast<SymVal##SYMOP*>(old_one); old_Val != nullptr){                          \
        new_one = new SymVal##SYMOP(*old_Val);  \
    }
SymGraph::SymGraph(const SymGraph& other):funcname(other.funcname),symID2offMap(other.symID2offMap),changed(false),symIDReditectMap(other.symIDReditectMap){
    //we're not going to use these 2 anyway
    cfg = nullptr;
    dfg = nullptr;
    // this also won't be used
    assert(ver2offMap.size() == 0);

    Nodes.assign(other.Nodes.size(), nullptr);
    std::map<Val*, Val*> old2new;
    for(auto i = 0 ; i < other.Nodes.size() ; i ++){
        Val * old_one = other.Nodes[i];
        Val * new_one = nullptr;
        if(auto old_Val = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(old_one); old_Val != nullptr){
            new_one = new SymVal_sym_FalsePhiLeaf(*old_Val);
        }else if(auto old_Val = dynamic_cast<SymVal_sym_FalsePhiRoot*>(old_one); old_Val != nullptr){
            new_one = new SymVal_sym_FalsePhiRoot(*old_Val);
        }COPY_CONSTRUCT_VAL(ConstantIntVal)
        COPY_CONSTRUCT_VAL(ConstantFloatVal)
        COPY_CONSTRUCT_VAL(ConstantDoubleVal)
        COPY_CONSTRUCT_VAL(RuntimeIntVal)
        COPY_CONSTRUCT_VAL(RuntimeFloatVal)
        COPY_CONSTRUCT_VAL(RuntimeDoubleVal)
        COPY_CONSTRUCT_VAL(RuntimePtrVal)
        COPY_CONSTRUCT_SymVAL(_sym_notify_call)
        COPY_CONSTRUCT_SymVAL(_sym_try_alternative)
        COPY_CONSTRUCT_SymVAL(_NULL)
        COPY_CONSTRUCT_SymVAL(_sym_build_integer)
        COPY_CONSTRUCT_SymVAL(_sym_build_float)
        COPY_CONSTRUCT_SymVAL(_sym_build_null_pointer)
        COPY_CONSTRUCT_SymVAL(_sym_build_true)
        COPY_CONSTRUCT_SymVAL(_sym_build_false)
        COPY_CONSTRUCT_SymVAL(_sym_build_bool)
        COPY_CONSTRUCT_SymVAL(_sym_build_neg)
        COPY_CONSTRUCT_SymVAL(_sym_build_add)
        COPY_CONSTRUCT_SymVAL(_sym_build_sub)
        COPY_CONSTRUCT_SymVAL(_sym_build_mul)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_div)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_div)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_rem)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_rem)
        COPY_CONSTRUCT_SymVAL(_sym_build_shift_left)
        COPY_CONSTRUCT_SymVAL(_sym_build_logical_shift_right)
        COPY_CONSTRUCT_SymVAL(_sym_build_arithmetic_shift_right)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_add)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_sub)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_mul)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_div)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_rem)
        COPY_CONSTRUCT_SymVAL(_sym_build_fp_abs)
        COPY_CONSTRUCT_SymVAL(_sym_build_not)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_less_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_less_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_greater_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_signed_greater_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_less_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_less_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_greater_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_unsigned_greater_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_not_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_bool_and)
        COPY_CONSTRUCT_SymVAL(_sym_build_and)
        COPY_CONSTRUCT_SymVAL(_sym_build_bool_or)
        COPY_CONSTRUCT_SymVAL(_sym_build_or)
        COPY_CONSTRUCT_SymVAL(_sym_build_bool_xor)
        COPY_CONSTRUCT_SymVAL(_sym_build_xor)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_greater_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_greater_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_less_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_less_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered_not_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_ordered)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_greater_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_greater_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_less_than)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_less_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_unordered_not_equal)
        COPY_CONSTRUCT_SymVAL(_sym_build_sext)
        COPY_CONSTRUCT_SymVAL(_sym_build_zext)
        COPY_CONSTRUCT_SymVAL(_sym_build_trunc)
        COPY_CONSTRUCT_SymVAL(_sym_build_bswap)
        COPY_CONSTRUCT_SymVAL(_sym_build_int_to_float)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_to_float)
        COPY_CONSTRUCT_SymVAL(_sym_build_bits_to_float)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_to_bits)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_to_signed_integer)
        COPY_CONSTRUCT_SymVAL(_sym_build_float_to_unsigned_integer)
        COPY_CONSTRUCT_SymVAL(_sym_build_bool_to_bits)
        COPY_CONSTRUCT_SymVAL(_sym_set_parameter_expression)
        COPY_CONSTRUCT_SymVAL(_sym_get_parameter_expression)
        COPY_CONSTRUCT_SymVAL(_sym_set_return_expression)
        COPY_CONSTRUCT_SymVAL(_sym_get_return_expression)
        COPY_CONSTRUCT_SymVAL(_sym_build_path_constraint)
        COPY_CONSTRUCT_SymVAL(_sym_build_read_memory)
        COPY_CONSTRUCT_SymVAL(_sym_build_write_memory)
        COPY_CONSTRUCT_SymVAL(_sym_build_memcpy)
        COPY_CONSTRUCT_SymVAL(_sym_build_memset)
        COPY_CONSTRUCT_SymVAL(_sym_build_memmove)
        COPY_CONSTRUCT_SymVAL(_sym_build_insert)
        COPY_CONSTRUCT_SymVAL(_sym_build_extract)
        COPY_CONSTRUCT_SymVAL(_sym_TruePhi)
        else{
            std::cerr << " unhandled node in copy constructing type";
            assert(false);
        }
        Nodes[i] = new_one;
        //old2new[old_one] = new_one;

        old2new.insert(make_pair(old_one, new_one));
    }
    for(auto each_old2new:  old2new){
        auto old_one = each_old2new.first;
        auto new_one = each_old2new.second;
        new_one->FinishCopyConstructing(old_one->In_edges, old_one->UsedBy, old2new);
        if(auto new_false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(new_one); new_false_phi_root != nullptr){
            auto old_false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(old_one);
            new_false_phi_root->FinishCopyConstructing(old_false_phi_root->falsePhiLeaves,old2new);
        }else if(auto new_false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(new_one); new_false_phi_leaf != nullptr){
            auto old_false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(old_one);
            new_false_phi_leaf->FinishCopyConstructing(old_false_phi_leaf->peerOriginals,old2new);
        }
    }
    for(auto eachOldGetPara : other.getParametersSym){
        auto newGetPara = dynamic_cast<SymVal_sym_get_parameter_expression*>(old2new.at(eachOldGetPara));
        assert(newGetPara != nullptr);
        getParametersSym.push_back(newGetPara);
    }
    if(other.setRetSym != nullptr){
        auto new_set_ret = dynamic_cast<SymVal_sym_set_return_expression*>(old2new.at(other.setRetSym));
        assert(new_set_ret != nullptr);
        setRetSym = new_set_ret;
    }

    for(auto old_call : other.callInsts){
        auto new_call = dynamic_cast<SymVal_sym_notify_call*>(old2new.at(old_call.second));
        assert(new_call != nullptr);
        callInsts.insert(make_pair(old_call.first, new_call));
    }

    assert(rootTasks.size() == 0);
    for(auto eachOldBBTask : other.bbTasks){
        auto* oldBBTask = eachOldBBTask.second;
        auto* newBBTask = new BasicBlockTask(*oldBBTask);

        newBBTask->FinishCopyConstructing(oldBBTask->nonReadyRoots, oldBBTask->leaves, oldBBTask->roots,old2new);
        bbTasks.insert(make_pair(eachOldBBTask.first,newBBTask));
    }

}
std::set<Val::BasicBlockIdType> SymGraph::domChildrenOf(Val::BasicBlockIdType src_id, map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t> id2vertMap, RuntimeCFG::DominanceTree& dGraph) {

    std::set<Val::BasicBlockIdType> visited;
    std::stack<Val::BasicBlockIdType> work_stack;
    // src_id  dominates/post dominates itself
    visited.insert(src_id);
    work_stack.push(src_id);

    while(!work_stack.empty()) {
        Val::BasicBlockIdType bbid = work_stack.top();
        RuntimeCFG::pd_vertex_t cur_vert = id2vertMap.at(bbid);
        work_stack.pop();
        RuntimeCFG::pd_oedge_it pd_eit, pd_eit_end;
        for(boost::tie(pd_eit,pd_eit_end) = boost::out_edges(cur_vert,dGraph); pd_eit!= pd_eit_end; ++pd_eit){
            RuntimeCFG::pd_vertex_t  target_node = boost::target(*pd_eit, dGraph);
            unsigned next_id = dGraph[target_node].id;
            if(visited.find(next_id) == visited.end()){
                visited.insert(next_id);
                work_stack.push(next_id);
            }
        }
    }
    return visited;
}

void DataDependents::nonTruePhiDataDependentsOf( set<Val*> ancesters){
    ancesters.insert(root);
    if(SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(root); true_phi != nullptr){
        truePhiDependences[root] = map<Val::ArgIndexType, DataDependents* >{};
        for(auto eachTruePhiDep: dynamic_cast<SymVal_sym_TruePhi*>(root)->In_edges){
            Val* new_root = eachTruePhiDep.second;
            if(ancesters.find(new_root) == ancesters.end()){
                DataDependents* child_node = new DataDependents(new_root);
                truePhiDependences.at(root)[eachTruePhiDep.first] = child_node;
                child_node->nonTruePhiDataDependentsOf(ancesters);
            }
        }
        return;
    }

    std::stack<Val*> work_stack;
    vector<Val*> truePhis;
    for(auto eachRootDep : root->In_edges){
        deps.insert(eachRootDep.second);
        work_stack.push(eachRootDep.second);
    }
    while(!work_stack.empty()){
        Val* cur_ver = work_stack.top();
        work_stack.pop();
        if(SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(cur_ver); true_phi != nullptr){
            truePhis.push_back(cur_ver);
        }
        for(auto eachWorkDep : cur_ver->In_edges){
            Val* new_depNpde = eachWorkDep.second;
            if(deps.find(new_depNpde) == deps.end() && ancesters.find(new_depNpde) == ancesters.end()){
                deps.insert(new_depNpde);
                ancesters.insert(new_depNpde);
                work_stack.push(eachWorkDep.second);
            }
        }
    }
    for(auto eachTruePhi: truePhis){
        truePhiDependences[eachTruePhi] = map<Val::ArgIndexType, DataDependents* >{};
        for(auto eachTruePhiDep: dynamic_cast<SymVal_sym_TruePhi*>(eachTruePhi)->In_edges){
            Val* new_root = eachTruePhiDep.second;
            if(ancesters.find(new_root) == ancesters.end()){
                DataDependents* child_node = new DataDependents(new_root);
                truePhiDependences.at(eachTruePhi)[eachTruePhiDep.first] = child_node;
                child_node->nonTruePhiDataDependentsOf(ancesters);
            }
        }
    }
}

void DataDependents::allPossibleDataDependencies(set<Val*> parent, vector<set<Val*> >& all) {
    parent.insert(deps.begin(), deps.end());
    if(truePhiDependences.empty()){
        all.push_back(parent);
    }
    for(auto eachTruePhi : truePhiDependences){
        for(auto eachValueOfTruePhi : eachTruePhi.second){
            eachValueOfTruePhi.second->allPossibleDataDependencies(parent, all);
        }
    }
}

bool SymGraph::sortNonLoopBB(BasicBlockTask* a, BasicBlockTask* b) {
    if(a->dominance.find(b->BBID) != a->dominance.end()){
        // a dominate b
        return true;
    } else if (b->post_dominance.find(a->BBID) != b->post_dominance.end()){
        // b post-dominate a
        return true;
    }else{
        //b dominate a OR a post-dominate b
        assert(b->dominance.find(a->BBID) != b->dominance.end() || a->post_dominance.find(b->BBID) != a->post_dominance.end());

        return false;
    }
}


#ifdef DEBUG_OUTPUT
set<string> SinkOps;
set<string> leaveOps;


void SymGraph::dbgBBLeaves(Val::ValVertexType v) {
    SymVal*         tmpSymVal = dynamic_cast<SymVal*>(Nodes.at(ver2offMap.at(ver2offMap.at(v))));
    RuntimeVal* tmpRuntime =  dynamic_cast<RuntimeVal*>(Nodes.at(ver2offMap.at(ver2offMap.at(v))));
    ConstantVal* tmpConst =  dynamic_cast<ConstantVal*>(Nodes.at(ver2offMap.at(ver2offMap.at(v))));
    if(tmpSymVal != nullptr){
        leaveOps.insert(tmpSymVal->Op);
        if(tmpSymVal->Op == "_sym_FalsePhi"){
            __asm__("nop");
        }
        if(tmpSymVal->Op == "_sym_TruePhi"){
            __asm__("nop");
        }
    }else if (tmpRuntime != nullptr){
        leaveOps.insert("Runtime");
    }else if(tmpConst != nullptr){
        leaveOps.insert("Const");
    }else{
        assert(false);
    }
}

void SymGraph::dbgBBRoot(Val::ValVertexType v) {
    SymVal* tmpSymVal = dynamic_cast<SymVal*>(Nodes.at(ver2offMap.at(v)));
    if(tmpSymVal != nullptr){
        SinkOps.insert(tmpSymVal->Op);
        if(tmpSymVal->Op == "_sym_FalsePhi"){
            __asm__("nop");
        }
        if(tmpSymVal->Op == "_sym_TruePhi"){
            __asm__("nop");
        }
    }else{
        RuntimeVal* tmpRuntime =  dynamic_cast<RuntimeVal*>(Nodes.at(ver2offMap.at(v)));
        assert( tmpRuntime != nullptr);
        SinkOps.insert("Runtime");
    }
}
#endif
/*
Val* SymGraph::stripPhis(Val* nodeInQuestion, Val* root) {
    Val* prev_node = nullptr;
    Val* cur_node = nodeInQuestion;
    SymVal_sym_TruePhi *      true_phi       = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
    SymVal_sym_FalsePhiRoot * false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node);
    SymVal_sym_FalsePhiLeaf * false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node);

    while(true_phi != nullptr || false_phi_root != nullptr || false_phi_leaf != nullptr ){
        // assuming the ReportTruePhi has been sent from the MCU.
        Val* new_vert;
        if(true_phi != nullptr){
            // for truePhi, its ready number might legally be larger than root_ready
            // we already execute truePhi here, just return prevNode
            return prev_node;
        }else if(false_phi_root != nullptr){
            if(false_phi_root->falsePhiLeaves.size() == 0){
                // this root has all constant dependencies, which makes it impossible to be symbolized.
                return nullptr;
            }
            if(root->isThisNodeReady( cur_node, (root->ready + 1) )){
                // cur_node is already constructed
                return prev_node;
            }
            new_vert = false_phi_root->In_edges.at(1);// 1 is the non-false value

        }else{
            if(root->isThisNodeReady(cur_node, (root->ready + 1) )){
                // cur_node is already constructed
                return prev_node;
            }
            new_vert = false_phi_leaf->In_edges.at(0);// 0 is the original value
        }
        prev_node = cur_node;
        cur_node = new_vert;
        true_phi = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
        false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node);
        false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node);
    }
    return cur_node;
}

Val* SymGraph::stripTruePhi(Val* nodeInQuestion, Val* root) {
    Val* cur_node = nodeInQuestion;
    SymVal_sym_TruePhi *      true_phi       = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);

    while(true_phi != nullptr  ){
        // assuming the ReportTruePhi has been sent from the MCU.
        Val* new_vert;
        if(true_phi != nullptr){
            // for truePhi, its ready number might legally be larger than root_ready, we'll just choose the history value we want
            auto whichBranchItTook = true_phi->historyValues.at(root->ready + 1).first;
            new_vert = true_phi->In_edges.at(whichBranchItTook);
        }
        cur_node = new_vert;
        true_phi = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
    }
    return cur_node;
}
*/
bool SymGraph::ReleaseOrRemoveRootTask(SymVal * root) {
    if(funcname == "receive_cgc_until" && root->symID == 25){
        __asm__("nop");
    }
    if(rootTasks.find(root) != rootTasks.end()){
        auto * target_rootTask = rootTasks.at(root);
        if(target_rootTask->occupied){
            return false;
        }
        if(root->ready == (target_rootTask->old_ready + 1)){
            rootTasks.erase(root);
            delete target_rootTask;
            return true;
        }else{
#ifdef DEBUG_CHECKING
            assert(root->ready == target_rootTask->old_ready);
#endif
            return false;
        }
    }else{
        return false;
    }
}

SymGraph::RootTask* SymGraph::GetRootTask(SymVal * root) {
    {
        // dont' regard true phi as a root, as its ready value does not reflect it's constructed or not.
        SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(root);
        assert(true_phi == nullptr);
    }
    Val::ReadyType cur_ready = root->ready;
    if(rootTasks.find(root) != rootTasks.end()){
        RootTask* old_rootTask = rootTasks.at(root);

        assert(! old_rootTask->occupied);
        //only refresh when the ready value matches
        // otherwise, this task is not built for that, should start over
        assert(old_rootTask->old_ready == cur_ready);

        old_rootTask->occupied = true;
        old_rootTask->Refresh();
        return old_rootTask;
    }
#ifdef DEBUG_CHECKING
    if(funcname == "receive_cgc_until" && root->symID){
        __asm__("nop");
    }
#endif
    SymGraph::BasicBlockTask* rootBBTask = bbTasks.at(root->BBID);
    RootTask* rootTask = new RootTask(root, rootBBTask);
    queue<Val*> work_queue;

    set<Val*> visited {root};
    work_queue.push(root);
    rootTask->InsertNonReadyDep(root,bbTasks);
    visited.insert(root);

    while(!work_queue.empty()){
        Val* cur_node = work_queue.front();

        work_queue.pop();

        // HANDLE Phis
        {
            // because truePhi is always ready(like BB in the loop), so it shouldn't appear here
            SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
            assert(true_phi == nullptr);
            assert(!root->isThisNodeReady(cur_node,(root->ready + 1)));
        }
        for(auto eachNodeToAdd: cur_node->realChildren()){
            if(root->isThisNodeReady(eachNodeToAdd, (root->ready + 1) )){
                //this dep has been constructed
                continue;
            }
            if(visited.find((eachNodeToAdd)) != visited.end()){
                // this node has been visited before
                continue;
            }
#ifdef DEBUG_CHECKING
            auto tmpConstantVal = dynamic_cast<ConstantVal*>(eachNodeToAdd);
            assert(tmpConstantVal == nullptr);
#endif
            work_queue.push(eachNodeToAdd);
            rootTask->InsertNonReadyDep(eachNodeToAdd,bbTasks);
            visited.insert(eachNodeToAdd);
        }
    }
#ifdef DEBUG_CHECKING
    for(auto eachNonReadyNonLoopBB : rootTask->depNonReadyNonLoopBB){
        assert(eachNonReadyNonLoopBB->ready == 0);
        assert(eachNonReadyNonLoopBB->inLoop == false);
    }
#endif

    for(auto eachInBBNonReadyDep : rootTask->inBBNonReadyDeps){
        bool allChildReady = true;
        for(auto eachRealChild: eachInBBNonReadyDep->realChildren()){

            if( eachInBBNonReadyDep->isThisNodeReady(eachRealChild,(root->ready + 1))){
                continue;
            }else{
                if(eachRealChild->BBID != eachInBBNonReadyDep->BBID){
                    // if eachInBBNonReadyDep(A) is dependening on a non-ready nonde(B) in another non-loop BB
                    //we can assume (B) is ready, since we're going to execute the BB
                    assert(std::find(rootTask->depNonReadyNonLoopBB.begin(), rootTask->depNonReadyNonLoopBB.end(), bbTasks.at(eachRealChild->BBID)) != rootTask->depNonReadyNonLoopBB.end());
                    continue;
                }
#ifdef DEBUG_CHECKING
                assert(rootTask->inBBNonReadyDeps.find(eachRealChild) != rootTask->inBBNonReadyDeps.end());
#endif
                allChildReady = false;
            }
        }
        if(allChildReady){
            rootTask->inBBNonReadyLeafDeps.insert(eachInBBNonReadyDep);
        }
    }
    std::sort(rootTask->depNonReadyNonLoopBB.begin(), rootTask->depNonReadyNonLoopBB.end(), [this] (BasicBlockTask* a, BasicBlockTask* b) {
        return sortNonLoopBB(a, b);});

    rootTasks.insert(make_pair(root,rootTask));
    rootTask->occupied = true;
    return rootTask;
}
void SymGraph::prepareBBTask() {
    RuntimeCFG::vertex_it cfg_vi,cfg_vi_end;

    // dominance BBID 2 Vert Map
    map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t> dID2VertMap;
    RuntimeCFG::pd_vertex_it pd_v_it, pd_v_it_end;
    boost::tie(pd_v_it, pd_v_it_end) = boost::vertices(cfg->domTree);
    for(;pd_v_it != pd_v_it_end ; pd_v_it++){
        Val::BasicBlockIdType cur_bbid = cfg->domTree[*pd_v_it].id;
        dID2VertMap[cur_bbid] = *pd_v_it;
    }
    //postDom BBID 2 Vert Map
    map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t> pdId2VertMap;
    boost::tie(pd_v_it, pd_v_it_end) = boost::vertices(cfg->postDomTree);
    for(;pd_v_it != pd_v_it_end ; pd_v_it++){
        Val::BasicBlockIdType cur_bbid = cfg->postDomTree[*pd_v_it].id;
        pdId2VertMap[cur_bbid] = *pd_v_it;
    }

    // prepare per-BB task

    for(boost::tie(cfg_vi, cfg_vi_end) = boost::vertices(cfg->graph); cfg_vi != cfg_vi_end; cfg_vi++){
        unsigned cur_bbid = cfg->graph[*cfg_vi].id;
        bool inLoop = cfg->graph[*cfg_vi].inloop == '1' ? true : false;
        BasicBlockTask* task = new BasicBlockTask(cur_bbid, inLoop);
        RuntimeSymFlowGraph::edge_it ei, ei_end;
        /*
        for(auto eachNode : Nodes ){
            if(eachNode->BBID == cur_bbid){
                task->allNodes.insert(eachNode);
            }
        }*/

        //prepare leaves and roots
        for(boost::tie(ei,ei_end) = boost::edges(dfg->graph); ei != ei_end ; ei++){
            RuntimeSymFlowGraph::vertex_t from = boost::source(*ei, dfg->graph);

            RuntimeSymFlowGraph::vertex_t to = boost::target(*ei, dfg->graph);
            auto from_node = Nodes.at(ver2offMap.at(from));
            if(dfg->graph[from].BBID == cur_bbid && dfg->graph[to].BBID != cur_bbid){
                task->roots.insert(from_node);
#ifdef DEBUG_OUTPUT
                dbgBBRoot(ver2offMap.at(from));
#endif
            }
            if(dfg->graph[from].BBID != cur_bbid && dfg->graph[to].BBID == cur_bbid){
                task->leaves.insert(from_node);
#ifdef DEBUG_OUTPUT
                dbgBBLeaves(ver2offMap.at(from));
#endif
            }
        }
        RuntimeSymFlowGraph::vertex_it  vi,vi_end;
        for(boost::tie(vi, vi_end) = boost::vertices(dfg->graph) ; vi != vi_end ; vi++){
            if(dfg->graph[*vi].BBID == cur_bbid){
                auto cur_node = Nodes.at(ver2offMap.at(*vi));
#ifdef DEBUG_CHECKING
                auto sym_node = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
                if(funcname == "strlen_cgc" && sym_node != nullptr){
                    __asm__("nop");
                }
#endif
                if(boost::in_degree(*vi, dfg->graph) == 0){
                    task->leaves.insert(cur_node);
#ifdef DEBUG_CHECKING
                    assert(cur_node->In_edges.size() == 0);
#endif
#ifdef DEBUG_OUTPUT
                    dbgBBLeaves(ver2offMap.at(*vi));
#endif

                }
                if(boost::out_degree(*vi, dfg->graph) == 0){
                    task->roots.insert(cur_node);
#ifdef DEBUG_CHECKING
                    assert(cur_node->UsedBy.size() == 0);
#endif
#ifdef DEBUG_OUTPUT
                    dbgBBRoot(ver2offMap.at(*vi));
#endif
                }else{
                    // even if it has out degree, if the only out edges from this node is to TruePhi inside the same BB, it's still a root
                    RuntimeSymFlowGraph::o_edge_it tmp_ei, tmp_ei_end;
                    auto allOutToTruePhiWithinSameBB = true;
                    for(boost::tie(tmp_ei, tmp_ei_end) = boost::out_edges(  *vi, dfg->graph); tmp_ei != tmp_ei_end; tmp_ei++){
                        auto targetNode = Nodes.at(boost::target(*tmp_ei, dfg->graph));
                        auto targetTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(targetNode);
                        if(targetNode->BBID == cur_bbid && targetTruePhi == nullptr){
                            allOutToTruePhiWithinSameBB = false;
                        }
                    }
                    if(allOutToTruePhiWithinSameBB){
                        task->roots.insert(cur_node);
                    }
                }
            }
        }
        task->nonReadyRoots.insert(task->roots.begin(), task->roots.end());
        // post-dom relation
        task->dominance = domChildrenOf(cur_bbid, dID2VertMap, cfg->domTree);
        task->post_dominance = domChildrenOf(cur_bbid, pdId2VertMap, cfg->postDomTree);
        task->nonReadyPostDominance.insert(task->post_dominance.begin(), task->post_dominance.end());
        // construct bbid to postDomTree Ver Map
        bbTasks.insert(make_pair(cur_bbid, task));
    }
}

void SymGraph::RootTask::InsertNonReadyDep(Val *v, std::map<Val::BasicBlockIdType, BasicBlockTask *> &bbTasks) {
    //nonReadyDeps.insert(v);
    if(v->BBID == root->BBID){
        inBBNonReadyDeps.insert(v);
    }
    BasicBlockTask* depBBTask = bbTasks.at(v->BBID);
    //if(depBBTask->BBID != rootBBid && depBBTask->BBID != 0 && depBBTask->inLoop == false){
        // this BB is not in the loop, and it has node that has not been executed, then it must not be ready
    if(depBBTask->BBID != rootBBid && depBBTask->BBID != 0){
        assert(depBBTask->ready == 0);
        if(std::find(depNonReadyNonLoopBB.begin(), depNonReadyNonLoopBB.end(), depBBTask) == depNonReadyNonLoopBB.end()){
            depNonReadyNonLoopBB.push_back(depBBTask);
        }
    }
}

void SymGraph::RootTask::Refresh() {
    // we always assume root is yet to be constructed
    // bbs
    auto bbIter = depNonReadyNonLoopBB.begin();
    while(bbIter != depNonReadyNonLoopBB.end()){
        if((*bbIter)->ready == 1){
            bbIter = depNonReadyNonLoopBB.erase(bbIter);
        }else{
            ++bbIter;
        }
    }
    // in bb nodes
    bool inBBNodesChanged = false;
    auto nodeIter = inBBNonReadyDeps.begin();
    while(nodeIter != inBBNonReadyDeps.end()){
        if(root->isThisNodeReady(*nodeIter, (root->ready + 1 ) )){
            nodeIter = inBBNonReadyDeps.erase(nodeIter);
            inBBNodesChanged = true;
        }else{
            ++nodeIter;
        }
    }
    // in bb nodes leaves, only refresh when the in BB nodes changed.
    if(inBBNodesChanged){
        inBBNonReadyLeafDeps.clear();
        for(auto eachInBBNonReadyDep : inBBNonReadyDeps){
            bool allChildReady = true;
            for(auto eachInBBNonReadyDepInEdge : eachInBBNonReadyDep->In_edges){
                if( ! root->isThisNodeReady(eachInBBNonReadyDepInEdge.second, (root->ready + 1 ))){
                    allChildReady = false;
                }
            }
            if(allChildReady){
                // all of its children are ready, but not this node
                inBBNonReadyLeafDeps.insert(eachInBBNonReadyDep);
            }
        }
    }
}


bool SymGraph::BasicBlockTask::isBBReady() {
    for(auto eachRoot : roots){
        if(auto tmpTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(eachRoot); tmpTruePhi != nullptr){
            continue;
        }
        else if(auto tmpRuntime = dynamic_cast<RuntimeVal*>(eachRoot); tmpRuntime != nullptr){
            continue;
        }
        assert( eachRoot->ready >= (ready + 1) );
    }
    return true;
}

void SymGraph::RefreshBBTask(Val::BasicBlockIdType bbid,Val::ReadyType targetReady) {
    // TODO: find all all BBs that are sure to be executed before this BB
    auto bbtask = bbTasks.at(bbid);
    auto bbIter = bbtask->nonReadyPostDominance.begin();
    while (bbIter != bbtask->nonReadyPostDominance.end()){
        auto cur_bbtask = bbTasks.at(*bbIter);
        if(  cur_bbtask->inLoop == false &&  cur_bbtask->ready == 1){
            bbIter = bbtask->nonReadyPostDominance.erase(bbIter);
        }else if(cur_bbtask->inLoop == true){
            bbIter = bbtask->nonReadyPostDominance.erase(bbIter);
        }else{
            bbIter++;
        }
    }
    // in bb nodes
    bbtask->nonReadyRoots.clear();
    for(auto * eachRoot : bbtask->roots){
        if(eachRoot->ready < targetReady){
            bbtask->nonReadyRoots.insert(eachRoot);
        }
#ifdef DEBUG_CHECKING
        else if(eachRoot->ready > targetReady){
            //std::cerr<<" nodes' ready should not exceed its BB ready";
            //assert(false);
        }
#endif
    }
}