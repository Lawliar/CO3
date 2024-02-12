## Directory:
```
- USBtest: helper script to send and receive messages through the serial port for debugging purpose
- code_coverage: script that we used to compare code coverage and speed.
- deps: projects that CO3 is dependent upon
- firmware: source code for the firmware images used in the paper.
- fuzzer: fuzzing and CO3 coordinator script borrowed from SymCC.
- pass: the LLVM instrumentation pass
- sym_backend: the symbolizer running on the workstation.
- symbion_concolic: scripts that we use to run symbion in a concolic execution way.
- symcc_benchmark: benchmark we use to evaluate SymCC.
- test: visualization code to generate control- data flow graph. 
```
## before you start:
1. many components rely on cmake, if you see `cmake build`, it means 1. create an empty build dir, 2. cd to it, 3. type `cmake ..` , 4. type `make`.

2. before using the docker file, submodule init are required, also llvm prebuilt should be decompressed to the specified folder. 

## tested platform
- Native Ubuntu 22.04 (older version also should work)

## current llvm support:
- Both instrumentation and symbolic backend are built on LLVM-14. 

## Prerequisite 
- Download llvm-14.0x pre-built and unzip to deps/llvm through `tar -xf <llvm-14>.tar.gz -C ./deps/llvm/`
- `sudo apt install autoconf automake libtool`

## submodules:
- `git submodule init`
- `git submodule update`

### boost:
- cd to deps/boost
- `git submodule init`
- `git submodule update`
- `./bootstrap.sh`
- `./b2 --with-filesystem --with-graph --with-program_options`

### libserialport
- cd to deps/libserialport
- ./autogen.sh
- ./configure
- make

### z3
- cd to deps/z3
- mkdir build
- cmake -DCMAKE_INSTALL_PREFIX=`pwd`/install ..
- make
- make install 

## build symbolizer
- cd to pass/symbolizer
- [optional] double check `CO3_DEPS_DIR` `LLVM_HINTS` point to the right folder
- cmake build 

## instrument prepare firmware
- pick one firmware, e.g., "SpearCROMU_00001"
- make sure the ProtocolConfig.h fits your need
- make sure variables in clang.cmake and CMakeLists.txt point to the dependencies that you created before
- cmake build 
- flash the firmware

## build orchestrator
- cd to sym_backend
- double check the configurations fit your needs, there are three builds, debug, release, profiling
- cmake build 


## fair warning:
- Due to historical reasons, the whole codebase is filled with name referecens to `SPEAR`, which is the old name for `CO3`. If you see `SPEAR`, that means the same thing as `CO3`. 
- This is a research prototype, not intended for production. In the meantime, I am open for all helpful PRs that can make CO3 better. 
