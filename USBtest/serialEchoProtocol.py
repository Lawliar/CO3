#!/usr/bin/env python3
import struct
import serial
from struct import *
import zlib
import sys
import os
import ast
from IPython import embed


def main(argv):

    data= b''
 
    payloads = [
    b'Testing normal execution' ,                       # 0  Normal execution
    b'SEG I will catch you!!',                          # 1  triggers a segmementation fault in global variable (MPU) 
    b'HANG you are so slow',                            # 2  triggers timeout (FreeRTOS) 
    b'UAF this is a use after free',                    # 3  triggers use after free  (ASAN)
    b'2FR this is a double free',                       # 4  triggers double free (detected as invalid free) (ASAN)
    b'IFR this is an invalid free',                     # 5  invalid free (ASAN)
    b'OFS \4 buffer overflow stack',                    # 6  overflow stack (ASAN)
    b'OFH buffer overflow HEAP',                        # 7  overflow heap (ASAN)
    b'ZER0 \0 division by zero test',                   # 8  triggers a division by zero     (Cortex-M)
    b'UNAL \3 unaligned halfword access',               # 9  triggers unnaligned access      (Cortex-M)
    b'UDF undefined instruction',                       # 10 triggers undefined instruction  (Cortex-M)
    b'NULL de-reference on local variable',             # 11 triggers Null dereference   (MPU) 
    b'\x01\x03\x00\x00\x00\x02\xc4\x0b',                # 12 modbus read holding registers from address 0 2 registers 
    b'\x01\x06\x00\x80\x00\x00\x88\x1e',                # 13 modbus write single register from address 128  registers -> overflows by one byte
    b'\x01\x03\x00\x00\x00\x7e\x85\xeb',                # 14 modbus read holding registers from address 126 registers -> overflows by one byte
    b'\x81\x03\x04\x00\x00\x7e\x85\xeb',                # 15 MIDI crash
    b'\r\n+CMD:1',                                      # 16 AT parser
    b'\r\n+CMD:2',                                      # 17 AT parser
    b'\r\n+CMD:3',                                      # 18 AT parser
    b'\r\n+CMD:4',                                      # 19 AT parser
    b'\x01\x02\x01\x01\x01\x01\x04\x04',                # 20
    b'\x01'*48 + b'\x02'*48 + b'\xFF'*160
    ]


    returncodeStr = {
    '0':'FAULT_NONE',
    '1':'FAULT_TMOUT',
    '2':'FAULT_CRASH',
    '3':'FAULT_ERROR',
    '4':'FAULT_NOINST',
    '5':'FAULT_NOBITS',
    '6':'FAULT_COMM',
    '7':'FAULT_INLEGTH',
    '8':'FAULT_NONE_RTOS',
    '9':'FAULT_ASAN'
    }


    crashing_input_filename = "./hangs_inputs.txt"
    buggy_payloads = []
    if os.path.exists(crashing_input_filename):
        with open(crashing_input_filename,"r") as rfile:
            for each_buggy_input in rfile.readlines():
                each_buggy_input = each_buggy_input.strip()
                a = ast.literal_eval(each_buggy_input)
                buggy_payloads.append(a)

    print("{} buggy inputs".format(len(buggy_payloads)))
    payload = (payloads[int(argv[0])])
    if len(argv) == 2:
        payload = buggy_payloads[int(argv[1])]


    inputLen = len(payload)
    data =  pack('B', inputLen) + payload
    print("payload : {}".format(payload))
    print("Frame len: {}".format(len(data)))

    # add padding  if necessary


    ser = serial.Serial('/dev/ttyACM1')

    # write the test input to the board

    print("Total length TX: {}".format(len(data)))
    ser.write(data)

    ser.close() 


   


if __name__ == "__main__":
   main(sys.argv[1:])

   
