//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_RUNTIMEDATADEPGRAPH_H
#define SYMBACKEND_RUNTIMEDATADEPGRAPH_H

#include "DataDepGraph.h"

class RuntimeSymDepGraph: public SymDepGraph{
public:
    RuntimeSymDepGraph(){}
    void readGraphViz(std::string filename);

};
#endif //SYMBACKEND_RUNTIMEDATADEPGRAPH_H
