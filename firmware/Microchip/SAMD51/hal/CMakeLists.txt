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

file(GLOB FILES
  src/hal_atomic.c
  src/hal_cache.c
  src/hal_delay.c
  src/hal_gpio.c
  src/hal_init.c
  src/hal_io.c
  src/hal_sleep.c
  src/hal_usb_device.c
  src/hal_usart_sync.c
  utils/src/utils_assert.c
  utils/src/utils_event.c
  utils/src/utils_list.c
  utils/src/utils_syscalls.c
)

# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(hal OBJECT  ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  hal
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  hal
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)
