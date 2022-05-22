//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MSGQUEUE_H
#define SYMBACKEND_MSGQUEUE_H

#include <deque>
#include <mutex>

extern "C"{
#include "serialport.h"
}
#include "Message.h"
class MsgQueue {
public:
    MsgQueue(OpenedSP sp):sp(sp){}

    [[noreturn]] void Listen();
    size_t GetQueueSize();
    Message* Pop();
private:
    OpenedSP sp;
    std::mutex msgQMutex;
    std::deque<Message*> msgQueue;
};


#endif //SYMBACKEND_MSGQUEUE_H
