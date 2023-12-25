##############################################################################
# Compiler settings
#

TRGT = $(LLVM_TOOLCHAIN)
CC   = $(TRGT)/bin/clang
CPPC = $(TRGT)/bin/clang++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = arm-none-eabi-gcc
CP   = arm-none-eabi-objcopy
AS   = $(CC) -x assembler-with-cpp
AR   = arm-none-eabi-ar
OD   = arm-none-eabi-objdump
SZ   = arm-none-eabi-size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

#
# Compiler settings
##############################################################################
