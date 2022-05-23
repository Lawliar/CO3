//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_ORCHESTRATOR_H
#define SYMBACKEND_ORCHESTRATOR_H

#include <string>


#include "RuntimeCFG.h"
#include "RuntimeDataFlowGraph.h"
#include "MsgQueue.h"

extern "C" {
#include "serialport.h"
}


class Orchestrator{
public:
    Orchestrator(std::string cfg_filename, std::string pdt_filename, std::string dfg_filename, std::string sp_port, int);
    int StartListen();
    int Run();

    ~Orchestrator();



    OpenedSP sp;

    MsgQueue msgQueue;
    RuntimeCFG cfg;
    RuntimeSymFlowGraph dfg;


    // for debugging purpose
    std::deque<uint32_t> BBTrace;

};
#endif //SYMBACKEND_ORCHESTRATOR_H
