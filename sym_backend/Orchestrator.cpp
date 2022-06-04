//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"
#include <iostream>

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

int Orchestrator::StartListen() {

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