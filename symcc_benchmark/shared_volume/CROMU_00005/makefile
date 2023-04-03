ROOT_DIR := $(abspath ..)
PROJ_DIR := $(abspath .)
PROJ_NAME := $(notdir $(PROJ_DIR))

OUTPUT_DIR := $(PROJ_DIR)/intermediate_results
Q ?= @


INSTRUMENT := true

# CFLAGS which can provided from the CLI
# They will be appended last and will consequently override
# any pre-existing settings
CLI_CFLAG_OVERRIDES ?=
CLI_LDFLAG_OVERRIDES ?=

# Set things up to use the arm-none-eabi-gcc that is on
# our path by default but allow things to be easily overridden, i.e:
#
# COMPILER=<PATH_TO_OTHER_GCC_VERSION>/arm-none-eabi-gcc make
SYMCC_BUILD_DIR := /home/lcm/github/spear/symcc_latest/build

INSTRUMENTATION_LIB := $(SYMCC_BUILD_DIR)/libSymbolize.so 
RUNTIME_DIR         := $(SYMCC_BUILD_DIR)/SymRuntime-prefix/src/SymRuntime-build
Z3_BUILD_DIR        := /home/lcm/github/tools/z3/build
NATIVE_CLANG_COMPILER := /home/lcm/github/toolchains/x86_64/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04/bin/clang

ifdef INSTRUMENT
CLANG_COMPILER      := $(SYMCC_BUILD_DIR)/symcc
LDFLAGS += -L"$(RUNTIME_DIR)" -L"$(Z3_BUILD_DIR)" -Wl,-rpath,"$(RUNTIME_DIR)" -Wl,-rpath,"$(Z3_BUILD_DIR)" -lSymRuntime -lz3  -lm
BUILD_DIR := $(PROJ_DIR)/build
else
CLANG_COMPILER      := $(NATIVE_CLANG_COMPILER)
BUILD_DIR := $(PROJ_DIR)/build_native
LDFLAGS += -lm
endif 


CC := $(CLANG_COMPILER)


$(info $(CC))

## Coremark Dir, include, sources
COREMARK_HAL_DIR := $(PROJ_DIR)

COREMARK_HAL_INCLUDE_DIR := \
-I$(COREMARK_HAL_DIR)/Inc

COREMARK_HAL_SOURCES := \
$(COREMARK_HAL_DIR)/Src/libcgc.c \
$(COREMARK_HAL_DIR)/Src/mymath.c \
$(COREMARK_HAL_DIR)/Src/printf.c \
$(COREMARK_HAL_DIR)/Src/service.c \
$(COREMARK_HAL_DIR)/Src/stdlib.c 

UNINSTRUMENTED_DIR := $(PROJ_DIR)

UNINSTRUMENTED_SRC := $(UNINSTRUMENTED_DIR)/UninstrumentedSrc/wrapped_malloc.c

SRC_FILES +=  $(COREMARK_HAL_SOURCES) \
$(UNINSTRUMENTED_SRC)


OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

CFLAGS := -Os

INCLUDE_PATHS +=  $(COREMARK_HAL_INCLUDE_DIR)


TARGET_ELF = $(BUILD_DIR)/out


all: $(TARGET_ELF)
	 
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR): 
	@mkdir -p $(BUILD_DIR)

$(TARGET_ELF): $(OBJ_FILES)  $(LDSCRIPT)
	@echo "Linking library"
  #$(Q) $(CC) $(OBJ_FILES)  $(LDFLAGS) -o $@
	$(Q) $(CC) $(OBJ_FILES)  $(LDFLAGS) -o $@

$(OBJ_FILES): $(SRC_FILES) makefile 

#build instrumented coremark
$(BUILD_DIR)/$(PROJ_NAME)/Src/%.o: $(PROJ_DIR)/Src/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	@mkdir -p $(OUTPUT_DIR)
	$(Q) $(CLANG_COMPILER)  $(INCLUDE_PATHS) -c -g -o $@ $<

# build uninstrumented code
$(BUILD_DIR)/$(PROJ_NAME)/UninstrumentedSrc/%.o: $(PROJ_DIR)/UninstrumentedSrc/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(NATIVE_CLANG_COMPILER)  $(INCLUDE_PATHS) -c -g -o $@ $<