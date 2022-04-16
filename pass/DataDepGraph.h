//
// Created by charl on 4/8/2022.
//

#ifndef SYMBOLICCOMPILER_DATADEPGRAPH_H
#define SYMBOLICCOMPILER_DATADEPGRAPH_H

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"

#include "Runtime.h"

typedef enum _NodeType{
    NodeConst,
    NodeSym,
    NodeIntepretedFunc,
    NodeRuntime,
    NodeSymPara,//spacial case for parameters
}NodeType;
class SymDepGraph
{
public:
    struct Vertex_Properties                                    // property bundle for vertices
    {
        unsigned symID;
        std::string op;
        NodeType nodeType;
        long const_value;
        unsigned int bitwidth;
    };
    struct Edge_Properties                                    // property bundle for vertices
    {
        unsigned arg_no;
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS,  boost::directedS,
            Vertex_Properties,Edge_Properties> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
    typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
    typedef boost::graph_traits<Graph>::edge_iterator edge_it;


    SymDepGraph();
    SymDepGraph::vertex_t AddSymVertice(unsigned symID, llvm::StringRef op);
    SymDepGraph::vertex_t AddSymParaVertice(unsigned symID);
    SymDepGraph::vertex_t AddInterFuncVertice(unsigned symID, llvm::StringRef op);
    SymDepGraph::vertex_t AddConstVertice(unsigned long value, unsigned int bit_width);
    SymDepGraph::vertex_t AddRuntimeVertice(unsigned int bit_width);

    void AddEdge(unsigned from_symid, unsigned to_symid, unsigned arg_no);
    void AddEdge(vertex_t, vertex_t, unsigned);
    SymDepGraph::vertex_it GetVerticeBySymID(unsigned symID);

    SymDepGraph::vertex_it GetVerticeEndIt();

    void writeToFile(std::string filename);
private:
    Graph graph;                                                // the boost graph


};


#endif //SYMBOLICCOMPILER_DATADEPGRAPH_H
