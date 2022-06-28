from IPython import embed
from enum import IntEnum
import struct

class MsgTypes(IntEnum):
    SYM_BLD_INT_1 =      1
    SYM_BLD_INT_2 = 2
    SYM_BLD_INT_4  = 3 
    SYM_BLD_FLOAT  = 4
    SYM_BLD_FLOAT_DBL  = 5
    SYM_BLD_BOOL       = 6
    SYM_BLD_PATH_CNSTR = 7
    SYM_BLD_MEMCPY    = 8
    SYM_BLD_MEMSET    = 9
    SYM_BLD_MEMMOVE   = 10
    SYM_BLD_READ_MEM  = 11
    SYM_BLD_WRITE_MEM =12

    SYM_SET_PAR_EXP   = 13
    SYM_GET_PAR_EXP   = 14
    SYM_SET_RET_EXP   = 15
    SYM_GET_RET_EXP   = 16

    SYM_NTFY_PHI      = 17
    SYM_NTFY_CALL     = 18
    SYM_NTFY_FUNC     = 19
    SYM_NTFY_RET      = 20
    SYM_NTFY_BBLK     = 21
    SYM_NTFY_BBLK1    = 22
    SYM_INIT          = 23



with open("data", "rb") as rfile:
    data = rfile.read()
msgs = []


def parsePackage(package):
    cur = 0
    while cur < len(package):
        if(package[cur] == MsgTypes.SYM_BLD_INT_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("BuildInt 1: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_2):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 5],byteorder='little')
            cur += 5
            print("BuildInt 2: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_4):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("BuildInt 4: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = struct.unpack('f',package[cur + 3: cur + 7])
            cur += 7
            print("Build Float: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT_DBL):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = struct.unpack('f',package[cur + 3: cur + 11])
            cur += 11
            print("Build DBL: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_BOOL):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("Build bool: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_PATH_CNSTR):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("Push Constraint: symval:{}, val:{}".format(symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMCPY):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            dest = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            src = int.from_bytes(package[cur + 7: cur + 11],byteorder='little')
            length = int.from_bytes(package[cur + 11: cur + 13],byteorder='little')
            cur += 13
            print("MemCpy: symval:{}, dest:{}, src:{}, length:{}".format(symid, dest, src, length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMSET):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            mem = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            length = int.from_bytes(package[cur + 7: cur + 9],byteorder='little')
            cur += 9
            print("MemSet: symval:{}, mem:{},  length:{}".format(symid, mem, length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMMOVE):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            dest = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            src = int.from_bytes(package[cur + 7: cur + 11],byteorder='little')
            length = int.from_bytes(package[cur + 11: cur + 13],byteorder='little')
            cur += 13
            print("MemMove: symval:{}, dest:{}, src:{}, length:{}".format(symid, dest, src, length))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("ReadMem: symval:{}, addr:{}".format(symid,addr))
        elif(package[cur] == MsgTypes.SYM_BLD_WRITE_MEM):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("WriteMem: symval:{}, addr:{}".format(symid,addr))
        elif(package[cur] == MsgTypes.SYM_NTFY_PHI):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            branchNo = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("NotifyPhi: symval:{}, phi branch:{}".format(symid,branchNo))
        elif(package[cur] == MsgTypes.SYM_NTFY_CALL):
            callId = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("NotifyCall: call Id:{}".format(callId))
        elif(package[cur] == MsgTypes.SYM_NTFY_FUNC):
            funcid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("NotifyFunc: func id:{}".format(funcid))
        elif(package[cur] == MsgTypes.SYM_NTFY_RET):
            retId = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("NotifyRet: ret id:{}".format(retId))
        elif(package[cur] == MsgTypes.SYM_NTFY_BBLK):
            bbid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("NotifyBB: bb id:{}".format(bbid))
        elif(package[cur] == MsgTypes.SYM_NTFY_BBLK1):
            bbid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            cur += 3
            print("NotifyBB1: bb id:{}".format(bbid))
        else:
            embed()
            assert(False)
cur = 0
size = len(data)
print("total len:{}".format(size))
while cur < size:
    package_len = data[cur]
    package = data[cur + 1 : cur + 1 +  package_len]
    parsePackage(package)
    cur =  cur + 1 +  package_len
