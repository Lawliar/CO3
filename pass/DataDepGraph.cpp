//
// Created by charl on 4/8/2022.
//

#include "DataDepGraph.h"

SymDepGraph::SymDepGraph(const Runtime &runtime){
    // initialize OpMap
    unsigned cur_op = 1;
    for(auto eachSymOperation: runtime.SymOperators){
        OpMap.insert(std::pair<SymFnT,unsigned>(eachSymOperation,cur_op++));
    }
    for(auto eachSymOperation:runtime.comparisonHandlers){
        OpMap.insert(std::pair<SymFnT,unsigned>(eachSymOperation,cur_op++));
    }
    for(auto eachSymOperation:runtime.binaryOperatorHandlers){
        OpMap.insert(std::pair<SymFnT,unsigned>(eachSymOperation,cur_op++));
    }
}
SymDepGraph::vertex_t SymDepGraph::AddVertice(unsigned symID, SymFnT op, NodeType nodeType, bool c){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    graph[u].op = OpMap.at(op);
    graph[u].nodeType = nodeType;
    graph[u].concretenessCheck = c;
    return u;
}

void SymDepGraph::AddEdge(SymDepGraph::vertex_t f, SymDepGraph::vertex_t t){
    boost::add_edge(f,t,graph);
}