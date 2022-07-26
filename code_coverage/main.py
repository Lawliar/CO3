from distutils.command.build import build
import os
from unittest import result
from IPython import embed
from tqdm import tqdm
import subprocess

input_dir = "/home/lcm/github/spear/spear-code/code_coverage/CROMU_00001"
build_dir = os.path.join(input_dir,"build")
executable = os.path.join(build_dir,"cromu_00001")
symcc_input = "/home/lcm/github/spear/symcc/shared_volume/CROMU_00001/build/orig_input"
spear_input = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/SpearCROMU_00001/intermediate_results/orig_input"
def checkMake():
    assert(os.path.exists(os.path.join(input_dir,"makefile")))
    if( not os.path.exists(os.path.join(input_dir,"build")) ):
        os.chdir(input_dir)
        os.system("make")
        os.chdir("..")


def runOne(input_dir):
    ret = set()
    for eachfile in tqdm(os.listdir(input_dir)):
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
        assert(exit_status == 0)

        assert(os.path.exists(result_file))
        with open(result_file,"r") as rfile:
            lines = rfile.readlines()
        assert(len(lines) > 0)
        for eachline in lines:
            ret.add(eachline.strip())
        os.system("rm coverage")
    return ret
def runTest():
    os.chdir(os.path.join(input_dir,"build"))
    symcc_bb = runOne(symcc_input)
    spear_bb = runOne(spear_input)
    print("symcc covered BB:{}\n".format(len(symcc_bb)))
    print("spear covered BB:{}\n".format(len(spear_bb)))
    embed()
        
def main():
    checkMake()
    runTest()
if __name__ == '__main__':
    main()