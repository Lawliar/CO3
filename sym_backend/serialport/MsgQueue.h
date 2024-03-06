//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MSGQUEUE_H
#define SYMBACKEND_MSGQUEUE_H

#include <deque>
#include <mutex>
#include <map>


extern "C"{
#include "transmit.h"
}

#if defined(CO3_REPLACE)
#include "Message.h"
#else
#include "Message_no_replace.h"
#endif

class MsgQueue {
public:
    MsgQueue(CO3_SER* ser):ser(ser){

    }
    ~MsgQueue();
    bool RenderAndPush(char * buf, char size);
    bool ProcessMsgs();
    uint64_t Listen();
    Message* Pop();
    void Push(Message*);
private:
    CO3_SER* ser;
#ifdef DEBUG_CHECKING
    unsigned int dbgNumBytesReceived = 0;
#endif
    std::mutex msgQMutex;
    std::deque<Message*> msgQueue;
};


#endif //SYMBACKEND_MSGQUEUE_H
