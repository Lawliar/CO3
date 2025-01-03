# This file is part of SymCC.
#
# SymCC is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# SymCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# SymCC. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)
project(SymBackend)

option(QSYM_BACKEND "Use the Qsym backend instead of our own" ON)
option(Z3_TRUST_SYSTEM_VERSION "Use the system-provided Z3 without a version check" ON)
option(RELEASE_BUILD "BUILD for performance" ON)
option(DEBUG_BUILD "BUILD for adding debug macros" OFF)
option(PROFILING_BUILD "BUILD for profiling" OFF)
option(CO3_NO_MCU_SHADOW "MCU does not have SHADOW, so disable some sanity checks that helps debugging" OFF)
option(CO3_SER2NET "use serial2net instead of the usb serial device, if specified 0, Unix socket will be used" ON)
option(CO3_REPLACE "Replace the majority of SymCC's operation with CO3's optimization(should be used in combination with symbolizer, MCU's runtime). This should be turned ON by default for better performance" ON)
option(CO3_32BIT "Use 32-bit integers for the runtime" OFF)


set(CO3_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(LIBSERIALPORTS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/libserialport")
set(LIBSERIALPORTS_LIB_DIR "${LIBSERIALPORTS_INCLUDE_DIR}/.libs")

set(CO3_BOOST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/boost")
set(CO3_BOOST_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/boost/stage/lib")

set(HIREDIS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/hiredis")
set(HIREDIS_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../deps/hiredis/build")

if(${CO3_32BIT})
  add_compile_definitions(CO3_32BIT)
endif()

set(Z3_DIR "${PROJECT_SOURCE_DIR}/../deps/z3/build/install/lib/cmake")
set(LLVM_DIR "${PROJECT_SOURCE_DIR}/../deps/llvm/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)



if(${CO3_REPLACE})
  add_compile_definitions(CO3_REPLACE)
else()
  set(CO3_NO_MCU_SHADOW ON CACHE INTERNAL "no replace means no MCU side shadow too")
endif()

if(${CO3_SER2NET})
  add_compile_definitions(CO3_SER2NET)
endif()


if(${CO3_NO_MCU_SHADOW})
  add_compile_definitions(CO3_NO_MCU_SHADOW)
endif()

if (${PROFILING_BUILD})
  add_compile_definitions(GPROFILING)
endif()
if (${RELEASE_BUILD})
  set(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
elseif(${DEBUG_BUILD})
  set(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  add_compile_definitions(DEBUG_OUTPUT)
  add_compile_definitions(DEBUG_CHECKING)
endif()



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 \
-Wredundant-decls -Wcast-align -g \
-Wmissing-include-dirs -Wswitch-default \
-Wextra -Wall -Winvalid-pch -Wredundant-decls -Wformat=2 \
-Wmissing-format-attribute -Wformat-nonliteral")



# Place the final product in the top-level output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# There is list(TRANSFORM ... PREPEND ...), but it's not available before CMake 3.12.
set(SHARED_RUNTIME_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeCommon.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LibcWrappers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Shadow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GarbageCollection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Orchestrator.cpp)

set(SERIAL_PORT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/transmit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/co3_serialport.c
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/co3_socket.c
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/ring.c
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/ring.h
        ${CMAKE_CURRENT_SOURCE_DIR}/serialport/MsgQueue.cpp
)
if(${CO3_REPLACE})
  set(SERIAL_PORT_SOURCES  ${SERIAL_PORT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/serialport/Message.cpp)
  set(SHARED_RUNTIME_SOURCES ${SHARED_RUNTIME_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/ProcessMessages.cpp)
else ()
  set(SERIAL_PORT_SOURCES  ${SERIAL_PORT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/serialport/Message_no_replace.cpp)
  set(SHARED_RUNTIME_SOURCES ${SHARED_RUNTIME_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/ProcessMessages_no_replace.cpp)
endif ()
set(SERIAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/serialport)



set(GRAPH_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/graph/RuntimeDataFlowGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graph/RuntimeCFG.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graph/SymGraph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graph/SymGraph.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../pass/symbolizer/dfg/DataFlowGraph.cpp)
set(GRAPH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../pass/symbolizer/dfg)
set(BACKEND_GRAPH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/graph)
if (${QSYM_BACKEND})
  add_subdirectory(qsym_backend)
else()
  add_subdirectory(simple_backend)
endif()
