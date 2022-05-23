//
// Created by charl on 4/22/2022.
//

#include "RuntimeCFG.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"

void RuntimeCFG::readGraphViz(std::string cfg_filename, std::string dp_filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",             boost::get(&Vertex_Properties::name,                graph));
    dp.property(idPrefix,              boost::get(&Vertex_Properties::id,                  graph));
    dp.property(loopPrefix,            boost::get(&Vertex_Properties::inloop,              graph));
    dp.property(entryPrefix,           boost::get(&Vertex_Properties::entry,              graph));
    dp.property(exitPrefix,            boost::get(&Vertex_Properties::exit,              graph));
    std::ifstream cfg_file (cfg_filename);
    boost::read_graphviz(cfg_file, graph, dp);
    cfg_file.close();

    boost::dynamic_properties pd_dp(boost::ignore_other_properties);
    pd_dp.property("node_id",               boost::get(&PD_Vertex_Properties::name,                postDomTree));
    pd_dp.property(idPrefix,                boost::get(&PD_Vertex_Properties::id,                postDomTree));
    pd_dp.property(pdLevelPrefix,           boost::get(&PD_Vertex_Properties::level,                postDomTree));
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

std::set<unsigned> RuntimeCFG::postDominatedBy(pd_vertex_t src){
    std::set<unsigned> visited;
    unsigned src_id = postDomTree[src].id;
    // src_id post dominates itself
    visited.insert(src_id);
    std::stack<pd_vertex_t> work_stack;
    work_stack.push(src);

    while(!work_stack.empty()) {
        pd_vertex_t cur_node = work_stack.top();
        work_stack.pop();

        pd_vertex_it pd_it, pd_it_end;
        for(boost::tie(pd_it,pd_it_end) = boost::vertices(postDomTree); pd_it!= pd_it_end; ++pd_it){
            unsigned next_id = postDomTree[*pd_it].id;
            if (boost::edge(cur_node, *pd_it, postDomTree).second) {
                visited.insert(next_id);
                work_stack.push(*pd_it);
            }
        }
    }
    return visited;
}
void RuntimeCFG::preparePostDominance() {
    boost::graph_traits<PDTree>::vertex_iterator it, it_end;
    for(boost::tie(it, it_end) = boost::vertices(postDomTree); it != it_end; ++it){
        unsigned it_id = graph[*it].id;
        auto reachable_ids = postDominatedBy(*it);
        for(auto each_reachable_id : reachable_ids){
            post_dominance.insert(std::make_pair(it_id, each_reachable_id));
        }
    }
}