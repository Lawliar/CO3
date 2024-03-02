#include <Orchestrator.h>

int Orchestrator::ProcessMessage(Message* msg) {
    int ret = 1;
    if(auto end_msg = dynamic_cast<EndMessage*>(msg); end_msg != nullptr ){
        ret = 0;
    }
    return ret;
}