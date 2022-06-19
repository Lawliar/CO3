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

        std::set<Val::BasicBlockIdType> dominance;
        std::set<Val::BasicBlockIdType > post_dominance;// this BB post dominate these BBs
        std::set<Val*> allNodes;
        std::set<Val*> nonReadyRoots;
        // leaves are the "inputs" to this basic block
        std::set<Val*> leaves;
        // roots are the non-out vertices and the direct out vertices
        std::set<Val*> roots;
        bool isBBReady();
        void Refresh(Val::ReadyType targetReady);
        // map the root to its Dependent BBs that are not in the loop(because we already make sure loop is properly executed)
        //std::map<Val*, DataDependents* > nonLoopRootDependents;
    };

    class RootTask{
    public:
        // not really used
        //std::set<Val*> nonReadyDeps;
        //end
        std::set<Val*> inBBNonReadyLeafDeps;
        std::set<Val*> inBBNonReadyDeps;
        // because when we construct this node, we trust that
        // all its dependencies in other basic blocks(that dominates this BB)
        // have already been executed, thus we only record the BBID, and make sure that BB is executed
        // instead of the individual node.
        std::vector<BasicBlockTask*> depNonReadyNonLoopBB;
        SymVal *  root;
        Val::BasicBlockIdType rootBBid;
        BasicBlockTask* rootBBTask;
        void InsertNonReadyDep(Val* v,std::map<Val::BasicBlockIdType, BasicBlockTask*>& bbTasks);
        bool hasRuntimeDep(){
            for(auto eachLeafDep : inBBNonReadyLeafDeps){
                auto tmpRuntime = dynamic_cast<RuntimeVal*>(eachLeafDep);
                if(tmpRuntime != nullptr){
                    return true;
                }
            }
            return false;
        }

        void Refresh();

        RootTask(SymVal* r, BasicBlockTask* rootBBTask): root(r), rootBBid(r->BBID),rootBBTask(rootBBTask){};
    };
private:
    std::set<Val::BasicBlockIdType> domChildrenOf(Val::BasicBlockIdType, map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t>, RuntimeCFG::DominanceTree&);
    void dbgBBLeaves(Val::ValVertexType);
    void dbgBBRoot(Val::ValVertexType);
    void prepareBBTask();
    bool sortNonLoopBB(BasicBlockTask*, BasicBlockTask*);
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


    // some basic information
    std::string funcname;
    RuntimeCFG cfg;
    RuntimeSymFlowGraph dfg;

    // some mapping just to trade space for speed
    map<RuntimeSymFlowGraph::vertex_t, Val::ValVertexType> ver2offMap;
    map<Val::SymIDType , Val::ValVertexType> symID2offMap;
    std::map<Val::BasicBlockIdType, BasicBlockTask*> bbTasks;

    // the real thing
    vector<Val*> Nodes;
    vector<SymVal_sym_get_parameter_expression*> getParametersSym;
    SymVal_sym_set_return_expression* setRetSym;
    map<unsigned char, SymVal_sym_notify_call*> callInsts;

    Val* stripPhis(Val*, Val*);
    Val* stripTruePhi(Val*, Val*);

    RootTask* GetRootTask(SymVal*);

    map<SymVal*,RootTask*> rootTasks;
};



#endif //SYMBACKEND_SYMGRAPH_H
