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
        unsigned char entry;
        unsigned char exit;
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

    inline static const std::string idPrefix            = "id";
    inline static const std::string loopPrefix          = "loop";
    inline static const std::string entryPrefix         = "entry";
    inline static const std::string exitPrefix          = "exit";
    inline static const std::string pdLevelPrefix          = "level";
    struct Dominance_Vertex_Properties                                    // property bundle for vertices
    {
        std::string name;
        unsigned int id;
        unsigned int level;
    };
    struct Dominance_Edge_Properties                                    // property bundle for vertices
    {
    };

    typedef boost::adjacency_list<boost::listS, boost::vecS,  boost::directedS,
            Dominance_Vertex_Properties,Dominance_Edge_Properties> DominanceTree;
    typedef boost::graph_traits<DominanceTree>::vertex_descriptor pd_vertex_t;
    typedef boost::graph_traits<DominanceTree>::vertex_iterator pd_vertex_it;
    typedef boost::graph_traits<DominanceTree>::edge_descriptor pd_edge_t;
    typedef boost::graph_traits<DominanceTree>::edge_iterator pd_edge_it;
    typedef boost::graph_traits<DominanceTree>::out_edge_iterator pd_oedge_it;

    std::map<unsigned, std::set<unsigned >> post_dominance; // 1st id post-dominates 2nd id, trade memory for speed


    vertex_t cfgEntry;
    vertex_t cfgExit;
    RuntimeCFG(std::string cfg_filename, std::string d_filename, std::string pd_filename);
    void readGraphViz(std::string cfg,std::string, std::string pd);

    Graph graph;
    DominanceTree domTree;
    DominanceTree postDomTree;

};
#endif //SYMBACKEND_RUNTIMECFG_H
