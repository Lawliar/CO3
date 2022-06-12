//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"
#include <iostream>
#include <queue>
#include <fstream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
Orchestrator::Orchestrator(std::string inputDir, std::string sp_port, int baud_rate): \
sp(initSerialPort(sp_port.c_str(), baud_rate)), msgQueue(sp)
{
    boost::filesystem::path dir (inputDir);
    boost::filesystem::path funcIDFilePath = dir / "spear_func_id.txt";
    if(!boost::filesystem::exists(funcIDFilePath)){
        cerr<<"func id file does not exist";
        assert(false);
    }

    std::ifstream funcIDFile (funcIDFilePath);
    assert(funcIDFile.is_open());
    string line;
    while ( getline (funcIDFile,line) )
    {
        size_t tab_pos = line.find('\t');
        assert(tab_pos != std::string::npos);
        std::string cur_funcname =  line.substr(0, tab_pos);
        unsigned cur_id = stoi(line.substr(tab_pos + 1));

        boost::filesystem::path cfg_file = dir / (cur_funcname + "_cfg.dot");
        if(!boost::filesystem::exists(cfg_file)){
            cerr << cfg_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path dom_file = dir / (cur_funcname + "_dom.dot");
        if(!boost::filesystem::exists(dom_file)){
            cerr << dom_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path postDom_file = dir / (cur_funcname + "_postDom.dot");
        if(!boost::filesystem::exists(postDom_file)){
            cerr << postDom_file.string() <<" does not exist";
            assert(false);
        }
        boost::filesystem::path dfg_file = dir / (cur_funcname + "_dfg.dot");
        if(!boost::filesystem::exists(dfg_file)){
            cerr << dfg_file.string() <<" does not exist";
            assert(false);
        }
        cout << "processing func:"<< cur_funcname<<'\n';
        cout.flush();
        SymGraph* cur_symgraph = new SymGraph(cur_funcname, cfg_file.string(),dom_file.string(),postDom_file.string(),dfg_file.string());
        symGraphs[cur_id] = cur_symgraph;
    }
}
Orchestrator::~Orchestrator() {
    freeSerialPort(sp);
}

SymGraph* Orchestrator::getCurFunc() {
    return callStack.top();
}
int Orchestrator::StartListen() {

}

/*

as truePhi itself does not carry any real information

*/
Val* Orchestrator::stripPhis(Val* nodeInQuestion, Val* root) {
    SymGraph* cur_graph = getCurFunc();
    Val* prev_node = nullptr;
    Val* cur_node = nodeInQuestion;
    SymVal_sym_TruePhi *      true_phi       = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
    SymVal_sym_FalsePhiRoot * false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node);
    SymVal_sym_FalsePhiLeaf * false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node);

    while(true_phi != nullptr || false_phi_root != nullptr || false_phi_leaf != nullptr ){
        // assuming the ReportTruePhi has been sent from the MCU.
        Val::ValVertexType new_vert;
        if(true_phi != nullptr){
            // for truePhi, its ready number might legally be larger than root_ready, we'll just choose the history value we want
            new_vert = true_phi->In_edges.at(true_phi->historyValues.at(root->ready + 1).first);
        }else if(false_phi_root != nullptr){
            if(false_phi_root->falsePhiLeaves.size() == 0){
                // this root has all constant dependencies, which makes it impossible to be symbolized.
                return nullptr;
            }
            if(isNodeReady(cur_node, root)){
                // cur_node is already constructed
                return prev_node;
            }
            new_vert = false_phi_root->In_edges.at(false_phi_root->In_edges.at(1));// 1 is the non-false value

        }else{
            if(isNodeReady(cur_node, root)){
                // cur_node is already constructed
                return prev_node;
            }
            new_vert = false_phi_leaf->In_edges.at(false_phi_leaf->In_edges.at(0));// 0 is the original value
        }
        prev_node = cur_node;
        cur_node = cur_graph->Nodes[new_vert];
        true_phi = dynamic_cast<SymVal_sym_TruePhi*>(cur_node);
        false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(cur_node);
        false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(cur_node);
    }
    return cur_node;
}

