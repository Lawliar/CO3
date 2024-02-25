# file: Collect all files that need to be compiled. 
# You can use a GLOB function as shown here, or explicitly mention the specific files
#file(GLOB FILES *.c *.h)


file(GLOB FILESTARGET
  ./Src/libcgc.c 
  ./Src/mymath.c 
  ./Src/printf.c 
  ./Src/service.c
  ./Src/stdlib.c 
)


set_property(SOURCE ${FILESTARGET} APPEND PROPERTY COMPILE_OPTIONS 
#"-gdwarf-4" # to match with gcc
"-Xclang" "-load" 
"-Xclang" "${CO3_INSTRUMENTATION_LIB}" 
"-Xclang" "-fpass-plugin=${CO3_INSTRUMENTATION_LIB}" "-mllvm" "-out=${CO3_OUTPUT_DIR}"
)

# add_library: With this declaration, you express the intent to build a library. 
# The first argument, here its pico-shift-register, is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(Target OBJECT ${FILESTARGET})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(  
  Target
)

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  Target
  PUBLIC
  ${ALL_NINE_YARDS_INCLUDE_DIRS}
)
