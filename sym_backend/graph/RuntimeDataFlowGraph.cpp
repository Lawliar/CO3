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
    dp.property(widthPrefix,         boost::get(&Vertex_Properties::byteWidth,     graph));
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
        assert(g[e.m_target].nodeType == NodeTruePhi || g[e.m_target].nodeType == NodeFalsePhi || g[e.m_source].nodeType == NodeTruePhi || g[e.m_source].nodeType == NodeFalsePhi);
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

RuntimeSymFlowGraph::RuntimeSymFlowGraph(std::string filename, RuntimeCFG & cfg): SymDepGraph(false),cfg(cfg){
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

    /*
    edge_it ei, ei_end;
    set<unsigned> arg_nos;
    for(boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end ; ei++){
        vertex_t s = boost::source(*ei, graph);
        vertex_t d = boost::target(*ei, graph);
        if(graph[s].nodeType== "phi" || graph[d].nodeType =="phi")
            continue;

        unsigned label = graph[*ei].arg_no;
        arg_nos.insert(label);

    }
    for(auto each_arg: arg_nos){
        cout << each_arg<<" ";
        cout.flush();
    }
     */
    // prepare the task
    PreparePerBBTask();
}

void RuntimeSymFlowGraph::addToTaskDependents(vertex_t root){
    std::set<vertex_t> visited;
    // src_id post dominates itself
    visited.insert(root);
    std::stack<vertex_t> work_stack;
    work_stack.push(root);



}
void RuntimeSymFlowGraph::PreparePerBBTask(){
    loopCheck();
    RuntimeCFG::vertex_it cfg_vi,cfg_vi_end;
    std::set<unsigned long> allBBs;

    // prepare per-BB task
    for(boost::tie(cfg_vi, cfg_vi_end) = boost::vertices(cfg.graph); cfg_vi != cfg_vi_end; cfg_vi++){
        unsigned cur_bbid = cfg.graph[*cfg_vi].id;
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

void RuntimeSymFlowGraph::PreparePerRootTask(){

}