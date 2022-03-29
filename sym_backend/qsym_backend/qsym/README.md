## Environment
- Tested on Ubuntu 14.04 64bit and 16.04 64bit

## Installation

~~~~{.sh}
# disable ptrace_scope for PIN
$ echo 0|sudo tee /proc/sys/kernel/yama/ptrace_scope

# install z3 and system deps
$ ./setup.sh

# install using virtual env
$ virtualenv venv
$ source venv/bin/activate
$ pip install .
~~~~

## Installation using Docker

~~~~{.sh}
# disable ptrace_scope for PIN
$ echo 0|sudo tee /proc/sys/kernel/yama/ptrace_scope

# build docker image
$ docker build -t qsym ./

# run docker image
$ docker run --cap-add=SYS_PTRACE -it qsym /bin/bash
~~~~


## Run hybrid fuzzing with AFL

~~~~{.sh}
# require to set the following environment variables
#   AFL_ROOT: afl directory (http://lcamtuf.coredump.cx/afl/)
#   INPUT: input seed files
#   OUTPUT: output directory
#   AFL_CMDLINE: command line for a testing program for AFL (ASAN + instrumented)
#   QSYM_CMDLINE: command line for a testing program for QSYM (Non-instrumented)

# run AFL master
$ $(AFL_ROOT)/afl-fuzz -M afl-master -i $(INPUT) -o $(OUTPUT) -- $(AFL_CMDLINE)
# run AFL slave
$ $(AFL_ROOT)/afl-fuzz -S afl-slave -i $(INPUT) -o $(OUTPUT) -- $(AFL_CMDLINE)
# run QSYM
$ bin/run_qsym_afl.py -a afl-slave -o $(OUTPUT) -n qsym -- $(QSYM_CMDLINE)
~~~~

## Run for testing

~~~~{.sh}
$ cd tests
$ python build.py
$ python -m pytest -n $(nproc)
~~~~

## Authors
- Insu Yun <insu@gatech.edu>
- Sangho Lee <sangho@gatech.edu>
- Meng Xu <meng.xu@gatech.edu>
- Yeongjin Jang <yeongjin.jang@oregonstate.edu>
- Taesoo Kim <taesoo@gatech.edu>

## Publications
```
QSYM: A Practical Concolic Execution Engine Tailored for Hybrid Fuzzing

@inproceedings{yun:qsym,
  title        = {{QSYM: A Practical Concolic Execution Engine Tailored for Hybrid Fuzzing}},
  author       = {Insu Yun and Sangho Lee and Meng Xu and Yeongjin Jang and Taesoo Kim},
  booktitle    = {Proceedings of the 27th USENIX Security Symposium (Security)},
  month        = aug,
  year         = 2018,
  address      = {Baltimore, MD},
}
```
