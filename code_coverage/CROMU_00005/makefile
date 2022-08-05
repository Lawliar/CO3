ROOT_DIR := $(abspath ..)
PROJ_DIR := $(abspath .)
PROJ_NAME := $(notdir $(PROJ_DIR))

OUTPUT_DIR := $(PROJ_DIR)/intermediate_results
Q ?= @


# CFLAGS which can provided from the CLI
# They will be appended last and will consequently override
# any pre-existing settings
CLI_CFLAG_OVERRIDES ?=
CLI_LDFLAG_OVERRIDES ?=

# Set things up to use the arm-none-eabi-gcc that is on
# our path by default but allow things to be easily overridden, i.e:
#
# COMPILER=<PATH_TO_OTHER_GCC_VERSION>/arm-none-eabi-gcc make


CLANG_DIR           := /home/lcm/github/toolchains/x86_64/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04
OPT                 := ${CLANG_DIR}/bin/opt
LLC                 := ${CLANG_DIR}/bin/llc
INSTRUMENTATION_LIB := /home/lcm/github/spear/spear-code/code_coverage/compiler/build/libCoverageInstrument.so
RUNTIME_DIR         := /home/lcm/github/spear/spear-code/code_coverage/runtime/build

NATIVE_CLANG_COMPILER := ${CLANG_DIR}/bin/clang

LDFLAGS += -L"$(RUNTIME_DIR)" -lCoverageRuntime -Wl,-rpath,"$(RUNTIME_DIR)" -lm
BUILD_DIR := $(PROJ_DIR)/build


CC := $(NATIVE_CLANG_COMPILER)


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


SRC_FILES +=  $(COREMARK_HAL_SOURCES)


OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))



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
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) -O2  $(INCLUDE_PATHS) -S -emit-llvm -o $@_raw.ll $<
	$(Q) $(OPT) -load ${INSTRUMENTATION_LIB} --symbolize -o $@_ins.ll $@_raw.ll
	$(Q) $(LLC) -filetype=obj -o $@ $@_ins.ll