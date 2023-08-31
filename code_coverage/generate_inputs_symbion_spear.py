from generate_inputs_symcc_spear import serial_port, numIteration, time_budget, zfill_len, runSpear,get_highest_id
import os,shutil,time

import sys,subprocess

_benchmark = "PLC"


def runSymbion(benchmark):
    ## getting the input
    spear_inter_dir          = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Symbion{}/intermediate_results".format(benchmark)
    concrete_input           = "{}/concreteInputs.bin".format(spear_inter_dir)


    symbion_script         = "/home/lcm/github/spear/spear-code/symbion_concolic/test-{}.py".format(benchmark)

    symbion_dir                = os.path.join(os.path.dirname(symbion_script), benchmark) 
    symbion_output_dir          = "{}/output".format(symbion_dir)
    symbion_tmp_output_dir      = "{}/tmp_out".format(symbion_dir)
    


    if(os.path.exists(symbion_dir)):
        shutil.rmtree(symbion_dir)
    os.mkdir(symbion_dir)
    os.mkdir(symbion_output_dir)
    os.mkdir(symbion_tmp_output_dir)

    assert(os.path.exists(concrete_input))

    shutil.copyfile(concrete_input, os.path.join(symbion_output_dir, os.path.basename('0'.zfill(zfill_len))))

    symbion_input_cur_id = 0
    symbion_output_cur_id = 1
    symbion_start_time = time.time()
    symbion_break = False

    for it in range(numIteration):
        batch_size = len(range(symbion_input_cur_id, symbion_output_cur_id))
        cur_symbion_input_id = symbion_input_cur_id
        while True:
            if(cur_symbion_input_id >= symbion_output_cur_id):
                break
            if(time.time() - symbion_start_time >=  time_budget):
                symbion_break = True
                break
            symbion_input_file = os.path.join(symbion_output_dir, str(cur_symbion_input_id).zfill(zfill_len))
            p1 = subprocess.Popen(
                ["python",symbion_script,"-i",symbion_input_file, "-o",symbion_tmp_output_dir],
                stdout=subprocess.DEVNULL,
                #stderr=subprocess.DEVNULL
                )
            
            repeat = False
            try:
                p1.wait(45)
            except subprocess.TimeoutExpired:
                print("timed out, something is wrong at the MCU moved on")
                p1.kill()
                repeat = True
            finally:
                print("iter:{},cur: {} / {},total:{},time:{} / {}".format(it, cur_symbion_input_id - symbion_input_cur_id ,batch_size ,cur_symbion_input_id, int(time.time() - symbion_start_time) , time_budget))
            
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
    benchmark = "Steering_Control"

    symbion_output_num, symbion_input_num,symbion_total_time = runSymbion(benchmark)
    #spear_output_num, spear_input_num,spear_total_time = runSpear(benchmark)
    print("symbion generate:{} with {} runs using {}us\n".format(symbion_output_num, symbion_input_num,symbion_total_time))
    #print("spear generate:{} with {} runs using {}us\n".format(spear_output_num, spear_input_num,spear_total_time))

if __name__ == '__main__':
    main()