#!/usr/bin/env python3
import serial
from struct import *

ser = serial.Serial('/dev/ttyACM1')
ser.timeout = 1
print('timeout: {}'.format(ser.timeout))

data = b'hello serial!'
inputLen = len(data)
data =  pack('B', inputLen) + data

ser.write(data)
r = ser.read(240)
print(r)
f=open("data",'wb')
f.write(r)
f.close()
ser.close()             # close port
