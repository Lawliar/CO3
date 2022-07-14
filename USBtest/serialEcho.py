#!/usr/bin/env python3
import serial
from struct import *
import sys,os

args = sys.argv
if(len(args) == 2 ):
    assert(os.path.exists(args[1]))

input_data = b'' 
with open(args[1],"rb") as rfile:
    input_data = rfile.read()

ser = serial.Serial('/dev/ttyACM1')
ser.timeout = 2
print('timeout: {}'.format(ser.timeout))

if(len(input_data) == 0):
    data = b'hello serial!'
else:
    data = input_data
inputLen = len(data)
data =  pack('B', inputLen) + data

ser.write(data)
frameNum = 1024
r = b''
while True:
    tmp_r = ser.read(frameNum)
    r += tmp_r
    if(len(tmp_r) != frameNum):
        break
print(len(r))
f=open("data",'wb')
f.write(r)
f.close()
ser.close()             # close port
