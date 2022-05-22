//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"


#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
Orchestrator::Orchestrator(std::string cfg_filename, std::string ddg_filename, std::string sp_port, int baud_rate): sp(initSerialPort(sp_port.c_str(), baud_rate)),
                                                                                                                    msgQueue(sp)
{
    cfg.readGraphViz(cfg_filename);
    ddg.readGraphViz(ddg_filename);
}
Orchestrator::~Orchestrator() {
    freeSerialPort(sp);
}

int Orchestrator::StartListen() {

}
int Orchestrator::Run() {
    while(true){
        Message* cur_msg = msgQueue.Pop();
        if(cur_msg == nullptr){
            //TODO: do checking
            break;
        }else{
            if(RuntimeValueMessage * rv_msg = dynamic_cast<RuntimeValueMessage*>(cur_msg)){
                //
            }else if(ControlMessgaes* ctrl_msg = dynamic_cast<ControlMessgaes*>(cur_msg)){
                if(ctrl_msg->type == Message::BasicBlockMsg){
                    // for dbg purpose
                    BBTrace.push_back(ctrl_msg->id);

                }else if(ctrl_msg->type == Message::CallMsg){

                }else if(ctrl_msg->type == Message::RetMsg){

                }
            }
        }
    }
    return 0;
}
#pragma clang diagnostic pop