from IPython import embed
from enum import IntEnum
import struct
from traceback import print_stack
class MsgTypes(IntEnum):
    SYM_BLD_INT_1       =      1
    SYM_BLD_INT_1_1     =      2
    SYM_BLD_INT_2       =      3
    SYM_BLD_INT_2_1     =      4
    SYM_BLD_INT_4       =      5
    SYM_BLD_INT_4_1     =      6 
    SYM_BLD_FLOAT       =      7
    SYM_BLD_FLOAT_1     =      8
    SYM_BLD_FLOAT_DBL   =      9
    SYM_BLD_FLOAT_DBL_1 =      10
    SYM_BLD_BOOL        =      11
    SYM_BLD_BOOL_1      =      12
    SYM_BLD_PATH_CNSTR  =      13
    SYM_BLD_PATH_CNSTR_1=      14
    SYM_BLD_MEMCPY      =      15
    SYM_BLD_MEMCPY_1    =      16
    SYM_BLD_MEMSET      =      17
    SYM_BLD_MEMSET_1    =      18
    SYM_BLD_MEMMOVE     =      19
    SYM_BLD_MEMMOVE_1   =      20
    SYM_BLD_READ_MEM    =      21
    SYM_BLD_READ_MEM_1  =      22
    
    SYM_BLD_READ_MEM_HW  =      23
    SYM_BLD_READ_MEM_HW_1 =      24
    SYM_BLD_READ_MEM_W   =      25
    SYM_BLD_READ_MEM_W_1 =      26
    
    SYM_BLD_WRITE_MEM   =      27
    SYM_BLD_WRITE_MEM_1 =      28


    SYM_NTFY_PHI      = 29
    SYM_NTFY_PHI_1    = 30
    SYM_NTFY_CALL     = 31
    SYM_NTFY_FUNC     = 32
    SYM_NTFY_RET      = 33
    SYM_NTFY_BBLK     = 34
    SYM_NTFY_BBLK1    = 35
    SYM_INIT          = 36
    SYM_END          = 37


indent = 0

with open("data", "rb") as rfile:
    data = rfile.read()
msgs = []


