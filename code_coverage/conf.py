import os,re
benchmark =  "CROMU_00001"
time_budget = 60 * 60 * 24 # in seconds
serial_port = os.path.join("/","dev","ttyACM1")
zfill_len = 12
numExecution = 1000
timeout = 10
sleep_time = 10

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
        try:
            s = splitted[cur].decode("ascii")
        except IndexError as e :
            raise e
        if("total_time" in s):
            total_time = re.search('''total_time": (\d+)''', s).group(1)
            return int(total_time)
        cur -= 1