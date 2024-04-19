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

ROOT_DIR := $(abspath ..)
PROJ_DIR := $(abspath .)
PROJ_NAME := $(notdir $(PROJ_DIR))
BUILD_DIR  := $(PROJ_DIR)/build
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
LLVM_TOOLCHAIN := /home/lcm/github/toolchains/x86_64/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04

GCC_COMPILER        := arm-none-eabi-gcc
CLANG_COMPILER      := $(LLVM_TOOLCHAIN)/bin/clang
OPT                 := $(LLVM_TOOLCHAIN)/bin/opt
LLC                 := $(LLVM_TOOLCHAIN)/bin/llc
INSTRUMENTATION_LIB := /home/lcm/github/spear/spear-code/code_coverage/compiler_fw/build/libCoverageInstrument.so
CC_RUNTIME_DIR      := /home/lcm/github/spear/spear-code/code_coverage/runtime_fw/build
#COMPILER ?= arm-none-eabi-gcc
CC := $(CLANG_COMPILER)

$(info $(CC))

LLVM_MAJOR_VER := $(shell $(LLVM_CONFIG) --version | sed -n 's/\([0-9]*\).*/\1/p')


## Middleware Dir, include and sources
MIDDLEWARE_DIR := $(PROJ_DIR)/Middlewares

MIDDLEWARE_INCLUDE_DIR := \
-I$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Core/Inc \
-I$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Class/CDC/Inc

MIDDLEWARES_SOURCES := \
$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
$(MIDDLEWARE_DIR)/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c


## USB-DEVICE Dir, include and sources

USB_DEVICE_DIR := $(PROJ_DIR)/USB_DEVICE

USB_DEVICE_INCLUDE_DIR := -I$(USB_DEVICE_DIR)/App \
-I$(USB_DEVICE_DIR)/Target

USB_DEVICE_SOURCES := \
$(USB_DEVICE_DIR)/App/usb_device.c \
$(USB_DEVICE_DIR)/App/usbd_cdc_if.c \
$(USB_DEVICE_DIR)/App/usbd_desc.c \
$(USB_DEVICE_DIR)/Target/usbd_conf.c

## Drivers Dir, include and sources
DRIVERS_ROOT_DIR := $(PROJ_DIR)/Drivers

DRIVERS_INCLUDE_DIR := -I$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Inc \
-I$(DRIVERS_ROOT_DIR)/CMSIS/Device/ST/STM32H7xx/Include \
-I$(DRIVERS_ROOT_DIR)/CMSIS/Include \
-I$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Inc/Legacy



DRIVERS_SOURCES := \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
$(DRIVERS_ROOT_DIR)/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c


## Protocol Dir, include and sources
PROTOCOL_DIR := $(ROOT_DIR)/Common/Protocol

PROTOCOL_INCLUDE_DIR :=  -I$(PROTOCOL_DIR)

PROTOCOL_SOURCES := \
$(PROTOCOL_DIR)/monitor-dma-midi.c \
$(PROTOCOL_DIR)/protocol.c \
$(PROTOCOL_DIR)/ring.c




## SymRuntime Dir, include and sources
SYMRUNTIME_DIR := $(ROOT_DIR)/Common/SymRuntime

SYMRUNTIME_INCLUDE_DIR := -I$(SYMRUNTIME_DIR)

SYMRUNTIME_SOURCES := $(SYMRUNTIME_DIR)/runtime.c



## FREERTOS Dir, include, sources
FREERTOS_DIR := $(ROOT_DIR)/Common/FreeRTOS-Kernel

FREERTOS_INCLUDE_DIR := \
-I$(FREERTOS_DIR)/include \
-I$(FREERTOS_DIR)/portable/GCC/ARM_CM4F

FREERTOS_SOURCES := \
$(FREERTOS_DIR)/croutine.c \
$(FREERTOS_DIR)/event_groups.c \
$(FREERTOS_DIR)/list.c \
$(FREERTOS_DIR)/queue.c \
$(FREERTOS_DIR)/stream_buffer.c \
$(FREERTOS_DIR)/tasks.c \
$(FREERTOS_DIR)/timers.c \
$(FREERTOS_DIR)/portable/GCC/ARM_CM4F/port.c \
$(FREERTOS_DIR)/portable/MemMang/heap_4.c

