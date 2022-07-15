#!/usr/bin/env python3
import serial
from struct import *
import sys,os

MAX_INPUT_LEN = 1024

plaintext_input = b'hello serial!'

args = sys.argv
input_data = b'' 
if(len(args) == 2 ):
    assert(os.path.exists(args[1]))
    with open(args[1],"rb") as rfile:
        input_data = rfile.read()

if(len(input_data) > 1024):
    input_data = input_data[:1024]

if(len(input_data) == 0):
    data = plaintext_input
else:
    data = input_data

inputLen = len(data)
data =  pack('<I', inputLen) + data

with open("input_data","wb") as wfile:
    wfile.write(data)

with serial.Serial('/dev/ttyACM1') as ser:
    ser.timeout = 2
    print('timeout: {}'.format(ser.timeout))

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
