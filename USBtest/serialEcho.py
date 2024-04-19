#!/usr/bin/env python3

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

import serial
from struct import *
import sys,os
import argparse
import time
from displayCodeCoverage import parsePackage

MAX_INPUT_LEN = 1024

plaintext_input = b'hello serial!'


def bb_send_receive(data,rate,port):
    inputLen = len(data) + 4
    data =  pack('<I', inputLen) + data
    parse_start = True
    result = []
    ser =  serial.Serial(port,baudrate=rate)
    ser.write(data)
    while(True):
        pack_len = ser.read(1)[0]
        pack_len = pack_len - 1
        package = ser.read(pack_len)
        try:
            bbs, ended = parsePackage(package,parseStart=parse_start)
            result += bbs
            if(ended):
                ser.close()
                return result
        except:
            print("parsing wrong")
            assert(False)
        parse_start = False
        
def send_receive(data,rate,port):
    inputLen = len(data) + 4
    data =  pack('<I', inputLen) + data
    with open("input_data","wb") as wfile:
        wfile.write(data)
    
    with serial.Serial(port,baudrate=rate) as ser:
        ser.timeout = 5
        print('timeout: {}'.format(ser.timeout))
        print("msg len:{}".format(len(data)))
        print("sent data:",data)
        ser.write(data)
        frameNum = 1024
        r = b''
        while True:
            tmp_r = ser.read(frameNum)
            r += tmp_r
            if(len(tmp_r) != frameNum):
                break
        print(len(r))
        with open("data",'wb') as wfile:
            wfile.write(r)
    return r

def send(data,rate,port):
    inputLen = len(data) + 4
    data =  pack('<I', inputLen) + data

    #with open("input_data","wb") as wfile:
    #    wfile.write(data)

    with serial.Serial(port,baudrate=rate) as ser:
        ser.timeout = 5
        ser.write(data)

def prepare_data(input_file):
    assert(os.path.exists(input_file))
    with open(input_file,"rb") as rfile:
        input_data = rfile.read()

    if(len(input_data) > MAX_INPUT_LEN):
        input_data = input_data[:MAX_INPUT_LEN]

    return input_data

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p","--port",type=str,required=False, help="Serial port for feeding input",default="/dev/ttyACM1")
    parser.add_argument("-b","--baud", type=int,required=False, help="Baud rate for the specified serial port",default=115200)
    parser.add_argument("-i","--input_file",type=str,required=False,help="if you want to feed a binary file instead of the default string", default="")
    args = parser.parse_args()
    if(args.input_file != "" ):
        input_data = prepare_data(args.input_file)
    else:
        input_data = plaintext_input
    send_receive(input_data,args.baud,args.port)
    
if __name__ == "__main__":
    main()
