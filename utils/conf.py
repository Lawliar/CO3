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
from serialEcho import send, bb_send_receive,prepare_data


benchmark =  "CROMU_00001"
time_budget = 60 * 24 # in seconds
serial_port = os.path.join("/","dev","ttyACM1")
bb_serial_port = os.path.join("/","dev","ttyACM3")
zfill_len = 12
timeout = 10
sleep_time = 5


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

def estimate_inputs_needed(num_execute, time_cost, time_budget):
    inputs_needed = (num_execute / time_cost) * time_budget * 1.3 ## some buffer
    return int(inputs_needed)