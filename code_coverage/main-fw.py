from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import threading
import time
import struct

import sys
sys.path.append("../USBtest")

from serialEcho import send

import displayCodeCoverage

import serial

dbg_print = False

    

def runOne(input_dir):
    '''
    Kill and spawn a new thread for each execution somehow causes problem
    although given that it is well sequantialized
    , so I guess I'll just let the listening thread keep running
    '''
    #global bbs
    global dbg_print
    ret = set()
    for eachfile in tqdm(os.listdir(input_dir)):
        input_file = os.path.join(input_dir, eachfile)
        concrete_input = None
        with open(input_file,"rb") as rfile:
            concrete_input = rfile.read()
        inputLen = len(concrete_input) + 4
        concrete_input =  struct.pack('<I', inputLen) + concrete_input

        broken = False
        with serial.Serial('/dev/ttyACM1',baudrate=1000000) as ser:
            ser.timeout = 0.1
            ser.write(concrete_input)
            r = b''
            while True:
                p_len = ser.read(1)
                if(len(p_len) != 1):
                    break
                r += p_len
                content_len = int.from_bytes(p_len,byteorder='little') - 1
                p_content = ser.read(content_len)
                if(len(p_content) != content_len):
                    break
                r += p_content
                if r[-1] == displayCodeCoverage.MsgTypes.SYM_END and r[-2] == 10:
                    broken = True
                    break
        parsed_bbs = displayCodeCoverage.main(r)
        for bb in parsed_bbs:
            ret.add(bb)
        ## reset the bbs
        
        
    return ret
def runTest(benchmark):
    input_dir = "/home/lcm/github/spear/spear-code/code_coverage/CodeCoverage{}".format(benchmark)
    symbion_input = "/home/lcm/github/spear/spear-code/symbion_concolic/{}/output".format(benchmark)
    spear_input = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results/output".format(benchmark)
    
    assert(os.path.exists(input_dir))
    assert(os.path.exists(spear_input))

    #listening_thread = threading.Thread(target=listen, args=("/dev/ttyACM0", 1000000))
    #listening_thread.start()

    symbion_bb = runOne(symbion_input)
    spear_bb = runOne(spear_input)

    with open(os.path.join(input_dir,"result.txt"),"w") as wfile:
        wfile.write("symbion edges, {}:{}".format(len(symbion_bb),symbion_bb))
        wfile.write("spear edges, {}:{}".format(len(spear_bb),spear_bb))
    print("symbion covered edges:{}\n".format(len(symbion_bb)))
    print("spear covered edges:{}\n".format(len(spear_bb)))
    
def main():
    runTest("PLC")
if __name__ == '__main__':
    main()
