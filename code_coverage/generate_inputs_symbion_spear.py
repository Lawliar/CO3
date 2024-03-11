from conf import time_budget, zfill_len,get_highest_id,convert_size


benchmark = "PLC"
import sys

symbion_dir = "../symbion_concolic"
sys.path.append(symbion_dir)
import testPLC

import os,shutil,time
from IPython import embed
import sys
import threading


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
    symbion_output_dir          = "{}/output".format(symbion_bm_dir)
    symbion_tmp_output_dir      = "{}/tmp_out".format(symbion_bm_dir)
    


    if(os.path.exists(symbion_bm_dir)):
        shutil.rmtree(symbion_bm_dir)
    os.mkdir(symbion_bm_dir)
    os.mkdir(symbion_output_dir)
    os.mkdir(symbion_tmp_output_dir)

    assert(os.path.exists(concrete_input))

    shutil.copyfile(concrete_input, os.path.join(symbion_output_dir, os.path.basename('0'.zfill(zfill_len))))

    symbion_input_cur_id = 0
    symbion_output_cur_id = 1
    symbion_start_time = time.time()
    symbion_break = False

    total_time = 0
    trans_time = 0
    totalNumBytes = 0
    while(True):
        batch_size = len(range(symbion_input_cur_id, symbion_output_cur_id))
        cur_symbion_input_id = symbion_input_cur_id
        while True:
            if(cur_symbion_input_id >= symbion_output_cur_id):
                break
            if(total_time >=  time_budget):
                symbion_break = True
                break
            symbion_input_file = os.path.join(symbion_output_dir, str(cur_symbion_input_id).zfill(zfill_len))
            with open(symbion_input_file,"rb") as wfile:
                concrete_input  = wfile.read()
            
            repeat = False
            t = RunWithTimeout(testPLC.runSymbion, (concrete_input, symbion_tmp_output_dir))
            answer = t.run(40)
            if(answer == None):
                repeat  = True
            if(repeat == True):
                continue
            
            cur_total_time, cur_trans_time, numBytes = answer
            total_time += cur_total_time
            trans_time += cur_trans_time
            totalNumBytes += numBytes
            print("cur: {},  transmit {} costs:{:.2f} , total time:{:.2f} / {}".format(cur_symbion_input_id, convert_size(totalNumBytes), trans_time, total_time, time_budget))
            
            if(repeat):
                continue
            
            tmp_output_id = get_highest_id(symbion_output_dir) + 1
            print("copying symbion generated {} inputs...".format(len(os.listdir(symbion_tmp_output_dir))))
            for each_symbion_output in os.listdir(symbion_tmp_output_dir):
                src_file = os.path.join(symbion_tmp_output_dir, each_symbion_output)
                dest_file_name = str(tmp_output_id).zfill(zfill_len)
                tmp_output_id += 1
                shutil.copyfile(src_file, os.path.join(symbion_output_dir, dest_file_name))
            shutil.rmtree(symbion_tmp_output_dir)
            os.mkdir(symbion_tmp_output_dir)
            
            ## successfully execute one input, move on to the next one
            cur_symbion_input_id += 1 ## increament
        symbion_input_cur_id = symbion_output_cur_id
        symbion_output_cur_id = get_highest_id(symbion_output_dir) + 1
        if symbion_break:
            break
    shutil.rmtree(symbion_tmp_output_dir)
    return symbion_output_cur_id, cur_symbion_input_id, time.time() - symbion_start_time
def main():

    symbion_output_num, symbion_input_num,symbion_total_time = runSymbion(benchmark)
    #spear_output_num, spear_input_num,spear_total_time = runSpear(benchmark)
    print("symbion generate:{} with {} runs using {}us\n".format(symbion_output_num, symbion_input_num,symbion_total_time))
    #print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,spear_total_time))

if __name__ == '__main__':
    main()