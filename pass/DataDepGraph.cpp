//
// Created by charl on 4/8/2022.
//

#include "DataDepGraph.h"
#include "boost/graph/graphviz.hpp"

SymDepGraph::SymDepGraph(){}
SymDepGraph::vertex_t SymDepGraph::AddSymVertice(unsigned symID, llvm::StringRef op){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    //make a copy
    graph[u].op = std::string(op.str());
    graph[u].nodeType = NodeSym;
    graph[u].bitwidth = 0;
    graph[u].const_value = 0;
    return u;
}
SymDepGraph::vertex_t SymDepGraph::AddSymParaVertice(unsigned symID){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    //make a copy
    graph[u].op = "";
    graph[u].nodeType = NodeSymPara;
    graph[u].bitwidth = 0;
    graph[u].const_value = 0;
    return u;
}

SymDepGraph::vertex_t SymDepGraph::AddConstVertice(unsigned long value, unsigned int bit_width){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = 0;
    //make a copy
    graph[u].op = "";
    graph[u].nodeType = NodeConst;
    graph[u].bitwidth = bit_width;
    graph[u].const_value = value;
    return u;
}

SymDepGraph::vertex_t SymDepGraph::AddRuntimeVertice(unsigned int bit_width){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = 0;
    //make a copy
    graph[u].op = "";
    graph[u].nodeType = NodeRuntime;
    graph[u].bitwidth = bit_width;
    graph[u].const_value = 0;
    return u;
}

void SymDepGraph::AddEdge(unsigned from_symid, unsigned to_symid, unsigned arg_no){
    auto from = GetVerticeBySymID(from_symid);
    auto to = GetVerticeBySymID(to_symid);
    assert(from != GetVerticeEndIt());
    assert(to != GetVerticeEndIt());
    boost::add_edge(*from,*to,Edge_Properties{arg_no},graph);
}

void SymDepGraph::AddEdge( vertex_t from, vertex_t to, unsigned arg_no){
    boost::add_edge(from,to,Edge_Properties{arg_no},graph);
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

void SymDepGraph::writeToFile(std::string filename){
    std::ofstream f(filename);
    boost::write_graphviz(f,graph,boost::make_label_writer(""));
    f.close();
}