//
// Created by charl on 5/25/2022.
//

#include "SymGraph.h"

#include "SymGraph.h"

SymGraph::SymGraph(std::string cfg_filename,std::string dt_filename, std::string pdt_filename, std::string dfg_filename) \
:cfg(cfg_filename,dt_filename, pdt_filename),dfg(dfg_filename, cfg) {

    unsigned numNodes = boost::num_vertices(dfg.graph);
    Nodes.reserve(numNodes);
    RuntimeSymFlowGraph::vertex_it dfg_vi, dfg_vi_end;
    map<RuntimeSymFlowGraph::vertex_t, unsigned> index_map;
    unsigned cur = 0;// must start from 0
    for (boost::tie(dfg_vi, dfg_vi_end) = boost::vertices(dfg.graph); dfg_vi != dfg_vi_end; ++dfg_vi){
        // SINCE in vecS, Vertex_t is also unsigned int, so this part of code is just mapping N to N, but we do this just in case
        assert(index_map.find(*dfg_vi) == index_map.end());
        index_map[*dfg_vi] = cur;
        cur ++;
    }
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
            //std::cout<< in_degree <<','<< op  <<'\n';
            //std::cout.flush();
        }else if( nodeType == NodeTruePhi){
            // some special case
        }
        Nodes[index_map.at(cur_ver)] = cur_node;
    }
}