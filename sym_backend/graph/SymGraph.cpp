//
// Created by charl on 5/25/2022.
//

#include "SymGraph.h"

#define CHECK_SYM_OP0(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 0);                           \
    cur_node = new SymVal##OP(bbid);}

#define CHECK_SYM_OP1(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 1);             \
    cur_node = new SymVal##OP(bbid, in_paras.at(0));}

#define CHECK_SYM_OP2(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 2);             \
    cur_node = new SymVal##OP(bbid, in_paras.at(0),in_paras.at(1));}

#define CHECK_SYM_OP3(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 3);             \
    cur_node = new SymVal##OP(bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2));}

#define CHECK_SYM_OP4(OP)               \
    else if(op == #OP){                          \
    assert(in_degree == 4);             \
    cur_node = new SymVal##OP(bbid, in_paras.at(0),in_paras.at(1),in_paras.at(2),in_paras.at(3));}

SymGraph::SymGraph(std::string cfg_filename,std::string dt_filename, std::string pdt_filename, std::string dfg_filename) \
:cfg(cfg_filename,dt_filename, pdt_filename),dfg(dfg_filename, cfg) {

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
                cur_node = new SymVal_NULL(bbid);
                NULL_sym++;
                assert(NULL_sym == 1);
            } CHECK_SYM_OP2(_sym_try_alternative)
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


        }else if( nodeType == NodeTruePhi || nodeType == NodeFalsePhi){
            if(nodeType == NodeTruePhi){
                map<unsigned short, unsigned short> in_paras;
                for(;in_eit != in_eit_end; in_eit++ ){
                    unsigned arg_index = dfg.graph[*in_eit].arg_no;
                    RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg.graph);
                    assert(in_paras.find(arg_index) == in_paras.end());
                    in_paras[arg_index] = ver2offMap.at(source);
                }
                cur_node = new SymVal_sym_TruePhi(bbid, in_paras);
            }else{
                vector<pair<unsigned short, unsigned short>> in_paras;
                for(;in_eit != in_eit_end; in_eit++ ){
                    unsigned arg_index = dfg.graph[*in_eit].arg_no;
                    RuntimeSymFlowGraph::vertex_t source = boost::source(*in_eit,dfg.graph);
                    in_paras.push_back(make_pair(arg_index, ver2offMap.at(source)));
                }
                cur_node = new SymVal_sym_FalsePhi(bbid, in_paras);
            }
        }
        Nodes[ver2offMap.at(cur_ver)] = cur_node;
    }
    for(unsigned index = 0 ; index < Nodes.size() ; index++){
        assert(Nodes[index]!= nullptr);// make sure every node is parsed.
        for(auto each_in_edge : Nodes[index]->In_edges){
            Nodes[each_in_edge.second]->UsedBy.insert(index);
        }
    }

    prepareBBTask();
}

void SymGraph::prepareBBTask() {
    RuntimeCFG::vertex_it cfg_vi,cfg_vi_end;

    // prepare per-BB task
    for(boost::tie(cfg_vi, cfg_vi_end) = boost::vertices(cfg.graph); cfg_vi != cfg_vi_end; cfg_vi++){
        unsigned cur_bbid = cfg.graph[*cfg_vi].id;
        BasicBlockTask* task = new BasicBlockTask(cur_bbid);
        RuntimeSymFlowGraph::edge_it ei, ei_end;
        for(boost::tie(ei,ei_end) = boost::edges(dfg.graph); ei != ei_end ; ei++){
            RuntimeSymFlowGraph::vertex_t from = boost::source(*ei, dfg.graph);
            RuntimeSymFlowGraph::vertex_t to = boost::target(*ei, dfg.graph);
            if(dfg.graph[from].BBID == cur_bbid && dfg.graph[to].BBID != cur_bbid){
                task->roots.insert(ver2offMap.at(from));
            }
            if(dfg.graph[from].BBID != cur_bbid && dfg.graph[to].BBID == cur_bbid){
                task->leaves.insert(ver2offMap.at(from));
            }
        }
        RuntimeSymFlowGraph::vertex_it  vi,vi_end;
        for(boost::tie(vi, vi_end) = boost::vertices(dfg.graph) ; vi != vi_end ; vi++){
            if(dfg.graph[*vi].BBID == cur_bbid){
                if(boost::in_degree(*vi, dfg.graph) == 0){
                    task->leaves.insert(*vi);
                    assert(Nodes[ver2offMap.at(*vi)]->In_edges.size() == 0);
                }
                if(boost::out_degree(*vi, dfg.graph) == 0){
                    task->roots.insert(*vi);
                    assert(Nodes[ver2offMap.at(*vi)]->UsedBy.size() == 0);
                }
            }
        }
        bbTasks.insert(make_pair(cur_bbid, task));
    }
}