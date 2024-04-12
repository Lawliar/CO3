cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)
option(CO3_32BIT "Build for 32-bit target" OFF)
# enable export of compiler commands to compile_commands.json
#set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to toolchain file. This one has to be before 'project()' below

set(CMAKE_TOOLCHAIN_FILE clang.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CO3CROMU_00001_Workstation C)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CO3_INSTRUMENTATION_LIB "/home/lcm/github/spear/spear-code/pass/symbolizer/build_workstation/libSymbolize.so")
set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results")

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
  ${CMAKE_SOURCE_DIR}/../Common/*.c
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

