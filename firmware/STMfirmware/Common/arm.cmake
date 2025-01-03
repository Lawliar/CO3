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

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CO3_ROOT_DIR   ${CMAKE_CURRENT_LIST_DIR}/../../..)
set(CO3_COMMON_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CLANG_TOOLCHAIN_BIN_DIR ${CO3_ROOT_DIR}/deps/llvm/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04/bin)

set(ARM_TOOLCHAIN_BIN_DIR  ${CO3_ROOT_DIR}/deps/arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin)

# point ARM_TOOLCHAIN_BIN_DIR to things like
# "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2020-q4-major/bin")
# "C:/NXP/MCUXpressoIDE_11.7.0_9198/ide/tools/bin")
# on the system level, or have it defined here.
set(ARM_TOOLCHAIN_DIR ${ARM_TOOLCHAIN_BIN_DIR})
set(BINUTILS_PATH ${ARM_TOOLCHAIN_DIR}) 

set(TOOLCHAIN_PREFIX ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(ARM_GNU_CC ${CMAKE_C_COMPILER})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)