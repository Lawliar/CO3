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


from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import threading
import time
import struct



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
    counter = 0
    for eachfile in tqdm(os.listdir(input_dir)):
        input_file = os.path.join(input_dir, eachfile)
        concrete_input = None
        with open(input_file,"rb") as rfile:
            concrete_input = rfile.read()
        inputLen = len(concrete_input) + 4
        concrete_input =  struct.pack('<I', inputLen) + concrete_input

        
        for bb in parsed_bbs:
            ret.add(bb)
        counter += 1
        if counter == 10000:
            break
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
    runTest("Steering_Control")
if __name__ == '__main__':
    main()
