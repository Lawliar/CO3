//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_ORCHESTRATOR_H
#define SYMBACKEND_ORCHESTRATOR_H

#include <string>


#include "RuntimeCFG.h"
#include "RuntimeDataFlowGraph.h"
#include "MsgQueue.h"
#include "SymGraph.h"

#include "boost/filesystem.hpp"
extern "C" {
#include "serialport.h"
}


class Orchestrator{
public:
    Orchestrator(std::string inputDir, std::string sp_port, int);
    int StartListen();
    int Run();

    ~Orchestrator();



    OpenedSP sp;

    MsgQueue msgQueue;
    map<unsigned, SymGraph*> symGraphs;


    // for debugging purpose
    std::deque<uint32_t> BBTrace;

};
#endif //SYMBACKEND_ORCHESTRATOR_H
