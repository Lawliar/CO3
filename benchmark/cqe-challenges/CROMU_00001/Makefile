#POLLS_RELEASE_SEED= 1381088693
#POLLS_TESTING_SEED= 2104697472
#AUTHOR_ID  = CROMU
#SERVICE_ID = 00001
CFLAGS     := -O0 -Werror -Wno-overlength-strings -Wno-packed -fcommon -nostdlib -m32 -fno-builtin -w -g3
Q ?= @
CC := clang
PROJECT_DIR := $(abspath .)
PROJ_NAME := $(notdir $(PROJECT_DIR))
CGC_DIR := $(abspath ..)
BUILD_DIR  := $(PROJECT_DIR)/build

INCLUDE_DIR := -I$(CGC_DIR)/libcgc -I$(PROJECT_DIR)/lib

PRJ_SOURCE_DIR := $(PROJECT_DIR)/src/service.c \
$(PROJECT_DIR)/lib/stdlib.c \
$(PROJECT_DIR)/lib/printf.c \
$(PROJECT_DIR)/lib/mymath.c

LIBCGC_SOURCE_DIR := $(CGC_DIR)/libcgc/libcgc.c

SRC_FILES := $(LIBCGC_SOURCE_DIR) $(PRJ_SOURCE_DIR)

OBJ_FILES := $(patsubst $(CGC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))


TARGET := $(PROJECT_DIR)/build/out.elf

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(Q) $(CC) $(OBJ_FILES)  $(LDFLAGS) -lm -o $@

$(BUILD_DIR)/libcgc/%.o: $(CGC_DIR)/libcgc/%.c
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(CFLAGS) $(INCLUDE_DIR) -c  -o $@ $<

$(BUILD_DIR)/$(PROJ_NAME)/lib/%.o: $(CGC_DIR)/$(PROJ_NAME)/lib/%.c 
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(CFLAGS) $(INCLUDE_DIR) -c  -o $@ $<

$(BUILD_DIR)/$(PROJ_NAME)/src/%.o: $(CGC_DIR)/$(PROJ_NAME)/src/%.c 
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(CFLAGS) $(INCLUDE_DIR) -c  -o $@ $<
