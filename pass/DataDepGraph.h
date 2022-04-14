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
    NodeRuntime,
}NodeType;
class SymDepGraph
{
public:
    struct Vertex_Properties                                    // property bundle for vertices
    {
        unsigned symID;
        std::string op;
        NodeType nodeType;
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
    SymDepGraph::vertex_t AddVertice(unsigned symID, llvm::StringRef calleeName, NodeType nodeType);
    void AddEdge(SymDepGraph::vertex_t f, SymDepGraph::vertex_t t);
    SymDepGraph::vertex_it GetVerticeBySymID(unsigned symID);

    SymDepGraph::vertex_it GetVerticeEndIt();
private:
    Graph graph;                                                // the boost graph


};


#endif //SYMBOLICCOMPILER_DATADEPGRAPH_H
