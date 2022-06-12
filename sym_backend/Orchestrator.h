//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_ORCHESTRATOR_H
#define SYMBACKEND_ORCHESTRATOR_H

#include <string>

#include "MsgQueue.h"
#include "SymGraph.h"
#include <stack>
#include <boost/filesystem.hpp>
extern "C" {
#include "serialport.h"
}


class Orchestrator{
public:
    Orchestrator(std::string inputDir, std::string sp_port, int);
    int StartListen();
    int Run();
    SymGraph* getCurFunc();

    inline bool isNodeReady(Val*, Val*);
    Val* stripPhis(Val*, Val*);

    set<Val*> getNonReadyDeps(Val*);
    void ExecuteBasicBlock(Val::BasicBlockIdType, bool);
    void PreparingCalling(NotifyCallMessage*);
    void ForwardExecution(Val*, bool, bool);
    void BackwardExecution(SymVal*);
    ~Orchestrator();


    OpenedSP sp;
    MsgQueue msgQueue;
    std::stack<SymGraph*> callStack;
    std::map<unsigned, SymGraph*> symGraphs;

    // for debugging purpose
    std::deque<uint32_t> BBTrace;

};
#endif //SYMBACKEND_ORCHESTRATOR_H
