//
// Created by lcm on 2/29/24.
//
#include <Message.h>
#include <MsgQueue.h>
#include <iostream>
#include <assert.h>

bool MsgQueue::RenderAndPush(char * buf, char size){
    int cur = 0;
    bool ret_received = false;
    while(cur < size ){
        if(buf[cur] == SYM_BLD_INT_1){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t *)(buf + cur + 2);
            Push(new BuildIntValueMessage(symid, 1, val ));
            cur += SIZE_SYM_BLD_INT_1;
        }else if(buf[cur] == SYM_BLD_INT_1_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t *)(buf + cur + 2);
            Push(new BuildIntValueMessage(symid, 1, val ));
            cur += SIZE_SYM_BLD_INT_1_1;
        }else if(buf[cur] == SYM_BLD_INT_2){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint16_t val = *(uint16_t *)(buf + cur + 2);
            Push(new BuildIntValueMessage(symid, 2, val ));
            cur += SIZE_SYM_BLD_INT_2;
        }else if(buf[cur] == SYM_BLD_INT_2_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint16_t val = *(uint16_t *)(buf + cur + 3);
            Push(new BuildIntValueMessage(symid, 2, val ));
            cur += SIZE_SYM_BLD_INT_2_1;
        }else if(buf[cur] == SYM_BLD_INT_4){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t *)(buf + cur + 2);
            Push(new BuildIntValueMessage(symid, 4, val ));
            cur += SIZE_SYM_BLD_INT_4;
        }else if(buf[cur] == SYM_BLD_INT_4_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t *)(buf + cur + 3);
            Push(new BuildIntValueMessage(symid, 4, val ));
            cur += SIZE_SYM_BLD_INT_4_1;
        }else if(buf[cur] == SYM_BLD_FLOAT){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t*)(buf + cur + 2);
            Push(new BuildFloatValueMessage(symid, static_cast<float>(val)));
            cur += SIZE_SYM_BLD_FLOAT;
        }else if(buf[cur] == SYM_BLD_FLOAT_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t val = *(uint32_t*)(buf + cur + 3);
            Push(new BuildFloatValueMessage(symid, static_cast<float>(val)));
            cur += SIZE_SYM_BLD_FLOAT_1;
        }else if(buf[cur] == SYM_BLD_FLOAT_DBL){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint64_t val = *(uint64_t*)(buf + cur + 2);
            Push(new BuildDoubleValueMessage(symid, static_cast<double>(val) ));
            cur += SIZE_SYM_BLD_FLOAT_DBL;
        }else if(buf[cur] == SYM_BLD_FLOAT_DBL_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint64_t val = *(uint64_t*)(buf + cur + 3);
            Push(new BuildDoubleValueMessage(symid, static_cast<double>(val) ));
            cur += SIZE_SYM_BLD_FLOAT_DBL_1;
        }else if(buf[cur] == SYM_BLD_BOOL){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            Push(new BuildBoolValueMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_BOOL;
        }else if(buf[cur] == SYM_BLD_BOOL_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            Push(new BuildBoolValueMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_BOOL_1;
        }else if(buf[cur] == SYM_BLD_PATH_CNSTR){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            Push(new PushConstraintMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_PATH_CNSTR;
        }else if(buf[cur] == SYM_BLD_PATH_CNSTR_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            Push(new PushConstraintMessage(symid, static_cast<bool>(val)));
            cur += SIZE_SYM_BLD_PATH_CNSTR_1;
        }

        else if(buf[cur] == SYM_BLD_MEMCPY){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 6);
            uint16_t len = *(uint16_t*)(buf + cur + 10);
            Push(new MemCpyMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMCPY;
        }else if(buf[cur] == SYM_BLD_MEMCPY_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 7);
            uint16_t len = *(uint16_t*)(buf + cur + 11);
            Push(new MemCpyMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMCPY_1;
        }
        else if(buf[cur] == SYM_BLD_MEMSET){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint16_t len = *(uint16_t*)(buf + cur + 6);
            Push(new MemSetMessage(symid, ptr, len));
            cur += SIZE_SYM_BLD_MEMSET;
        }else if(buf[cur] == SYM_BLD_MEMSET_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint16_t len = *(uint16_t*)(buf + cur + 7);
            Push(new MemSetMessage(symid, ptr, len));
            cur += SIZE_SYM_BLD_MEMSET_1;
        }
        else if(buf[cur] == SYM_BLD_MEMMOVE){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 6);
            uint16_t len = *(uint16_t*)(buf + cur + 10);
            Push(new MemMoveMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMMOVE;
        }else if(buf[cur] == SYM_BLD_MEMMOVE_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t dest_ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t src_ptr = *(uint32_t*)(buf + cur + 7);
            uint16_t len = *(uint16_t*)(buf + cur + 11);
            Push(new MemMoveMessage(symid, dest_ptr, src_ptr, len));
            cur += SIZE_SYM_BLD_MEMMOVE_1;
        }

        else if(buf[cur] == SYM_BLD_READ_MEM){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            Push(new ReadMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_READ_MEM;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            Push(new ReadMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_READ_MEM_1;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_HW){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint16_t val = *(uint16_t*)(buf + cur + 6);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            Push(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_HW;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_HW_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint16_t val = *(uint16_t*)(buf + cur + 7);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            Push(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_HW_1;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_W){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            uint32_t val = *(uint32_t*)(buf + cur + 6);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            Push(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_W;
        }
        else if(buf[cur] == SYM_BLD_READ_MEM_W_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            uint32_t val = *(uint32_t*)(buf + cur + 7);
            auto cur_msg = new ReadMemMessage(symid,ptr);
            cur_msg->AddConcreteValue(val);
            Push(cur_msg);
            cur += SIZE_SYM_BLD_READ_MEM_W_1;
        }
        else if(buf[cur] == SYM_BLD_WRITE_MEM){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 2);
            Push(new WriteMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_WRITE_MEM;
        }else if(buf[cur] == SYM_BLD_WRITE_MEM_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint32_t ptr = *(uint32_t*)(buf + cur + 3);
            Push(new WriteMemMessage(symid,ptr));
            cur += SIZE_SYM_BLD_WRITE_MEM_1;
        }
        else if(buf[cur] == SYM_NTFY_PHI){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 2);
            Push(new PhiValueMessage(symid, val));
            cur += SIZE_SYM_NTFY_PHI;
        }else if(buf[cur] == SYM_NTFY_PHI_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            uint8_t val = *(uint8_t*)(buf + cur + 3);
            Push(new PhiValueMessage(symid, val));
            cur += SIZE_SYM_NTFY_PHI_1;
        }
        else if(buf[cur] == SYM_NTFY_CALL){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            Push(new NotifyCallMessage(id));
            cur += SIZE_SYM_NTFY_CALL;
        }else if(buf[cur] == SYM_NTFY_FUNC){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            Push(new NotifyFuncMessage(id));
            cur += SIZE_SYM_NTFY_FUNC;
        }else if(buf[cur] == SYM_NTFY_RET){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            Push(new NotifyRetMessage(id));
            cur += SIZE_SYM_NTFY_RET;
        }else if(buf[cur] == SYM_NTFY_BBLK){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            Push(new NotifyBasicBlockMessage(static_cast<uint16_t>(id)));
            cur += SIZE_SYM_NTFY_BBLK;
        }else if(buf[cur] == SYM_NTFY_BBLK1){
            uint16_t  id = *(uint16_t*)(buf + cur + 1);
            Push(new NotifyBasicBlockMessage(id));
            cur += SIZE_SYM_NTFY_BBLK1;
        }else if(buf[cur] == SYM_NTFY_SELECT){
            uint8_t symid = *(uint8_t*)(buf + cur + 1);
            bool cond = static_cast<bool>(*(uint8_t*)(buf + cur + 2));
            Push(new SelectMessage(cond, static_cast<uint16_t>(symid)));
            cur += SIZE_SYM_NTFY_SELECT;
        }else if(buf[cur] == SYM_NTFY_SELECT_1){
            uint16_t symid = *(uint16_t*)(buf + cur + 1);
            bool cond = static_cast<bool>(*(uint8_t*)(buf + cur + 3));
            Push(new SelectMessage(cond, symid));
            cur += SIZE_SYM_NTFY_SELECT_1;
        }else if(buf[cur] == SYM_TRY_ALT){
            uint8_t id = *(uint8_t*)(buf + cur + 1);
            Push(new TryAltMessage(static_cast<uint16_t>(id)));
            cur += SIZE_SYM_TRY_ALTERNATIVE;
        }else if(buf[cur] == SYM_TRY_ALT_1){
            uint16_t  id = *(uint16_t*)(buf + cur + 1);
            Push(new TryAltMessage(id));
            cur += SIZE_SYM_TRY_ALTERNATIVE_1;
        }
        else if(buf[cur] == SYM_INIT){
            char * addr = reinterpret_cast<char *>(*(uint32_t*)(buf + cur + 1));
            Push(new InitMessage(addr, false));
            cur += SIZE_SYM_INIT;
        }else if(buf[cur] == SYM_INIT_DR){
            char * addr = reinterpret_cast<char *>(*(uint32_t*)(buf + cur + 1));
            Push(new InitMessage(addr, true));
            cur += SIZE_SYM_INIT;
        }
        else if(buf[cur] == SYM_END){
            Push(new EndMessage());
            std::cout<<"end msg received\n";
            std::cout.flush();
            ret_received = true;
            cur += SIZE_SYM_END;
        }else{
            std::cerr <<"unhandled msg type:"<< static_cast<unsigned>(buf[cur])  <<", the connection is corrupted";
            abort();
        }
    }
    assert(cur == size);
    return ret_received;
}