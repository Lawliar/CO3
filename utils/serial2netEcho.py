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
from IPython import embed
import socket 
import select

MAX_INPUT_LEN = 1024

plaintext_input = b'hello serial!'


def protocol_recv(port):
    buf = b''
    HEADER_LEN = 1
    r = port.recv(HEADER_LEN)
    packet_len = int.from_bytes(r, "little")
    if(packet_len == 1):
        return r
    buf += r
    to_read = packet_len - HEADER_LEN
    cur = 0
    while (cur < to_read):
        r = port.recv(packet_len - HEADER_LEN - cur)
        buf += r
        cur += len(r)
    return buf
def send_receive(data,port):

    inputLen = len(data) + 4
    data =  pack('<I', inputLen) + data

    with open("input_data","wb") as wfile:
        wfile.write(data)
    
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
        s.settimeout(5)
        s.connect(("localhost",3002))
        print("msg len:{}".format(len(data)))
        print("sent data:",data)
        s.send(data)
        frameNum = 1024
        r = b''
        while True:
            try:
                r += protocol_recv(s)
            except TimeoutError as e:
                break
        print(len(r))
        with open("data",'wb') as wfile:
            wfile.write(r)

def send(data,rate,port):
    inputLen = len(data) + 4
    data =  pack('<I', inputLen) + data

    #with open("input_data","wb") as wfile:
    #    wfile.write(data)

    with serial.Serial(port,baudrate=rate) as ser:
        ser.timeout = 5
        ser.write(data)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p","--port",type=str,required=False, help="Serial port for feeding input",default=3002)
    parser.add_argument("-i","--input_file",type=str,required=False,help="if you want to feed a binary file instead of the default string", default="")
    args = parser.parse_args()
    input_data = b'' 
    if(args.input_file != "" ):
        assert(os.path.exists(args.input_file))
        with open(args.input_file,"rb") as rfile:
            input_data = rfile.read()

    if(len(input_data) > MAX_INPUT_LEN):
        input_data = input_data[:MAX_INPUT_LEN]

    if(len(input_data) == 0):
        data = plaintext_input
    else:
        data = input_data
    send_receive(data,args.port)
    
if __name__ == "__main__":
    main()
