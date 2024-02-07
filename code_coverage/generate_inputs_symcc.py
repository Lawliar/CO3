import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import shutil
import time
import re
import json

from conf import benchmark,time_budget, zfill_len, get_highest_id, get_total_time_out_err,sleep_time, timeout
from conf import coverage_dir

from main import single_coverage_worker

def runSymcc(benchmark):
    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results".format(benchmark)
    symcc_dir                = "/home/lcm/github/spear/spear-code/symcc_benchmark/shared_volume/{}".format(benchmark)
    symcc_output_dir          = "{}/output".format(symcc_dir)
    symcc_tmp_output_dir      = "{}/tmp_out".format(symcc_output_dir)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)
    symcc_executable         = "{}/build/out".format(symcc_dir)
    coverage_file            = os.path.join(coverage_dir,"symcc.json")


    if(os.path.exists(symcc_output_dir)):
        shutil.rmtree(symcc_output_dir)
    os.mkdir(symcc_output_dir)
    
    if(os.path.exists(symcc_tmp_output_dir)):
        shutil.rmtree(symcc_tmp_output_dir)
    os.mkdir(symcc_tmp_output_dir)
    
    assert(os.path.exists(concrete_input))
    shutil.copyfile(concrete_input, os.path.join(symcc_output_dir,os.path.basename('0'.zfill(zfill_len))))
    
    batch_input_id_start = 0
    batch_input_id_end = 1
    
    
    symcc_break = False

    total_time = 0
    it = 0

    coverage = {}
    while (True):
        for cur_input_id in range(batch_input_id_start, batch_input_id_end):
            input_file = os.path.join(symcc_output_dir, str(cur_input_id).zfill(zfill_len))
            if( not os.path.exists(input_file)):
                assert(os.path.exists(input_file + '-optimistic'))
                input_file += '-optimistic'
            
            p1 = subprocess.Popen([symcc_executable],stdin=open(input_file),stdout=subprocess.PIPE,stderr=subprocess.PIPE,
                                env={**os.environ, 'SYMCC_OUTPUT_DIR': symcc_tmp_output_dir})
            try:
                p1.wait()
            except subprocess.TimeoutExpired:
                print("time out, something is wrong, sleep for sometime")
                p1.kill()
                time.sleep(sleep_time)
                total_time += timeout + sleep_time
                continue
            _, e = p1.communicate()
            try:
                total_time += get_total_time_out_err(e) / 1000000
            except IndexError as err:
                print("Symcc did not end well")
                embed()

            print("iter:{},cur at {} from {} to {}, time:{} / {}, edge:{}".format(it, cur_input_id, batch_input_id_start , batch_input_id_end, total_time , time_budget, len(coverage)))
            ## copy the newly generated file to other places
            tmp_output_id = get_highest_id(symcc_output_dir) + 1
            
            new_edges = 0
            for each_symcc_output in os.listdir(symcc_tmp_output_dir):
                src_file = os.path.join(symcc_tmp_output_dir, each_symcc_output)
                
                found_new_edge = False
                for each_covered_edge in single_coverage_worker(src_file):
                    if(each_covered_edge not in coverage):
                        coverage[each_covered_edge] = total_time
                        found_new_edge = True
                        new_edges += 1
                if found_new_edge:
                    with open(coverage_file,"w") as wfile:
                        json.dump(coverage, wfile)
                dest_file_name = str(tmp_output_id).zfill(zfill_len) 
                tmp_output_id += 1
                if("-optimistic" in each_symcc_output):
                    dest_file_name += "-optimistic"
                shutil.copyfile(src_file, os.path.join(symcc_output_dir, dest_file_name))
            print("symcc generated {} inputs, {} new edge found".format(len(os.listdir(symcc_tmp_output_dir)), new_edges))
            
            ## clean the tmp output dir
            shutil.rmtree(symcc_tmp_output_dir)
            os.mkdir(symcc_tmp_output_dir)
            ## delete input file to save space
            os.remove(input_file)
            #if cur_input_id >= numExecution:
            if total_time >= time_budget:
                symcc_break = True
                break
        ## update input_cur_id and output_cur_id
        batch_input_id_start = batch_input_id_end
        batch_input_id_end = get_highest_id(symcc_output_dir) + 1
        if symcc_break:
            break
        it += 1
    shutil.rmtree(symcc_tmp_output_dir)
    return batch_input_id_end,cur_input_id, total_time, coverage
def main():
    num_generated_inputs, num_run_inputs, total_time, coverage = runSymcc(benchmark)
    print("symcc generate:{} with {} runs covering {} edges, using {}us\n".format(num_generated_inputs, num_run_inputs,coverage, total_time))

if __name__ == '__main__':
    main()