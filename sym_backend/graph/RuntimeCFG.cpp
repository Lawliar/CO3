//
// Created by charl on 4/22/2022.
//

#include "RuntimeCFG.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"

void RuntimeCFG::readGraphViz(std::string cfg_filename, std::string p_filename, std::string dp_filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",             boost::get(&Vertex_Properties::name,                graph));
    dp.property(idPrefix,              boost::get(&Vertex_Properties::id,                  graph));
    dp.property(loopPrefix,            boost::get(&Vertex_Properties::inloop,              graph));
    dp.property(entryPrefix,           boost::get(&Vertex_Properties::entry,              graph));
    dp.property(exitPrefix,            boost::get(&Vertex_Properties::exit,              graph));
    std::ifstream cfg_file (cfg_filename);
    boost::read_graphviz(cfg_file, graph, dp);
    cfg_file.close();

    boost::dynamic_properties d_dp(boost::ignore_other_properties);
    d_dp.property("node_id",               boost::get(&Dominance_Vertex_Properties::name,                domTree));
    d_dp.property(idPrefix,                boost::get(&Dominance_Vertex_Properties::id,                domTree));
    d_dp.property(pdLevelPrefix,           boost::get(&Dominance_Vertex_Properties::level,                domTree));
    std::ifstream d_file (p_filename);
    boost::read_graphviz(d_file, domTree, d_dp);
    d_file.close();

    boost::dynamic_properties pd_dp(boost::ignore_other_properties);
    pd_dp.property("node_id",               boost::get(&Dominance_Vertex_Properties::name,                postDomTree));
    pd_dp.property(idPrefix,                boost::get(&Dominance_Vertex_Properties::id,                postDomTree));
    pd_dp.property(pdLevelPrefix,           boost::get(&Dominance_Vertex_Properties::level,                postDomTree));
    std::ifstream dp_file (dp_filename);
    boost::read_graphviz(dp_file, postDomTree, pd_dp);
    dp_file.close();


    // some sanity check
    assert(graph.m_vertices.size() == postDomTree.m_vertices.size());
    vertex_it cfg_it, cfg_it_end;
    std::set<unsigned> cfg_ids;
    for(boost::tie(cfg_it,cfg_it_end) = boost::vertices(graph); cfg_it != cfg_it_end; ++cfg_it){
        unsigned node_id = graph[*cfg_it].id;
        assert(cfg_ids.find(node_id) == cfg_ids.end());
        cfg_ids.insert(node_id);
    }
    pd_vertex_it pd_it, pd_it_end;
    std::set<unsigned> pd_ids;
    for(boost::tie(pd_it,pd_it_end) = boost::vertices(postDomTree); pd_it!= pd_it_end; ++pd_it){
        unsigned node_id = postDomTree[*pd_it].id;
        assert(pd_ids.find(node_id) == pd_ids.end());
        pd_ids.insert(node_id);
    }
}
