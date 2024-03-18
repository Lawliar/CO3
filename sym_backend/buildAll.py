import os
for each_file in os.listdir("."):
    each_file = os.path.join('.',each_file)
    if (os.path.isdir(each_file)  and "build" in each_file):
        print("in {}".format(each_file))
        os.system("cd {} && make && cd ..".format(each_file))
