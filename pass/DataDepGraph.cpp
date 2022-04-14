//
// Created by charl on 4/8/2022.
//

#include "DataDepGraph.h"

SymDepGraph::SymDepGraph(){}
SymDepGraph::vertex_t SymDepGraph::AddVertice(unsigned symID, llvm::StringRef op, NodeType nodeType){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    //make a copy
    graph[u].op = std::string(op.str());
    graph[u].nodeType = nodeType;
    return u;
}

void SymDepGraph::AddEdge(SymDepGraph::vertex_t f, SymDepGraph::vertex_t t){
    boost::add_edge(f,t,graph);
}

SymDepGraph::vertex_it SymDepGraph::GetVerticeBySymID(unsigned int symID) {
    vertex_it vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi) {
        if(graph[*vi].symID == symID) return vi;
    }
    return vi_end;
}

SymDepGraph::vertex_it SymDepGraph::GetVerticeEndIt(){
    return boost::vertices(graph).second;
}