# file: Collect all files that need to be compiled. 
# You can use a GLOB function as shown here, or explicitly mention the specific files
#file(GLOB FILES *.c *.h)

file(GLOB FILES
  ./src/*.c
 )

# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(TargetEApp OBJECT ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  TargetEApp
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  CommonCO3
  PUBLIC
  ./
  ../CMSIS
  ../source
  ../board
  ../device
  ../drivers
  ./inc
  ${CommonDIR}/Protocol
  ${CommonDIR}/SymRuntime
  ${FREERTOS_DIR}/include
  ${FREERTOS_DIR}/portable/GCC/ARM_CM4F

)
