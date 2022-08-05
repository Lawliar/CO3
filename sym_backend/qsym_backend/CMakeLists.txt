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

# Build the parts of the Qsym backend that are relevant for us

set(QSYM_SOURCE_DIR "qsym/qsym/pintool")
set(boost_hints "/usr/local/lib/cmake")

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/expr__gen.cpp
  COMMAND python2 gen_expr.py ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${QSYM_SOURCE_DIR}/codegen
  COMMENT "Generating Qsym's expr__gen.cpp"
  VERBATIM
  DEPENDS
  ${QSYM_SOURCE_DIR}/codegen/expr.cpp
  ${QSYM_SOURCE_DIR}/codegen/gen_expr.py)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/expr_builder__gen.cpp
  COMMAND python2 gen_expr_builder.py ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${QSYM_SOURCE_DIR}/codegen
  COMMENT "Generating Qsym's expr__gen.cpp"
  VERBATIM
  DEPENDS
  ${QSYM_SOURCE_DIR}/codegen/expr_builder.cpp
  ${QSYM_SOURCE_DIR}/codegen/gen_expr_builder.py)

find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_DIR})
add_definitions(${LLVM_DEFINITIONS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

# Qsym doesn't work with old versions of Z3
find_package(Z3 4.5 CONFIG HINTS ${Z3_DIR})
if (NOT Z3_FOUND)
  if (NOT Z3_TRUST_SYSTEM_VERSION)
    message(FATAL_ERROR "Couldn't locate Z3. \
If you want me to trust that a suitable version is available nonetheless, \
configure CMake with -DZ3_TRUST_SYSTEM_VERSION=on (see also docs/Configuration.txt).")
  else()
    set(Z3_LIBRARIES "z3")
  endif()
endif()

## BOOST
set(Boost_DEBUG 1)
find_package( Boost REQUIRED HINTS ${boost_hints})
if(NOT Boost_FOUND)
  message( SEND_ERROR "Boost Library not found" )
endif()
message(STATUS "Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")

include_directories( ${Boost_INCLUDE_DIR} )

## Serial Port Source
include_directories(${SERIAL_INCLUDE_DIR})

## libserialport
find_package(PkgConfig REQUIRED)


add_executable(orchestrator
  ${CMAKE_CURRENT_BINARY_DIR}/expr__gen.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/expr_builder__gen.cpp
  ${QSYM_SOURCE_DIR}/expr_cache.cpp
  ${QSYM_SOURCE_DIR}/expr_evaluate.cpp
  ${QSYM_SOURCE_DIR}/solver.cpp
  ${QSYM_SOURCE_DIR}/dependency.cpp
  ${QSYM_SOURCE_DIR}/logging.cpp
  ${QSYM_SOURCE_DIR}/afl_trace_map.cpp
  ${QSYM_SOURCE_DIR}/allocation.cpp
  ${QSYM_SOURCE_DIR}/call_stack_manager.cpp
  ${QSYM_SOURCE_DIR}/third_party/llvm/range.cpp
  ${QSYM_SOURCE_DIR}/third_party/xxhash/xxhash.cpp
  ${SHARED_RUNTIME_SOURCES}
  ${SERIAL_PORT_SOURCES}
  ${GRAPH_SOURCES}
  Runtime.cpp ../ThreadPool.h ../graph/Val.cpp ../graph/Val.h ../getTimeStamp.h ../getTimeStamp.cpp)


## even when LIBSERIALPORT is found by pkg_check_modules, no related variables are set, so we have to do this in this ugly way, thanks cmake!!
## when i tried to manually run pkg-config with execute_process, and find_library. While libserialport is literally under /usr/local/lib, and I specify this as hints to the find_library , it gave me NOTFOUND, this has been truly amazing.
target_include_directories(orchestrator PRIVATE "/usr/local/include/")
target_link_libraries(orchestrator "/usr/local/lib/libserialport.so")

##graph
target_include_directories(orchestrator PRIVATE ${GRAPH_INCLUDE_DIR})
target_include_directories(orchestrator PRIVATE ${RUNTIME_GRAPH_INCLUDE_DIR})
## link boost
target_link_libraries( orchestrator LINK_PUBLIC ${Boost_LIBRARIES} )
target_link_libraries( orchestrator boost_graph )
target_link_libraries( orchestrator boost_filesystem )
target_link_libraries( orchestrator boost_program_options)

if (${PROFILING_BUILD})
  target_link_libraries( orchestrator profiler)
endif()

target_include_directories(orchestrator PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}     # for our fake pin.H and Runtime.h
  ${CMAKE_CURRENT_SOURCE_DIR}/..) # for common runtime components


target_include_directories(orchestrator SYSTEM PRIVATE
  ${Z3_CXX_INCLUDE_DIRS}        # for Z3 support
  ${QSYM_SOURCE_DIR})
# We need to get the LLVM support component for llvm::APInt.
llvm_map_components_to_libnames(QSYM_LLVM_DEPS support)

target_link_libraries(orchestrator ${Z3_LIBRARIES} ${QSYM_LLVM_DEPS})
# We use std::filesystem, which has been added in C++17. Before its official
# inclusion in the standard library, Clang shipped the feature first in
# libc++experimental and later in libc++fs; GCC used to require libstdc++fs. We
# make no attempt to support those older compiler versions, with one exception:
# some current LTS distributions ship a GCC that requires libstdc++fs for
# std::filesystem - we catch this case in order to enable users of such systems
# to build with the default compiler.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_link_libraries(orchestrator stdc++fs)
endif()