## Coremark Dir, include, sources
COREMARK_HAL_DIR := $(PROJ_DIR)/Target

COREMARK_HAL_INCLUDE_DIR := \
-I$(COREMARK_HAL_DIR)/Inc

COREMARK_HAL_SOURCES := \
$(COREMARK_HAL_DIR)/Src/leds.c \
$(COREMARK_HAL_DIR)/Src/main_original.c \
$(COREMARK_HAL_DIR)/Src/mm_midiccrouter.c \
$(COREMARK_HAL_DIR)/Src/mm_midievent.c \
$(COREMARK_HAL_DIR)/Src/mm_midieventbuilder.c \
$(COREMARK_HAL_DIR)/Src/mm_midimsg.c \
$(COREMARK_HAL_DIR)/Src/mm_midimsgbuilder.c \
$(COREMARK_HAL_DIR)/Src/mm_midirouter.c \
$(COREMARK_HAL_DIR)/UninstrumentedSrc/midi_lowlevel.c \
$(COREMARK_HAL_DIR)/UninstrumentedSrc/mm_midirouter_standard.c
## Project core Dir, Include, sources

PROJECT_CORE_DIR := $(PROJ_DIR)/Core

PROJECT_CORE_INCLUDE_DIR := -I$(PROJECT_CORE_DIR)/Inc

PROJECT_CORE_SOURCES := \
$(PROJECT_CORE_DIR)/Src/main.c \
$(PROJECT_CORE_DIR)/Src/stm32h7xx_hal_msp.c \
$(PROJECT_CORE_DIR)/Src/stm32h7xx_hal_timebase_tim.c \
$(PROJECT_CORE_DIR)/Src/stm32h7xx_it.c \
$(PROJECT_CORE_DIR)/Src/syscalls.c \
$(PROJECT_CORE_DIR)/Src/sysmem.c \
$(PROJECT_CORE_DIR)/Src/system_stm32h7xx.c







SRC_FILES +=  \
$(PROJECT_CORE_SOURCES) \
$(DRIVERS_SOURCES) \
$(COREMARK_HAL_SOURCES) \
$(FREERTOS_SOURCES) \
$(SYMRUNTIME_SOURCES) \
$(PROTOCOL_SOURCES) \
$(USB_DEVICE_SOURCES) \
$(MIDDLEWARES_SOURCES)




OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))


PROJECT_STARTUP_DIR = $(PROJ_DIR)/Core/Startup

STARTUP_SOURCES += \
$(PROJECT_STARTUP_DIR)/startup_stm32h743zitx.s


OBJ_FILES := $(OBJ_FILES) \
$(patsubst $(ROOT_DIR)/%.s,$(BUILD_DIR)/%.o,$(STARTUP_SOURCES))


INCLUDE_PATHS += \
  $(DRIVERS_INCLUDE_DIR) \
  $(PROJECT_CORE_INCLUDE_DIR) \
  $(COREMARK_HAL_INCLUDE_DIR) \
  $(FREERTOS_INCLUDE_DIR) \
  $(SYMRUNTIME_INCLUDE_DIR) \
  $(PROTOCOL_INCLUDE_DIR) \
  $(USB_DEVICE_INCLUDE_DIR) \
  $(MIDDLEWARE_INCLUDE_DIR)


# Note: These arguments will be used by clang to select
# the specific architecture target when we use the
# generic --target=arm-none-eabi
ARCH_CFLAGS += \
  -mthumb -mcpu=cortex-m7 \
  -mfloat-abi=hard -mfpu=fpv5-d16

#  -mfloat-abi=soft
#-mfloat-abi=hard -mfpu=fpv4-sp-d16

CC_VERSION_INFO ="$(shell $(CC) -v 2>&1 | head -2)"

ifneq '' '$(findstring clang,$(CC_VERSION_INFO))'
$(info ===Clang Compiler Detected===)

