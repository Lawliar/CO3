# file: Collect all files that need to be compiled. 
# You can use a GLOB function as shown here, or explicitly mention the specific files
cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../arm.cmake)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SAMD51_CMake_HPL)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(ARCH_CFLAGS
  -mthumb -mcpu=cortex-m4 -mfloat-abi=hard  -mfpu=fpv4-sp-d16 
) 

file(GLOB FILES
  cmcc/hpl_cmcc.c
  core/hpl_core_m4.c
  core/hpl_init.c
  dmac/hpl_dmac.c
  gclk/hpl_gclk.c
  mclk/hpl_mclk.c
  osc32kctrl/hpl_osc32kctrl.c
  oscctrl/hpl_oscctrl.c
  pm/hpl_pm.c
  ramecc/hpl_ramecc.c
  sercom/hpl_sercom.c
  usb/hpl_usb.c
)

# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(hpl ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  hpl
)

set(ALL_NINE_YARDS_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/../usb
  ${PROJECT_SOURCE_DIR}/../usb/class/cdc
  ${PROJECT_SOURCE_DIR}/../hpl/port
  ${PROJECT_SOURCE_DIR}/../hpl/osc32kctrl
  ${PROJECT_SOURCE_DIR}/../hpl/oscctrl
  ${PROJECT_SOURCE_DIR}/../hpl/cmcc
  ${PROJECT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/../hal/utils/include
  ${PROJECT_SOURCE_DIR}/../hal/include
  ${PROJECT_SOURCE_DIR}/../hpl/pm
  ${PROJECT_SOURCE_DIR}/../usb/class/cdc/device
  ${PROJECT_SOURCE_DIR}/../usb/device
  ${PROJECT_SOURCE_DIR}/../hri
  ${PROJECT_SOURCE_DIR}/../hpl/gclk
  ${PROJECT_SOURCE_DIR}/../hpl/mclk
  ${PROJECT_SOURCE_DIR}/../hpl/core
  ${PROJECT_SOURCE_DIR}/../hpl/ramecc
  ${PROJECT_SOURCE_DIR}/../hpl/dmac
  ${PROJECT_SOURCE_DIR}/../examples
  ${PROJECT_SOURCE_DIR}/../Target
  ${PROJECT_SOURCE_DIR}/../hpl/usb
  ${PROJECT_SOURCE_DIR}/../config
  ${PROJECT_SOURCE_DIR}/../CMSIS/Core/Include
  ${PROJECT_SOURCE_DIR}/../stdio_redirect
  ${PROJECT_SOURCE_DIR}/../samd51a/include
  ${PROJECT_SOURCE_DIR}/../CMSIS/Core/Include
  ${CommonDIR}/Protocol
  ${CommonDIR}/SymRuntime
  ${CommonDIR}/FreeRTOS-Kernel
  ${CommonDIR}/FreeRTOS-Kernel/include
  ${CommonDIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
)

target_include_directories(
  hpl
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)

add_compile_options(
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -O1
  -g 
  -ffunction-sections 
  -mlong-calls 
  -Wall
  -g3
  ${ARCH_CFLAGS} 
)

target_compile_options(
  hpl PRIVATE
  -D__SAMD51J19A__ 
  -x c 
  -c
  -DXPRJ_Default=Default
  -O1
  -g 
  -ffunction-sections 
  -mlong-calls 
  -Wall
  -g3
  ${ARCH_CFLAGS} 
)