from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess

input_dir = "/home/lcm/github/spear/spear-code/code_coverage/CROMU_00002"
build_dir = os.path.join(input_dir,"build")
executable = os.path.join(build_dir,"out")
symcc_input = "/home/lcm/github/spear/symcc/shared_volume/CROMU_00002/output"
spear_input = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/SpearCROMU_00002/intermediate_results/output"

assert(os.path.exists(input_dir))
assert(os.path.exists(build_dir))
assert(os.path.exists(executable))
assert(os.path.exists(symcc_input))
assert(os.path.exists(spear_input))



def runOne(input_dir, just_one = False):
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
        assert(exit_status == 0)
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
def runTest():
    os.chdir(build_dir)
    symcc_bb = runOne(symcc_input)
    spear_bb = runOne(spear_input)
    print("symcc covered BB:{}\n".format(len(symcc_bb)))
    print("spear covered BB:{}\n".format(len(spear_bb)))
    embed()
        
def main():
    #checkMake()
    runTest()
if __name__ == '__main__':
    main()