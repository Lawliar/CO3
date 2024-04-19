
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

# Call Cmake from the 'build' subfolder with the command below.
# For using Make:
# cmake -DCMAKE_MAKE_PROGRAM=make.exe -DCMAKE_TOOLCHAIN_FILE="arm-none-eabi-gcc.cmake" -G "Unix Makefiles" ..
# followed by
# 'make' or 'cmake --build .' to build it

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)


set(CLANG_TOOLCHAIN_BIN_DIR ${CMAKE_SOURCE_DIR}/../../../deps/llvm/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04/bin)

set(ARM_TOOLCHAIN_BIN_DIR  /home/lcm/github/toolchains/arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin)




set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(ARM_TOOLCHAIN_PREFIX "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-")

set(ARM_GNU_CC "${ARM_TOOLCHAIN_PREFIX}gcc")
#set(CMAKE_C_COMPILER "${ARM_TOOLCHAIN_PREFIX}gcc")
#set(CMAKE_CXX_COMPILER "${ARM_TOOLCHAIN_PREFIX}g++")

set(CMAKE_C_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang")
set(CMAKE_CXX_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang++")

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})



set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