# Default assumes an GNU ARM toolchain is on your path
# but an explicit path can also be provided by doing
# ARM_GNU_CC=<PATH_TO_GCC> make
ARM_GNU_CC ?= arm-none-eabi-gcc

ARM_CORTEXM_SYSROOT = \
  $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-sysroot 2>&1)

# The directory where Newlib's libc.a & libm.a reside
# for the specific target architecture
ARM_CORTEXM_MULTI_DIR = \
  $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-multi-directory 2>&1)

# Pick up builtins needed for compilation
ARM_CORTEXM_BUILTINS ?= \
  $(shell $(ARM_GNU_CC) $(ARCH_CFLAGS) -print-libgcc-file-name 2>&1)

COMPILER_SPECIFIC_CFLAGS += \
  --target=arm-none-eabi \
  --sysroot=$(ARM_CORTEXM_SYSROOT)


#-Wthread-safety \
#  -Wthread-safety-beta

COMPILER_SPECIFIC_LDFLAGS += \
  -L$(ARM_CORTEXM_SYSROOT)/lib/$(ARM_CORTEXM_MULTI_DIR) \
  $(ARM_CORTEXM_BUILTINS)

endif

ifneq '' '$(findstring gcc,$(CC_VERSION_INFO))'
$(info ===GCC Compiler Detected===)


COMPILER_SPECIFIC_LDFLAGS += \
  -Wl,-lgcc 

endif

CFLAGS += -O0
CFLAGS += -std=gnu11
CFLAGS += -g
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -Wall
#CFLAGS += -fstack-usage
#CFLAGS += --specs=nano.specs 
CFLAGS += -Werror 
CFLAGS += -nostdlib 
#CFLAGS += -DDEBUG 
CFLAGS += -DUSE_HAL_DRIVER 
CFLAGS += -DSTM32H743xx 
#CFLAGS += -Wno-parentheses-equality 
CFLAGS +=  $(COMPILER_SPECIFIC_CFLAGS) 
CFLAGS +=  $(ARCH_CFLAGS) 
CFLAGS +=  $(CLI_CFLAG_OVERRIDES)



#ASFLAGS += -g3
ASFLAGS += -DDEBUG -c 
ASFLAGS += -x assembler-with-cpp 
#ASFLAGS += --specs=nano.specs 
ASFLAGS += $(ARCH_CFLAGS)


TARGET ?= stm32h743
LDSCRIPT = $(PROJ_DIR)/STM32H743ZITX_FLASH.ld
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf


LDFLAGS += $(COMPILER_SPECIFIC_LDFLAGS)
LDFLAGS +=  $(ARCH_CFLAGS) 
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += --specs=nosys.specs
LDFLAGS += -fno-short-enums
LDFLAGS += -Wl,-Map="$(BUILD_DIR)/$(TARGET).map"
LDFLAGS += -Wl,--gc-sections -static 
LDFLAGS += --specs=nano.specs
LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
LDFLAGS += -u _printf_float
LDFLAGS += $(CLI_LDFLAG_OVERRIDES)


LINKER_OVERRIDE ?=

DEP_DIR = $(BUILD_DIR)/dep
DEP_CFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$(*F).d


all: $(TARGET_ELF)
	 

