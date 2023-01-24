//
// Created by charl on 4/29/2022.
//

#ifndef SYMBACKEND_MSGQUEUE_H
#define SYMBACKEND_MSGQUEUE_H

#include <deque>
#include <mutex>
#include <map>

enum {
    SYM_BLD_INT_1 =      1,
    SYM_BLD_INT_1_1       ,
    SYM_BLD_INT_2         ,
    SYM_BLD_INT_2_1       ,
    SYM_BLD_INT_4         ,
    SYM_BLD_INT_4_1       ,
    SYM_BLD_FLOAT         ,
    SYM_BLD_FLOAT_1       ,
    SYM_BLD_FLOAT_DBL     ,
    SYM_BLD_FLOAT_DBL_1   ,
    SYM_BLD_BOOL          ,
    SYM_BLD_BOOL_1        ,
    SYM_BLD_PATH_CNSTR    ,
    SYM_BLD_PATH_CNSTR_1  ,
    SYM_BLD_MEMCPY        ,
    SYM_BLD_MEMCPY_1      ,
    SYM_BLD_MEMSET        ,
    SYM_BLD_MEMSET_1      ,
    SYM_BLD_MEMMOVE       ,
    SYM_BLD_MEMMOVE_1     ,
    SYM_BLD_READ_MEM      ,
    SYM_BLD_READ_MEM_1    ,
    SYM_BLD_READ_MEM_HW   ,
    SYM_BLD_READ_MEM_HW_1 ,
    SYM_BLD_READ_MEM_W    ,
    SYM_BLD_READ_MEM_W_1  ,

    SYM_BLD_WRITE_MEM     ,
    SYM_BLD_WRITE_MEM_1   ,

// SYM_SET_PAR_EXP       ,  // these are not sent to the PC
// SYM_GET_PAR_EXP       ,
// SYM_SET_RET_EXP       ,
// SYM_GET_RET_EXP       ,

    SYM_NTFY_PHI          ,
    SYM_NTFY_PHI_1        ,

    SYM_NTFY_CALL         ,
    SYM_NTFY_FUNC         ,
    SYM_NTFY_RET          ,
    SYM_NTFY_BBLK         ,
    SYM_NTFY_BBLK1        ,

    SYM_TRY_ALT           ,
    SYM_TRY_ALT_1         ,

    SYM_INIT              ,
    SYM_INIT_DR           ,
    SYM_END
};





//legend FIELDNAME (SIZE in bytes)
#define SIZE_SYM_BLD_INT_1          3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_INT_1_1        4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_INT_2          4      // | FCODE(1) | symID(1)         | val(2)      |
#define SIZE_SYM_BLD_INT_2_1        5      // | FCODE(1) | symID(2)         | val(2)      |

#define SIZE_SYM_BLD_INT_4          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_INT_4_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT          6      // | FCODE(1) | symID(1)         | val(4)      |
#define SIZE_SYM_BLD_FLOAT_1        7      // | FCODE(1) | symID(2)         | val(4)      |

#define SIZE_SYM_BLD_FLOAT_DBL      10     // | FCODE(1) | symID(1)         | val(8)      |
#define SIZE_SYM_BLD_FLOAT_DBL_1    11     // | FCODE(1) | symID(2)         | val(8)      |

#define SIZE_SYM_BLD_BOOL           3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_BOOL_1         4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_PATH_CNSTR     3      // | FCODE(1) | symID(1)         | val(1)      |
#define SIZE_SYM_BLD_PATH_CNSTR_1   4      // | FCODE(1) | symID(2)         | val(1)      |

#define SIZE_SYM_BLD_MEMCPY         12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMCPY_1       13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_MEMSET         8      // | FCODE(1) | symID(1)         | mem(4)      | length(2) |
#define SIZE_SYM_BLD_MEMSET_1       9      // | FCODE(1) | symID(2)         | mem(4)      | length(2) |

#define SIZE_SYM_BLD_MEMMOVE        12     // | FCODE(1) | symID(1)         | dest(4)     | src(4)    | length(2) |
#define SIZE_SYM_BLD_MEMMOVE_1      13     // | FCODE(1) | symID(2)         | dest(4)     | src(4)    | length(2) |

#define SIZE_SYM_BLD_READ_MEM       6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_READ_MEM_1     7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_BLD_READ_MEM_HW    8      // | FCODE(1) | symID(1)         | addr(4)     | val(2)    |
#define SIZE_SYM_BLD_READ_MEM_HW_1  9      // | FCODE(1) | symID(2)         | addr(4)     | val(2)    |

#define SIZE_SYM_BLD_READ_MEM_W     10     // | FCODE(1) | symID(1)         | addr(4)     | val(4)    |
#define SIZE_SYM_BLD_READ_MEM_W_1   11     // | FCODE(1) | symID(2)         | addr(4)     | val(4)    |

#define SIZE_SYM_BLD_WRITE_MEM      6      // | FCODE(1) | symID(1)         | addr(4)     |
#define SIZE_SYM_BLD_WRITE_MEM_1    7      // | FCODE(1) | symID(2)         | addr(4)     |

#define SIZE_SYM_NTFY_PHI           3      // | FCODE(1) | symID(1)         | branchNo(1) |
#define SIZE_SYM_NTFY_PHI_1         4      // | FCODE(1) | symID(2)         | branchNo(1) |

#define SIZE_SYM_NTFY_CALL          2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_FUNC          2      // | FCODE(1) | func_id(1)       |
#define SIZE_SYM_NTFY_RET           2      // | FCODE(1) | call_inst_id(1)  |
#define SIZE_SYM_NTFY_BBLK          2      // | FCODE(1) | bbid(1)          |
#define SIZE_SYM_NTFY_BBLK1         3      // | FCODE(1) | bbid(2)          |

#define SIZE_SYM_TRY_ALTERNATIVE    3      // | FCODE(1) | symID(2)         |
#define SIZE_SYM_TRY_ALTERNATIVE_1  2      // | FCODE(1) | symID(1)         |

#define SIZE_SYM_INIT               5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_INIT_DR            5      // | FCODE(1) | addr(4)          |
#define SIZE_SYM_END                1      // | FCODE(1) |

extern "C"{
#include "serialport.h"
}
#include "Message.h"
class MsgQueue {
public:
    MsgQueue(OpenedSP sp):sp(sp){

    }
    ~MsgQueue();
    bool RenderAndPush(char * buf, char size);
    bool ProcessMsgs();
    uint64_t Listen();
    size_t GetQueueSize();
    Message* Pop();
    void Push(Message*);
private:
    OpenedSP sp;
#ifdef DEBUG_CHECKING
    unsigned int dbgNumBytesReceived = 0;
#endif
    std::mutex msgQMutex;
    std::deque<Message*> msgQueue;
};


#endif //SYMBACKEND_MSGQUEUE_H
