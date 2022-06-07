## how to instrument the firmware
1. required utility
    - `sudo apt install pkg-config`
2. building the pass:
    - build boost library
        - at the same dir as the README.md
            - `git submodule init`
            - `git submodule update`
        - cd to deps/boost
            - `git submodule init`
            - `git submodule update`
            - `./bootstrap.sh --prefix=./install`
            - `./b2 install --with-filesystem --with-graph --with-program_options`
    - download llvm 12.0:
        - download from https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.0/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz and unzip it
        - under ./pass/
            - `mkdir build`
            - `cd build`
            - `cmake .. -DLLVM_HINTS=<abs path to llvm you just download>/lib/cmake -DBOOST_HINTS=<abs path to the boost install dir>/lib/cmake `
            - `make`
3. instrument the firmware
    - `make LLVM_TOOLCHAIN=<llvm folder you just unzip> INSTRUMENTATION_LIB=<llvm pass generated>`

## build libserialport
- cd to deps/libserialport
- ./autogen.sh
- ./configure
- make
- sudo make install
