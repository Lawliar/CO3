//
// Created by charl on 4/22/2022.
//

#include "RuntimeCFG.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
#include "fstream"

void RuntimeCFG::readGraphViz(std::string filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);

    dp.property("node_id",             boost::get(&Vertex_Properties::name,                graph));
    dp.property(idPrefix,              boost::get(&Vertex_Properties::id,                  graph));
    dp.property(loopPrefix,            boost::get(&Vertex_Properties::inloop,              graph));
    dp.property(entryPrefix,           boost::get(&Vertex_Properties::entry,              graph));
    dp.property(exitPrefix,            boost::get(&Vertex_Properties::exit,              graph));

    std::ifstream myfile (filename);
    boost::read_graphviz(myfile, graph, dp);
    myfile.close();
}