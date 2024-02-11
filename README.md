## Directory:
```
- USBtest: helper script to send and receive messages through the serial port for debugging purpose
- benchmark: benchmark that we considered, not used.
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

## current llvm support:
- Both instrumentation and symbolic backend are built on LLVM-14. 

## Prerequisite 
### OS-agnostic part:
1. - Download llvm-14.0x pre-built and unzip to folders of your preference and memorize it. 
### Ubuntu:
    - `sudo apt install pkg-config autoconf automake libtool`
### macOS:
    - `brew install pkg-config autoconf automake libtool`

## how to instrument the firmware
### mac:

1. required utility
    - 
    - 
2. building the pass:
    - build boost library
        - if you have boost library installed from apt, please considering removing them. Check out if there is any input from `apt list --installed | grep boost` we apologize for the inconvenience.
        - at the same dir as the README.md
            - `git submodule init`
            - `git submodule update`
        - cd to deps/boost
            - `git submodule init`
            - `git submodule update`
            - `./bootstrap.sh`
            - `sudo ./b2 install --with-filesystem --with-graph --with-program_options`
    - download llvm 12.0:
        - download from https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.0/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz and unzip it
        - under ./pass/
            - `mkdir build`
            - `cd build`
            - `cmake .. -DLLVM_HINTS=<abs path to llvm you just download>/lib/cmake`
            - `make`
3. instrument the firmware
    - `make LLVM_TOOLCHAIN=<llvm folder you just unzip> INSTRUMENTATION_LIB=<llvm pass generated>`

## build libserialport
- cd to deps/libserialport
- ./autogen.sh
- ./configure
- make
- sudo make install

## fair warning:
Due to historical reasons, the whole codebase is filled with name referecens to `SPEAR`, which is the old name for `CO3`. If you see `SPEAR`, that means the same thing as `CO3`. 