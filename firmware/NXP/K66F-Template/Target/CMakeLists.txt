# This file is part of CO3.
#
# CO3 is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# CO3. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)
option(LLVM_BUILD "USE LLVM instead of ARM to build" ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../../STMfirmware/Common/clang.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(EVKMIMXRT1176_Target_CMake)


set(CO3_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../intermediate_results")
set(CO3_INSTRUMENTATION_LIB "${CO3_ROOT_DIR}/pass/symbolizer/build_release/libSymbolize.so")
set(CommonDIR ${CO3_COMMON_DIR})

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)


set(ARCH_CFLAGS
  -mthumb -mcpu=cortex-m4 -mfloat-abi=hard  -mfpu=fpv4-sp-d16 
) 

file(GLOB FILESNORMAL
  ./src/test.c
)

file(GLOB FILESTARGET
  ./src/sys_command_line.c
  ./src/sys_queue.c
 )

set_property(SOURCE ${FILESTARGET} APPEND PROPERTY COMPILE_OPTIONS
        "-Xclang" "-load"
        "-Xclang" "${CO3_INSTRUMENTATION_LIB}"
        "-Xclang" "-fpass-plugin=${CO3_INSTRUMENTATION_LIB}" "-mllvm" "-out=${CO3_OUTPUT_DIR}"
)


execute_process (
    COMMAND ${ARM_GNU_CC} ${ARCH_CFLAGS} -print-sysroot 2>&1
    OUTPUT_VARIABLE ARM_CORTEXM_SYSROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${ARM_GNU_CC} ${ARCH_CFLAGS} -print-multi-directory 2>&1
  OUTPUT_VARIABLE ARM_CORTEXM_MULTI_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${ARM_GNU_CC} ${ARCH_CFLAGS} -print-libgcc-file-name 2>&1
  OUTPUT_VARIABLE ARM_CORTEXM_BUILTINS_LIBGCC
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_STRIP_TRAILING_WHITESPACE
)


set(COMPILER_SPECIFIC_CFLAGS
  --target=arm-none-eabi --sysroot=${ARM_CORTEXM_SYSROOT}
)

set(COMPILER_SPECIFIC_LDFLAGS
  -L${ARM_CORTEXM_SYSROOT}/lib/${ARM_CORTEXM_MULTI_DIR} ${ARM_CORTEXM_BUILTINS_LIBGCC}
)



# Build the executable based on the source files
add_library(${CMAKE_PROJECT_NAME} STATIC ${FILESNORMAL} ${FILESTARGET})
set(CO3_OPTIMIZATION_FLAG -O2)
# List of compiler defines, prefix with -D compiler option. Copy from the project settings

set(CO3_COMPILE_OPTIONS
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
-mthumb
${ARCH_CFLAGS} 
-fno-common 
#-specs=nano.specs
#-specs=nosys.specs
)
if(${LLVM_BUILD})
  set(CO3_COMPILE_OPTIONS ${CO3_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS})
endif()

add_compile_options(
  ${CO3_COMPILE_OPTIONS}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE

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

set(CO3_TARGET_COMPILE_OPTIONS
-std=gnu99
${CO3_OPTIMIZATION_FLAG}
-g 
-gdwarf-4 
-c 
-ffunction-sections 
-fdata-sections 
-fno-builtin 
-mthumb
-Wall -fstack-usage  
${ARCH_CFLAGS}
-fno-common 
#-specs=nano.specs
#-specs=nosys.specs
)
if(${LLVM_BUILD})
  set(CO3_TARGET_COMPILE_OPTIONS ${CO3_TARGET_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS})
endif()

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
${CO3_TARGET_COMPILE_OPTIONS} 
)

if(${LLVM_BUILD})
  set(CMAKE_C_LINK_EXECUTABLE 
    "${ARM_GNU_CC} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
  )
endif()


target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PUBLIC
  ${CommonDIR}/Protocol
  ${CommonDIR}/SymRuntime
  ./
  ../CommonCO3
  ../CMSIS
  ../source
  ../board
  ../device
  ../drivers
  ./inc
  ../freertos/freertos_kernel/include
  ../freertos/freertos_kernel/portable/GCC/ARM_CM4F
  ../usb/device/include
  ../usb/include
  ../component/osa
  ../component/lists
)
