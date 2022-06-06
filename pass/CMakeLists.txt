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
project(SymInstrumentation)

set(llvm_hints ${LLVM_HINTS})
set(boost_hints "../deps/boost/install/lib/cmake")

find_package(LLVM REQUIRED CONFIG HINTS ${llvm_hints})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake from ${LLVM_DIR}")
#if (${LLVM_VERSION_MAJOR} LESS 8 OR ${LLVM_VERSION_MAJOR} GREATER 11)
#  message(WARNING "The software has been developed for LLVM 8 through 11; it is unlikely to work with other versions!")
#endif()

add_definitions(${LLVM_DEFINITIONS})
message(STATUS "LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})



FIND_PACKAGE( Boost REQUIRED HINTS ${boost_hints})
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 \
-Wredundant-decls -Wcast-align -Wmissing-include-dirs -Wswitch-default \
-Wextra -Wall -Winvalid-pch -Wredundant-decls -Wformat=2 \
-Wmissing-format-attribute -Wformat-nonliteral -Werror")

# Mark nodelete to work around unload bug in upstream LLVM 5.0+
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-z,nodelete")

# This is the compiler pass that we later load into clang or opt. If LLVM is
# built without RTTI we have to disable it for our library too, otherwise we'll
# get linker errors.
add_library(Symbolize MODULE
            Symbolizer.cpp
            Pass.cpp
            Runtime.cpp
            Main.cpp
            Lower.cpp
        dfg/DataFlowGraph.cpp)

if (NOT LLVM_ENABLE_RTTI)
  #set_target_properties(Symbolize PROPERTIES COMPILE_FLAGS "-fno-rtti")
  set_source_files_properties(Symbolizer.cpp PROPERTIES COMPILE_FLAGS "-fno-rtti")
  set_source_files_properties(Pass.cpp PROPERTIES COMPILE_FLAGS "-fno-rtti")
  set_source_files_properties(Runtime.cpp PROPERTIES COMPILE_FLAGS "-fno-rtti")
  set_source_files_properties(Main.cpp PROPERTIES COMPILE_FLAGS "-fno-rtti")
endif()

TARGET_LINK_LIBRARIES( Symbolize LINK_PUBLIC ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( Symbolize boost_graph )
TARGET_LINK_LIBRARIES( Symbolize boost_filesystem )

find_program(CLANG_BINARY "clang"
  HINTS ${LLVM_TOOLS_BINARY_DIR}
  DOC "The clang binary to use in the symcc wrapper script.")
find_program(CLANGPP_BINARY "clang++"
  HINTS ${LLVM_TOOLS_BINARY_DIR}
  DOC "The clang binary to use in the sym++ wrapper script.")
if (NOT CLANG_BINARY)
  message(FATAL_ERROR "Clang not found; please make sure that the version corresponding to your LLVM installation is available.")
endif()

#configure_file("compiler/symcc.in" "symcc" @ONLY)
#configure_file("compiler/sym++.in" "sym++" @ONLY)

