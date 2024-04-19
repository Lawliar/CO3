// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.



#include "RuntimeCFG.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"

void RuntimeCFG::readGraphViz(std::string cfg_filename, std::string p_filename, std::string dp_filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id",             boost::get(&Vertex_Properties::name,                graph));
    dp.property(idPrefix,              boost::get(&Vertex_Properties::id,                  graph));
    dp.property(loopPrefix,            boost::get(&Vertex_Properties::inloop,              graph));
    dp.property(entryPrefix,           boost::get(&Vertex_Properties::entry,               graph));
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


    assert(graph.m_vertices.size() == postDomTree.m_vertices.size());
    assert(graph.m_vertices.size() == domTree.m_vertices.size());
    vertex_it cfg_it, cfg_it_end;
    for(boost::tie(cfg_it,cfg_it_end) = boost::vertices(graph); cfg_it != cfg_it_end; ++cfg_it){
        unsigned bbid = graph[*cfg_it].id;
        bool inloop = graph[*cfg_it].inloop == '1' ? true : false;
        bbid2loop[bbid] = inloop;
    }
}

RuntimeCFG::RuntimeCFG(std::string cfg_filename, std::string d_filename, std::string pd_filename){
    readGraphViz(cfg_filename,d_filename, pd_filename);
    vertex_it vt, vt_end;
    unsigned numExit = 0;
    unsigned numEntry = 0;
    for(boost::tie(vt, vt_end) = boost::vertices(graph); vt != vt_end ; vt ++){
        if(graph[*vt].entry == '1'){
            cfgEntry = *vt;
            numEntry++;
        }
        if(graph[*vt].exit == '1'){
            cfgExit = *vt;
            numExit++;
        }
    }
    assert(numEntry == 1 && numExit == 1);
}