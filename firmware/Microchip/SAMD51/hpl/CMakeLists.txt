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
add_library(hpl OBJECT  ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  hpl
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  hpl
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)

