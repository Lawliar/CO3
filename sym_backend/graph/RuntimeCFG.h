//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_RUNTIMECFG_H
#define SYMBACKEND_RUNTIMECFG_H

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"

class RuntimeCFG{
public:
    struct Vertex_Properties                                    // property bundle for vertices
    {
        std::string name;
        unsigned int id;
        unsigned char inloop;
    };
    struct Edge_Properties                                    // property bundle for vertices
    {
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS,  boost::directedS,
            Vertex_Properties,Edge_Properties> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
    typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
    typedef boost::graph_traits<Graph>::edge_iterator edge_it;

    inline static const std::string idPrefix         = "id";
    inline static const std::string loopPrefix         = "loop";

    RuntimeCFG(){}
    void readGraphViz(std::string filename);

    Graph graph;                                                // the boost graph
};
#endif //SYMBACKEND_RUNTIMECFG_H
