from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import threading

lock = threading.Lock()
from conf import coverage_executable as executable
from conf import coverage_dir

def single_coverage_worker(input_file):
    global executable
    os.path.exists(input_file)
    assert(os.path.isabs(input_file))
    p1 = subprocess.Popen(["cat", input_file], stdout=subprocess.PIPE)
    exit_status = p1.wait()
    if(exit_status != 0):
        print("cat failed")
        embed()
    p2 = subprocess.run([executable], stdin=p1.stdout, stdout=subprocess.PIPE)
    lines = p2.stdout.decode('utf-8')
    for eachline in lines.split('\n'):
        if(not eachline.startswith('cc:')):
            continue
        eachline = eachline.strip()[3:]
        yield eachline

def batch_worker(input_dir, executable, files, start_index,end_index,ret):
    for i in tqdm(range(start_index, end_index)):
        eachfile = files[i]
        input_file = os.path.join(input_dir, eachfile)

        p1 = subprocess.Popen(["cat", input_file], stdout=subprocess.PIPE)
        exit_status = p1.wait()
        if(exit_status != 0):
            embed()
        p2 = subprocess.run([executable], stdin=p1.stdout,stdout=subprocess.PIPE)
        
        lines = p2.stdout.decode('utf-8')
        lock.acquire()
        for eachline in lines.split('\n'):
            if(not eachline.startswith('cc:')):
                continue
            eachline = eachline.strip()[3:]
            ret.add(eachline)
        lock.release()
        


def runBatch(input_dir,executable):
    files = os.listdir(input_dir)
    files.remove('tmp_out')
    len_files = len(files)
    num_cores = 4
    threads = []
    ret = set()
    partition = len_files // num_cores
    for i in range(num_cores):
        if i == num_cores - 1:
            start = partition * i
            end = len_files
        else:
            start = partition * i
            end = partition * (i + 1)
        p = threading.Thread(target=batch_worker,args=[input_dir, executable, files, start,end,ret])
        p.start()
        threads.append(p)
    for p in threads:
        p.join()
    print("finished")
    return ret
def runBenchmark(benchmark):
    symcc_input = "/home/lcm/github/spear/spear-code/symcc_benchmark/shared_volume/{}/output".format(benchmark)
    spear_input = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results/output".format(benchmark)
    
    assert(os.path.exists(executable))
    assert(os.path.exists(symcc_input))
    assert(os.path.exists(spear_input))

    
    symcc_bb = runBatch(symcc_input, executable)
    #spear_bb = runBatch(spear_input,build_dir, executable)

    with open(os.path.join(coverage_dir,"result.txt"),"w") as wfile:
        wfile.write("symcc edges, {}:{}".format(len(symcc_bb),symcc_bb))
    #    wfile.write("spear edges, {}:{}".format(len(spear_bb),spear_bb))
    print("symcc covered edges:{}\n".format(len(symcc_bb)))
    #print("spear covered edges:{}\n".format(len(spear_bb)))
    
def main():
    #checkMake()
    for each_bench in ["CROMU_00001"]:
        runBenchmark(each_bench)
if __name__ == '__main__':
    main()
