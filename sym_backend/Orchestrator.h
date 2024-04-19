// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.



#ifndef SYMBACKEND_ORCHESTRATOR_H
#define SYMBACKEND_ORCHESTRATOR_H

#include <string>

#include "MsgQueue.h"
#include "SymGraph.h"
#include <stack>
#include <boost/filesystem.hpp>
extern "C" {
#include "transmit.h"
}
#include "ThreadPool.h"
#include "Shadow.h"

typedef enum _SpecialNodeReturn{
    SpecialConstructed,
    SpecialNotReady,
    NotSpecial
}SpecialNodeReturn;


class Orchestrator{
public:
    Orchestrator(std::string inputDir, std::string sp_port, int);
    ~Orchestrator();

    void SendInput();
    int Run();
    void ClearCallStack();
    SymGraph* getCurFunc();

    SymGraph* GetNextFunc(unsigned);
    void UpdateCallStackHashBB(Val::BasicBlockIdType);
    void UpdateCallStackHashCall(unsigned);
    void UpdateCallStackRet(unsigned);


    bool ExecuteFalsePhiRoot(SymVal_sym_FalsePhiRoot*, Val::ReadyType);
    bool ExecuteFalsePhiLeaf(SymVal_sym_FalsePhiLeaf*, Val::ReadyType);

    SpecialNodeReturn ExecuteSpecialNode(SymVal*, Val::ReadyType);

    bool ExecuteNode(Val*, Val::ReadyType);
    void ExecuteBasicBlock(Val::BasicBlockIdType);
    void PreparingCalling(NotifyCallMessage*);
    void SetRetAndRecycleGraph();
    void ForwardExecution(Val*, SymGraph::RootTask*,unsigned);
    void BackwardExecution(SymVal*, Val::ReadyType);
    int ProcessMessage(Message*, int);


    ThreadPool pool;
    CO3_SER * ser;
    MsgQueue msgQueue;
    std::string symInputFile;


    Val::BasicBlockIdType lastBBID = 0;
    std::stack<SymGraph*> callStack;

    std::map<unsigned, vector<SymGraph*> > recyclable;
    std::map<unsigned, SymGraph*> vanillaSymGraphs;
    //std::map<string, FuncFileNames> funcFiles;
};


#endif //SYMBACKEND_ORCHESTRATOR_H
