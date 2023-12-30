#!/usr/bin/env python3
import serial
from struct import *
import sys,os
import argparse
import time

MAX_INPUT_LEN = 1024

plaintext_input = b'hello serial!'

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
    parser.add_argument("-p","--port",type=str,required=False, help="Serial port for feeding input",default="/dev/ttyACM1")
    parser.add_argument("-b","--baud", type=int,required=False, help="Baud rate for the specified serial port",default=115200)
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
    send_receive(data,args.baud,args.port)
    
if __name__ == "__main__":
    main()
