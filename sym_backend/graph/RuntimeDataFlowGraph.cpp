//
// Created by charl on 4/22/2022.
//

#include "RuntimeDataFlowGraph.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"
#include "boost/graph/depth_first_search.hpp"
void RuntimeSymDepGraph::readGraphViz(std::string filename) {
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
    void back_edge(Edge, Graph&) {
        _has_cycle = true;
    }
protected:
    bool& _has_cycle;
};

void RuntimeSymDepGraph::loopCheck() {
    bool has_cycle = false;
    cycle_detector vis(has_cycle);
    boost::depth_first_search(graph, visitor(vis));
    if(has_cycle == true){
        fputs ("ddg has back edges which it should not\n",stderr);
        abort();
    }
    /*
    edge_it ei,ei_end;
    boost::tie(ei, ei_end) = boost::edges(graph);

    vertex_t s = boost::source(*ei,graph);
    vertex_t d = boost::target(*ei,graph);
    boost::add_edge(d,s,Edge_Properties{2},graph);
    */
}

void RuntimeSymDepGraph::PrepareTask(){
    loopCheck();
    vertex_it vi,vi_end;
    std::set<unsigned long> allBBs;
    for(boost::tie(vi,vi_end) = boost::vertices(graph); vi != vi_end ; vi++){
        unsigned long curBB = graph[*vi].BBID;
        if(curBB != 0 && allBBs.find(curBB) == allBBs.end()){
            allBBs.insert(curBB);
        }
    }
    for(auto eachBB : allBBs){
        BasicBlockTask task();
        for(boost::tie(vi,vi_end) = boost::vertices(graph); vi != vi_end ; vi++){
            if(graph[*vi].BBID == eachBB){
                in_edge_it in_ei, in_ei_end;
                boost::tie(in_ei, in_ei_end) = boost::in_edges(*vi, graph);
            }
        }
    }
}