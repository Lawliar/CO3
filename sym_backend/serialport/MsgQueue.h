// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.



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
    bool RenderAndPush(char * buf, int size);
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
