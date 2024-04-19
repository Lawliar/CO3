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

import os,re,math,subprocess
from IPython import embed

benchmark =  "CROMU_00001"
NO_SHADOW = False
NO_REPLACE = True
time_budget = 60 * 60 * 24 # in seconds
serial_port = os.path.join("/","dev","ttyACM1")
bb_serial_port = os.path.join("/","dev","ttyACM3")
tcp_port = 3002
zfill_len = 12
numExecution = 1000
timeout = 10
sleep_time = 10
baud_rate = 75000000
SER2NET = False
FW_COVERAGE = False

coverage_dir = "/home/lcm/github/spear/spear-code/code_coverage/{}".format(benchmark)

if not FW_COVERAGE:
    coverage_build_dir = os.path.join(coverage_dir,"build")
    coverage_executable = os.path.join(coverage_build_dir,"out")

    assert(os.path.exists(coverage_dir))
    assert(os.path.exists(coverage_build_dir))
    assert(os.path.exists(coverage_executable))



import sys
sys.path.append("../USBtest")

from serialEcho import send, bb_send_receive,prepare_data


import displayCodeCoverage


def get_highest_id(input_dir):
    ## this aggegates all Qsym generated file, and get the highest id
    highest = 0
    for each_input_file in os.listdir(input_dir):
        if('tmp' in each_input_file):
            continue
        
        digits = int(each_input_file[:zfill_len],10)
        
        if highest < digits:
            highest = digits
    return highest

def estimate_inputs_needed(num_execute, time_cost, time_budget):
    inputs_needed = (num_execute / time_cost) * time_budget * 1.3 ## some buffer
    return int(inputs_needed)



def get_total_time_out_err(input):
    splitted = input.split(b'\n')
    cur = len(splitted) - 1
    if(cur == 0 and splitted[0] == b''):
        return 0
    while True:
        try:
            s = splitted[cur].decode("ascii")
        except IndexError as e :
            raise e
        if("total_time" in s):
            total_time = re.search('''total_time": (\d+)''', s).group(1)
            return int(total_time)
        cur -= 1

def process_co3_output(input):
    splitted = input.split(b'\n')
    cur = len(splitted) - 1
    building_time = 0
    receiving_time = 0
    num_bytes = 0
    while True:
        if(cur < 0):
            return building_time, receiving_time, num_bytes
        try:
            s = splitted[cur].decode("ascii")
        except IndexError as e :
            raise e
        if("Orchestrator processing time" in s):
            t = re.search('''Orchestrator processing time:(\d+)''', s).group(1)
            building_time = int(t)
        elif("receiving time costs:" in s):
            t = re.search('''receiving time costs:(\d+)''',s).group(1)
            receiving_time = int(t)
        elif("Bytes transmitted:" in s):
            t = re.search('''Bytes transmitted:(\d+)''',s).group(1)
            num_bytes = int(t)
        if(building_time != 0 and receiving_time != 0 and num_bytes != 0):
            break
        cur -= 1
    assert(building_time != 0)
    assert(receiving_time != 0)
    return building_time, receiving_time, num_bytes

def convert_size(size_bytes):
   if size_bytes == 0:
       return "0B"
   size_name = ("B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB")
   i = int(math.floor(math.log(size_bytes, 1024)))
   p = math.pow(1024, i)
   s = round(size_bytes / p, 2)
   return "%s %s" % (s, size_name[i])

def fw_coverage_worker(input_file):
    input_data = prepare_data(input_file)
    parsed_bb =  bb_send_receive(input_data, baud_rate, bb_serial_port)
    return parsed_bb

def single_coverage_worker(input_file):
    os.path.exists(input_file)
    assert(os.path.isabs(input_file))
    p1 = subprocess.Popen(["cat", input_file], stdout=subprocess.PIPE)
    exit_status = p1.wait()
    if(exit_status != 0):
        print("cat failed")
        embed()
    p2 = subprocess.run([coverage_executable], stdin=p1.stdout, stdout=subprocess.PIPE)
    lines = p2.stdout.decode('utf-8')
    for eachline in lines.split('\n'):
        if(not eachline.startswith('cc:')):
            continue
        eachline = eachline.strip()[3:]
        yield eachline
