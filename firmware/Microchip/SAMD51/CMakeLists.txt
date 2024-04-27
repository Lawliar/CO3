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

# enable export of compiler commands to compile_commands.json
#set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(LLVM_BUILD "USE LLVM instead of ARM to build" OFF)
# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../STMfirmware/Common/arm.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup project, output and linker file
set(BUILD_FOLDER ./build)
project(samd51j19a_CMake)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(LINKER_FILE "${CMAKE_SOURCE_DIR}/samd51a/gcc/gcc/samd51j19a_flashMPU.ld")
set(CO3_TARGET_INSTRUMENTED_LIB "${CMAKE_SOURCE_DIR}/Target/build/libsamd51j19a_Target_CMake.a")

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)


set(ARCH_CFLAGS
-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
)

# List of source files
set(SRC_FILES
  main.c
  driver_init.c
  usb_start.c
  atmel_start.c
  stdio_start.c
)

set(CommonDIR ${CO3_COMMON_DIR})

set(ALL_NINE_YARDS_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/CommonCO3
  ${PROJECT_SOURCE_DIR}/usb
  ${PROJECT_SOURCE_DIR}/usb/class/cdc
  ${PROJECT_SOURCE_DIR}/hpl/port
  ${PROJECT_SOURCE_DIR}/hpl/osc32kctrl
  ${PROJECT_SOURCE_DIR}/hpl/oscctrl
  ${PROJECT_SOURCE_DIR}/hpl/cmcc
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/hal/utils/include
  ${PROJECT_SOURCE_DIR}/hal/include
  ${PROJECT_SOURCE_DIR}/hpl/pm
  ${PROJECT_SOURCE_DIR}/usb/class/cdc/device
  ${PROJECT_SOURCE_DIR}/usb/device
  ${PROJECT_SOURCE_DIR}/hri
  ${PROJECT_SOURCE_DIR}/hpl/gclk
  ${PROJECT_SOURCE_DIR}/hpl/mclk
  ${PROJECT_SOURCE_DIR}/hpl/core
  ${PROJECT_SOURCE_DIR}/hpl/ramecc
  ${PROJECT_SOURCE_DIR}/hpl/dmac
  ${PROJECT_SOURCE_DIR}/examples
  ${PROJECT_SOURCE_DIR}/Target
  ${PROJECT_SOURCE_DIR}/hpl/usb
  ${PROJECT_SOURCE_DIR}/config
  ${PROJECT_SOURCE_DIR}/CMSIS/Core/Include
  ${PROJECT_SOURCE_DIR}/stdio_redirect
  ${PROJECT_SOURCE_DIR}/samd51a/include
  ${PROJECT_SOURCE_DIR}/CMSIS/Core/Include
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


set(CO3_OPTIMIZATION_FLAGS -O0 -g -g3)
# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

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
  ${EXECUTABLE} PRIVATE
  ${CO3_TARGET_COMPILE_OPTIONS}
)

set(CMAKE_C_LINK_EXECUTABLE 
  "${ARM_GNU_CC} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
  ${ARCH_CFLAGS}
  -D__SAMD51J19A__
  --specs=nosys.specs
  -Wl,--no-warn-mismatch
  -Wl,-Map=SAMD51.map
  -DXPRJ_Default=Default
  -Wl,--defsym=__MPLAB_BUILD=1
  -L "${PROJECT_SOURCE_DIR}/samd51a/gcc/gcc"
  -Wl,--gc-sections 
  -T "${LINKER_FILE}"
)

add_subdirectory(./CommonCO3   build/CommonCO3)
add_subdirectory(./examples    build/examples)
add_subdirectory(./hal         build/hal)
add_subdirectory(./printf      build/printf)
add_subdirectory(./stdio_redirect   build/stdio_redirect)
add_subdirectory(./usb         build/usb)
add_subdirectory(./samd51a     build/samd51a)
add_subdirectory(./hpl         build/hpl)


target_link_libraries(
  ${EXECUTABLE}
  CommonCO3
  examples
  hal
  #hpl
  printf
  usb
  stdio_redirect
  samd51a
  hpl
  ${CO3_TARGET_INSTRUMENTED_LIB}
)

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}
)

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.s19"
  COMMAND ${CMAKE_OBJCOPY} -O ihex "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex"
  COMMAND ${CMAKE_OBJCOPY} -O binary "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin" 
)
