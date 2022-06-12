//
// Created by charl on 5/25/2022.
//

#include "SymGraph.h"
#include <stack>
#define CHECK_SYM_OP0(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 0);                           \
    cur_node = new SymVal##OP(symid, bbid);}

#define CHECK_SYM_OP1(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 1);             \
    cur_node = new SymVal##OP(symid, bbid, in_paras.at(0));}

#define CHECK_SYM_OP2(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 2);             \
    cur_node = new SymVal##OP(symid, bbid, in_paras.at(0),in_paras.at(1));}

#define CHECK_SYM_OP3(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 3);             \
    cur_node = new SymVal##OP(symid, bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2));}

#define CHECK_SYM_OP4(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 4);             \
    cur_node = new SymVal##OP(symid, bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2),in_paras.at(3));}

set<string> leaves;

SymGraph::SymGraph(std::string funcname,std::string cfg_filename,std::string dt_filename, std::string pdt_filename, std::string dfg_filename) \
:funcname(funcname),cfg(cfg_filename,dt_filename, pdt_filename),dfg(dfg_filename, cfg) {

    unsigned numNodes = boost::num_vertices(dfg.graph);
    Nodes.assign(numNodes, nullptr);
    RuntimeSymFlowGraph::vertex_it dfg_vi, dfg_vi_end;
    unsigned cur = 0;// must start from 0
    for (boost::tie(dfg_vi, dfg_vi_end) = boost::vertices(dfg.graph); dfg_vi != dfg_vi_end; ++dfg_vi){
        // SINCE in vecS, Vertex_t is also unsigned int, so this part of code is just mapping N to N, but we do this just in case
        assert(ver2offMap.find(*dfg_vi) == ver2offMap.end());
        ver2offMap[*dfg_vi] = cur;
        cur ++;
    }
    unsigned NULL_sym =0; // just for sanity check
    for (boost::tie(dfg_vi, dfg_vi_end) = boost::vertices(dfg.graph); dfg_vi != dfg_vi_end; ++dfg_vi){
        RuntimeSymFlowGraph::vertex_t cur_ver = *dfg_vi;
        //unpack the info
        std::string nodeType = dfg.graph[cur_ver].nodeType;
        unsigned bbid = dfg.graph[cur_ver].BBID;
        int symid = dfg.graph[cur_ver].symID;
        unsigned width = dfg.graph[cur_ver].byteWidth;
        unsigned long const_val = dfg.graph[cur_ver].const_value;
        std::string op = dfg.graph[cur_ver].op;
        // check in-edges
        RuntimeSymFlowGraph::in_edge_it in_eit, in_eit_end;
        boost::tie(in_eit, in_eit_end) = boost::in_edges(cur_ver, dfg.graph);
        unsigned in_degree = boost::in_degree(cur_ver, dfg.graph);

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
                unsigned arg_index = dfg.graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg.graph);
                assert(in_paras.find(arg_index) == in_paras.end());
                in_paras[arg_index] = ver2offMap.at(source);
            }
            if(op == VoidStr){
                assert(bbid == 0);
                cur_node = new SymVal_NULL(symid, bbid);
                NULL_sym++;
                assert(NULL_sym == 1);
            } else if(op == "_sym_notify_call"){
                cur_node = new SymVal_sym_notify_call(symid, bbid, in_paras);
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
            CHECK_SYM_OP2(_sym_build_float_to_bits)
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

        else if( nodeType == NodeTruePhi || nodeType == NodeFalseLeafPhi){
            map<unsigned short, unsigned short> in_paras;
            map<unsigned short, unsigned short> argNo2BBMap;
            for(;in_eit != in_eit_end; in_eit++ ){
                unsigned arg_index = dfg.graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg.graph);
                assert(in_paras.find(arg_index) == in_paras.end());
                in_paras[arg_index] = ver2offMap.at(source);
                assert(dfg.graph[*in_eit].incomingBB > 0);
                argNo2BBMap[arg_index] = dfg.graph[*in_eit].incomingBB;
            }
            if(nodeType == NodeTruePhi){
                cur_node = new  SymVal_sym_TruePhi(symid, bbid, in_paras,argNo2BBMap);
            } else{
                assert(in_paras.size() == 2 && argNo2BBMap.size() == 2);
                cur_node = new SymVal_sym_FalsePhiLeaf(symid, bbid, in_paras,argNo2BBMap);
            }
        }else if(nodeType == NodeFalseRootPhi ){
            map<unsigned short, unsigned short> in_paras;
            map<unsigned short, unsigned short> argNo2BBMap;
            set<Val::ValVertexType> falsePhiLeaves;
            for(;in_eit != in_eit_end; in_eit++ ){
                unsigned arg_index = dfg.graph[*in_eit].arg_no;
                RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg.graph);
                if(arg_index == 0 || arg_index == 1){
                    assert(in_paras.find(arg_index) == in_paras.end());
                    in_paras[arg_index] = ver2offMap.at(source);
                    assert(dfg.graph[*in_eit].incomingBB > 0);
                    argNo2BBMap[arg_index] = dfg.graph[*in_eit].incomingBB;
                }else{
                    assert(arg_index == 2);
                    falsePhiLeaves.insert(ver2offMap.at(source));
                }
            }
            assert(in_paras.size() == 2 && argNo2BBMap.size() == 2);
            cur_node = new SymVal_sym_FalsePhiRoot(symid, bbid, in_paras, falsePhiLeaves, argNo2BBMap);
        }

        Nodes[ver2offMap.at(cur_ver)] = cur_node;
    }

    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        // make sure every node is parsed.
        Val* cur_node = Nodes[index];
        assert(cur_node!= nullptr);
        // some sanity check for some special node
        if(SymVal_sym_FalsePhiRoot * falsePhiRoot = dynamic_cast<SymVal_sym_FalsePhiRoot*>(Nodes[index])){
            SymVal_NULL * op1 = dynamic_cast<SymVal_NULL*>(Nodes.at(falsePhiRoot->In_edges.at(0)));
            SymVal_NULL * op2 = dynamic_cast<SymVal_NULL*>(Nodes.at(falsePhiRoot->In_edges.at(1)));
            assert(op1 != nullptr && op2 == nullptr); // op1 is always pointing to false, op2 is always not pointing to false
        }
        if(SymVal_sym_FalsePhiLeaf * falsePhiLeaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(Nodes[index])){
            SymVal_NULL * op1 = dynamic_cast<SymVal_NULL*>(Nodes.at(falsePhiLeaf->In_edges.at(0)));
            SymVal_NULL * op2 = dynamic_cast<SymVal_NULL*>(Nodes.at(falsePhiLeaf->In_edges.at(1)));
            assert(op1 == nullptr && op2 == nullptr); // both of the operands are not pointing to false
        }
        // end of the sanity check

        // complete the use relation
        if(cur_node->BBID == 0){
            cur_node->inLoop = false;
        }else{
            cur_node->inLoop = cfg.bbid2loop.at(cur_node->BBID);
        }
        for(auto each_in_edge : cur_node->In_edges){
            Nodes[each_in_edge.second]->UsedBy.insert(static_cast<Val::ValVertexType>(index));
        }

        //complete the symid to off map
        if(SymVal* symval = dynamic_cast<SymVal*>(cur_node) ; symval != nullptr){
            Val::SymIDType symid = symval->symID;
            assert(symID2offMap.find(symid) == symID2offMap.end());
            symID2offMap[symid] = static_cast<Val::ValVertexType>(index);
        }
    }

    // just to debugging and know more
    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        auto * node = Nodes.at(index);
        if(node->In_edges.size() == 0){
            if(auto * constVal = dynamic_cast<ConstantVal*>(node); constVal != nullptr){
                leaves.insert("const");
            }else if(auto * runtimeVal = dynamic_cast<RuntimeVal*>(node); runtimeVal != nullptr){
                leaves.insert("runtime");
            }else if(auto * symnull = dynamic_cast<SymVal_NULL*>(node); symnull != nullptr){
                leaves.insert("symnull");
            }else if(auto *symVal = dynamic_cast<SymVal*>(node);symVal != nullptr ){
                leaves.insert(symVal->Op);
            }else{
                assert(false);
            }
        }
    }
    //

    // get the getPara setRet for this function
    Val::BasicBlockIdType entryBBID = static_cast<Val::BasicBlockIdType>(cfg.graph[cfg.cfgEntry].id);
    Val::BasicBlockIdType exitBBID = static_cast<Val::BasicBlockIdType>(cfg.graph[cfg.cfgExit].id);
    for(auto nodeIt = Nodes.begin(); nodeIt != Nodes.end(); nodeIt ++){
        if((*nodeIt)->BBID == entryBBID && (*nodeIt)->type == Val::SymValTy){
            if(SymVal_sym_get_parameter_expression * getPara = dynamic_cast<SymVal_sym_get_parameter_expression*>((*nodeIt)); getPara !=
                    nullptr){
                getParametersSym.push_back(static_cast<Val::ValVertexType>(nodeIt - Nodes.begin()));
            }
        }
        if((*nodeIt)->BBID == exitBBID && (*nodeIt)->type == Val::SymValTy){
            if(SymVal_sym_set_return_expression * setRet = dynamic_cast<SymVal_sym_set_return_expression*>((*nodeIt)); setRet !=
                                                                                                                              nullptr){
                setRetSym = static_cast<Val::ValVertexType>(nodeIt - Nodes.begin());
            }
        }
    }
    prepareBBTask();
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

