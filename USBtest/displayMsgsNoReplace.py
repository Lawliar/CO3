# This file is part of CO3.
#
# CO3 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# CO3. If not, see <https://www.gnu.org/licenses/>.


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

    SYM_NTFY_SELECT   = 36
    SYM_NTFY_SELECT_1 = 37

    SYM_TRY_ALT       = 38
    SYM_TRY_ALT_1     = 39

    SYM_INIT          = 40
    SYM_INIT_DR       = 41
    SYM_END           = 42


indent = 0

with open("data", "rb") as rfile:
    data = rfile.read()
msgs = []


def parsePackage(package):
    cur = 0
    global indent
    while cur < len(package):
        if(package[cur] == MsgTypes.SYM_BLD_INT_1):
        
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