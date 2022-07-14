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
#include "ThreadPool.h"


typedef enum _SpecialNodeReturn{
    SpecialConstructed,
    SpecialNotReady,
    NotSpecial
}SpecialNodeReturn;

class Orchestrator{
public:
    Orchestrator(std::string inputDir, std::string sp_port, int);
    int Run();
    SymGraph* getCurFunc();

    bool ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot*, Val::ReadyType);
    bool ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf*, Val::ReadyType);

    SpecialNodeReturn ExecuteSpecialNode(SymVal*, Val::ReadyType);

    bool ExecuteNode(Val*, Val::ReadyType);
    void ExecuteBasicBlock(Val::BasicBlockIdType);
    void PreparingCalling(NotifyCallMessage*);
    void SetRetAndRefreshGraph();
    void ForwardExecution(Val*, SymGraph::RootTask*,unsigned);
    void BackwardExecution(SymVal*, Val::ReadyType);
    ~Orchestrator();


    ThreadPool pool;

    OpenedSP sp;
    MsgQueue msgQueue;
    Val::BasicBlockIdType lastBBID = 0;
    std::stack<SymGraph*> callStack;
    std::map<unsigned, SymGraph*> symGraphs;
    std::map<unsigned, SymGraph*> vanillaSymGraphs;
    //std::map<string, FuncFileNames> funcFiles;
};
#endif //SYMBACKEND_ORCHESTRATOR_H
