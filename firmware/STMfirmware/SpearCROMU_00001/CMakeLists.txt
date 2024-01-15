cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# enable export of compiler commands to compile_commands.json
#set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE clang.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup project, output and linker file
set(BUILD_FOLDER ./build)
project(CO3CROMU_00001_CMake)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(EXECUTABLE_OUTPUT_PATH ./build)
set(LINKER_FILE "STM32H743ZITX_FLASH.ld")

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

set(CommonDIR /home/lcm/github/spear/spear-code/firmware/STMfirmware/Common)

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




set(ALL_NINE_YARDS_INCLUDE_DIRS
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
add_executable(${EXECUTABLE} ${SRC_FILES})

add_compile_options(
#  --target=arm-none-eabi
#  --sysroot=/home/lcm/github/toolchains/arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi
  -mcpu=cortex-m4
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -mthumb
  
  -O1
  -g 
  -ffunction-sections 
  -mlong-calls 
  -Wall

  -mfpu=fpv4-sp-d16 
  -mfloat-abi=hard 
  -mthumb
  -g3
)

target_compile_options(
  ${EXECUTABLE} PRIVATE
#  --target=arm-none-eabi
#  --sysroot=/home/lcm/github/toolchains/arm/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi
  -mcpu=cortex-m4
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -mthumb
  
  -O1
  -g 
  -ffunction-sections 
  -mlong-calls 
  -Wall

  -mfpu=fpv4-sp-d16 
  -mfloat-abi=hard 
  -mthumb
  -g3
)


# Linker options
target_link_options(${EXECUTABLE} PRIVATE
#  --target=arm-none-eabi 
#  --sysroot=/home/lcm/github/toolchains/arm/gcc-arm-none-eabi-10.3-2021.12/bin/../arm-none-eabi  
  -mcpu=cortex-m4  
  -D__SAMD51J19A__
  -mthumb 
  --specs=nosys.specs
  -Wl,-Map="SAMD51.map"
  -DXPRJ_Default=Default
  -mfpu=fpv4-sp-d16 
  -mfloat-abi=hard 
  -mthumb 
  -Wl,--defsym=__MPLAB_BUILD=1
  -L "${PROJECT_SOURCE_DIR}/samd51a/gcc/gcc"
  -Wl,--gc-sections 
  -T "${PROJECT_SOURCE_DIR}/samd51a/gcc/gcc/samd51j19a_flashMPU.ld"
  
)
set(CommonDIR /home/lcm/github/spear/spear-code/firmware/Microchip/Common)

add_subdirectory(./CommonCO3   build/CommonCO3)
add_subdirectory(./Target      build/Target)
add_subdirectory(./examples    build/examples)
add_subdirectory(./hal         build/hal)
add_subdirectory(./hpl         build/hpl)
add_subdirectory(./printf      build/printf)
add_subdirectory(./stdio_redirect   build/stdio_redirect)
add_subdirectory(./usb         build/usb)
add_subdirectory(./samd51a     build/samd51a)


target_link_libraries(
  ${EXECUTABLE}
  CommonCO3
  Target
  examples
  hal
  hpl
  printf
  usb
  stdio_redirect
  samd51a
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
