from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess

benchmark = "CROMU_00001"





def runOne(input_dir,build_dir,executable, just_one = False):
    ret = set()
    for eachfile in tqdm(os.listdir(input_dir)):
        if(just_one and eachfile != "000000"):
            continue
        input_file = os.path.join(input_dir, eachfile)
        result_file = os.path.join(build_dir,"coverage")
        if(os.path.exists(result_file)):
            os.system("rm coverage")
        assert(not os.path.exists(result_file))

        p1 = subprocess.Popen(["cat", input_file], stdout=subprocess.PIPE)
        exit_status = p1.wait()
        if(exit_status != 0):
            embed()
        p2 = subprocess.Popen([executable], stdin=p1.stdout)
        exit_status = p2.wait()
        #embed()
        #assert(exit_status == 0)

        assert(os.path.exists(result_file))
        with open(result_file,"r") as rfile:
            lines = rfile.readlines()
        assert(len(lines) > 0)
        for eachline in lines:
            ret.add(eachline.strip())
        os.system("rm coverage")
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
    for each_bench in ["CROMU_00001","CROMU_00002","CROMU_00003"]:
        runTest(each_bench)
if __name__ == '__main__':
    main()