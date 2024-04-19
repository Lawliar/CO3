

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


set(CO3_COMMON_FILES
        ${CommonDIR}/Protocol/monitor.c
        ${CommonDIR}/Protocol/protocol.c
        ${CommonDIR}/Protocol/ring.c
        ${CommonDIR}/SymRuntime/wrapped_malloc.c
        ${CommonDIR}/SymRuntime/FreeRTOS_callbacks.c
        ${CommonDIR}/FreeRTOS-Kernel/tasks.c
        ${CommonDIR}/FreeRTOS-Kernel/stream_buffer.c
        ${CommonDIR}/FreeRTOS-Kernel/queue.c
        ${CommonDIR}/FreeRTOS-Kernel/croutine.c
        ${CommonDIR}/FreeRTOS-Kernel/event_groups.c
        ${CommonDIR}/FreeRTOS-Kernel/timers.c
        ${CommonDIR}/FreeRTOS-Kernel/list.c
        ${CommonDIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
        ${CommonDIR}/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)
if(${CO3_REPLACE})
 set(CO3_COMMON_FILES ${CO3_COMMON_FILES} "${CommonDIR}/SymRuntime/runtime.c")
else()
 set(CO3_COMMON_FILES ${CO3_COMMON_FILES} "${CommonDIR}/SymRuntime/runtime_no_replace.c")
endif()
# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(CommonCO3 OBJECT ${CO3_COMMON_FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
    CommonCO3
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  CommonCO3
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)
