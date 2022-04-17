//
// Created by charl on 4/8/2022.
//

#include "DataDepGraph.h"
#include "boost/graph/graphviz.hpp"

SymDepGraph::SymDepGraph(){}

SymDepGraph::vertex_t SymDepGraph::AddVertice(unsigned symID,std::string op,NodeType nodeType,long const_value,unsigned int bitwidth){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    //make a copy
    graph[u].op = std::string(op);
    graph[u].nodeType = nodeType;
    graph[u].const_value = const_value;
    graph[u].bitwidth = bitwidth;
    return u;
}

SymDepGraph::vertex_t SymDepGraph::AddSymVertice(unsigned symID, llvm::StringRef op){
    return AddVertice(symID,op.str(),NodeSym, 0, 0);
}
SymDepGraph::vertex_t SymDepGraph::AddInterFuncVertice(unsigned symID, llvm::StringRef op){
    return AddVertice(symID,op.str(),NodeIntepretedFunc, 0, 0);
}
SymDepGraph::vertex_t SymDepGraph::AddSymParaVertice(unsigned symID){
    return AddVertice(symID,"",NodeSymPara, 0, 0);
}
SymDepGraph::vertex_t SymDepGraph::AddSymReturnVertice(unsigned symID){
    return AddVertice(symID,"",NodeSymReturn, 0, 0);
}

SymDepGraph::vertex_t SymDepGraph::AddConstVertice(unsigned long value, unsigned int bit_width){
    return AddVertice(0,"",NodeConst, value,bit_width);
}

SymDepGraph::vertex_t SymDepGraph::AddRuntimeVertice(unsigned int bit_width){
    return AddVertice(0,"",NodeRuntime, 0,bit_width);
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