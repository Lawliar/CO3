//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
#define SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H

#include "DataFlowGraph.h"
#include "RuntimeCFG.h"
#include "Runtime.h"
#include <map>
#include <set>
class RuntimeSymFlowGraph: public SymDepGraph{
public:

   typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_it;

    RuntimeSymFlowGraph(std::string, RuntimeCFG&);
    void readGraphViz(std::string filename);
    void loopCheck();

    const RuntimeCFG& cfg;
};
#endif //SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
