//
// Created by charl on 5/25/2022.
//

#ifndef SYMBACKEND_SYMGRAPH_H
#define SYMBACKEND_SYMGRAPH_H


#include "RuntimeCFG.h"
#include "RuntimeDataFlowGraph.h"
#include "Val.h"

class DataDependents{
public:
    Val* root;
    set<Val*> deps;
    map<Val*, map<Val::ArgIndexType, DataDependents* > > truePhiDependences;

    DataDependents(Val* root): root(root){}
    ~DataDependents(){
        deps.clear();
        for(auto eachTruePhi: truePhiDependences){
            for(auto eachTruePhiVal: eachTruePhi.second){
                delete eachTruePhiVal.second;
            }
            eachTruePhi.second.clear();
        }
        truePhiDependences.clear();
    }
    void nonTruePhiDataDependentsOf(set<Val*> ancesters);
    void allPossibleDataDependencies(set<Val*>, vector<set<Val*> >& );
};

class SymGraph {
private:
    std::set<Val::BasicBlockIdType> domChildrenOf(Val::BasicBlockIdType, map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t>, RuntimeCFG::DominanceTree&);
    void dbgBBLeaves(Val::ValVertexType);
    void dbgBBRoot(Val::ValVertexType);
    void prepareBBTask();
    bool sortNonLoopBB(Val::BasicBlockIdType, Val::BasicBlockIdType);
    list<Val::BasicBlockIdType> sortNonLoopBBs(set<Val::BasicBlockIdType>);
public:


    SymGraph(std::string funcname, std::string cfg, std::string dt, std::string pdt, std::string dfg );
    ~SymGraph(){
        ver2offMap.clear();
        symID2offMap.clear();
        for(auto eachBBTask: bbTasks){
            delete eachBBTask.second;
            bbTasks[eachBBTask.first] = nullptr;
        }
        for(unsigned i = 0 ; i < Nodes.size(); i ++){
            delete Nodes[i];
            Nodes[i] = nullptr;
        }
    }

    class BasicBlockTask{
    public:

        BasicBlockTask(unsigned id, bool loop):BBID(id),inLoop(loop),ready(0){}
        ~BasicBlockTask(){
            dominance.clear();
            post_dominance.clear();
            leaves.clear();
            roots.clear();
            //for(auto eachRootDep:nonLoopRootDependents){
            //    delete eachRootDep.second;
            //}
            //nonLoopRootDependents.clear();
        }
        Val::BasicBlockIdType BBID;
        bool inLoop;
        Val::ReadyType ready;

        std::set<string> fakeRoots{"_sym_notify_call", "_sym_try_alternative"};
        std::set<Val::BasicBlockIdType> dominance;
        std::set<Val::BasicBlockIdType > post_dominance;// this BB post dominate these BBs
        // leaves are the "inputs" to this basic block
        std::set<Val*> leaves;
        // roots are the non-out vertices and the direct out vertices
        std::set<Val*> roots;

        // map the root to its Dependent BBs that are not in the loop(because we already make sure loop is properly executed)
        //std::map<Val*, DataDependents* > nonLoopRootDependents;
    };

    class RootTask{
    public:
        std::set<Val*> nonReadyDeps;
        std::set<Val*> inBBNonReadyLeafDeps;
        std::vector<Val::BasicBlockIdType> depNonReadyNonLoopBB;
        Val *  root;
        Val::BasicBlockIdType rootBBid;
        BasicBlockTask* rootBBTask;
        void InsertNonReadyDep(Val* v){
            nonReadyDeps.insert(v);
            if(rootBBTask->leaves.find(v) != rootBBTask->leaves.end()){
                inBBNonReadyLeafDeps.insert(v);
            }
            Val::BasicBlockIdType depBBID = v->BBID;
            if(depBBID != rootBBid && depBBID != 0 && v->inLoop == false){
                if(std::find(depNonReadyNonLoopBB.begin(), depNonReadyNonLoopBB.end(), depBBID) == depNonReadyNonLoopBB.end()){
                    depNonReadyNonLoopBB.push_back(depBBID);
                }
            }
        }
        RootTask(Val* r, BasicBlockTask* rootBBTask): root(r), rootBBid(r->BBID),rootBBTask(rootBBTask){};
    };
    // some basic information
    std::string funcname;
    RuntimeCFG cfg;
    RuntimeSymFlowGraph dfg;
    // some mapping
    map<RuntimeSymFlowGraph::vertex_t, Val::ValVertexType> ver2offMap;
    map<Val::SymIDType , Val::ValVertexType> symID2offMap;
    std::map<Val::BasicBlockIdType, BasicBlockTask*> bbTasks;
    vector<Val*> Nodes;
    vector<Val*> getParametersSym;
    Val* setRetSym;

    Val* stripPhis(Val*, Val*);
    Val* stripTruePhi(Val*, Val*);
    inline bool isNodeReady(Val*, Val*);

    RootTask* CreateRootTask(Val*);
    // for debugging purpose only, it's just to keep the record, not referenced to when running
    vector<RootTask*> rootTasks;
};



#endif //SYMBACKEND_SYMGRAPH_H
