//
// Created by charl on 4/29/2022.
//

#include "MsgQueue.h"
#include <assert.h>
#include <iostream>
#include <fstream>
extern ring_buffer_t RingBuffer;

size_t MsgQueue::GetQueueSize(){
    size_t ret = 0;
    //msgQMutex.lock();
    ret = msgQueue.size();
    //msgQMutex.unlock();
    return ret;
}
Message* MsgQueue::Pop(){
    Message* ret;
    //msgQMutex.lock();
    size_t size = msgQueue.size();
    if(size == 0){
        ret = nullptr;
    }else{
        ret = msgQueue.front();
        msgQueue.pop_front();
    }
    //msgQMutex.unlock();
    return ret;
}
extern std::string dbgInputFileName;
int MsgQueue::Listen() {
    unsigned bytesWaiting;
    unsigned frameLen = 64;
    if(sp.port != nullptr){
        while(true){
            bytesWaiting = GetNumBytesWaiting(sp);
            if(bytesWaiting > 0){
                //receive data and write it to ringbuffer
                if(bytesWaiting <= frameLen){
                    receiveData(sp.port, bytesWaiting, 1000);
                    ProcessMsgs();
                }else{
                    unsigned cursor = 0;
                    for(; cursor < bytesWaiting ; cursor += frameLen){
                        unsigned remaining = bytesWaiting - cursor;
                        if( remaining >= frameLen){
                            receiveData(sp.port, frameLen, 1000);
                            ProcessMsgs();
                        }else{
                            receiveData(sp.port, remaining, 1000);
                            ProcessMsgs();
                        }
                    }
                }
            }else{
            }
        }
    }else{
        std::ifstream inputFile(dbgInputFileName, std::ios::binary);
        assert(inputFile.is_open());
        char buf[128];
        while(!inputFile.eof()){
            inputFile.read(buf, frameLen);
            std::streamsize s = inputFile.gcount();
            unsigned emptyBytes = ring_buffer_num_empty_items(&RingBuffer);
            assert(emptyBytes >= frameLen);// I trusted when calling ProcessMsg, at least one whole frame is processed
            ring_buffer_queue_arr(&RingBuffer, buf, s);
            ProcessMsgs();
        }
    }
    return 0;
}

MsgQueue::~MsgQueue() {
    assert(ring_buffer_num_items(&RingBuffer) == 0);
    msgQueue.clear();
}

