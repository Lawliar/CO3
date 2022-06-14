//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MSGQUEUE_H
#define SYMBACKEND_MSGQUEUE_H

#include <deque>
#include <mutex>
#include <map>

extern "C"{
#include "serialport.h"
}
#include "Message.h"
class MsgQueue {
    std::map<unsigned, unsigned> MsgLen;
public:
    MsgQueue(OpenedSP sp):sp(sp){
        MsgLen = {
                {SYM_BLD_INT_1      , 4 },
                {SYM_BLD_INT_2      , 5 },
                {SYM_BLD_INT_4      , 7 },
                {SYM_BLD_FLOAT      , 7 },
                {SYM_BLD_FLOAT_DBL  , 11 },
                {SYM_BLD_BOOL       , 4 },
                {SYM_BLD_PATH_CNSTR , 4 },
                {SYM_BLD_MEMCPY     , 13 },
                {SYM_BLD_MEMSET     , 9 },
                {SYM_BLD_MEMMOVE    , 13 },
                {SYM_BLD_READ_MEM   , 7 },
                {SYM_BLD_WRITE_MEM  , 7 },
                {SYM_NTFY_PHI       ,4 },
                {SYM_NTFY_CALL      ,2 },
                {SYM_NTFY_FUNC      ,2 },
                {SYM_NTFY_RET       ,2 },
                {SYM_NTFY_BBLK      ,2 },
                {SYM_NTFY_BBLK1     ,3 }
        };
    }
    ~MsgQueue();
    void RenderAndPush(char * buf, char size);
    void ProcessMsgs();
    [[noreturn]] int Listen();
    size_t GetQueueSize();
    Message* Pop();
private:
    OpenedSP sp;
    //std::mutex msgQMutex;
    std::deque<Message*> msgQueue;
};


#endif //SYMBACKEND_MSGQUEUE_H
