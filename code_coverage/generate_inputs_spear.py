import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import shutil
import time
import re
import json

from conf import benchmark,time_budget, zfill_len, get_highest_id, get_total_time_out_err,sleep_time, timeout,serial_port,baud_rate,SER2NET, tcp_port
from conf import coverage_dir, estimate_inputs_needed

from main import single_coverage_worker

def runSpear(benchmark, debug = False, buggy_index = 0):
    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results".format(benchmark)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)
    output_dir          = "{}/output".format(spear_inter_dir)
    tmp_output_dir      = "{}/tmp_out".format(output_dir)
    backend_executable = "/home/lcm/github/spear/spear-code/sym_backend/build_release/qsym_backend/orchestrator"
    coverage_file            = os.path.join(coverage_dir,"co3.json")
    if(debug == False and os.path.exists(output_dir)):
        shutil.rmtree(output_dir)
        os.mkdir(output_dir)
    if(os.path.exists(tmp_output_dir)):
        shutil.rmtree(tmp_output_dir)
    os.mkdir(tmp_output_dir)
    
    assert(os.path.exists(concrete_input))
    shutil.copyfile(concrete_input, os.path.join(output_dir,os.path.basename('0'.zfill(zfill_len))))

    batch_input_id_start = 0
    batch_input_id_end = 1

    spear_break = False

    total_time = 0
    it = 0
    coverage = {}
    while (True):
        for cur_input_id in range(batch_input_id_start, batch_input_id_end):
            if debug == True and cur_input_id < buggy_index:
                continue
            input_file = os.path.join(output_dir, str(cur_input_id).zfill(zfill_len))
            if(not os.path.exists(input_file)):
                assert(os.path.exists(input_file + '-optimistic'))
                input_file += '-optimistic'
            if SER2NET:
                cmd = [backend_executable,"-i",spear_inter_dir,"-s",str(tcp_port),"-b",str(baud_rate)]
            else:
                cmd = [backend_executable,"-i",spear_inter_dir,"-s",serial_port,"-b",str(baud_rate)]
            p1 = subprocess.Popen(cmd, \
                         stdout=subprocess.PIPE,stderr=subprocess.PIPE, \
                        env={**os.environ,'SYMCC_INPUT_FILE':input_file, 'SYMCC_OUTPUT_DIR': tmp_output_dir})
            
            try:
                p1.wait(timeout)
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
                print("program did not end well")
                embed()
            
            print("iter:{},cur at {} from {} to {}, time:{:.2f} / {}, edge:{}, need {} inputs to finish".format(it, cur_input_id, batch_input_id_start , batch_input_id_end, total_time , time_budget, len(coverage), estimate_inputs_needed(cur_input_id + 1, total_time, time_budget)))
            tmp_output_id = get_highest_id(output_dir) + 1

            new_edges = 0

            for each_spear_output in os.listdir(tmp_output_dir):
                src_file = os.path.join(tmp_output_dir, each_spear_output)
                found_new_edge = False
                for each_covered_edge in single_coverage_worker(src_file):
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
                if("-optimistic" in each_spear_output):
                    dest_file_name += "-optimistic"
                shutil.copyfile(src_file, os.path.join(output_dir, dest_file_name))
            print("co3 generated {} inputs, {} new edge found".format(len(os.listdir(tmp_output_dir)), new_edges))
            ## clean up tmp output
            shutil.rmtree(tmp_output_dir)
            os.mkdir(tmp_output_dir)
            os.remove(input_file)
            
            #if cur_spear_input_id >= numExecution:
            if total_time >= time_budget:
                spear_break = True
                break
        it += 1
        batch_input_id_start = batch_input_id_end
        batch_input_id_end = get_highest_id(output_dir) + 1
        if spear_break:
            break
    shutil.rmtree(tmp_output_dir)
    return batch_input_id_end,cur_input_id, total_time

def main():
    
    spear_output_num, spear_input_num,total_time = runSpear(benchmark, False, 1309)
    print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,total_time))

if __name__ == '__main__':
    main()