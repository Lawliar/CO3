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
public:
    class BasicBlockTask{
    public:

        BasicBlockTask(unsigned id, bool loop):BBID(id),inLoop(loop),ready(0){}
        BasicBlockTask(const BasicBlockTask& other):BBID(other.BBID),inLoop(other.inLoop),ready(0),dominance(other.dominance),\
        post_dominance(other.post_dominance),nonReadyPostDominance(other.nonReadyPostDominance){}
        void FinishCopyConstructing(std::set<Val*>& old_nonReadyRoots,std::set<Val*>& old_leaves,\
        std::set<Val*>& old_roots,std::map<Val*,Val*>& old2new){
            for(auto eachOldNonReadyRoot : old_nonReadyRoots){
                nonReadyRoots.insert(old2new.at(eachOldNonReadyRoot));
            }
            for(auto eachOldLeaf : old_leaves){
                leaves.insert(old2new.at(eachOldLeaf));
            }
            for(auto eachOldRoot : old_roots){
                roots.insert(old2new.at(eachOldRoot));
            }
        }
        ~BasicBlockTask(){
            dominance.clear();
            post_dominance.clear();
            //allNodes.clear();
            nonReadyRoots.clear();
            leaves.clear();
            roots.clear();
            //for(auto eachRootDep:nonLoopRootDependents){
            //    delete eachRootDep.second;
            //}
            //nonLoopRootDependents.clear();
        }
        bool isPostDominatedReady(Val::BasicBlockIdType);
        bool isBBReady();

        Val::BasicBlockIdType BBID = 0;
        bool inLoop = false;
        Val::ReadyType ready = 0;

        std::set<Val::BasicBlockIdType> dominance;
        std::set<Val::BasicBlockIdType > post_dominance;// this BB post dominate these BBs
        //std::set<Val*> allNodes;
        std::set<Val::BasicBlockIdType> nonReadyPostDominance;
        std::set<Val*> nonReadyRoots;
        // leaves are the "inputs" to this basic block
        std::set<Val*> leaves;
        // roots are the non-out vertices and the direct out vertices
        std::set<Val*> roots;

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
        Val::ReadyType old_ready = 0;
        Val::BasicBlockIdType rootBBid = 0;
        BasicBlockTask* rootBBTask = nullptr;
        bool occupied = false;
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

        RootTask(SymVal* r, BasicBlockTask* rootBBTask): root(r), old_ready(r->ready),rootBBid(r->BBID),rootBBTask(rootBBTask){};
        ~RootTask(){
            inBBNonReadyLeafDeps.clear();
            inBBNonReadyDeps.clear();
            depNonReadyNonLoopBB.clear();
            root = nullptr;
        }
    };
    RootTask* GetRootTask(SymVal*);
    bool ReleaseOrRemoveRootTask(SymVal*);
    void RefreshBBTask(Val::BasicBlockIdType, Val::ReadyType);
private:
    std::set<Val::BasicBlockIdType> domChildrenOf(Val::BasicBlockIdType, map<Val::BasicBlockIdType, RuntimeCFG::pd_vertex_t>, RuntimeCFG::DominanceTree&);

#ifdef DEBUG_OUTPUT
    void dbgBBLeaves(Val::ValVertexType);
    void dbgBBRoot(Val::ValVertexType);
#endif
    void prepareBBTask();
    bool sortNonLoopBB(BasicBlockTask*, BasicBlockTask*);
public:


    SymGraph(std::string funcname, std::string cfg, std::string dt, std::string pdt, std::string dfg );
    SymGraph(const SymGraph&);
    ~SymGraph(){
        ver2offMap.clear();
        symID2offMap.clear();
        for(auto eachBBTask: bbTasks){
            delete eachBBTask.second;
            bbTasks[eachBBTask.first] = nullptr;
        }
        for(auto eachRootTask: rootTasks){
            delete eachRootTask.second;
            rootTasks.at(eachRootTask.first) = nullptr;
        }
        for(unsigned i = 0 ; i < Nodes.size(); i ++){
            delete Nodes[i];
            Nodes[i] = nullptr;
        }
    }


    // some basic information
    std::string funcname;
    RuntimeCFG *cfg;
    RuntimeSymFlowGraph *dfg;

    // some mapping just to trade space for speed
    map<RuntimeSymFlowGraph::vertex_t, Val::ValVertexType> ver2offMap;
    map<Val::SymIDType , Val::ValVertexType> symID2offMap;

    bool changed = false;
    // the real thing
    vector<Val*> Nodes;
    vector<SymVal_sym_get_parameter_expression*> getParametersSym;
    SymVal_sym_set_return_expression* setRetSym = nullptr;
    map<unsigned char, SymVal_sym_notify_call*> callInsts;
    map<Val::SymIDType, Val::SymIDType> symIDReditectMap;

    map<SymVal*,RootTask*> rootTasks;
    std::map<Val::BasicBlockIdType, BasicBlockTask*> bbTasks;
};



#endif //SYMBACKEND_SYMGRAPH_H
