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

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../STMfirmware/Common/arm.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Setup project, output and linker file
set(BUILD_FOLDER ${CMAKE_BINARY_DIR})
project(EVKMIMXRT1176_CMake)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(LINKER_FILE "frdmk66f_dev_cdc_vcom_freertos_Debug.ld")



set(CO3_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/intermediate_results")
set(CO3_INSTRUMENTATION_LIB "${CO3_ROOT_DIR}/pass/symbolizer/build_replace/libSymbolize.so")
set(CO3_TARGET_INSTRUMENTED_LIB "${CMAKE_SOURCE_DIR}/Target/build/libEVKMIMXRT1176_Target_CMake.a")


enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)


set(ARCH_CFLAGS
  -mthumb -mcpu=cortex-m4 -mfloat-abi=hard  -mfpu=fpv4-sp-d16 
) 

#set(START_UP_LIB
#  ${CMAKE_SOURCE_DIR}/startup/build/libstartup.a
#)
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
  source/printhelpers.c
  source/virtual_com.c
  source/usb_device_descriptor.c
  source/virtual_com.c
  startup/startup_mk66f18.c  
  device/system_MK66F18.c
)

include_directories(
./usb/device/class
./usb/device/class/cdc
./CommonCO3
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


message(STATUS "ARM_CORTEXM_SYSROOT:${ARM_CORTEXM_SYSROOT}")
message(STATUS "COMPILER_SPECIFIC_LDFLAGS:${COMPILER_SPECIFIC_LDFLAGS}")
message(STATUS "ARM_CORTEXM_BUILTINS_LIBGCC:${ARM_CORTEXM_BUILTINS_LIBGCC}")
#message(STATUS "CMAKE_C_LINK_EXECUTABLE:${CMAKE_C_LINK_EXECUTABLE}")


# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})
set(CO3_OPTIMIZATION_FLAG -Os)
# List of compiler defines, prefix with -D compiler option. Copy from the project settings

set(CO3_COMPILE_OPTIONS
-DCPU_MK66FN2M0VMD18 
-DCPU_MK66FN2M0VMD18_cm4 
-DSDK_OS_FREE_RTOS 
-DSERIAL_PORT_TYPE_UART=1 
-D_DEBUG=1 
-DSDK_DEBUGCONSOLE=1 
-DUSB_STACK_BM 
-DFSL_OSA_BM_TASK_ENABLE=0 
-DFSL_OSA_BM_TIMER_CONFIG=0 
-DMCUXPRESSO_SDK 
-DSDK_OS_BAREMETAL 
-DCR_INTEGER_PRINTF 
-D__MCUXPRESSO 
-D__USE_CMSIS 
-DDEBUG
-D__NEWLIB__
-std=gnu99 
${CO3_OPTIMIZATION_FLAG}
-g 
-c 
-ffunction-sections 
-fdata-sections 
-fno-builtin 
-Wall -fstack-usage  
-mthumb
${ARCH_CFLAGS} 
-fno-common 
#-specs=nano.specs
#-specs=nosys.specs
)
if(${LLVM_BUILD})
  set(CO3_COMPILE_OPTIONS ${CO3_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS})
endif()

add_compile_options(
  ${CO3_COMPILE_OPTIONS}
)

target_compile_definitions(${EXECUTABLE} PRIVATE

-DCPU_MK66FN2M0VMD18 
-DCPU_MK66FN2M0VMD18_cm4 
-DSDK_OS_FREE_RTOS 
-DSERIAL_PORT_TYPE_UART=1 
-D_DEBUG=1 
-DSDK_DEBUGCONSOLE=1 
-DUSB_STACK_BM 
-DFSL_OSA_BM_TASK_ENABLE=0 
-DFSL_OSA_BM_TIMER_CONFIG=0 
-DMCUXPRESSO_SDK 
-DSDK_OS_BAREMETAL 
-DCR_INTEGER_PRINTF 
-D__MCUXPRESSO 
-D__USE_CMSIS 
-DDEBUG
-D__NEWLIB__ 

)

set(CO3_TARGET_COMPILE_OPTIONS
-std=gnu99
${CO3_OPTIMIZATION_FLAG}
-g 
-gdwarf-4 
-c 
-ffunction-sections 
-fdata-sections 
-fno-builtin 
-mthumb
-Wall -fstack-usage  
${ARCH_CFLAGS}
-fno-common 
#-specs=nano.specs
#-specs=nosys.specs
)
if(${LLVM_BUILD})
  set(CO3_TARGET_COMPILE_OPTIONS ${CO3_TARGET_COMPILE_OPTIONS} ${COMPILER_SPECIFIC_CFLAGS})
endif()

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
${CO3_TARGET_COMPILE_OPTIONS} 
)

if(${LLVM_BUILD})
  set(CMAKE_C_LINK_EXECUTABLE 
    "${ARM_GNU_CC} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
  )
endif()
# Linker options
target_link_options(${EXECUTABLE} PRIVATE
  -g
  -Wall
  -Wl,--no-warn-mismatch
  -fno-common 
  -ffunction-sections 
  -fdata-sections 
  #-ffreestanding 
  -fno-builtin 
  -nostdlib 
  #${START_UP_LIB}
  -Xlinker -Map=${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.map
  ${ARCH_CFLAGS}
  -specs=nosys.specs
  -L "${CMAKE_SOURCE_DIR}/ld"
  -T "${LINKER_FILE}"
)


set(CommonDIR ${CO3_COMMON_DIR})

add_subdirectory(./CommonCO3   build/CommonCO3)
#add_subdirectory(./Target      build/Target)
add_subdirectory(./board       build/board)
add_subdirectory(./component   build/component)
add_subdirectory(./drivers     build/drivers)
add_subdirectory(./freertos    build/freertos)
add_subdirectory(./usb         build/usb)
add_subdirectory(./utilities   build/utilities)


target_link_libraries(
  ${EXECUTABLE}
  utilities
  board
  component
  drivers
  freertos
  usb
  CommonCO3
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
