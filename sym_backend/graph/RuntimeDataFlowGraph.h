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
    class BasicBlockTask{
    public:
        BasicBlockTask(unsigned id):BBID(id){}
        const unsigned BBID;
        // leaves are the "inputs" to this basic block
        std::set<vertex_t> leaves;
        // roots are the non-out vertices and the direct out vertices
        std::set<vertex_t> roots;
    };

    typedef enum _SymStatus{
        SymDummy,
        SymConcrete,
        SymSymbolic,
    }SymStatus;
   typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_it;

    RuntimeSymFlowGraph(std::string, RuntimeCFG&);
    void readGraphViz(std::string filename);
    void loopCheck();
    void PrepareTask();

    const RuntimeCFG& cfg;

    std::map<uint32_t, BasicBlockTask*> bbTasks;
    std::map<uint32_t, std::pair<SymStatus, SymExpr>> SymbolicStatus;

};
#endif //SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
