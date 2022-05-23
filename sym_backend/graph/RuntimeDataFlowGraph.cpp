//
// Created by charl on 4/22/2022.
//

#include "RuntimeDataFlowGraph.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"
#include "boost/graph/depth_first_search.hpp"
void RuntimeSymFlowGraph::readGraphViz(std::string filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);

    dp.property("node_id",           boost::get(&Vertex_Properties::name,         graph));
    dp.property(symIDPrefix,         boost::get(&Vertex_Properties::symID,        graph));
    dp.property(opPrefix,            boost::get(&Vertex_Properties::op,           graph));
    dp.property(nodeTPrefix,         boost::get(&Vertex_Properties::nodeType,     graph));
    dp.property(constantValuePrefix, boost::get(&Vertex_Properties::const_value,  graph));
    dp.property(widthPrefix,         boost::get(&Vertex_Properties::bitwidth,     graph));
    dp.property(BasicBlockPrefix,    boost::get(&Vertex_Properties::BBID,         graph));
    dp.property("label",             boost::get(&Edge_Properties::arg_no,         graph));
    std::ifstream myfile (filename);
    boost::read_graphviz(myfile, graph, dp);
    myfile.close();
}

struct cycle_detector : public boost::dfs_visitor<>
{
    cycle_detector( bool& has_cycle)
            : _has_cycle(has_cycle) { }

    template <class Edge, class Graph>
    void back_edge(Edge e, Graph& g) {
        assert(g[e.m_target].nodeType == "phi" || g[e.m_source].nodeType == "phi");
        _has_cycle = true;
    }
protected:
    bool& _has_cycle;
};

void RuntimeSymFlowGraph::loopCheck() {
    bool has_cycle = false;
    cycle_detector vis(has_cycle);
    boost::depth_first_search(graph, visitor(vis));

    /*
    edge_it ei,ei_end;
    boost::tie(ei, ei_end) = boost::edges(graph);

    vertex_t s = boost::source(*ei,graph);
    vertex_t d = boost::target(*ei,graph);
    boost::add_edge(d,s,Edge_Properties{2},graph);
    */
}

RuntimeSymFlowGraph::RuntimeSymFlowGraph(std::string filename, RuntimeCFG & cfg): cfg(cfg){
    readGraphViz(filename);

    // initialize the global status map
    vertex_it vi,vi_end;
    for(boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end ; vi++){
        int symid = graph[*vi].symID;
        if(symid > 0){
            assert(SymbolicStatus.find(symid) == SymbolicStatus.end());
            SymbolicStatus[symid] = make_pair(SymDummy, nullptr);
        }
    }
    // prepare the task
    PrepareTask();
}
void RuntimeSymFlowGraph::PrepareTask(){
    loopCheck();
    RuntimeCFG::vertex_it cfg_vi,cfg_vi_end;
    std::set<unsigned long> allBBs;

    // prepare per-BB task
    for(boost::tie(cfg_vi, cfg_vi_end) = boost::vertices(cfg.graph); cfg_vi != cfg_vi_end; cfg_vi++){
        unsigned cur_bbid = cfg.graph[*cfg_vi].id;
        bool in_loop = cfg.graph[*cfg_vi].inloop == '1' ? true : false;
        // we only do per-BB task for in loop BB
        if(!in_loop){
            continue;
        }
        BasicBlockTask* task = new BasicBlockTask(cur_bbid);
        edge_it ei, ei_end;
        for(boost::tie(ei,ei_end) = boost::edges(graph); ei != ei_end ; ei++){
            vertex_t from = ei->m_source;
            vertex_t to = ei->m_target;
            if(graph[from].BBID == cur_bbid && graph[to].BBID != cur_bbid){
                task->roots.insert(from);
            }
            if(graph[from].BBID != cur_bbid && graph[to].BBID == cur_bbid){
                task->leaves.insert(from);
            }
        }
        vertex_it  vi,vi_end;
        for(boost::tie(vi, vi_end) = boost::vertices(graph) ; vi != vi_end ; vi++){
            if(graph[*vi].BBID == cur_bbid){
                if(boost::in_degree(*vi, graph) == 0){
                    task->leaves.insert(*vi);
                }
                if(boost::out_degree(*vi, graph) == 0){
                    task->roots.insert(*vi);
                }
            }
        }
        bbTasks.insert(make_pair(cur_bbid, task));
    }
}