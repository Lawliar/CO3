import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import shutil
import time
import re

from conf import benchmark,time_budget, serial_port, zfill_len, numExecution, get_highest_id, get_total_time_out_err,sleep_time, timeout
    
def runSpear(benchmark):

    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results".format(benchmark)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)
    spear_output_dir          = "{}/output".format(spear_inter_dir)
    spear_tmp_output_dir      = "{}/tmp_out".format(spear_output_dir)
    spear_backend_executable = "/home/lcm/github/spear/spear-code/sym_backend/build_release/qsym_backend/orchestrator"
    
    if(os.path.exists(spear_output_dir)):
        shutil.rmtree(spear_output_dir)
    os.mkdir(spear_output_dir)
    if(os.path.exists(spear_tmp_output_dir)):
        shutil.rmtree(spear_tmp_output_dir)
    os.mkdir(spear_tmp_output_dir)
    
    assert(os.path.exists(concrete_input))
    shutil.copyfile(concrete_input, os.path.join(spear_output_dir,os.path.basename('0'.zfill(zfill_len))))

    spear_input_cur_id = 0
    spear_output_cur_id = 1

    spear_start_time = time.time()
    spear_break = False

    spear_total_time = 0
    it = 0
    while (True):
        ## run Spear
        batch_size = len(range(spear_input_cur_id, spear_output_cur_id))
        for cur_spear_input_id in range(spear_input_cur_id, spear_output_cur_id):
            spear_input_file = os.path.join(spear_output_dir, str(cur_spear_input_id).zfill(zfill_len))
            if(not os.path.exists(spear_input_file)):
                assert(os.path.exists(spear_input_file + '-optimistic'))
                spear_input_file += '-optimistic'
            cmd = [spear_backend_executable,"-i",spear_inter_dir,"-s",serial_port,"-b",str(10000000)]
            p1 = subprocess.Popen(cmd, \
                         stdout=subprocess.PIPE,stderr=subprocess.PIPE, \
                        env={**os.environ,'SYMCC_INPUT_FILE':spear_input_file, 'SYMCC_OUTPUT_DIR': spear_tmp_output_dir})
            
            try:
                p1.wait(timeout)
            except subprocess.TimeoutExpired:
                print("time out, something is wrong, sleep for sometime")
                p1.kill()
                time.sleep(sleep_time)
                spear_total_time += timeout + sleep_time
                continue
            _, e = p1.communicate()
            try:
                spear_total_time += get_total_time_out_err(e) / 1000000
            except IndexError as err:
                print("program did not end well")
                embed()
            
            print("iter:{}, cur:{}/{},total:{},time:{} / {}".format(it, cur_spear_input_id - spear_input_cur_id , batch_size,cur_spear_input_id,spear_total_time, time_budget))
            tmp_output_id = get_highest_id(spear_output_dir) + 1
            print("copying spear generated {} inputs...".format(len(os.listdir(spear_tmp_output_dir))))
            for each_spear_output in os.listdir(spear_tmp_output_dir):
                src_file = os.path.join(spear_tmp_output_dir, each_spear_output)
                dest_file_name = str(tmp_output_id).zfill(zfill_len)
                tmp_output_id += 1
                if("-optimistic" in each_spear_output):
                    dest_file_name += "-optimistic"
                shutil.copyfile(src_file, os.path.join(spear_output_dir, dest_file_name))
            ## clean up tmp output
            shutil.rmtree(spear_tmp_output_dir)
            os.mkdir(spear_tmp_output_dir)
            #if cur_spear_input_id >= numExecution:
            if spear_total_time >= time_budget:
                spear_break = True
                break
            it += 1
        spear_input_cur_id = spear_output_cur_id
        spear_output_cur_id = get_highest_id(spear_output_dir) + 1
        if spear_break:
            break
    shutil.rmtree(spear_tmp_output_dir)
    return spear_output_cur_id,cur_spear_input_id, spear_total_time

def main():
    
    spear_output_num, spear_input_num,spear_total_time = runSpear(benchmark)
    print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,spear_total_time))

if __name__ == '__main__':
    main()