void DataDependents::nonTruePhiDataDependentsOf( set<Val::ValVertexType> ancesters){
    ancesters.insert(root);
    if(SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(allNodes[root]); true_phi != nullptr){
        truePhiDependences[root] = map<Val::ArgIndexType, DataDependents* >{};
        for(auto eachTruePhiDep: dynamic_cast<SymVal_sym_TruePhi*>(allNodes[root])->In_edges){
            Val::ValVertexType new_root = eachTruePhiDep.second;
            if(ancesters.find(new_root) == ancesters.end()){
                DataDependents* child_node = new DataDependents(eachTruePhiDep.second, allNodes);
                truePhiDependences.at(root)[eachTruePhiDep.first] = child_node;
                child_node->nonTruePhiDataDependentsOf(ancesters);
            }
        }
        return;
    }

    std::stack<Val::ValVertexType> work_stack;
    vector<Val::ValVertexType> truePhis;
    for(auto eachRootDep : allNodes[root]->In_edges){
        deps.insert(eachRootDep.second);
        work_stack.push(eachRootDep.second);
    }
    while(!work_stack.empty()){
        Val::ValVertexType cur_ver = work_stack.top();
        work_stack.pop();
        if(SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(allNodes[cur_ver]); true_phi != nullptr){
            truePhis.push_back(cur_ver);
        }
        for(auto eachWorkDep : allNodes[cur_ver]->In_edges){
            if(deps.find(eachWorkDep.second) == deps.end() && ancesters.find(eachWorkDep.second) == ancesters.end()){
                deps.insert(eachWorkDep.second);
                ancesters.insert(eachWorkDep.second);
                work_stack.push(eachWorkDep.second);
            }
        }
    }
    for(auto eachTruePhi: truePhis){
        truePhiDependences[eachTruePhi] = map<Val::ArgIndexType, DataDependents* >{};
        for(auto eachTruePhiDep: dynamic_cast<SymVal_sym_TruePhi*>(allNodes[eachTruePhi])->In_edges){
            Val::ValVertexType new_root = eachTruePhiDep.second;
            if(ancesters.find(new_root) == ancesters.end()){
                DataDependents* child_node = new DataDependents(eachTruePhiDep.second, allNodes);
                truePhiDependences.at(eachTruePhi)[eachTruePhiDep.first] = child_node;
                child_node->nonTruePhiDataDependentsOf(ancesters);
            }
        }
    }
}

