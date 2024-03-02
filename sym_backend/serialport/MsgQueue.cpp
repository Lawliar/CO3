//
// Created by charl on 4/29/2022.
//

#include "MsgQueue.h"
#include <assert.h>
#include <fstream>
#include "getTimeStamp.h"

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
    if(ser.used == true){
        // if we use the serial port (including the real serial port and TCP  port)
        while(true){
            int received = receiveData(ser);
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
    return  getTimeStamp();
}

MsgQueue::~MsgQueue() {
    //assert(ring_buffer_num_items(&RingBuffer) == 0);
    msgQueue.clear();
}



#define frameByteNum 64
bool MsgQueue::ProcessMsgs() {
    ring_buffer_size_t avaiNumBytes = ring_buffer_num_items(&RingBuffer);
    assert(avaiNumBytes > 0);

    char packetLen = 0;
    char tempBuffer[frameByteNum];
    bool end_received = false;
    while( avaiNumBytes > 0){
        int cur_len = 0;
        if(avaiNumBytes > frameByteNum){
            ring_buffer_dequeue_arr(&RingBuffer,tempBuffer, frameByteNum);
            cur_len = frameByteNum;
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