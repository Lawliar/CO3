cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# enable export of compiler commands to compile_commands.json
#set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(LLVM_BUILD "USE LLVM instead of ARM to build" OFF)
# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../clang.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



set(CO3_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../intermediate_results")
set(CO3_INSTRUMENTATION_LIB "${CMAKE_SOURCE_DIR}/../../../../pass/symbolizer/cmake-build-debug/libSymbolize.so")
set(CommonDIR "${CMAKE_SOURCE_DIR}/../../Common")

# Setup project, output and linker file
project(samd51j19a_Target_CMake)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)


set(ARCH_CFLAGS
-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
)


set(ALL_NINE_YARDS_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/../usb
  ${CMAKE_SOURCE_DIR}/../usb/class/cdc
  ${CMAKE_SOURCE_DIR}/../hpl/port
  ${CMAKE_SOURCE_DIR}/../hpl/osc32kctrl
  ${CMAKE_SOURCE_DIR}/../hpl/oscctrl
  ${CMAKE_SOURCE_DIR}/../hpl/cmcc
  ${CMAKE_SOURCE_DIR}/..
  ${CMAKE_SOURCE_DIR}/../hal/utils/include
  ${CMAKE_SOURCE_DIR}/../hal/include
  ${CMAKE_SOURCE_DIR}/../hpl/pm
  ${CMAKE_SOURCE_DIR}/../usb/class/cdc/device
  ${CMAKE_SOURCE_DIR}/../usb/device
  ${CMAKE_SOURCE_DIR}/../hri
  ${CMAKE_SOURCE_DIR}/../hpl/gclk
  ${CMAKE_SOURCE_DIR}/../hpl/mclk
  ${CMAKE_SOURCE_DIR}/../hpl/core
  ${CMAKE_SOURCE_DIR}/../hpl/ramecc
  ${CMAKE_SOURCE_DIR}/../hpl/dmac
  ${CMAKE_SOURCE_DIR}/../examples
  ${CMAKE_SOURCE_DIR}/../Target
  ${CMAKE_SOURCE_DIR}/../hpl/usb
  ${CMAKE_SOURCE_DIR}/../config
  ${CMAKE_SOURCE_DIR}/../CMSIS/Core/Include
  ${CMAKE_SOURCE_DIR}/../stdio_redirect
  ${CMAKE_SOURCE_DIR}/../samd51a/include
  ${CMAKE_SOURCE_DIR}/../CMSIS/Core/Include
  ${CommonDIR}/Protocol
  ${CommonDIR}/SymRuntime
  ${CommonDIR}/FreeRTOS-Kernel
  ${CommonDIR}/FreeRTOS-Kernel/include
  ${CommonDIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
)


include_directories(
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
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


file(GLOB FILESTARGET
  test.c
  ATParser.c
)
set_property(SOURCE ${FILESTARGET} APPEND PROPERTY COMPILE_OPTIONS
 "-Xclang" "-load"
 "-Xclang" "${CO3_INSTRUMENTATION_LIB}"
 "-Xclang" "-fpass-plugin=${CO3_INSTRUMENTATION_LIB}" "-mllvm" "-out=${CO3_OUTPUT_DIR}"
)


add_library(${CMAKE_PROJECT_NAME} OBJECT ${FILESTARGET})



set(CO3_OPTIMIZATION_FLAGS -Os)

set(CO3_COMPILE_OPTIONS
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -ffunction-sections 
  -mlong-calls 
  -Wall
  ${CO3_OPTIMIZATION_FLAGS}
  ${ARCH_CFLAGS} 
)

if(${LLVM_BUILD})
  set(CO3_COMPILE_OPTIONS ${CO3_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS}) 
endif()

add_compile_options(
  ${CO3_COMPILE_OPTIONS}
)

set(CO3_TARGET_COMPILE_OPTIONS
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -ffunction-sections 
  -mlong-calls 
  -Wall
  ${CO3_OPTIMIZATION_FLAGS}
  ${ARCH_CFLAGS} 
)

if(${LLVM_BUILD})
  set(CO3_TARGET_COMPILE_OPTIONS ${CO3_TARGET_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS})
endif()
target_compile_options(
  ${CMAKE_PROJECT_NAME} PRIVATE
  ${CO3_TARGET_COMPILE_OPTIONS}
)

