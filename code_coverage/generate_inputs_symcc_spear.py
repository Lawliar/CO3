from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import shutil
import time
import re

serial_port = os.path.join("/","dev","ttyACM1")


numIteration = 5
time_budget = 3600 # in seconds
zfill_len = 10
numExecution = 1000

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

def get_total_time_out_err(input):
    splitted = input.split(b'\n')
    cur = len(splitted) - 1
    while True:
        s = splitted[cur].decode("ascii")
        if("total_time" in s):
            total_time = re.search('''total_time": (\d+)''', s).group(1)
            return int(total_time)
        cur -= 1
    
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
    for it in range(numIteration):
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
            p1.wait()
            _, e = p1.communicate()
            spear_total_time += get_total_time_out_err(e) / 1000000

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
            if cur_spear_input_id >= numExecution:
                spear_break = True
                break
        spear_input_cur_id = spear_output_cur_id
        spear_output_cur_id = get_highest_id(spear_output_dir) + 1
        if spear_break:
            break
    shutil.rmtree(spear_tmp_output_dir)
    return spear_output_cur_id,cur_spear_input_id, spear_total_time
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
    for it in range(numIteration):
        batch_size = len(range(symcc_input_cur_id, symcc_output_cur_id))
        for cur_symcc_input_id in range(symcc_input_cur_id, symcc_output_cur_id):
            symcc_input_file = os.path.join(symcc_output_dir, str(cur_symcc_input_id).zfill(zfill_len))
            if( not os.path.exists(symcc_input_file)):
                assert(os.path.exists(symcc_input_file + '-optimistic'))
                symcc_input_file += '-optimistic'
            
            p1 = subprocess.Popen([symcc_executable],stdin=open(symcc_input_file),stdout=subprocess.PIPE,stderr=subprocess.PIPE,
                                env={**os.environ, 'SYMCC_OUTPUT_DIR': symcc_tmp_output_dir})
            p1.wait()
            _, e = p1.communicate()
            symcc_total_time += get_total_time_out_err(e) / 1000000


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
            if cur_symcc_input_id >= numExecution:
                symcc_break = True
                break
        ## update input_cur_id and output_cur_id
        symcc_input_cur_id = symcc_output_cur_id
        symcc_output_cur_id = get_highest_id(symcc_output_dir) + 1
        if symcc_break:
            break
    shutil.rmtree(symcc_tmp_output_dir)
    return symcc_output_cur_id,cur_symcc_input_id,symcc_total_time
def main():
    benchmark =  "CROMU_00001"
    symcc_output_num, symcc_input_num,symcc_total_time = runSymcc(benchmark)
    spear_output_num, spear_input_num,spear_total_time = runSpear(benchmark)
    print("symcc generate:{} with {} runs using {}us\n".format(symcc_output_num, symcc_input_num,symcc_total_time))
    print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,spear_total_time))

if __name__ == '__main__':
    main()