void DataDependents::allPossibleDataDependencies(set<Val::ValVertexType> parent, vector<set<Val::ValVertexType> >& all) {
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

bool SymGraph::sortNonLoopBB(Val::BasicBlockIdType a, Val::BasicBlockIdType b) {
    BasicBlockTask* a_task = bbTasks.at(a);
    BasicBlockTask* b_task = bbTasks.at(b);
    if(a_task->dominance.find(b) != a_task->dominance.end()){
        // a dominate b
        return true;
    } else if (b_task->post_dominance.find(a) != b_task->post_dominance.end()){
        // b post-dominate a
        return true;
    }else{
        //b dominate a OR a post-dominate b
        assert(b_task->dominance.find(a) != b_task->dominance.end() || a_task->post_dominance.find(b) != a_task->post_dominance.end());

        return false;
    }
}

list<Val::BasicBlockIdType> SymGraph::sortNonLoopBBs(set<Val::BasicBlockIdType> unsorted) {
    list<Val::BasicBlockIdType> sortedBBs;
    sortedBBs.push_back(*unsorted.begin());
    unsorted.erase(unsorted.begin());

    for(auto eachUnsorted : unsorted){

    }
}


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

void SymGraph::prepareBBTask() {
    RuntimeCFG::vertex_it cfg_vi,cfg_vi_end;

    // dominance BBID 2 Vert Map
    map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t> dID2VertMap;
    RuntimeCFG::pd_vertex_it pd_v_it, pd_v_it_end;
    boost::tie(pd_v_it, pd_v_it_end) = boost::vertices(cfg.domTree);
    for(;pd_v_it != pd_v_it_end ; pd_v_it++){
        Val::BasicBlockIdType cur_bbid = cfg.domTree[*pd_v_it].id;
        dID2VertMap[cur_bbid] = *pd_v_it;
    }
    //postDom BBID 2 Vert Map
    map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t> pdId2VertMap;
    boost::tie(pd_v_it, pd_v_it_end) = boost::vertices(cfg.postDomTree);
    for(;pd_v_it != pd_v_it_end ; pd_v_it++){
        Val::BasicBlockIdType cur_bbid = cfg.postDomTree[*pd_v_it].id;
        pdId2VertMap[cur_bbid] = *pd_v_it;
    }

    // prepare per-BB task
    for(boost::tie(cfg_vi, cfg_vi_end) = boost::vertices(cfg.graph); cfg_vi != cfg_vi_end; cfg_vi++){
        unsigned cur_bbid = cfg.graph[*cfg_vi].id;
        bool inLoop = cfg.graph[*cfg_vi].inloop == '1' ? true : false;
        BasicBlockTask* task = new BasicBlockTask(cur_bbid, inLoop);
        RuntimeSymFlowGraph::edge_it ei, ei_end;
        //prepare leaves and roots
        for(boost::tie(ei,ei_end) = boost::edges(dfg.graph); ei != ei_end ; ei++){
            RuntimeSymFlowGraph::vertex_t from = boost::source(*ei, dfg.graph);
            RuntimeSymFlowGraph::vertex_t to = boost::target(*ei, dfg.graph);
            if(dfg.graph[from].BBID == cur_bbid && dfg.graph[to].BBID != cur_bbid){
                task->roots.insert(ver2offMap.at(from));
                //for debug purpose only
                dbgBBRoot(ver2offMap.at(from));
                //end of debug
            }
            if(dfg.graph[from].BBID != cur_bbid && dfg.graph[to].BBID == cur_bbid){
                task->leaves.insert(ver2offMap.at(from));
                //for debug purpose only
                dbgBBLeaves(ver2offMap.at(from));
                //end of debug

            }
        }
        RuntimeSymFlowGraph::vertex_it  vi,vi_end;
        for(boost::tie(vi, vi_end) = boost::vertices(dfg.graph) ; vi != vi_end ; vi++){
            if(dfg.graph[*vi].BBID == cur_bbid){
                if(boost::in_degree(*vi, dfg.graph) == 0){
                    task->leaves.insert(ver2offMap.at(*vi));
                    assert(Nodes[ver2offMap.at(*vi)]->In_edges.size() == 0);
                    //for debug purpose only
                    dbgBBLeaves(ver2offMap.at(*vi));
                    //end of debug

                }
                if(boost::out_degree(*vi, dfg.graph) == 0){
                    task->roots.insert(ver2offMap.at(*vi));
                    assert(Nodes[ver2offMap.at(*vi)]->UsedBy.size() == 0);
                    //for debug purpose only
                    dbgBBRoot(ver2offMap.at(*vi));
                    //end of debug
                }
            }
        }
        // post-dom relation
        task->dominance = domChildrenOf(cur_bbid, dID2VertMap, cfg.domTree);
        task->post_dominance = domChildrenOf(cur_bbid, pdId2VertMap, cfg.postDomTree);
        // construct bbid to postDomTree Ver Map
        bbTasks.insert(make_pair(cur_bbid, task));
    }
    for(auto eachBBTask : bbTasks){
        Val::BasicBlockIdType root_bbid = eachBBTask.first;
        for(auto eachRoot : eachBBTask.second->roots){
            set<Val::ValVertexType> visited;
            eachBBTask.second->nonLoopRootDependents[eachRoot] = new DataDependents(eachRoot, Nodes);
            eachBBTask.second->nonLoopRootDependents[eachRoot]->nonTruePhiDataDependentsOf(visited);
            //just for sanity check
            vector<set<Val::ValVertexType> > allDependencies;
            set<Val::ValVertexType> parent;
            eachBBTask.second->nonLoopRootDependents[eachRoot]->allPossibleDataDependencies(parent, allDependencies);
            for(auto eachPossibleDataDep : allDependencies){
                vector<Val::BasicBlockIdType> dependentBBs;
                for(auto eachDependentSymNode : eachPossibleDataDep){
                    Val::BasicBlockIdType nodeBBID = Nodes[eachDependentSymNode]->BBID;
                    if(nodeBBID == 0 || nodeBBID == root_bbid){
                        // not interested in constants(which are in 0-numbered BB) and self
                        continue;
                    }
                    if(std::find(dependentBBs.begin(),dependentBBs.end(),nodeBBID) == dependentBBs.end() && bbTasks.at(nodeBBID)->inLoop == false){
                        dependentBBs.push_back(nodeBBID);// we only deal with the non-loop BB
                    }
                }
                // just ensure the order between those BB exists
                std::sort(dependentBBs.begin(), dependentBBs.end(), [this] (Val::BasicBlockIdType a, Val::BasicBlockIdType b) {
                    return sortNonLoopBB(a, b);});
                //eachBBTask.second->nonLoopRootDependents[eachRoot] = dependentBBs;
            }
            //end of sanity check
        }
    }
}