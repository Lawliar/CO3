//
// Created by charl on 4/22/2022.
//

#include "RuntimeDataDepGraph.h"
#include "boost/property_map/dynamic_property_map.hpp"
#include "boost/graph/graphviz.hpp"
void RuntimeSymDepGraph::readGraphViz(std::string filename) {
    boost::dynamic_properties dp(boost::ignore_other_properties);

    dp.property(symIDPrefix,         boost::get(&Vertex_Properties::symID,        graph));
    dp.property(opPrefix,            boost::get(&Vertex_Properties::op,       graph));
    dp.property(nodeTPrefix,         boost::get(&Vertex_Properties::nodeType, graph));
    dp.property(constantValuePrefix, boost::get(&Vertex_Properties::const_value,         graph));
    dp.property(widthPrefix,         boost::get(&Vertex_Properties::bitwidth,         graph));
    dp.property(BasicBlockPrefix,    boost::get(&Vertex_Properties::BBID,         graph));

    bool status = boost::read_graphviz(std::cin, graph, dp);
}