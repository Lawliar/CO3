import os,re
benchmark =  "CROMU_00001"
SER2NET = True
time_budget = 60 * 60 * 24 # in seconds
serial_port = os.path.join("/","dev","ttyACM1")
tcp_port = 3002
zfill_len = 12
numExecution = 1000
timeout = 10
sleep_time = 10
baud_rate = 75000000

coverage_dir = "/home/lcm/github/spear/spear-code/code_coverage/{}".format(benchmark)
coverage_build_dir = os.path.join(coverage_dir,"build")
coverage_executable = os.path.join(coverage_build_dir,"out")

assert(os.path.exists(coverage_dir))
assert(os.path.exists(coverage_build_dir))
assert(os.path.exists(coverage_executable))

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
    while True:
        try:
            s = splitted[cur].decode("ascii")
        except IndexError as e :
            raise e
        if("total_time" in s):
            total_time = re.search('''total_time": (\d+)''', s).group(1)
            return int(total_time)
        cur -= 1