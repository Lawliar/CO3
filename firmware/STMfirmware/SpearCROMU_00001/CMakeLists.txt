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

# Path to toolchain file. This one has to be before 'project()' below

set(CO3_INSTRUMENT ON)
set(CO3_REPLACE ON)


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../Common/clang.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Setup project, output and linker file
project(CO3CROMU_00001_CMake C ASM)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LINKER_FILE "${PROJECT_SOURCE_DIR}/STM32H743ZITX_FLASH.ld")



if(${CO3_REPLACE})
  set(CO3_INSTRUMENTATION_LIB "${CO3_ROOT_DIR}/pass/symbolizer/build_release/libSymbolize.so")
  set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results")
else()
  set(CO3_INSTRUMENTATION_LIB "${CO3_ROOT_DIR}/pass/symbolizer/build_no_replace/libSymbolize.so")
  set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results_no_replace")
endif() 

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

set(ARCH_CFLAGS
  -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
) 


set(CommonDIR ${CO3_COMMON_DIR})

set(SRC_C_FILES
  Core/Src/main.c 
  Core/Src/stm32h7xx_hal_timebase_tim.c 
  Core/Src/syscalls.c 
  Core/Src/system_stm32h7xx.c 
  Core/Src/stm32h7xx_hal_msp.c 
  Core/Src/stm32h7xx_it.c 
  Core/Src/sysmem.c
)

set(SRC_ASM_FILES
  Core/Startup/startup_stm32h743zitx.s
)
set_property(SOURCE ${SRC_ASM_FILES} APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")

set(ALL_NINE_YARDS_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/CommonCO3
  ${PROJECT_SOURCE_DIR}/Core/Inc
  ${PROJECT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
  ${PROJECT_SOURCE_DIR}/Middlewares//ST/STM32_USB_Device_Library/Class/CDC/Inc
  ${PROJECT_SOURCE_DIR}/USB_DEVICE/App
  ${PROJECT_SOURCE_DIR}/USB_DEVICE/Target
  ${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
  ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
  ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include
  ${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
  ${CommonDIR}/Protocol
  ${CommonDIR}/SymRuntime
  ${CommonDIR}/FreeRTOS-Kernel/include
  ${CommonDIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
  ${PROJECT_SOURCE_DIR}/Target/Inc
)

include_directories(
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_C_FILES} ${SRC_ASM_FILES})


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

get_filename_component(
  ARM_CORTEXM_BUILTINS_LIBGCC_FOLDER
  ${ARM_CORTEXM_BUILTINS_LIBGCC}
  DIRECTORY
)

set(CFLAGS
  -Os
  -std=gnu11
  -ffunction-sections
  -fdata-sections
  -Wall
  -Werror 
  -nostdlib 
  -DUSE_HAL_DRIVER 
  -DSTM32H743xx 
  ${COMPILER_SPECIFIC_CFLAGS} 
  ${ARCH_CFLAGS}
)
message(STATUS "ARM_CORTEXM_BUILTINS_LIBGCC_FOLDER:${ARM_CORTEXM_BUILTINS_LIBGCC_FOLDER}")
message(STATUS "COMPILER_SPECIFIC_CFLAGS:${COMPILER_SPECIFIC_CFLAGS}")
message(STATUS "COMPILER_SPECIFIC_LDFLAGS:${COMPILER_SPECIFIC_LDFLAGS}")
message(STATUS "ARM_CORTEXM_BUILTINS_LIBGCC:${ARM_CORTEXM_BUILTINS_LIBGCC}")
message(STATUS "CMAKE_C_LINK_EXECUTABLE:${CMAKE_C_LINK_EXECUTABLE}")

## not sure how long this hack is gonna be
set(CMAKE_C_LINK_EXECUTABLE 
  "${ARM_GNU_CC} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
)

set(LDFLAGS
#  ${COMPILER_SPECIFIC_CFLAGS}
#  ${COMPILER_SPECIFIC_LDFLAGS}
  ${ARCH_CFLAGS} 
  -T${LINKER_FILE}
  -fno-short-enums 
  -Wl,-Map=${PROJECT_NAME}.map 
  -Wl,--no-warn-mismatch 
  -Wl,--gc-sections -static 
  -Wl,--no-undefined 
  --specs=nano.specs
  -u _printf_float 
)

add_compile_options(
  ${CFLAGS}
)

target_compile_options(
  ${EXECUTABLE} PRIVATE
  ${CFLAGS}
)


# Linker options
target_link_options(${EXECUTABLE} PRIVATE
  ${LDFLAGS}
)
set(CommonDIR /home/lcm/github/spear/spear-code/firmware/STMfirmware/Common)

add_subdirectory(./CommonCO3        ${EXECUTABLE_OUTPUT_PATH}/CommonCO3)
add_subdirectory(./Target           ${EXECUTABLE_OUTPUT_PATH}/Target)
add_subdirectory(./Drivers          ${EXECUTABLE_OUTPUT_PATH}/Drivers)
add_subdirectory(./Middlewares      ${EXECUTABLE_OUTPUT_PATH}/Middlewares)
add_subdirectory(./USB_DEVICE       ${EXECUTABLE_OUTPUT_PATH}/USB_DEVICE)


target_link_libraries(
  ${EXECUTABLE}
  CommonCO3
  Target
  Drivers
  Middlewares
  USB_DEVICE
)

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}
)

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex"
  COMMAND ${CMAKE_OBJCOPY} -O binary "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin" 
)