def parsePackage(package):
    cur = 0
    global indent
    while cur < len(package):
        if(package[cur] == MsgTypes.SYM_BLD_INT_1):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = int.from_bytes(package[cur + 2: cur + 3],byteorder='little')
            cur += 3
            print("{}BuildInt 1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_1_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("{}BuildInt 1_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_2):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = int.from_bytes(package[cur + 2: cur + 4],byteorder='little')
            cur += 4
            print("{}BuildInt 2: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_2_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 5],byteorder='little')
            cur += 5
            print("{}BuildInt 2_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_4):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            cur += 6
            print("{}BuildInt 4: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_INT_4_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("{}BuildInt 4_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = struct.unpack('f',package[cur + 2: cur + 6])
            cur += 6
            print("{}Build Float: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = struct.unpack('f',package[cur + 3: cur + 7])
            cur += 7
            print("{}Build Float_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT_DBL):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = struct.unpack('d',package[cur + 2: cur + 10])
            cur += 10
            print("{}Build DBL: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_FLOAT_DBL_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = struct.unpack('d',package[cur + 3: cur + 11])
            cur += 11
            print("{}Build DBL_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_BOOL):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = int.from_bytes(package[cur + 2: cur + 3],byteorder='little')
            cur += 3
            print("{}Build bool: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_BOOL_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("{}Build bool_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_PATH_CNSTR):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            val = int.from_bytes(package[cur + 2: cur + 3],byteorder='little')
            cur += 3
            print("{}Push Constraint: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_PATH_CNSTR_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            val = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("{}Push Constraint_1: symval:{}, val:{}".format(indent * '\t',symid, val))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMCPY):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            dest = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            src = int.from_bytes(package[cur + 6: cur + 10],byteorder='little')
            length = int.from_bytes(package[cur + 10: cur + 12],byteorder='little')
            cur += 12
            print("{}MemCpy: symval:{}, dest:{}, src:{}, length:{}".format(indent * '\t',symid, hex(dest), hex(src), length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMCPY_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            dest = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            src = int.from_bytes(package[cur + 7: cur + 11],byteorder='little')
            length = int.from_bytes(package[cur + 11: cur + 13],byteorder='little')
            cur += 13
            print("{}MemCpy_1: symval:{}, dest:{}, src:{}, length:{}".format(indent * '\t',symid, hex(dest), hex(src), length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMSET):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            mem = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            length = int.from_bytes(package[cur + 6: cur + 8],byteorder='little')
            cur += 8
            print("{}MemSet: symval:{}, mem:{},  length:{}".format(indent * '\t',symid, hex(mem), length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMSET_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            mem = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            length = int.from_bytes(package[cur + 7: cur + 9],byteorder='little')
            cur += 9
            print("{}MemSet_1: symval:{}, mem:{},  length:{}".format(indent * '\t',symid, hex(mem), length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMMOVE):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            dest = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            src = int.from_bytes(package[cur + 6: cur + 10],byteorder='little')
            length = int.from_bytes(package[cur + 10: cur + 12],byteorder='little')
            cur += 12
            print("{}MemMove: symval:{}, dest:{}, src:{}, length:{}".format(indent * '\t',symid, hex(dest), hex(src), length))
        elif(package[cur] == MsgTypes.SYM_BLD_MEMMOVE_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            dest = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            src = int.from_bytes(package[cur + 7: cur + 11],byteorder='little')
            length = int.from_bytes(package[cur + 11: cur + 13],byteorder='little')
            cur += 13
            print("{}MemMove_1: symval:{}, dest:{}, src:{}, length:{}".format(indent * '\t',symid, hex(dest), hex(src), length))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            addr = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            cur += 6
            print("{}ReadMem: symval:{}, addr:{}".format(indent * '\t',symid,hex(addr)))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("{}ReadMem_1: symval:{}, addr:{}".format(indent * '\t',symid,hex(addr)))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM_HW):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            addr = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            val = int.from_bytes(package[cur + 6: cur + 8],byteorder='little')
            cur += 8
            print("{}ReadMem_HW: symval:{}, addr:{}, val:{}".format(indent * '\t',symid,hex(addr),val))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM_HW_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            val = int.from_bytes(package[cur + 7: cur + 9],byteorder='little')
            cur += 9
            print("{}ReadMem_HW_1: symval:{}, addr:{}, val:{}".format(indent * '\t',symid,hex(addr),val))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM_W):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            addr = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            val = int.from_bytes(package[cur + 6: cur + 10],byteorder='little')
            cur += 10
            print("{}ReadMem_W: symval:{}, addr:{},val:{}".format(indent * '\t',symid,hex(addr),val))
        elif(package[cur] == MsgTypes.SYM_BLD_READ_MEM_W_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            val = int.from_bytes(package[cur + 7: cur + 11],byteorder='little')
            cur += 11
            print("{}ReadMem_W_1: symval:{}, addr:{},val:{}".format(indent * '\t',symid,hex(addr),val))
        elif(package[cur] == MsgTypes.SYM_BLD_WRITE_MEM):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            addr = int.from_bytes(package[cur + 2: cur + 6],byteorder='little')
            cur += 6
            print("{}WriteMem: symval:{}, addr:{}".format(indent * '\t',symid,hex(addr)))
        elif(package[cur] == MsgTypes.SYM_BLD_WRITE_MEM_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            addr = int.from_bytes(package[cur + 3: cur + 7],byteorder='little')
            cur += 7
            print("{}WriteMem: symval:{}, addr:{}".format(indent * '\t',symid,hex(addr)))
        elif(package[cur] == MsgTypes.SYM_NTFY_PHI):
            symid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            branchNo = int.from_bytes(package[cur + 2: cur + 3],byteorder='little')
            cur += 3
            print("{}NotifyPhi: symval:{}, phi branch:{}".format(indent * '\t',symid,branchNo))
        elif(package[cur] == MsgTypes.SYM_NTFY_PHI_1):
            symid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            branchNo = int.from_bytes(package[cur + 3: cur + 4],byteorder='little')
            cur += 4
            print("{}NotifyPhi: symval:{}, phi branch:{}".format(indent * '\t',symid,branchNo))
        elif(package[cur] == MsgTypes.SYM_NTFY_CALL):
            callId = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("{}NotifyCall: call Id:{}".format(indent * '\t',callId))
            indent += 1
        elif(package[cur] == MsgTypes.SYM_NTFY_FUNC):
            funcid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("{}NotifyFunc: func id:{}".format(indent * '\t',funcid))
        elif(package[cur] == MsgTypes.SYM_NTFY_RET):
            retId = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            indent -= 1
            print("{}NotifyRet: ret id:{}".format(indent * '\t',retId))
        elif(package[cur] == MsgTypes.SYM_NTFY_BBLK):
            bbid = int.from_bytes(package[cur + 1: cur + 2],byteorder='little')
            cur += 2
            print("{}NotifyBB: bb id:{}".format(indent * '\t',bbid))
        elif(package[cur] == MsgTypes.SYM_NTFY_BBLK1):
            bbid = int.from_bytes(package[cur + 1: cur + 3],byteorder='little')
            cur += 3
            print("{}NotifyBB1: bb id:{}".format(indent * '\t',bbid))
        elif(package[cur] == MsgTypes.SYM_INIT):
            addr  = int.from_bytes(package[cur + 1: cur + 5],byteorder='little')
            cur += 5
            print("{}SymInit: addr:{}".format(indent * '\t',hex(addr)))
        elif(package[cur] == MsgTypes.SYM_END):
            cur += 1
            print("SymEnd")
            assert(indent == 0)
        else:
            embed()
            assert(False)
cur = 0
size = len(data)
print("total len:{}".format(size))
while cur < size:
    package_len = data[cur]
    payload_len = package_len - 1
    package = data[cur + 1 : cur + 1 +  payload_len]

    parsePackage(package)
    cur =  cur +  package_len
    #print("{}/{}".format(cur,size),end=' ')