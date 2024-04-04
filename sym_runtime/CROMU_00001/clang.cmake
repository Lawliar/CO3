# Call Cmake from the 'build' subfolder with the command below.
# For using Make:
# cmake -DCMAKE_MAKE_PROGRAM=make.exe -DCMAKE_TOOLCHAIN_FILE="arm-none-eabi-gcc.cmake" -G "Unix Makefiles" ..
# followed by
# 'make' or 'cmake --build .' to build it

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CLANG_TOOLCHAIN_BIN_DIR ${CMAKE_SOURCE_DIR}/../../deps/llvm/clang+llvm-14.0.0-x86_64-linux-gnu-ubuntu-18.04/bin)

set(CMAKE_C_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang")
set(CMAKE_CXX_COMPILER "${CLANG_TOOLCHAIN_BIN_DIR}/clang++")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
