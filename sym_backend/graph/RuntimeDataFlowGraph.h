//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
#define SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H

#include "DataFlowGraph.h"
#include <map>
#include <vector>
class RuntimeSymDepGraph: public SymDepGraph{
public:
    class BasicBlockTask{
    public:
        // leaves are the "inputs" to this basic block
        std::vector<vertex_t> leaves;
        // nonLeaves are the operations that need to be done within this BB
        std::vector<vertex_t> nonLeaves;

        // outEdges are the connections between these nodes
        std::map<vertex_t, std::vector<edge_t> > outEdges;
    };

   typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_it;

    RuntimeSymDepGraph(){}
    void readGraphViz(std::string filename);
    void loopCheck();
    void PrepareTask();
    std::map<uint32_t, BasicBlockTask*> tasks;
};
#endif //SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
