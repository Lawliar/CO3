from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess
import threading

lock = threading.Lock()



def worker(index,input_dir, build_dir, executable, files, start_index,end_index,ret):
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
        


def runOne(input_dir,build_dir,executable, just_one = False):
    files = os.listdir(input_dir)
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
        p = threading.Thread(target=worker,args=[i, input_dir, build_dir, executable, files, start,end,ret])
        p.start()
        threads.append(p)
    for p in threads:
        p.join()
    print("finished")
    embed()
    return ret
def runTest(benchmark):
    input_dir = "/home/lcm/github/spear/spear-code/code_coverage/{}".format(benchmark)
    build_dir = os.path.join(input_dir,"build")
    executable = os.path.join(build_dir,"out")
    symcc_input = "/home/lcm/github/spear/spear-code/symcc_benchmark/shared_volume/{}/output".format(benchmark)
    spear_input = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/Spear{}/intermediate_results/output".format(benchmark)
    
    assert(os.path.exists(input_dir))
    assert(os.path.exists(build_dir))
    assert(os.path.exists(executable))
    assert(os.path.exists(symcc_input))
    assert(os.path.exists(spear_input))

    os.chdir(build_dir)## we can always change to this, since this is an absolute path
    symcc_bb = runOne(symcc_input,build_dir, executable)
    spear_bb = runOne(spear_input,build_dir, executable)

    with open(os.path.join(input_dir,"result.txt"),"w") as wfile:
        wfile.write("symcc edges, {}:{}".format(len(symcc_bb),symcc_bb))
        wfile.write("spear edges, {}:{}".format(len(spear_bb),spear_bb))
    print("symcc covered edges:{}\n".format(len(symcc_bb)))
    print("spear covered edges:{}\n".format(len(spear_bb)))
    
def main():
    #checkMake()
    for each_bench in ["CROMU_00001"]:
        runTest(each_bench)
if __name__ == '__main__':
    main()
