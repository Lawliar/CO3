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



#include "MsgQueue.h"
#include <assert.h>
#include <fstream>
extern "C"{
#include "getTimeStamp.h"
}
#include <iostream>
extern ring_buffer_t RingBuffer;


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

void MsgQueue::Push(Message *msg) {
    msgQMutex.lock();
    msgQueue.push_back(msg);
    msgQMutex.unlock();
}

extern std::string dbgUsbFileName;
uint64_t MsgQueue::Listen() {
    uint64_t start_time = 0;
    uint64_t end_time = 0;
    uint64_t total_receiving_time = 0;
    if(ser->used == true){
        // if we use the serial port (including the real serial port and TCP  port)
        while(true){
            start_time = getTimeStamp();
            int received = receiveData(ser);
            end_time = getTimeStamp();
            total_receiving_time += (end_time - start_time);
#ifdef DEBUG_CHECKING
            dbgNumBytesReceived += received;
#endif
            if(received != 0 && received != 1){
                bool end_received = ProcessMsgs();
                if(end_received){
                    break;
                }
            }else{
                break;
            }
        }
    }else{
        // we are reading from a file
        std::ifstream inputFile(dbgUsbFileName, std::ios::binary);
        assert(inputFile.is_open());
        char buf[128];
        while(!inputFile.eof()){
            inputFile.read(buf, 1);
            std::streamsize s = inputFile.gcount();
            if(s == 0){
                break;
            }
            assert(s == 1);
            int packet_len = (int)*(char *)buf;

            inputFile.read(buf, packet_len - 1);
            s = inputFile.gcount();
            assert(s == packet_len - 1 );

            unsigned emptyBytes = ring_buffer_num_empty_items(&RingBuffer);
            assert(emptyBytes >= packet_len - 1);// I trusted when calling ProcessMsg, at least one whole frame is processed

            ring_buffer_queue_arr(&RingBuffer, buf, packet_len - 1);
            ProcessMsgs();
        }
        inputFile.close();
    }

    std::cout << "receiving time costs:"<< total_receiving_time <<'\n';
    return  end_time;
}

MsgQueue::~MsgQueue() {
    //assert(ring_buffer_num_items(&RingBuffer) == 0);
    msgQueue.clear();
}




bool MsgQueue::ProcessMsgs() {
    ring_buffer_size_t avaiNumBytes = ring_buffer_num_items(&RingBuffer);
    assert(avaiNumBytes > 0);

    char tempBuffer[CO3_FRAME_LEN];
    bool end_received = false;
    while( avaiNumBytes > 0){
        int cur_len = 0;
        if(avaiNumBytes > CO3_FRAME_LEN){
            ring_buffer_dequeue_arr(&RingBuffer,tempBuffer, CO3_FRAME_LEN);
            cur_len = CO3_FRAME_LEN;
        }else{
            ring_buffer_dequeue_arr(&RingBuffer,tempBuffer, avaiNumBytes);
            cur_len = avaiNumBytes;
        }
        //render these data and push to the queue
        end_received = RenderAndPush(tempBuffer,  cur_len);
        if(end_received){
            return true;
        }else{
            avaiNumBytes = ring_buffer_num_items(&RingBuffer);
        }
    }
    return end_received;
}