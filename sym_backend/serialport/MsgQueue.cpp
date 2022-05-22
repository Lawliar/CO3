//
// Created by charl on 4/29/2022.
//

#include "MsgQueue.h"

size_t MsgQueue::GetQueueSize(){
    size_t ret = 0;
    msgQMutex.lock();
    ret = msgQueue.size();
    msgQMutex.unlock();
    return ret;
}
Message* MsgQueue::Pop(){
    Message* ret;
    msgQMutex.lock();
    size_t size = msgQueue.size();
    if(size == 0){
        ret = nullptr;
    }else{
        ret = msgQueue.front();
        msgQueue.pop_front();
    }
    msgQMutex.unlock();
    return ret;
}

[[noreturn]] void MsgQueue::Listen() {
    while(true){
        int bytesWaiting = GetNumBytesWaiting(sp);
        if(bytesWaiting > 0){
            ReceivedBuf buf = receiveData(sp.port, bytesWaiting, 1000);
        }else{
            // maybe sleep, but we can designate a core, so who cares
        }
        bytesWaiting = GetNumBytesWaiting(sp);
    }

}