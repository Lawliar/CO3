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
option(CO3_32BIT "Build for 32-bit target" OFF)
option(CO3_NO_SHADOW "Build without app-side shadow" OFF)
option(CO3_DOCKER_BUILD "build with docker" OFF)
# enable export of compiler commands to compile_commands.json
#set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to toolchain file. This one has to be before 'project()' below

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
if(${CO3_DOCKER_BUILD})
  set(CMAKE_C_COMPILER "clang-14")
  set(CMAKE_CXX_COMPILER "clang++-14")
  set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
else()
  set(CLANG_TOOLCHAIN_BIN_DIR ${CMAKE_SOURCE_DIR}/../../deps/llvm/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04/bin)
  set(CMAKE_C_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang")
  set(CMAKE_CXX_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang++")
  set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CO3CROMU_00001_Workstation C)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CO3_INSTRUMENTATION_LIB "${CMAKE_SOURCE_DIR}/../../pass/symbolizer/build_workstation/libSymbolize.so")
if(${CO3_NO_SHADOW})
  set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results_no_shadow")
else()
  set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results")
endif()

enable_language(C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

if(${CO3_32BIT})
  add_compile_definitions(CO3_32BIT)
endif()


# Optional: issue a message to be sure it uses the correct toolchain file.
FILE(GLOB SRC_INSTRUMENTATION_FILES
  ${CMAKE_SOURCE_DIR}/Src/*.c
)
FILE(GLOB SRC_UNINSTRUMENTATION_FILES
  ${CMAKE_SOURCE_DIR}/UninstrumentedSrc/*.c
)
FILE(GLOB RUNTIME_SRC 
  ${CMAKE_SOURCE_DIR}/../Common/runtime.c
  ${CMAKE_SOURCE_DIR}/../Common/wrapped_malloc.c
  ${CMAKE_SOURCE_DIR}/../Common/shadow.c
)
set_property(SOURCE ${SRC_INSTRUMENTATION_FILES} APPEND PROPERTY COMPILE_OPTIONS
            #"-gdwarf-4" # to match with gcc
            "-Xclang" "-load"
            "-Xclang" "${CO3_INSTRUMENTATION_LIB}"
            "-Xclang" "-fpass-plugin=${CO3_INSTRUMENTATION_LIB}" "-mllvm" "-out=${CO3_OUTPUT_DIR}"
)



include_directories(
  ${CMAKE_SOURCE_DIR}/Inc
  ${CMAKE_SOURCE_DIR}/../Common
  ${CMAKE_SOURCE_DIR}/../../sym_backend/serialport
)

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_INSTRUMENTATION_FILES} ${SRC_UNINSTRUMENTATION_FILES} ${RUNTIME_SRC})

## some macro defintions
target_compile_definitions(${EXECUTABLE} PUBLIC CO3_PROJECT_DIR=${CO3_OUTPUT_DIR})

## flags
set(CFLAGS
  -g
  -Os
)

target_compile_options(
  ${EXECUTABLE} PRIVATE
  ${CFLAGS}
)

target_link_libraries(${EXECUTABLE} m)

