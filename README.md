## Directory:
```
- deps: projects that CO3 is dependent upon (except symcc). 
- firmware: source code for the firmware images used in the paper.
- fuzzer: fuzzing and CO3 coordinator script borrowed from SymCC.
- pass: the LLVM instrumentation pass. 
- sym_backend: the symbolic constraint builder and solver that are based on CO3's protocol.
- sym_runtime: the runtime for workstation applications (to interact with the sym_backend). 
- utils: helper scripts to visualize the SVFG, communicate with the MCU. 
```
## before you start:
1. many components rely on cmake, if you see `cmake build`, it means:
    1. create an empty build dir, 
    2. cd to it, 
    3. type `cmake ..` , 
    4. type `make`.

2. before using the docker file, submodule init are required, also llvm prebuilt should be decompressed to the specified folder. 

3. CO3 features concolic executing the firmware on the MCU, so the physical MCUs are necessary. 

## tested platform

- Workstation:
    1. Native Ubuntu 22.04 (older version also should work)

- MCUs:
    1. STM32H743ZI
    2. STM32L4R5ZI
    3. STM32F429ZI
    4. Microchip SAMD51
    5. NXP K66F

## current llvm support:
- Both instrumentation and symbolic backend are built on LLVM-14. 

## Prerequisite 

### llvm prebuilt
- Download llvm-14.0x pre-built and unzip to deps/llvm through `tar -xf <llvm-14>.tar.gz -C ./deps/llvm/`. 
    - If you are using docker to build orchestrator, you still need this step. 
    However, instead of the pre-built for you hosting OS, you can just use [llvm-prebuit-ubuntu](https://github.com/llvm/llvm-project/releases/download/llvmorg-14.0.0/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz)

### arm cross compiler:
- This is only needed to build the firmware image. Simply select that one fitting your situation from 
[arm-gnu-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). Unzip it to `deps/arm` and make sure the CMakeLists.txt under the firmware source code points to it. The version that we use is **11.3.rel1**. 

### dependent packages
- `sudo apt install autoconf automake libtool`
- python3 with `pyserial`

## submodule initialization:
- `git submodule init`
- `git submodule update`

### boost:
- cd to deps/boost
- `git submodule init`
- `git submodule update`


## Native build dependencies

### boost
- cd to deps/boost
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

### build symbolizer
- cd to pass/symbolizer
- [optional] double check `CO3_DEPS_DIR` `LLVM_HINTS` point to the right folder
- cmake build 

### instrument prepare firmware
- pick one firmware, e.g., "SpearCROMU_00001"
- make sure the ProtocolConfig.h fits your need
- make sure variables in clang.cmake and CMakeLists.txt point to the dependencies that you created before
- cmake build 
- flash the firmware

### build orchestrator
- cd to sym_backend
- double check the configurations fit your needs, there are three builds, debug, release, profiling
- cmake build 

## docker build
### symbolizer
- there is no dockerfile to build symbolizer, not because of the technical difficulty but because of the mixed of toolchain (e.g., cross-compiler from gcc) required to build a firmware image. 
The symbolizer is just one part of the firmware building process. Simply building it is not useful. 



### orchestrator
the dockerfile in this repo is for the orchestrator. In order to use this docker file, 
steps in [submodule initialization](#submodule-initialization) and [llvm-prebuilt](#llvm-prebuilt) are still required. 


## fair warning:
- Due to historical reasons, the whole codebase is filled with name referecens to `SPEAR`, which is the old name for `CO3`. If you see `SPEAR`, that means the same thing as `CO3`. 
- This is a research prototype, not intended for production. In the meantime, I am open for all helpful PRs that can make CO3 better. 
- If your research prototype is built on top of CO3, I encourage you to open source too.

## If you want to contribute:
- There is roadmap for future development, please see [#1](/../../issues/1).
    - Feel free to comment in that issue to share your ideas about what could be the next step. You ideas will be considered proactively. 
    - If you want to take on some of the tasks, feel free to let me know, I might have something done already. 