
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


# file: Collect all files that need to be compiled. 
# You can use a GLOB function as shown here, or explicitly mention the specific files
#file(GLOB FILES *.c *.h)


file(GLOB FILESTARGET
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c 
  ./STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
)

# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(Drivers OBJECT ${FILESTARGET})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  Drivers
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  Drivers
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)
