## Intro

This is the repo for *CO3: Concolic Co-execution for Firmware* from [NU Seclab](https://seclab.nu/). CO3 features in instrumenting the firmware for a much faster (1000x) firmware concolic execution while running directly on the MCU hardware. 

## Directory:
```
- deps: projects that CO3 is dependent upon, except the SymCC folder which is there because that is the comparison target.  
- firmware: source code for the firmware images used in the paper.
- fuzzer: fuzzing (i.e., SHiFT) and CO3 coordinator script borrowed from SymCC. 
- pass: the LLVM instrumentation pass. 
- sym_backend (aka the orchestrator): the symbolic constraint builder and solver that are based on CO3's protocol.
- sym_runtime: the runtime for workstation applications (to interact with the orchestrator). 
- utils: helper scripts to visualize the SVFG, communicate with the MCU. 
```
## Before you start:
1. all artifacts and projects (including the firmware) are built with cmake, if you see `cmake build`, it means: create an empty `build` dir, cd to it, type `cmake ..`, and type `make`.

2. before using the docker file, the arm-gcc toolchain should also be decompressed to the specified folder if you want to build and flash the firmware. If you just want to try out the desktop program, you can just go ahead and use docker build. 

3. CO3 features concolic executing the firmware on the MCU; for concolic executing the firmware, physical MCU is required. 

4. We also have ports for desktop programs (see sym_runtime); however, this is currently under construction and only supports CGC programs. 


## Supported Platforms

- Workstation:
    1. Native Ubuntu 22.04 (older version also should work)

- MCUs:
    1. STM32H743ZI
    2. STM32L4R5ZI
    3. STM32F429ZI
    4. Microchip SAMD51
    5. NXP K66F


## Prerequisite 

### llvm prebuilt (not needed for docker build)

- Download llvm-14.0x pre-built and unzip to deps/llvm through `tar -xf <llvm-14>.tar.gz -C ./deps/llvm/`. 



### arm cross compiler (required for docker build too):

- Simply select that one fitting your situation from [arm-gnu-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). The version that we use is **11.3.rel1**. 
- Unzip it to `deps/arm` and make sure the arm.cmake under the firmware source code (firmware/STMfirmware/Common) points to it. 

### dependent packages

- `sudo apt install autoconf automake libtool`
- python3 install with `pyserial`

## submodule initialization:

- `git submodule update --init --recursive`


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
- cmake -DCMAKE_INSTALL_PREFIX=\`pwd\`/install ..
- make
- make install 

## Native Build and Run

### Native build

#### build symbolizer
- cd to pass/symbolizer
- Configure the pass to fit your needs:
    1. `CO3_MCUS`: enable when instrumenting a MCU, disable when instrumenting a workstation application.
    2. `CO3_REPLACE`: if this pass will replace the symbolic instructions to a or instruction. (this corresponds to the `Report All` mode in the paper)
- cmake build 

#### instrument the firmware
- Please refer to [firmware building doc](docs/firmware_building.md). 

#### build orchestrator
- cd to sym_backend
- Configure the orchestrator to fit your needs:
    1. Building Types:
        1. Release Build. 
        2. Debug Build: besides less compile-time optimization, there are more sanity checks, and log prints. 
        3. Profile Build: with google profiler. 
    2. `CO3_NO_MCU_SHADOW`: consistent with the MCU-side configuration. What it does internally is diabling a few sanity checks.
    3. `CO3_REPLACE`: consistent with the MCU-side configuration. 
    4. `CO3_SER2NET`: use TCP/UNIX socket instead of the serial port. 
    5. `CO3_32BIT`: enabled when talking with a MCU. disabled when talking with a 64-bit workstation application.
- cmake build 

### Native run

1. Flash the firmware to the board and connect the USB/physical serial port to the workstation. 
2. run `orchestrator -i <SVFG dir> -p <serial port> -b <baud rate of the serial port>`
    1. SVFG dir is the folder automatically generated in the firmware building process. 
    2. serial port is the USB-CDC/physical serial port (e.g., /dev/ttyACM1). 
        - This port also accepts number as parameters if the orchestrator is built with `SER2NET`. In this case, the serial port can be a TCP port where the SER2NET is carried through. Furthermore, if you specify `0`, the orchestrator will look for UNIX socket directly located in the `<SVFG dir>`. UNIX socket is used in workstation mode.  
    3. baud rate is the baud rate for the serial port. This only matters when a physical serial port is used. For any other cases, specifying different number makes no difference. 

## Docker build and run

### Docker build
- In order to use this docker file, step in [arm cross compiler](#arm-cross-compiler) are still required. llvm-prebuilt will not be necessary, as we will use the one from apt. 
- You can use the docker environment to build the firmware. Everything else is prepared. 

### Docker run
- Running CO3 requires the serial port to be exposed to the docker container. 
- Docker Linux does not have issue with this. 
- For MacOS, however, since it does not support serial port bypassing, running on MacOS even inside docker is difficult. SER2NET directs serial port to a TCP port, however, we ran into issue even when we expose the TCP port to docker container on MacOS. For more on running dockerized CO3 on MacOS please refer to [#2](/../../issues/2). 

## Fair warning:
- Due to historical reasons, the whole codebase is filled with name referecens to `SPEAR`, which is the old name for `CO3`. If you see `SPEAR`, that means the same thing as `CO3`. 
- This is a research prototype, not intended for production. In the meantime, I am open for all helpful PRs that can make CO3 better. 
- If your research prototype is built on top of CO3, I encourage you to open source too.

## Way to contribute:
- There is roadmap for future development, please see [#1](/../../issues/1).
    - Feel free to comment in that issue to share your ideas about what could be the next step. You ideas will be considered proactively. 
    - If you want to take on some of the tasks, feel free to let me know, I might have something done already. 

## Cite this work:
```
@inproceedings{liu2024co3,
  title={CO3: Concolic Co-execution for Firmware},
  author={Liu, Changming and Mera, Alejandro and Kirda, Engin and Xu, Meng and Lu, Long},
  booktitle={33rd USENIX Security Symposium (USENIX Security 24)},
  year={2024}
}
```