-include $(wildcard $(DEP_DIR)/*.d)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)/output
	rm $(OUTPUT_DIR)/*.ll $(OUTPUT_DIR)/*.dot $(OUTPUT_DIR)/*.svg $(OUTPUT_DIR)/*.txt





#flash: $(TARGET_ELF)

H743AM := $(shell STM32_Programmer_CLI --Connect port=SWD sn=002800243156501420323443 > /dev/null 2>&1 ; echo $$?)
H743CH := $(shell STM32_Programmer_CLI --Connect port=SWD sn=0046003D3137511239383538 > /dev/null 2>&1 ; echo $$?)

flash: $(TARGET_ELF)
ifeq ($(H743AM),0)
	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/stm32h743.elf -v -ob displ -hardRst  sn=002800243156501420323443
else
ifeq ($(H743CH),0)
	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/stm32h743.elf -v -ob displ -hardRst  sn=0046003D3137511239383538
else
	@echo "ERROR: No Board H743 found"
endif	
endif	
    

# CheckBoard ="$(shell $(CC) -v 2>&1 | head -2)"




$(TARGET_ELF): $(OBJ_FILES)  $(LDSCRIPT)
	@echo "Linking library"
  #$(Q) $(CC) $(OBJ_FILES)  $(LDFLAGS) -o $@
	$(Q) arm-none-eabi-gcc $(OBJ_FILES)  $(LDFLAGS) -o $@
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"
	arm-none-eabi-objcopy -O binary $(TARGET_ELF)  "$(BUILD_DIR)/$(TARGET).bin"

$(BUILD_DIR): 
	@mkdir -p $(BUILD_DIR)

$(DEP_DIR): 
	@mkdir -p $(DEP_DIR)

# FreeRTOS should really be included as a submodule in a real project
# Let's do a cheap hack and clone it here instead
#$(FREERTOS_PORT_ROOT):
#	git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git $(FREERTOS_ROOT_DIR)
#

$(OBJ_FILES): $(SRC_FILES) makefile 


# build Middleware

$(BUILD_DIR)/$(PROJ_NAME)/Middlewares/%.o: $(PROJ_DIR)/Middlewares/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<


# build USB_DEVICES

$(BUILD_DIR)/$(PROJ_NAME)/USB_DEVICE/%.o: $(PROJ_DIR)/USB_DEVICE/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

# build core files
$(BUILD_DIR)/$(PROJ_NAME)/Core/Src/%.o:  $(PROJ_DIR)/Core/Src/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<


# build drivers
$(BUILD_DIR)/$(PROJ_NAME)/Drivers/%.o: $(PROJ_DIR)/Drivers/%.c  | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<


# build Assembly startup code
$(BUILD_DIR)/$(PROJ_NAME)/Core/Startup/%.o: $(PROJ_DIR)/Core/Startup/%.s | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(ASFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

# build freertos
$(BUILD_DIR)/Common/FreeRTOS-Kernel/%.o: $(ROOT_DIR)/Common/FreeRTOS-Kernel/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

# build protocol code
$(BUILD_DIR)/Common/Protocol/%.o: $(ROOT_DIR)/Common/Protocol/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

# build Runtime Code
$(BUILD_DIR)/Common/SymRuntime/%.o: $(ROOT_DIR)/Common/SymRuntime/%.c  | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

#$(BUILD_DIR)/$(PROJ_NAME)/Target/Src/%.o: $(PROJ_DIR)/Target/Src/%.c | $(BUILD_DIR) $(DEP_DIR) 
#	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
#	@mkdir -p $(dir $@)
#	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<


#build instrumented coremark
$(BUILD_DIR)/$(PROJ_NAME)/Target/Src/%.o: $(PROJ_DIR)/Target/Src/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	@mkdir -p $(OUTPUT_DIR)
ifeq "13" "$(word 1, $(sort 13 $(LLVM_MAJOR_VER)))"
	$(Q) $(CLANG_COMPILER) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -Xclang -load -Xclang ${INSTRUMENTATION_LIB} -Xclang -fpass-plugin=${INSTRUMENTATION_LIB} -c -o $@ $< -Qunused-arguments
else
	$(Q) $(CLANG_COMPILER) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -S -emit-llvm -o $@_raw.ll $<
	$(Q) $(OPT) -load ${INSTRUMENTATION_LIB} --symbolize -out $(OUTPUT_DIR) -o $@_ins.ll $@_raw.ll
	$(Q) $(LLC) -filetype=obj -o $@ $@_ins.ll
endif

#build the uninstrumented coremark
$(BUILD_DIR)/$(PROJ_NAME)/Target/UninstrumentedSrc/%.o: $(PROJ_DIR)/Target/UninstrumentedSrc/%.c | $(BUILD_DIR) $(DEP_DIR) 
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

#$(CC) -mcpu=cortex-m3 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(DEP_DIR)/startup_stm32l152retx.d" -MT"$(BUILD_DIR)/Core/Startup/startup_stm32l152retx.o" --specs=nano.specs -mfloat-abi=soft -mthumb  -o $@ $< 
   
