//
// Created by charl on 4/8/2022.
//

#include "DataDepGraph.h"
#include "boost/graph/graphviz.hpp"

SymDepGraph::SymDepGraph(){
    AddVertice(0,"",NodeSym, 0,0,0);
}

SymDepGraph::vertex_t SymDepGraph::AddVertice(int symID,std::string op,NodeType nodeType,long const_value,unsigned int bitwidth,unsigned long BBID){
    vertex_t u = boost::add_vertex(graph);
    graph[u].symID = symID;
    //make a copy
    graph[u].op = std::string(op);
    graph[u].nodeType = nodeType;
    graph[u].const_value = const_value;
    graph[u].bitwidth = bitwidth;
    graph[u].BBID = BBID;
    return u;
}

SymDepGraph::vertex_t SymDepGraph::AddSymVertice(unsigned symID, llvm::StringRef op, unsigned long BBID){
    assert(symID !=0);
    return AddVertice(symID,op.str(),NodeSym, 0, 0,BBID);
}
SymDepGraph::vertex_t SymDepGraph::AddInterFuncVertice(unsigned symID, llvm::StringRef op,unsigned long BBID){
    assert(symID !=0);
    return AddVertice(symID,op.str(),NodeIntepretedFunc, 0, 0,BBID);
}
SymDepGraph::vertex_t SymDepGraph::AddContextVertice(unsigned symID,NodeType nt,unsigned long BBID){
    assert(symID !=0);
    return AddVertice(symID,"",nt, 0, 0,BBID);
}

SymDepGraph::vertex_t SymDepGraph::AddConstVertice(unsigned long value, unsigned int bit_width,unsigned long BBID){
    return AddVertice(-1,"",NodeConst, value,bit_width,BBID);
}

SymDepGraph::vertex_t SymDepGraph::AddRuntimeVertice(unsigned int bit_width,unsigned long BBID){
    return AddVertice(-1,"",NodeRuntime, 0,bit_width,BBID);
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
        if(graph[*vi].symID == (int)symID) return vi;
    }
    return vi_end;
}

SymDepGraph::vertex_it SymDepGraph::GetVerticeEndIt(){
    return boost::vertices(graph).second;
}


void SymDepGraph::writeToFile(std::string filename){
    std::ofstream f(filename);

    boost::write_graphviz(f,graph,
                          make_node_writer(boost::get(&Vertex_Properties::symID,graph),
                                           boost::get(&Vertex_Properties::op,graph),
                                           boost::get(&Vertex_Properties::nodeType,graph),
                                           boost::get(&Vertex_Properties::const_value,graph),
                                           boost::get(&Vertex_Properties::bitwidth,graph),
                                           boost::get(&Vertex_Properties::BBID,graph)
                                           ),
                          boost::make_label_writer(boost::get(&Edge_Properties::arg_no,graph))
                          );

    f.close();
}