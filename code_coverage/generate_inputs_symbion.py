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


from conf import time_budget, zfill_len,get_highest_id,convert_size,fw_coverage_worker,coverage_dir,estimate_inputs_needed


benchmark = "ModbusDMA"
import sys

symbion_dir = "../symbion_concolic"
sys.path.append(symbion_dir)

#import testPLC as test_mod
import testModbusDMA as test_mod

import os,shutil,time
from IPython import embed
import sys
import threading
import json

class RunWithTimeout(object):
    def __init__(self, function, args):
        self.function = function
        self.args = args
        self.answer = None

    def worker(self):
        self.answer = self.function(*self.args)

    def run(self, timeout):
        thread = threading.Thread(target=self.worker)
        thread.start()
        thread.join(timeout)
        return self.answer

def runSymbion(benchmark):
    ## getting the input
    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Symbion{}/intermediate_results".format(benchmark)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)

     

    symbion_bm_dir                = os.path.join( symbion_dir, benchmark) 
    output_dir          = "{}/output".format(symbion_bm_dir)
    tmp_output_dir      = "{}/tmp_out".format(symbion_bm_dir)
    
    coverage_file            = os.path.join(coverage_dir,"symbion.json")

    if(os.path.exists(symbion_bm_dir)):
        shutil.rmtree(symbion_bm_dir)
    os.mkdir(symbion_bm_dir)
    os.mkdir(output_dir)
    os.mkdir(tmp_output_dir)

    assert(os.path.exists(concrete_input))

    shutil.copyfile(concrete_input, os.path.join(output_dir, os.path.basename('0'.zfill(zfill_len))))

    batch_input_id_start = 0
    batch_input_id_end = 1

    total_time = 0
    trans_time = 0
    totalNumBytes = 0
    coverage = {}
    symbion_break = False
    while(True):
        for cur_input_id in range(batch_input_id_start, batch_input_id_end):
            if(total_time >=  time_budget):
                symbion_break = True
                break
            symbion_input_file = os.path.join(output_dir, str(cur_input_id).zfill(zfill_len))
            with open(symbion_input_file,"rb") as wfile:
                concrete_input  = wfile.read()

            repeat = False
            t = RunWithTimeout(test_mod.runSymbion, (concrete_input, tmp_output_dir))
            answer = t.run(40)
            if(answer == None):
                repeat  = True
            if(repeat == True):
                continue
            
            cur_total_time, cur_trans_time, numBytes = answer
            total_time += cur_total_time
            trans_time += cur_trans_time
            totalNumBytes += numBytes
            print("cur: {},  transmit {} costs:{:.2f} , total time:{:.2f} / {}".format(cur_input_id, convert_size(totalNumBytes), trans_time, total_time, time_budget))


            tmp_output_id = get_highest_id(output_dir) + 1
            new_edges = 0
            for each_symbion_output in os.listdir(tmp_output_dir):
                src_file = os.path.join(tmp_output_dir, each_symbion_output)
                found_new_edge = False
                bbs = fw_coverage_worker(src_file)
                for each_covered_edge in bbs:
                    if(each_covered_edge not in coverage):
                        coverage[each_covered_edge] = total_time
                        found_new_edge = True
                        new_edges += 1
                if found_new_edge:
                    with open(coverage_file,"w") as wfile:
                        json.dump(coverage, wfile)
                if(tmp_output_id >= estimate_inputs_needed(cur_input_id + 1, total_time, time_budget)):
                    continue ## not copying
                dest_file_name = str(tmp_output_id).zfill(zfill_len)
                tmp_output_id += 1
                shutil.copyfile(src_file, os.path.join(output_dir, dest_file_name))
            print("symbion generated {} inputs, {} new edge found\n".format(len(os.listdir(tmp_output_dir)), new_edges))
            shutil.rmtree(tmp_output_dir)
            os.mkdir(tmp_output_dir)
        batch_input_id_start = batch_input_id_end
        batch_input_id_end = get_highest_id(output_dir) + 1
        if symbion_break:
            break
    shutil.rmtree(tmp_output_dir)
    return batch_input_id_end,cur_input_id, total_time
def main():

    symbion_output_num, symbion_input_num,symbion_total_time = runSymbion(benchmark)
    #spear_output_num, spear_input_num,spear_total_time = runSpear(benchmark)
    print("symbion generate:{} with {} runs using {}us\n".format(symbion_output_num, symbion_input_num,symbion_total_time))
    #print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,spear_total_time))

if __name__ == '__main__':
    main()