void MsgQueue::RenderAndPush(char * buf, char size){
    int cur = 0;
    while(cur < size ){
        if(buf[cur] == SYM_BLD_INT_1){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t *)(buf + cur + 2);
            msgQueue.push_back(new BuildIntValueMessage(symid, 1, val ));
            cur += SIZE_SYM_BLD_INT_1;
        }else if(buf[cur] == SYM_BLD_INT_1_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t *)(buf + cur + 2);
            msgQueue.push_back(new BuildIntValueMessage(symid, 1, val ));
            cur += SIZE_SYM_BLD_INT_1_1;
        }else if(buf[cur] == SYM_BLD_INT_2){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint16_t val = *(uint16_t *)(buf + cur + 2);
            msgQueue.push_back(new BuildIntValueMessage(symid, 2, val ));
            cur += SIZE_SYM_BLD_INT_2;
        }else if(buf[cur] == SYM_BLD_INT_2_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint16_t val = *(uint16_t *)(buf + cur + 3);
            msgQueue.push_back(new BuildIntValueMessage(symid, 2, val ));
            cur += SIZE_SYM_BLD_INT_2_1;
        }else if(buf[cur] == SYM_BLD_INT_4){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t *)(buf + cur + 2);
            msgQueue.push_back(new BuildIntValueMessage(symid, 4, val ));
            cur += SIZE_SYM_BLD_INT_4;
        }else if(buf[cur] == SYM_BLD_INT_4_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t *)(buf + cur + 3);
            msgQueue.push_back(new BuildIntValueMessage(symid, 4, val ));
            cur += SIZE_SYM_BLD_INT_4_1;
        }else if(buf[cur] == SYM_BLD_FLOAT){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t*)(buf + cur + 2);
            msgQueue.push_back(new BuildFloatValueMessage(symid, static_cast<float>(val)));
            cur += SIZE_SYM_BLD_FLOAT;
        }else if(buf[cur] == SYM_BLD_FLOAT_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t*)(buf + cur + 3);
            msgQueue.push_back(new BuildFloatValueMessage(symid, static_cast<float>(val)));
            cur += SIZE_SYM_BLD_FLOAT_1;
        }else if(buf[cur] == SYM_BLD_FLOAT_DBL){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint64_t val = *(uint64_t*)(buf + cur + 2);
            msgQueue.push_back(new BuildDoubleValueMessage(symid, static_cast<double>(val) ));
            cur += SIZE_SYM_BLD_FLOAT_DBL;
        }else if(buf[cur] == SYM_BLD_FLOAT_DBL_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint64_t val = *(uint64_t*)(buf + cur + 3);
            msgQueue.push_back(new BuildDoubleValueMessage(symid, static_cast<double>(val) ));
            cur += SIZE_SYM_BLD_FLOAT_DBL_1;
        }else if(buf[cur] == SYM_BLD_BOOL){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            msgQueue.push_back(new BuildBoolValueMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_BOOL;
        }else if(buf[cur] == SYM_BLD_BOOL_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            msgQueue.push_back(new BuildBoolValueMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_BOOL_1;
        }else if(buf[cur] == SYM_BLD_PATH_CNSTR){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            msgQueue.push_back(new PushConstraintMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_PATH_CNSTR;
        }else if(buf[cur] == SYM_BLD_PATH_CNSTR_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            msgQueue.push_back(new PushConstraintMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_PATH_CNSTR_1;
        }

        else if(buf[cur] == SYM_BLD_MEMCPY){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 6);
            uint16_t len = *(uint16_t*)(buf + cur + 10);
            msgQueue.push_back(new MemCpyMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMCPY;
        }else if(buf[cur] == SYM_BLD_MEMCPY_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 7);
            uint16_t len = *(uint16_t*)(buf + cur + 11);
            msgQueue.push_back(new MemCpyMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMCPY_1;
        }
        else if(buf[cur] == SYM_BLD_MEMSET){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint16_t len = *(uint16_t*)(buf + cur + 6);
            msgQueue.push_back(new MemSetMessage(symid, ptr, len));
            cur += SIZE_SYM_BLD_MEMSET;
        }else if(buf[cur] == SYM_BLD_MEMSET_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint16_t len = *(uint16_t*)(buf + cur + 7);
            msgQueue.push_back(new MemSetMessage(symid, ptr, len));
            cur += SIZE_SYM_BLD_MEMSET_1;
        }

        else if(buf[cur] == SYM_BLD_MEMMOVE){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 6);
            uint16_t len = *(uint16_t*)(buf + cur + 10);
            msgQueue.push_back(new MemMoveMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMMOVE;
        }else if(buf[cur] == SYM_BLD_MEMMOVE_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 7);
            uint16_t len = *(uint16_t*)(buf + cur + 11);
            msgQueue.push_back(new MemMoveMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMMOVE_1;
        }

        else if(buf[cur] == SYM_BLD_READ_MEM){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            msgQueue.push_back(new ReadMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_READ_MEM;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            msgQueue.push_back(new ReadMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_READ_MEM_1;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_HW){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint16_t val = *(uint16_t*)(buf + cur + 6);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            msgQueue.push_back(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_HW;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_HW_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint16_t val = *(uint16_t*)(buf + cur + 7);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            msgQueue.push_back(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_HW_1;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_W){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t val = *(uint32_t*)(buf + cur + 6);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            msgQueue.push_back(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_W;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_W_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t val = *(uint32_t*)(buf + cur + 7);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            msgQueue.push_back(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_W_1;
        }

        else if(buf[cur] == SYM_BLD_WRITE_MEM){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            msgQueue.push_back(new WriteMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_WRITE_MEM;
        }else if(buf[cur] == SYM_BLD_WRITE_MEM_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            msgQueue.push_back(new WriteMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_WRITE_MEM_1;
        }
        else if(buf[cur] == SYM_NTFY_PHI){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            msgQueue.push_back(new PhiValueMessage(symid, val));
            cur += SIZE_SYM_NTFY_PHI;
        }else if(buf[cur] == SYM_NTFY_PHI_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            msgQueue.push_back(new PhiValueMessage(symid, val));
            cur += SIZE_SYM_NTFY_PHI_1;
        }
        else if(buf[cur] == SYM_NTFY_CALL){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            msgQueue.push_back(new NotifyCallMessage(id));
            cur += SIZE_SYM_NTFY_CALL;
        }else if(buf[cur] == SYM_NTFY_FUNC){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            msgQueue.push_back(new NotifyFuncMessage(id));
            cur += SIZE_SYM_NTFY_FUNC;
        }else if(buf[cur] == SYM_NTFY_RET){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            msgQueue.push_back(new NotifyRetMessage(id));
            cur += SIZE_SYM_NTFY_RET;
        }else if(buf[cur] == SYM_NTFY_BBLK){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            msgQueue.push_back(new NotifyBasicBlockMessage(static_cast<uint16_t>(id)));
            cur += SIZE_SYM_NTFY_BBLK;
        }else if(buf[cur] == SYM_NTFY_BBLK1){
            uint16_t  id = *(uint16_t*)(buf + cur + 1);
            msgQueue.push_back(new NotifyBasicBlockMessage(id));
            cur += SIZE_SYM_NTFY_BBLK1;
        }else{
            std::cerr <<"unhandled msg type:"<< buf[cur] <<", the connection is corrupted";
            assert(false);
        }
    }
    assert(cur == size);
}

void MsgQueue::ProcessMsgs() {
    ring_buffer_size_t avaiNumBytes = ring_buffer_num_items(&RingBuffer);
    assert(avaiNumBytes > 0);

    unsigned processedBytes = 0;
    char numBytesForPacket;
    char tempBuffer[64];

    ring_buffer_peek(&RingBuffer, &numBytesForPacket, 0);

    while( static_cast<unsigned>(numBytesForPacket + 1) <= (avaiNumBytes - processedBytes)){
        //we only deal with a whole packet, if what's remaining is not enough, we just wait for another turn

        //retrive numBytesForPacket out from the ring buffer
        ring_buffer_dequeue(&RingBuffer, &numBytesForPacket); // dequeue one byte for the length
        ring_buffer_dequeue_arr(&RingBuffer,tempBuffer, numBytesForPacket); // dequeue the content
        //render these data and push to the queue
        RenderAndPush(tempBuffer, static_cast<unsigned char>(numBytesForPacket) );
        // mark these are processed
        processedBytes += (static_cast<unsigned char>(numBytesForPacket)  + 1);

        // peek numBytesForPacket for the next packet
        ring_buffer_peek(&RingBuffer, &numBytesForPacket, 0);
    }
}