inline bool Orchestrator::isNodeReady(Val * nodeInQuestion, Val *  root_in_construct) {
    auto * nodeIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(nodeInQuestion);
    auto * rootIsTruePhi = dynamic_cast<SymVal_sym_TruePhi*>(root_in_construct);
    assert(nodeIsTruePhi == nullptr and rootIsTruePhi == nullptr);
    if(nodeInQuestion == root_in_construct){
        //if it's the same node, then it's not ready
        return false;
    }
    if(!nodeInQuestion->inLoop){
        if(nodeInQuestion->ready == 1){
            return true;
        }else if (nodeInQuestion->ready == 0){
            return false;
        }else{
            cerr<<"Nodes in the non-loop BB should at executed at most once.";
            assert(false);
        }
    }else{
        // now nodeInquest is in loop, and root is in or out of a loop
        if(nodeInQuestion->BBID == root_in_construct->BBID){
            // if they are in the same BB, then it's just a comparison of ready value
            if(nodeInQuestion->ready == root_in_construct->ready){
                // the root we're trying to construct, and this nodeInQuestion has same ready Value
                return false;
            }else if (nodeInQuestion->ready == (root_in_construct->ready + 1)){
                return true;
            }else{
                cerr<<"the dep can only be one more or equal to the root in terms of ready value when they are in the same loop BB";
                assert(false);
            }
        }else{
            // for BB in the loop we execute in the per-BB level
            // given the root is under construction, no matter it's in the loop or not
            // the nodeInQuestion is inside a loop, and they are in different BB
            // then it must mean the construction for the nodeInQuestion must have been finished.
            return true;
        }
    }
}
set<Val*> Orchestrator::getNonReadyDeps(Val* root) {
    SymGraph* cur_graph = getCurFunc();
    {
        // dont' regard true phi as a root, as its ready value does not reflect it's constructed or not.
        SymVal_sym_TruePhi * true_phi = dynamic_cast<SymVal_sym_TruePhi*>(root);
        assert(true_phi == nullptr);
    }
    set<Val*> nonReadyDeps;
    queue<Val*> work_queue;

    set<Val*> visited {root};
    work_queue.push(root);

    while(!work_queue.empty()){
        Val* cur_node = work_queue.front();
        work_queue.pop();
        // stripping nasty phis
        Val * strippedPhis = stripPhis(cur_node, root);
        if(strippedPhis == nullptr){
            // met FalsePhiRoot which is all constants(it means it's just a false), ignore it
            continue;
        }
        SymVal_sym_FalsePhiRoot * false_phi_root = dynamic_cast<SymVal_sym_FalsePhiRoot*>(strippedPhis);
        SymVal_sym_FalsePhiLeaf * false_phi_leaf = dynamic_cast<SymVal_sym_FalsePhiLeaf*>(strippedPhis);
        if(false_phi_root != nullptr || false_phi_leaf != nullptr){
            // we stripped it, and it is still here, that can only mean this is the one we should start building
            assert(!isNodeReady(strippedPhis, root));
            nonReadyDeps.insert(strippedPhis);
        }
        for(auto eachDep : cur_node->In_edges){
            Val* depNode = cur_graph->Nodes[eachDep.second];
            if(isNodeReady(depNode, root)){
                //this dep has been constructed
                continue;
            }
            if(visited.find((depNode)) != visited.end()){
                // this node has been visited before
                continue;
            }
            work_queue.push(depNode);
            nonReadyDeps.insert(depNode);
            visited.insert(depNode);
        }
    }
}

void Orchestrator::ExecuteBasicBlock(Val::BasicBlockIdType bbid, bool force) {

}



void Orchestrator::ForwardExecution(Val* source, bool crossBB, bool force) {

}


void Orchestrator::BackwardExecution(SymVal* sink) {
    //assuming all its dependents are ready
    SymGraph* cur_func = getCurFunc();
    Val::ReadyType root_ready = sink->ready;

    set<Val*> nonReadyDeps = getNonReadyDeps(sink);
    set<Val*> nonReadyLeavesInSameBB;
    vector<Val::BasicBlockIdType> nonReadyBB;

}

void Orchestrator::PreparingCalling(NotifyCallMessage* callMsg){

}

int Orchestrator::Run() {
    while(true){
        Message* msg = msgQueue.Pop();
        if(ControlMessgaes * cnt_msg = dynamic_cast<ControlMessgaes*>(msg); cnt_msg != nullptr){
            if(NotifyBasicBlockMessage * bb_msg = dynamic_cast<NotifyBasicBlockMessage*>(msg) ; bb_msg != nullptr){

            }else if(NotifyFuncMessage * func_msg = dynamic_cast<NotifyFuncMessage*>(msg); func_msg != nullptr){
                func_msg->id;
            }else if(NotifyCallMessage * call_msg = dynamic_cast<NotifyCallMessage*>(msg) ; call_msg != nullptr){

            }else if(NotifyRetMessage * ret_msg = dynamic_cast<NotifyRetMessage*>(msg) ; ret_msg != nullptr){

            }else{
                std::cerr<<"seriously?";
                assert(false);
            }

        }else if (SymSourceMessage* sym_source_msg = dynamic_cast<SymSourceMessage*>(msg) ; sym_source_msg != nullptr){

        }else if (SymSinkMessage* sym_sink_msg = dynamic_cast<SymSinkMessage*>(msg) ; sym_sink_msg != nullptr){

        }else{
            std::cerr<<"seriously?";
            assert(false);
        }
    }
    return 0;
}
#pragma clang diagnostic pop