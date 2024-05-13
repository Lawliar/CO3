## How it works:
- We use cmake for all the firmware building process (you are welcome), you can build the firmware image like a normal cmake executable, however a few things need attention. 

- 1. Configure to the `CO3_INSTRUMENT` and `CO3_REPLACE` in CMakelist.txt in each firmware's root directory.
    - 1.  `CO3_INSTRUMENT` denotes if you want to instrument the code or not (the monitor is still in place regardless). 
    - 2. `CO3_REPLACE` denotes if you want to replace the symbolic operations to logic operations (e.g., logic disjunction) or not. This corresponds to `Report-All` mode which is not advised to use unless for debugging or other special purpose. Since the building process will generate the SVFG; `Report-All` and other modes generate different SVFGs, please use different folder names to store these SVFGs. 
- 2. configure the ProtocolConfig.h.
    - There is the working ProtocolConfig.h file for every project in place. So, if you have the same MCU that we have, you can just go ahead, compile and flash. 
    - If you want to port CO3 to other MCU, we provide ProtocolConfigTemplate.h which shows all the macros that are needed in order to configure CO3 (e.g., runtime and monitor). 
    - The process in ProtocolConfig.h is self-explainatory and follows this procedure:
        - if `CO3_NO_MCU_SIDE_SHADOW` is defined, then it means we do not use MCU's side shadow. This corresponds to the `Shadowless` mode in the paper. 
            - If `CO3_NO_MCU_SIDE_SHADOW` is not defined (i.e., you will use shadow on the MCU), you also need to define the relevant information for the shadow (e.g., the address and legnth of the RAM you want to use shadow to, the address and length of the shadow itself)
        - define the MACROs that correspond to your case in terms of which vendor, which MCU, which RTOS, if you use USB or physical USART as communication channel, and finally, the embedded application that you instrument. 
## STM32 firmware
- After the CMakelists.txt file and ProtocolConfig.h are properly configured, you can just cmake build the firmware like a normal cmake project. 

## Non-STM32 firmware
- This is a little more complicated: 
    - For the code that needs to be instrumented: we still use LLVM. 
    - For the code that does not need instrumentation (e.g., drivers, startup assembly) we use GCC. 
    - We also use GCC's linker for the linking stage. 
    - This results in a two-stage building process. This is a compromise due to the current LLVM support for embedded projects. 
- Steps:
    1. configure the ProtocolConfig.h and CMakeLists.txt file as described before. 
    1. dive into the Target folder which contains the code that needs symbolic instrumentation. 
    2. create a `build` folder and cmake build the target.
    3. cd to the root folder, and cmake build the whole firmware. 
