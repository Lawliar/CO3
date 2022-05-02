#!/usr/bin/env python3
import serial

ser = serial.Serial('/dev/ttyACM1')
data = b'hello serial!'
ser.write(data)
r = ser.read(len(data))
print(r)
ser.close()             # close port
