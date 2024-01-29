cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../arm.cmake)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(EVKMIMXRT1176_CMake_Startup)


enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)



set(ARCH_CFLAGS
  -mthumb -mcpu=cortex-m4 -mfloat-abi=hard  -mfpu=fpv4-sp-d16 
) 

file(GLOB FILES
  startup_mk66f18.c
)

add_library(startup ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  startup
)

set(CO3_OPTIMIZATION_FLAG -O0)


# List of compiler defines, prefix with -D compiler option. Copy from the project settings
add_compile_options(
-DCPU_MK66FN2M0VMD18 
-DCPU_MK66FN2M0VMD18_cm4 
-DSDK_OS_FREE_RTOS 
-DSERIAL_PORT_TYPE_UART=1 
-D_DEBUG=1 
-DSDK_DEBUGCONSOLE=1 
-DUSB_STACK_BM 
-DFSL_OSA_BM_TASK_ENABLE=0 
-DFSL_OSA_BM_TIMER_CONFIG=0 
-DMCUXPRESSO_SDK 
-DSDK_OS_BAREMETAL 
-DCR_INTEGER_PRINTF 
-D__MCUXPRESSO 
-D__USE_CMSIS 
-DDEBUG
-D__NEWLIB__
-std=gnu99 
${CO3_OPTIMIZATION_FLAG}
-g 
-c 
-ffunction-sections 
-fdata-sections 
-fno-builtin 
-Wall -fstack-usage  
-fno-common 
${ARCH_CFLAGS}
)

target_compile_definitions(startup PRIVATE

-DCPU_MK66FN2M0VMD18 
-DCPU_MK66FN2M0VMD18_cm4 
-DSDK_OS_FREE_RTOS 
-DSERIAL_PORT_TYPE_UART=1 
-D_DEBUG=1 
-DSDK_DEBUGCONSOLE=1 
-DUSB_STACK_BM 
-DFSL_OSA_BM_TASK_ENABLE=0 
-DFSL_OSA_BM_TIMER_CONFIG=0 
-DMCUXPRESSO_SDK 
-DSDK_OS_BAREMETAL 
-DCR_INTEGER_PRINTF 
-D__MCUXPRESSO 
-D__USE_CMSIS 
-DDEBUG
-D__NEWLIB__ 
)

# Compiler options
target_compile_options(startup PRIVATE
${ARCH_CFLAGS}
-std=gnu99
${CO3_OPTIMIZATION_FLAG}
-g 
-gdwarf-4 
-c 
-ffunction-sections 
-fdata-sections 
-fno-builtin 
-Wall -fstack-usage  
-fno-common 
)