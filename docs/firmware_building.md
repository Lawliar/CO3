## How it works:
- The building process is based on cmake. 
- Two toolchains are required: 
    1. llvm-14 (prebuilt is fine) 
    2. the official [arm-gnu-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). The version tested and working is **11.3.rel1**. Others should also work. 

## STM32 firmware
- Compilation is based on llvm-14, however, we did use the GCC linker because of its libraries. 
- The instrumentation is based on the symbolizer pass which is specified in the CMakeLists.txt file under the root directory. 
- Steps:
    1. specify the options in the CMakeLists.txt file. 
    2. configure the ProtocolConfig.h.
    3. cmake build the firmware. 

## Non-STM32
- This is a little more complicated: we use GCC to cross-compile and link except the code that needs instrumentation. For instrumentation, we still use llvm-14. 
- This is because llvm does not work really well on some of the drivers and assembly code from these MCUs. 
- Steps:
    1. configure the ProtocolConfig and the options in CMakeLists.txt file in the root folder. 
    1. dive into the Target folder which contains the code that needs symbolic instrumentation. 
    2. create a `build` folder and cmake build the target inside it.
    3. cd to the root folder, and cmake build the whole firmware. 


Then flash the built firmware to the board and you are good to go. 