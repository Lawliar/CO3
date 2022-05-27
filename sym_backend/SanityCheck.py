import os
import sys
def main():
    inputDir = "/home/lcm/github/spear/spear-code/firmware/STMfirmware/SpearRuntime/intermediate_results"
    exe = "/home/lcm/github/spear/spear-code/sym_backend/cmake-build-debug/qsym_backend/orchestrator"
    for eachfile in os.listdir(inputDir):
        if(eachfile.endswith("cfg.dot")):
            cfg_file = os.path.join(inputDir,eachfile)
            dfg_file = os.path.join(inputDir,eachfile.replace("cfg.dot","dfg.dot"))
            pdt_file = os.path.join(inputDir,eachfile.replace("cfg.dot","postDom.dot"))
            cmd = "{} -c {} -p {} -d {} -s dummy -b 10000000".format(exe, cfg_file,pdt_file, dfg_file)
            print(eachfile)
            os.system(cmd)
            print("\n\n")
if __name__ == '__main__':
    main()