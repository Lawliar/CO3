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



class Orchestrator{
public:
    Orchestrator(std::string inputDir, std::string sp_port, int);
    int Run();
    SymGraph* getCurFunc();

    void ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot*);
    void ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf*);
    void ExecuteBasicBlock(Val::BasicBlockIdType);
    void PreparingCalling(NotifyCallMessage*);
    void SetRet();
    void ForwardExecution(Val*,bool, bool, Val*,unsigned);
    void BackwardExecution(SymVal*, Val::ReadyType);
    ~Orchestrator();


    ThreadPool pool;

    OpenedSP sp;
    MsgQueue msgQueue;
    std::stack<SymGraph*> callStack;
    std::map<unsigned, SymGraph*> symGraphs;
    bool functionedPushed;
};
#endif //SYMBACKEND_ORCHESTRATOR_H
