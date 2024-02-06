import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import shutil
import time
import re

from conf import benchmark,time_budget, serial_port, zfill_len, numExecution, get_highest_id, get_total_time_out_err,sleep_time, timeout



def runSymcc(benchmark):
    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results".format(benchmark)
    symcc_dir                = "/home/lcm/github/spear/spear-code/symcc_benchmark/shared_volume/{}".format(benchmark)
    symcc_output_dir          = "{}/output".format(symcc_dir)
    symcc_tmp_output_dir      = "{}/tmp_out".format(symcc_output_dir)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)
    symcc_executable         = "{}/build/out".format(symcc_dir)
    


    if(os.path.exists(symcc_output_dir)):
        shutil.rmtree(symcc_output_dir)
    os.mkdir(symcc_output_dir)
    
    if(os.path.exists(symcc_tmp_output_dir)):
        shutil.rmtree(symcc_tmp_output_dir)
    os.mkdir(symcc_tmp_output_dir)
    
    assert(os.path.exists(concrete_input))
    shutil.copyfile(concrete_input, os.path.join(symcc_output_dir,os.path.basename('0'.zfill(zfill_len))))
    
    symcc_input_cur_id = 0
    symcc_output_cur_id = 1
    
    
    symcc_start_time = time.time()
    symcc_break = False

    symcc_total_time = 0
    it = 0
    while (True):
        batch_size = len(range(symcc_input_cur_id, symcc_output_cur_id))
        for cur_symcc_input_id in range(symcc_input_cur_id, symcc_output_cur_id):
            symcc_input_file = os.path.join(symcc_output_dir, str(cur_symcc_input_id).zfill(zfill_len))
            if( not os.path.exists(symcc_input_file)):
                assert(os.path.exists(symcc_input_file + '-optimistic'))
                symcc_input_file += '-optimistic'
            
            p1 = subprocess.Popen([symcc_executable],stdin=open(symcc_input_file),stdout=subprocess.PIPE,stderr=subprocess.PIPE,
                                env={**os.environ, 'SYMCC_OUTPUT_DIR': symcc_tmp_output_dir})
            try:
                p1.wait()
            except subprocess.TimeoutExpired:
                print("time out, something is wrong, sleep for sometime")
                p1.kill()
                time.sleep(sleep_time)
                symcc_total_time += timeout + sleep_time
                continue
            _, e = p1.communicate()
            try:
                symcc_total_time += get_total_time_out_err(e) / 1000000
            except IndexError as err:
                print("Symcc did not end well")
                embed()

            print("iter:{},cur: {} / {},total:{},time:{} / {}".format(it, cur_symcc_input_id - symcc_input_cur_id ,batch_size ,cur_symcc_input_id, symcc_total_time , time_budget))
            ## copy the newly generated file to other places
            tmp_output_id = get_highest_id(symcc_output_dir) + 1
            print("copying symcc generated {} inputs...".format(len(os.listdir(symcc_tmp_output_dir))))
            for each_symcc_output in os.listdir(symcc_tmp_output_dir):
                src_file = os.path.join(symcc_tmp_output_dir, each_symcc_output)
                ## copy the newly generated file into the output folder for archive
                dest_file_name = str(tmp_output_id).zfill(zfill_len) 
                tmp_output_id += 1
                if("-optimistic" in each_symcc_output):
                    dest_file_name += "-optimistic"
                shutil.copyfile(src_file, os.path.join(symcc_output_dir, dest_file_name))
            ## clean the tmp output dir
            shutil.rmtree(symcc_tmp_output_dir)
            os.mkdir(symcc_tmp_output_dir)
            #if cur_symcc_input_id >= numExecution:
            if symcc_total_time >= time_budget:
                symcc_break = True
                break
        ## update input_cur_id and output_cur_id
        symcc_input_cur_id = symcc_output_cur_id
        symcc_output_cur_id = get_highest_id(symcc_output_dir) + 1
        if symcc_break:
            break
        it += 1
    shutil.rmtree(symcc_tmp_output_dir)
    return symcc_output_cur_id,cur_symcc_input_id,symcc_total_time
def main():
    symcc_output_num, symcc_input_num,symcc_total_time = runSymcc(benchmark)
    print("symcc generate:{} with {} runs using {}us\n".format(symcc_output_num, symcc_input_num,symcc_total_time))
if __name__ == '__main__':
    main()