#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Default.mk)" "nbproject/Makefile-local-Default.mk"
include nbproject/Makefile-local-Default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=examples/driver_examples.c hal/src/hal_atomic.c hal/src/hal_cache.c hal/src/hal_delay.c hal/src/hal_gpio.c hal/src/hal_init.c hal/src/hal_io.c hal/src/hal_sleep.c hal/src/hal_usb_device.c hal/src/hal_usart_sync.c hal/utils/src/utils_assert.c hal/utils/src/utils_event.c hal/utils/src/utils_list.c hal/utils/src/utils_syscalls.c hpl/cmcc/hpl_cmcc.c hpl/core/hpl_core_m4.c hpl/core/hpl_init.c hpl/dmac/hpl_dmac.c hpl/gclk/hpl_gclk.c hpl/mclk/hpl_mclk.c hpl/osc32kctrl/hpl_osc32kctrl.c hpl/oscctrl/hpl_oscctrl.c hpl/pm/hpl_pm.c hpl/ramecc/hpl_ramecc.c hpl/sercom/hpl_sercom.c hpl/usb/hpl_usb.c printf/printf/printf.c samd51a/gcc/gcc/startup_samd51.c samd51a/gcc/system_samd51.c stdio_redirect/gcc/read.c stdio_redirect/gcc/write.c stdio_redirect/stdio_io.c usb/class/cdc/device/cdcdf_acm.c usb/device/usbdc.c usb/usb_protocol.c main.c driver_init.c usb_start.c atmel_start.c stdio_start.c ../Common/SymRuntime/wrapped_malloc.c ../Common/SymRuntime/FreeRTOS_callbacks.c ../Common/SymRuntime/runtime.c ../Common/FreeRTOS-Kernel/tasks.c ../Common/FreeRTOS-Kernel/stream_buffer.c ../Common/FreeRTOS-Kernel/queue.c ../Common/FreeRTOS-Kernel/croutine.c ../Common/FreeRTOS-Kernel/event_groups.c ../Common/FreeRTOS-Kernel/timers.c ../Common/FreeRTOS-Kernel/list.c ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c ../Common/Protocol/protocol.c ../Common/Protocol/ring.c ../Common/Protocol/monitor.c Target/ATParser.c Target/test.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/examples/driver_examples.o ${OBJECTDIR}/hal/src/hal_atomic.o ${OBJECTDIR}/hal/src/hal_cache.o ${OBJECTDIR}/hal/src/hal_delay.o ${OBJECTDIR}/hal/src/hal_gpio.o ${OBJECTDIR}/hal/src/hal_init.o ${OBJECTDIR}/hal/src/hal_io.o ${OBJECTDIR}/hal/src/hal_sleep.o ${OBJECTDIR}/hal/src/hal_usb_device.o ${OBJECTDIR}/hal/src/hal_usart_sync.o ${OBJECTDIR}/hal/utils/src/utils_assert.o ${OBJECTDIR}/hal/utils/src/utils_event.o ${OBJECTDIR}/hal/utils/src/utils_list.o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o ${OBJECTDIR}/hpl/core/hpl_core_m4.o ${OBJECTDIR}/hpl/core/hpl_init.o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o ${OBJECTDIR}/hpl/mclk/hpl_mclk.o ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o ${OBJECTDIR}/hpl/pm/hpl_pm.o ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o ${OBJECTDIR}/hpl/usb/hpl_usb.o ${OBJECTDIR}/printf/printf/printf.o ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o ${OBJECTDIR}/samd51a/gcc/system_samd51.o ${OBJECTDIR}/stdio_redirect/gcc/read.o ${OBJECTDIR}/stdio_redirect/gcc/write.o ${OBJECTDIR}/stdio_redirect/stdio_io.o ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o ${OBJECTDIR}/usb/device/usbdc.o ${OBJECTDIR}/usb/usb_protocol.o ${OBJECTDIR}/main.o ${OBJECTDIR}/driver_init.o ${OBJECTDIR}/usb_start.o ${OBJECTDIR}/atmel_start.o ${OBJECTDIR}/stdio_start.o ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o ${OBJECTDIR}/_ext/1423857446/runtime.o ${OBJECTDIR}/_ext/924481411/tasks.o ${OBJECTDIR}/_ext/924481411/stream_buffer.o ${OBJECTDIR}/_ext/924481411/queue.o ${OBJECTDIR}/_ext/924481411/croutine.o ${OBJECTDIR}/_ext/924481411/event_groups.o ${OBJECTDIR}/_ext/924481411/timers.o ${OBJECTDIR}/_ext/924481411/list.o ${OBJECTDIR}/_ext/1346478737/port.o ${OBJECTDIR}/_ext/1243373050/heap_4.o ${OBJECTDIR}/_ext/179706861/protocol.o ${OBJECTDIR}/_ext/179706861/ring.o ${OBJECTDIR}/_ext/179706861/monitor.o ${OBJECTDIR}/Target/ATParser.o ${OBJECTDIR}/Target/test.o
POSSIBLE_DEPFILES=${OBJECTDIR}/examples/driver_examples.o.d ${OBJECTDIR}/hal/src/hal_atomic.o.d ${OBJECTDIR}/hal/src/hal_cache.o.d ${OBJECTDIR}/hal/src/hal_delay.o.d ${OBJECTDIR}/hal/src/hal_gpio.o.d ${OBJECTDIR}/hal/src/hal_init.o.d ${OBJECTDIR}/hal/src/hal_io.o.d ${OBJECTDIR}/hal/src/hal_sleep.o.d ${OBJECTDIR}/hal/src/hal_usb_device.o.d ${OBJECTDIR}/hal/src/hal_usart_sync.o.d ${OBJECTDIR}/hal/utils/src/utils_assert.o.d ${OBJECTDIR}/hal/utils/src/utils_event.o.d ${OBJECTDIR}/hal/utils/src/utils_list.o.d ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o.d ${OBJECTDIR}/hpl/core/hpl_core_m4.o.d ${OBJECTDIR}/hpl/core/hpl_init.o.d ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d ${OBJECTDIR}/hpl/mclk/hpl_mclk.o.d ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o.d ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o.d ${OBJECTDIR}/hpl/pm/hpl_pm.o.d ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o.d ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d ${OBJECTDIR}/hpl/usb/hpl_usb.o.d ${OBJECTDIR}/printf/printf/printf.o.d ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o.d ${OBJECTDIR}/samd51a/gcc/system_samd51.o.d ${OBJECTDIR}/stdio_redirect/gcc/read.o.d ${OBJECTDIR}/stdio_redirect/gcc/write.o.d ${OBJECTDIR}/stdio_redirect/stdio_io.o.d ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o.d ${OBJECTDIR}/usb/device/usbdc.o.d ${OBJECTDIR}/usb/usb_protocol.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/driver_init.o.d ${OBJECTDIR}/usb_start.o.d ${OBJECTDIR}/atmel_start.o.d ${OBJECTDIR}/stdio_start.o.d ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o.d ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o.d ${OBJECTDIR}/_ext/1423857446/runtime.o.d ${OBJECTDIR}/_ext/924481411/tasks.o.d ${OBJECTDIR}/_ext/924481411/stream_buffer.o.d ${OBJECTDIR}/_ext/924481411/queue.o.d ${OBJECTDIR}/_ext/924481411/croutine.o.d ${OBJECTDIR}/_ext/924481411/event_groups.o.d ${OBJECTDIR}/_ext/924481411/timers.o.d ${OBJECTDIR}/_ext/924481411/list.o.d ${OBJECTDIR}/_ext/1346478737/port.o.d ${OBJECTDIR}/_ext/1243373050/heap_4.o.d ${OBJECTDIR}/_ext/179706861/protocol.o.d ${OBJECTDIR}/_ext/179706861/ring.o.d ${OBJECTDIR}/_ext/179706861/monitor.o.d ${OBJECTDIR}/Target/ATParser.o.d ${OBJECTDIR}/Target/test.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/examples/driver_examples.o ${OBJECTDIR}/hal/src/hal_atomic.o ${OBJECTDIR}/hal/src/hal_cache.o ${OBJECTDIR}/hal/src/hal_delay.o ${OBJECTDIR}/hal/src/hal_gpio.o ${OBJECTDIR}/hal/src/hal_init.o ${OBJECTDIR}/hal/src/hal_io.o ${OBJECTDIR}/hal/src/hal_sleep.o ${OBJECTDIR}/hal/src/hal_usb_device.o ${OBJECTDIR}/hal/src/hal_usart_sync.o ${OBJECTDIR}/hal/utils/src/utils_assert.o ${OBJECTDIR}/hal/utils/src/utils_event.o ${OBJECTDIR}/hal/utils/src/utils_list.o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o ${OBJECTDIR}/hpl/core/hpl_core_m4.o ${OBJECTDIR}/hpl/core/hpl_init.o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o ${OBJECTDIR}/hpl/mclk/hpl_mclk.o ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o ${OBJECTDIR}/hpl/pm/hpl_pm.o ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o ${OBJECTDIR}/hpl/usb/hpl_usb.o ${OBJECTDIR}/printf/printf/printf.o ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o ${OBJECTDIR}/samd51a/gcc/system_samd51.o ${OBJECTDIR}/stdio_redirect/gcc/read.o ${OBJECTDIR}/stdio_redirect/gcc/write.o ${OBJECTDIR}/stdio_redirect/stdio_io.o ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o ${OBJECTDIR}/usb/device/usbdc.o ${OBJECTDIR}/usb/usb_protocol.o ${OBJECTDIR}/main.o ${OBJECTDIR}/driver_init.o ${OBJECTDIR}/usb_start.o ${OBJECTDIR}/atmel_start.o ${OBJECTDIR}/stdio_start.o ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o ${OBJECTDIR}/_ext/1423857446/runtime.o ${OBJECTDIR}/_ext/924481411/tasks.o ${OBJECTDIR}/_ext/924481411/stream_buffer.o ${OBJECTDIR}/_ext/924481411/queue.o ${OBJECTDIR}/_ext/924481411/croutine.o ${OBJECTDIR}/_ext/924481411/event_groups.o ${OBJECTDIR}/_ext/924481411/timers.o ${OBJECTDIR}/_ext/924481411/list.o ${OBJECTDIR}/_ext/1346478737/port.o ${OBJECTDIR}/_ext/1243373050/heap_4.o ${OBJECTDIR}/_ext/179706861/protocol.o ${OBJECTDIR}/_ext/179706861/ring.o ${OBJECTDIR}/_ext/179706861/monitor.o ${OBJECTDIR}/Target/ATParser.o ${OBJECTDIR}/Target/test.o

# Source Files
SOURCEFILES=examples/driver_examples.c hal/src/hal_atomic.c hal/src/hal_cache.c hal/src/hal_delay.c hal/src/hal_gpio.c hal/src/hal_init.c hal/src/hal_io.c hal/src/hal_sleep.c hal/src/hal_usb_device.c hal/src/hal_usart_sync.c hal/utils/src/utils_assert.c hal/utils/src/utils_event.c hal/utils/src/utils_list.c hal/utils/src/utils_syscalls.c hpl/cmcc/hpl_cmcc.c hpl/core/hpl_core_m4.c hpl/core/hpl_init.c hpl/dmac/hpl_dmac.c hpl/gclk/hpl_gclk.c hpl/mclk/hpl_mclk.c hpl/osc32kctrl/hpl_osc32kctrl.c hpl/oscctrl/hpl_oscctrl.c hpl/pm/hpl_pm.c hpl/ramecc/hpl_ramecc.c hpl/sercom/hpl_sercom.c hpl/usb/hpl_usb.c printf/printf/printf.c samd51a/gcc/gcc/startup_samd51.c samd51a/gcc/system_samd51.c stdio_redirect/gcc/read.c stdio_redirect/gcc/write.c stdio_redirect/stdio_io.c usb/class/cdc/device/cdcdf_acm.c usb/device/usbdc.c usb/usb_protocol.c main.c driver_init.c usb_start.c atmel_start.c stdio_start.c ../Common/SymRuntime/wrapped_malloc.c ../Common/SymRuntime/FreeRTOS_callbacks.c ../Common/SymRuntime/runtime.c ../Common/FreeRTOS-Kernel/tasks.c ../Common/FreeRTOS-Kernel/stream_buffer.c ../Common/FreeRTOS-Kernel/queue.c ../Common/FreeRTOS-Kernel/croutine.c ../Common/FreeRTOS-Kernel/event_groups.c ../Common/FreeRTOS-Kernel/timers.c ../Common/FreeRTOS-Kernel/list.c ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c ../Common/Protocol/protocol.c ../Common/Protocol/ring.c ../Common/Protocol/monitor.c Target/ATParser.c Target/test.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samd51a/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Default.mk ${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMD51J19A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/examples/driver_examples.o: examples/driver_examples.c  .generated_files/flags/Default/2d8f9507b97c7cf619014004cb21bfbd27cbc173 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/examples" 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o.d 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/examples/driver_examples.o.d" -o ${OBJECTDIR}/examples/driver_examples.o examples/driver_examples.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_atomic.o: hal/src/hal_atomic.c  .generated_files/flags/Default/e841b71b49b7238ed482f49c7aabb84ab2ba5483 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_atomic.o.d" -o ${OBJECTDIR}/hal/src/hal_atomic.o hal/src/hal_atomic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_cache.o: hal/src/hal_cache.c  .generated_files/flags/Default/341d78eedb85bbf36293c1b5b20ea0a33877071 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_cache.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_cache.o.d" -o ${OBJECTDIR}/hal/src/hal_cache.o hal/src/hal_cache.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_delay.o: hal/src/hal_delay.c  .generated_files/flags/Default/2c5aa3ad573aed923840096c544d5541dcf38ce2 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_delay.o.d" -o ${OBJECTDIR}/hal/src/hal_delay.o hal/src/hal_delay.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_gpio.o: hal/src/hal_gpio.c  .generated_files/flags/Default/2aa4cb96f9865dd8f5f73e3e70375991e58db215 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_gpio.o.d" -o ${OBJECTDIR}/hal/src/hal_gpio.o hal/src/hal_gpio.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_init.o: hal/src/hal_init.c  .generated_files/flags/Default/4ab56476f6a49ae4a716c3437f2786941ca5a07a .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_init.o.d" -o ${OBJECTDIR}/hal/src/hal_init.o hal/src/hal_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_io.o: hal/src/hal_io.c  .generated_files/flags/Default/775dbc4f1bb2e330560cd319820874841f75e7d3 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_io.o.d" -o ${OBJECTDIR}/hal/src/hal_io.o hal/src/hal_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_sleep.o: hal/src/hal_sleep.c  .generated_files/flags/Default/572f106603f147f4d5f333dfa637cbbda74fdca6 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_sleep.o.d" -o ${OBJECTDIR}/hal/src/hal_sleep.o hal/src/hal_sleep.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_usb_device.o: hal/src/hal_usb_device.c  .generated_files/flags/Default/13514d567fd1e7e406bc279776fd32186f7779bc .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usb_device.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_usb_device.o.d" -o ${OBJECTDIR}/hal/src/hal_usb_device.o hal/src/hal_usb_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_usart_sync.o: hal/src/hal_usart_sync.c  .generated_files/flags/Default/e567b2bc14a6685bd0003d5d3a3f012f7f1e9796 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_usart_sync.o hal/src/hal_usart_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_assert.o: hal/utils/src/utils_assert.c  .generated_files/flags/Default/3dd33bd97ad7d81cad4a8e36dc2704a4d9cefd2c .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_assert.o hal/utils/src/utils_assert.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_event.o: hal/utils/src/utils_event.c  .generated_files/flags/Default/262d01a1308f8867978927ce126b00e4fccce07a .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_event.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_event.o hal/utils/src/utils_event.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_list.o: hal/utils/src/utils_list.c  .generated_files/flags/Default/92fd116d21e42435228af15501fddabeffd53a28 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_list.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_list.o hal/utils/src/utils_list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_syscalls.o: hal/utils/src/utils_syscalls.c  .generated_files/flags/Default/f20f2efe5f8570f4e1e98fd1cae62fbf22827b8e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o hal/utils/src/utils_syscalls.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o: hpl/cmcc/hpl_cmcc.c  .generated_files/flags/Default/50efd7adbc0c187802b771ecf91a4dc08c4b6c76 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/cmcc" 
	@${RM} ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o.d 
	@${RM} ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o.d" -o ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o hpl/cmcc/hpl_cmcc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/core/hpl_core_m4.o: hpl/core/hpl_core_m4.c  .generated_files/flags/Default/f889a7980500f5a654d8ccd3f3998d3e40c440c2 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m4.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/core/hpl_core_m4.o.d" -o ${OBJECTDIR}/hpl/core/hpl_core_m4.o hpl/core/hpl_core_m4.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/core/hpl_init.o: hpl/core/hpl_init.c  .generated_files/flags/Default/3c4082c3c17164a4219072afaad7d87c9dfd8784 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/core/hpl_init.o.d" -o ${OBJECTDIR}/hpl/core/hpl_init.o hpl/core/hpl_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/dmac/hpl_dmac.o: hpl/dmac/hpl_dmac.c  .generated_files/flags/Default/66400451c7ff3df88cf2098f0dabd94cd0504e29 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/dmac" 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" -o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o hpl/dmac/hpl_dmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/gclk/hpl_gclk.o: hpl/gclk/hpl_gclk.c  .generated_files/flags/Default/1405d2e4d1116b7c6ba62d58ce078f5acba9ea45 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/gclk" 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" -o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o hpl/gclk/hpl_gclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/mclk/hpl_mclk.o: hpl/mclk/hpl_mclk.c  .generated_files/flags/Default/1c39621d3432e296b3f24e73666f56e2d1ee4f9c .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/mclk" 
	@${RM} ${OBJECTDIR}/hpl/mclk/hpl_mclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/mclk/hpl_mclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/mclk/hpl_mclk.o.d" -o ${OBJECTDIR}/hpl/mclk/hpl_mclk.o hpl/mclk/hpl_mclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o: hpl/osc32kctrl/hpl_osc32kctrl.c  .generated_files/flags/Default/e65430a67a21b79feb20c20ebb11e15dc44c9e59 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/osc32kctrl" 
	@${RM} ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o.d" -o ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o hpl/osc32kctrl/hpl_osc32kctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o: hpl/oscctrl/hpl_oscctrl.c  .generated_files/flags/Default/a076e8f506bfc7029f946ede4a1dab8c44514885 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/oscctrl" 
	@${RM} ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o.d" -o ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o hpl/oscctrl/hpl_oscctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/pm/hpl_pm.o: hpl/pm/hpl_pm.c  .generated_files/flags/Default/b21380d41889c35fa4d0e82bdf2a082e701ec955 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/pm" 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o.d 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" -o ${OBJECTDIR}/hpl/pm/hpl_pm.o hpl/pm/hpl_pm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o: hpl/ramecc/hpl_ramecc.c  .generated_files/flags/Default/7217b51219d0a55a3e4f6542bab6344876d1b325 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/ramecc" 
	@${RM} ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o.d 
	@${RM} ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o.d" -o ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o hpl/ramecc/hpl_ramecc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/sercom/hpl_sercom.o: hpl/sercom/hpl_sercom.c  .generated_files/flags/Default/d754c394be2a583bcea3d9acc066f9c8bf6cd0b8 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/sercom" 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" -o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o hpl/sercom/hpl_sercom.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/usb/hpl_usb.o: hpl/usb/hpl_usb.c  .generated_files/flags/Default/2384761bdf0602c86e80d3a895d810a0421c8445 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/usb" 
	@${RM} ${OBJECTDIR}/hpl/usb/hpl_usb.o.d 
	@${RM} ${OBJECTDIR}/hpl/usb/hpl_usb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/usb/hpl_usb.o.d" -o ${OBJECTDIR}/hpl/usb/hpl_usb.o hpl/usb/hpl_usb.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/printf/printf/printf.o: printf/printf/printf.c  .generated_files/flags/Default/5510135aa36c0406f8b5c7b9240f3657c67ce3e6 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/printf/printf" 
	@${RM} ${OBJECTDIR}/printf/printf/printf.o.d 
	@${RM} ${OBJECTDIR}/printf/printf/printf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/printf/printf/printf.o.d" -o ${OBJECTDIR}/printf/printf/printf.o printf/printf/printf.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o: samd51a/gcc/gcc/startup_samd51.c  .generated_files/flags/Default/9b36f4020faaccce73fa2b7f2058734720d52e65 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/samd51a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o.d 
	@${RM} ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o.d" -o ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o samd51a/gcc/gcc/startup_samd51.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/samd51a/gcc/system_samd51.o: samd51a/gcc/system_samd51.c  .generated_files/flags/Default/260d19ee432c1b5a3a75b8b23b756d13dc08c2c2 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/samd51a/gcc" 
	@${RM} ${OBJECTDIR}/samd51a/gcc/system_samd51.o.d 
	@${RM} ${OBJECTDIR}/samd51a/gcc/system_samd51.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/samd51a/gcc/system_samd51.o.d" -o ${OBJECTDIR}/samd51a/gcc/system_samd51.o samd51a/gcc/system_samd51.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/gcc/read.o: stdio_redirect/gcc/read.c  .generated_files/flags/Default/52a674f18631cdea795dba66dcc094c4fed7f15f .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/read.o stdio_redirect/gcc/read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/gcc/write.o: stdio_redirect/gcc/write.c  .generated_files/flags/Default/ed6479f11ff579c0c3f499932563750bb5cbcc18 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/write.o stdio_redirect/gcc/write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/stdio_io.o: stdio_redirect/stdio_io.c  .generated_files/flags/Default/1a771017dee4d90172472ffe28975e411a0faff .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect" 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" -o ${OBJECTDIR}/stdio_redirect/stdio_io.o stdio_redirect/stdio_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o: usb/class/cdc/device/cdcdf_acm.c  .generated_files/flags/Default/67524ffcac42857d38f95455fe57c02b64a013f5 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb/class/cdc/device" 
	@${RM} ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o.d 
	@${RM} ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o.d" -o ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o usb/class/cdc/device/cdcdf_acm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/device/usbdc.o: usb/device/usbdc.c  .generated_files/flags/Default/896a4c176713b1c1ec82241a50796c6582e4bd70 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb/device" 
	@${RM} ${OBJECTDIR}/usb/device/usbdc.o.d 
	@${RM} ${OBJECTDIR}/usb/device/usbdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/device/usbdc.o.d" -o ${OBJECTDIR}/usb/device/usbdc.o usb/device/usbdc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/usb_protocol.o: usb/usb_protocol.c  .generated_files/flags/Default/7c68d7465f1f38538bf28750920df4070ce65e96 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_protocol.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/usb_protocol.o.d" -o ${OBJECTDIR}/usb/usb_protocol.o usb/usb_protocol.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Default/fa34ff83d31119013c4cbec6edefbdcbb5d4fe4 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/driver_init.o: driver_init.c  .generated_files/flags/Default/c49f4b3120f01c3829dfd0e3dcd5e6adbd08e95e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver_init.o.d 
	@${RM} ${OBJECTDIR}/driver_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/driver_init.o.d" -o ${OBJECTDIR}/driver_init.o driver_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb_start.o: usb_start.c  .generated_files/flags/Default/e0a04cf0d3d985a139eb7b056b04e21609bde123 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_start.o.d 
	@${RM} ${OBJECTDIR}/usb_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb_start.o.d" -o ${OBJECTDIR}/usb_start.o usb_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/atmel_start.o: atmel_start.c  .generated_files/flags/Default/2d1334aad09f9fe2af478d96f00554e592eaf2da .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/atmel_start.o.d 
	@${RM} ${OBJECTDIR}/atmel_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/atmel_start.o.d" -o ${OBJECTDIR}/atmel_start.o atmel_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_start.o: stdio_start.c  .generated_files/flags/Default/5049612673d2eea96e2884e7b9c292811328fb4f .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/stdio_start.o.d 
	@${RM} ${OBJECTDIR}/stdio_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_start.o.d" -o ${OBJECTDIR}/stdio_start.o stdio_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o: ../Common/SymRuntime/wrapped_malloc.c  .generated_files/flags/Default/7cd43968a4b3d927e135c55f6a942f316c1b0c8 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o.d" -o ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o ../Common/SymRuntime/wrapped_malloc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o: ../Common/SymRuntime/FreeRTOS_callbacks.c  .generated_files/flags/Default/6d403b87111c996835970ebd3a543814ff86f7fe .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o.d" -o ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o ../Common/SymRuntime/FreeRTOS_callbacks.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/runtime.o: ../Common/SymRuntime/runtime.c  .generated_files/flags/Default/84a8a2e2d4bc9241a43788dea732f47074019b12 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/runtime.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/runtime.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/runtime.o.d" -o ${OBJECTDIR}/_ext/1423857446/runtime.o ../Common/SymRuntime/runtime.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/tasks.o: ../Common/FreeRTOS-Kernel/tasks.c  .generated_files/flags/Default/450a50d4d6bbad2f7d63ffd2dba3d6371e685abe .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/tasks.o.d" -o ${OBJECTDIR}/_ext/924481411/tasks.o ../Common/FreeRTOS-Kernel/tasks.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/stream_buffer.o: ../Common/FreeRTOS-Kernel/stream_buffer.c  .generated_files/flags/Default/d356315d51fedbcf52e99d6608365fb86b010575 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/stream_buffer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/924481411/stream_buffer.o ../Common/FreeRTOS-Kernel/stream_buffer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/queue.o: ../Common/FreeRTOS-Kernel/queue.c  .generated_files/flags/Default/c87bea23987277c91607ea0ab0a76a763a86ec1a .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/queue.o.d" -o ${OBJECTDIR}/_ext/924481411/queue.o ../Common/FreeRTOS-Kernel/queue.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/croutine.o: ../Common/FreeRTOS-Kernel/croutine.c  .generated_files/flags/Default/fed9cd221e0362ffcdf6876ce05a1231efc90ab3 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/croutine.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/croutine.o.d" -o ${OBJECTDIR}/_ext/924481411/croutine.o ../Common/FreeRTOS-Kernel/croutine.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/event_groups.o: ../Common/FreeRTOS-Kernel/event_groups.c  .generated_files/flags/Default/2380aec12703b0220ade4936887dfa7359a3344 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/event_groups.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/event_groups.o.d" -o ${OBJECTDIR}/_ext/924481411/event_groups.o ../Common/FreeRTOS-Kernel/event_groups.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/timers.o: ../Common/FreeRTOS-Kernel/timers.c  .generated_files/flags/Default/e4214386959567df737419b0cbf513fa3eff8d66 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/timers.o.d" -o ${OBJECTDIR}/_ext/924481411/timers.o ../Common/FreeRTOS-Kernel/timers.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/list.o: ../Common/FreeRTOS-Kernel/list.c  .generated_files/flags/Default/d26c17e5dd57e2ae10335416d936adf6a25aa657 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/list.o.d" -o ${OBJECTDIR}/_ext/924481411/list.o ../Common/FreeRTOS-Kernel/list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1346478737/port.o: ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c  .generated_files/flags/Default/51cee64ab7f51c16d2cae6c6f3e987d633db64ea .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1346478737" 
	@${RM} ${OBJECTDIR}/_ext/1346478737/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1346478737/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1346478737/port.o.d" -o ${OBJECTDIR}/_ext/1346478737/port.o ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1243373050/heap_4.o: ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c  .generated_files/flags/Default/548f3c7a1b68dd5f613d347748133abd93a7bd59 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1243373050" 
	@${RM} ${OBJECTDIR}/_ext/1243373050/heap_4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1243373050/heap_4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1243373050/heap_4.o.d" -o ${OBJECTDIR}/_ext/1243373050/heap_4.o ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/protocol.o: ../Common/Protocol/protocol.c  .generated_files/flags/Default/851151c17351626f39f6a784b99e0a71e420e651 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/protocol.o.d" -o ${OBJECTDIR}/_ext/179706861/protocol.o ../Common/Protocol/protocol.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/ring.o: ../Common/Protocol/ring.c  .generated_files/flags/Default/50e0f48c409002cf1446b0b9e59422ba02024798 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/ring.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/ring.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/ring.o.d" -o ${OBJECTDIR}/_ext/179706861/ring.o ../Common/Protocol/ring.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/monitor.o: ../Common/Protocol/monitor.c  .generated_files/flags/Default/ff9f8e93f8921287948d361119692b51e5462415 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/monitor.o.d" -o ${OBJECTDIR}/_ext/179706861/monitor.o ../Common/Protocol/monitor.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/Target/ATParser.o: Target/ATParser.c  .generated_files/flags/Default/26ea16f02caff74d7be1f1a085012074d1eb0e4c .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Target" 
	@${RM} ${OBJECTDIR}/Target/ATParser.o.d 
	@${RM} ${OBJECTDIR}/Target/ATParser.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/Target/ATParser.o.d" -o ${OBJECTDIR}/Target/ATParser.o Target/ATParser.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/Target/test.o: Target/test.c  .generated_files/flags/Default/2914cb439450f157a93c9afeeed9a2207c515c67 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Target" 
	@${RM} ${OBJECTDIR}/Target/test.o.d 
	@${RM} ${OBJECTDIR}/Target/test.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/Target/test.o.d" -o ${OBJECTDIR}/Target/test.o Target/test.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
else
${OBJECTDIR}/examples/driver_examples.o: examples/driver_examples.c  .generated_files/flags/Default/71befc75cf1e95e726bc3ac207f2ec6d88c36b07 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/examples" 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o.d 
	@${RM} ${OBJECTDIR}/examples/driver_examples.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/examples/driver_examples.o.d" -o ${OBJECTDIR}/examples/driver_examples.o examples/driver_examples.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_atomic.o: hal/src/hal_atomic.c  .generated_files/flags/Default/14965f0e56e5427ea4c4fd756eb06b346e27b692 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_atomic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_atomic.o.d" -o ${OBJECTDIR}/hal/src/hal_atomic.o hal/src/hal_atomic.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_cache.o: hal/src/hal_cache.c  .generated_files/flags/Default/e3658024863ad7964f20777ebfb02e3b1bba8528 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_cache.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_cache.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_cache.o.d" -o ${OBJECTDIR}/hal/src/hal_cache.o hal/src/hal_cache.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_delay.o: hal/src/hal_delay.c  .generated_files/flags/Default/cc3c6fac5289b32cfe9d212c09a4bf7a853421cb .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_delay.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_delay.o.d" -o ${OBJECTDIR}/hal/src/hal_delay.o hal/src/hal_delay.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_gpio.o: hal/src/hal_gpio.c  .generated_files/flags/Default/7e316b93a6c395321c0fd7d30ba31bd881a1fabe .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_gpio.o.d" -o ${OBJECTDIR}/hal/src/hal_gpio.o hal/src/hal_gpio.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_init.o: hal/src/hal_init.c  .generated_files/flags/Default/5028e336d70f463d43cb20d9cb98b8d09f1e8e63 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_init.o.d" -o ${OBJECTDIR}/hal/src/hal_init.o hal/src/hal_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_io.o: hal/src/hal_io.c  .generated_files/flags/Default/96cd0ce143fb4849f161356ee8e16d34a74b068e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_io.o.d" -o ${OBJECTDIR}/hal/src/hal_io.o hal/src/hal_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_sleep.o: hal/src/hal_sleep.c  .generated_files/flags/Default/68e775bc5ebc64e2079a8b59b3a2630373284010 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_sleep.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_sleep.o.d" -o ${OBJECTDIR}/hal/src/hal_sleep.o hal/src/hal_sleep.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_usb_device.o: hal/src/hal_usb_device.c  .generated_files/flags/Default/2727c534d7fca26914d5377d4f331c7318d062d0 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usb_device.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_usb_device.o.d" -o ${OBJECTDIR}/hal/src/hal_usb_device.o hal/src/hal_usb_device.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/src/hal_usart_sync.o: hal/src/hal_usart_sync.c  .generated_files/flags/Default/3bfd09223c70b4622e7e5df02ccd9e4cf790cde9 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/src" 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o.d 
	@${RM} ${OBJECTDIR}/hal/src/hal_usart_sync.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/src/hal_usart_sync.o.d" -o ${OBJECTDIR}/hal/src/hal_usart_sync.o hal/src/hal_usart_sync.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_assert.o: hal/utils/src/utils_assert.c  .generated_files/flags/Default/77549c9846f16f263fd9429589a019c9dc146e50 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_assert.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_assert.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_assert.o hal/utils/src/utils_assert.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_event.o: hal/utils/src/utils_event.c  .generated_files/flags/Default/7df5c6e78558f90563fd30cda5a654d75b92a369 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_event.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_event.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_event.o hal/utils/src/utils_event.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_list.o: hal/utils/src/utils_list.c  .generated_files/flags/Default/c8f6d0c35efb1d9726d8ee0dbe530e9371c608c4 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_list.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_list.o hal/utils/src/utils_list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hal/utils/src/utils_syscalls.o: hal/utils/src/utils_syscalls.c  .generated_files/flags/Default/e18b1d37584115c3879c42bb3ea29da531b6ec0e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal/utils/src" 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d 
	@${RM} ${OBJECTDIR}/hal/utils/src/utils_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hal/utils/src/utils_syscalls.o.d" -o ${OBJECTDIR}/hal/utils/src/utils_syscalls.o hal/utils/src/utils_syscalls.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o: hpl/cmcc/hpl_cmcc.c  .generated_files/flags/Default/9d6fb2aeb657d67eb9547b55edf000bcbed3489c .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/cmcc" 
	@${RM} ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o.d 
	@${RM} ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o.d" -o ${OBJECTDIR}/hpl/cmcc/hpl_cmcc.o hpl/cmcc/hpl_cmcc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/core/hpl_core_m4.o: hpl/core/hpl_core_m4.c  .generated_files/flags/Default/4959ed33d0cf2aa4e013e34df34405dec2936e1d .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m4.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_core_m4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/core/hpl_core_m4.o.d" -o ${OBJECTDIR}/hpl/core/hpl_core_m4.o hpl/core/hpl_core_m4.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/core/hpl_init.o: hpl/core/hpl_init.c  .generated_files/flags/Default/cdf0fcd1ee771a15b1e728189a1be5e57629aef5 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/core" 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o.d 
	@${RM} ${OBJECTDIR}/hpl/core/hpl_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/core/hpl_init.o.d" -o ${OBJECTDIR}/hpl/core/hpl_init.o hpl/core/hpl_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/dmac/hpl_dmac.o: hpl/dmac/hpl_dmac.c  .generated_files/flags/Default/63e897542e6d70747ebcc40e03933c47e4ebfc23 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/dmac" 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d 
	@${RM} ${OBJECTDIR}/hpl/dmac/hpl_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/dmac/hpl_dmac.o.d" -o ${OBJECTDIR}/hpl/dmac/hpl_dmac.o hpl/dmac/hpl_dmac.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/gclk/hpl_gclk.o: hpl/gclk/hpl_gclk.c  .generated_files/flags/Default/af6ff40b4879f75873d0ff5e4d1e619ed1bc599f .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/gclk" 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/gclk/hpl_gclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/gclk/hpl_gclk.o.d" -o ${OBJECTDIR}/hpl/gclk/hpl_gclk.o hpl/gclk/hpl_gclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/mclk/hpl_mclk.o: hpl/mclk/hpl_mclk.c  .generated_files/flags/Default/3614b9dcfa4d844b3e6906570e989ba00e42c383 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/mclk" 
	@${RM} ${OBJECTDIR}/hpl/mclk/hpl_mclk.o.d 
	@${RM} ${OBJECTDIR}/hpl/mclk/hpl_mclk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/mclk/hpl_mclk.o.d" -o ${OBJECTDIR}/hpl/mclk/hpl_mclk.o hpl/mclk/hpl_mclk.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o: hpl/osc32kctrl/hpl_osc32kctrl.c  .generated_files/flags/Default/d55f276d43ed35428ce84939a1d62b65dbe56d8d .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/osc32kctrl" 
	@${RM} ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o.d" -o ${OBJECTDIR}/hpl/osc32kctrl/hpl_osc32kctrl.o hpl/osc32kctrl/hpl_osc32kctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o: hpl/oscctrl/hpl_oscctrl.c  .generated_files/flags/Default/6b8b6fb864da3429d2255d8aebd742983969205 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/oscctrl" 
	@${RM} ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o.d 
	@${RM} ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o.d" -o ${OBJECTDIR}/hpl/oscctrl/hpl_oscctrl.o hpl/oscctrl/hpl_oscctrl.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/pm/hpl_pm.o: hpl/pm/hpl_pm.c  .generated_files/flags/Default/c185d8bb2520085d9faaa06cf505a1108286050a .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/pm" 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o.d 
	@${RM} ${OBJECTDIR}/hpl/pm/hpl_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/pm/hpl_pm.o.d" -o ${OBJECTDIR}/hpl/pm/hpl_pm.o hpl/pm/hpl_pm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o: hpl/ramecc/hpl_ramecc.c  .generated_files/flags/Default/5fdca310b2226cfb98cf38659abb4fc053598dd9 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/ramecc" 
	@${RM} ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o.d 
	@${RM} ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o.d" -o ${OBJECTDIR}/hpl/ramecc/hpl_ramecc.o hpl/ramecc/hpl_ramecc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/sercom/hpl_sercom.o: hpl/sercom/hpl_sercom.c  .generated_files/flags/Default/28f6a101495cf13139eea88a6eced019929c3146 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/sercom" 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d 
	@${RM} ${OBJECTDIR}/hpl/sercom/hpl_sercom.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/sercom/hpl_sercom.o.d" -o ${OBJECTDIR}/hpl/sercom/hpl_sercom.o hpl/sercom/hpl_sercom.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/hpl/usb/hpl_usb.o: hpl/usb/hpl_usb.c  .generated_files/flags/Default/6fa89b67721b52d5479d2682c7aff97f6c441f5e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hpl/usb" 
	@${RM} ${OBJECTDIR}/hpl/usb/hpl_usb.o.d 
	@${RM} ${OBJECTDIR}/hpl/usb/hpl_usb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/hpl/usb/hpl_usb.o.d" -o ${OBJECTDIR}/hpl/usb/hpl_usb.o hpl/usb/hpl_usb.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/printf/printf/printf.o: printf/printf/printf.c  .generated_files/flags/Default/9487cc39131b7d5373425a0685db4815e7a68349 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/printf/printf" 
	@${RM} ${OBJECTDIR}/printf/printf/printf.o.d 
	@${RM} ${OBJECTDIR}/printf/printf/printf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/printf/printf/printf.o.d" -o ${OBJECTDIR}/printf/printf/printf.o printf/printf/printf.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o: samd51a/gcc/gcc/startup_samd51.c  .generated_files/flags/Default/445e9fe28f550d0e1848604887599e01d1b0ece7 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/samd51a/gcc/gcc" 
	@${RM} ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o.d 
	@${RM} ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o.d" -o ${OBJECTDIR}/samd51a/gcc/gcc/startup_samd51.o samd51a/gcc/gcc/startup_samd51.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/samd51a/gcc/system_samd51.o: samd51a/gcc/system_samd51.c  .generated_files/flags/Default/1499e50b324054c9675f9e890b6a51e92b173a9e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/samd51a/gcc" 
	@${RM} ${OBJECTDIR}/samd51a/gcc/system_samd51.o.d 
	@${RM} ${OBJECTDIR}/samd51a/gcc/system_samd51.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/samd51a/gcc/system_samd51.o.d" -o ${OBJECTDIR}/samd51a/gcc/system_samd51.o samd51a/gcc/system_samd51.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/gcc/read.o: stdio_redirect/gcc/read.c  .generated_files/flags/Default/bcdbce7bcfd5f4e9b52f7b4d7a232e92c1111108 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/read.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/read.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/read.o stdio_redirect/gcc/read.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/gcc/write.o: stdio_redirect/gcc/write.c  .generated_files/flags/Default/c44d3e681b200369a49c4e70a39dc4f7e3771e23 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect/gcc" 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/gcc/write.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/gcc/write.o.d" -o ${OBJECTDIR}/stdio_redirect/gcc/write.o stdio_redirect/gcc/write.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_redirect/stdio_io.o: stdio_redirect/stdio_io.c  .generated_files/flags/Default/8dd8274b2d1f3e9c54ed4896cd25954f327aff8f .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/stdio_redirect" 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o.d 
	@${RM} ${OBJECTDIR}/stdio_redirect/stdio_io.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_redirect/stdio_io.o.d" -o ${OBJECTDIR}/stdio_redirect/stdio_io.o stdio_redirect/stdio_io.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o: usb/class/cdc/device/cdcdf_acm.c  .generated_files/flags/Default/a252f64ca7f4c3d3bec34aac4d86d30bbbecc221 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb/class/cdc/device" 
	@${RM} ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o.d 
	@${RM} ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o.d" -o ${OBJECTDIR}/usb/class/cdc/device/cdcdf_acm.o usb/class/cdc/device/cdcdf_acm.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/device/usbdc.o: usb/device/usbdc.c  .generated_files/flags/Default/2374ca46885a23eaa4b98ac27b4233659ab73c78 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb/device" 
	@${RM} ${OBJECTDIR}/usb/device/usbdc.o.d 
	@${RM} ${OBJECTDIR}/usb/device/usbdc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/device/usbdc.o.d" -o ${OBJECTDIR}/usb/device/usbdc.o usb/device/usbdc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb/usb_protocol.o: usb/usb_protocol.c  .generated_files/flags/Default/498b8389f85c3340d6e84ebd7c19f7ef55c45468 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_protocol.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb/usb_protocol.o.d" -o ${OBJECTDIR}/usb/usb_protocol.o usb/usb_protocol.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/Default/4c8aee254523976acfe5160b5e54f33a80a1abe .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/driver_init.o: driver_init.c  .generated_files/flags/Default/a401f9c3fe08d4dd86806c25b55c5d22001b81f8 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver_init.o.d 
	@${RM} ${OBJECTDIR}/driver_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/driver_init.o.d" -o ${OBJECTDIR}/driver_init.o driver_init.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/usb_start.o: usb_start.c  .generated_files/flags/Default/75db0bfc3cd7efe15d7e804ccb33aa4f40e5b801 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_start.o.d 
	@${RM} ${OBJECTDIR}/usb_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/usb_start.o.d" -o ${OBJECTDIR}/usb_start.o usb_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/atmel_start.o: atmel_start.c  .generated_files/flags/Default/70fe369318f91a90a27dab8c54edc871a1a7850e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/atmel_start.o.d 
	@${RM} ${OBJECTDIR}/atmel_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/atmel_start.o.d" -o ${OBJECTDIR}/atmel_start.o atmel_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/stdio_start.o: stdio_start.c  .generated_files/flags/Default/49b4e31ee5f17fca1b3d660cd2263def373c98d5 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/stdio_start.o.d 
	@${RM} ${OBJECTDIR}/stdio_start.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/stdio_start.o.d" -o ${OBJECTDIR}/stdio_start.o stdio_start.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o: ../Common/SymRuntime/wrapped_malloc.c  .generated_files/flags/Default/8183adbfac7dc0206007a1e63b9b468a46068a30 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o.d" -o ${OBJECTDIR}/_ext/1423857446/wrapped_malloc.o ../Common/SymRuntime/wrapped_malloc.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o: ../Common/SymRuntime/FreeRTOS_callbacks.c  .generated_files/flags/Default/f4226b8b06c3a167c2a5ae0b0c415c49b92e14c8 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o.d" -o ${OBJECTDIR}/_ext/1423857446/FreeRTOS_callbacks.o ../Common/SymRuntime/FreeRTOS_callbacks.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1423857446/runtime.o: ../Common/SymRuntime/runtime.c  .generated_files/flags/Default/75236ab53e6961347655d05d7fffc7f27bd9c7c4 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1423857446" 
	@${RM} ${OBJECTDIR}/_ext/1423857446/runtime.o.d 
	@${RM} ${OBJECTDIR}/_ext/1423857446/runtime.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1423857446/runtime.o.d" -o ${OBJECTDIR}/_ext/1423857446/runtime.o ../Common/SymRuntime/runtime.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/tasks.o: ../Common/FreeRTOS-Kernel/tasks.c  .generated_files/flags/Default/df679fc5fc4be64a2d501d1860429062a3675ffc .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/tasks.o.d" -o ${OBJECTDIR}/_ext/924481411/tasks.o ../Common/FreeRTOS-Kernel/tasks.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/stream_buffer.o: ../Common/FreeRTOS-Kernel/stream_buffer.c  .generated_files/flags/Default/319a02d6687c5c527e2562074b80afe184151099 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/stream_buffer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/924481411/stream_buffer.o ../Common/FreeRTOS-Kernel/stream_buffer.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/queue.o: ../Common/FreeRTOS-Kernel/queue.c  .generated_files/flags/Default/4ed08dcf2377dc3cdc71aa2f61555da17141d6de .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/queue.o.d" -o ${OBJECTDIR}/_ext/924481411/queue.o ../Common/FreeRTOS-Kernel/queue.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/croutine.o: ../Common/FreeRTOS-Kernel/croutine.c  .generated_files/flags/Default/da4302014283bd17a647484a8e22e550c521ebb5 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/croutine.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/croutine.o.d" -o ${OBJECTDIR}/_ext/924481411/croutine.o ../Common/FreeRTOS-Kernel/croutine.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/event_groups.o: ../Common/FreeRTOS-Kernel/event_groups.c  .generated_files/flags/Default/4b12ffe43b62a0530d348e47c4c4103a06a31804 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/event_groups.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/event_groups.o.d" -o ${OBJECTDIR}/_ext/924481411/event_groups.o ../Common/FreeRTOS-Kernel/event_groups.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/timers.o: ../Common/FreeRTOS-Kernel/timers.c  .generated_files/flags/Default/2a3c6e46b7626e0a565ae49ed84ba3e74bfb5dc5 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/timers.o.d" -o ${OBJECTDIR}/_ext/924481411/timers.o ../Common/FreeRTOS-Kernel/timers.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/924481411/list.o: ../Common/FreeRTOS-Kernel/list.c  .generated_files/flags/Default/10af61da9b49256e1ec474283453f68b7157e78e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/924481411" 
	@${RM} ${OBJECTDIR}/_ext/924481411/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/924481411/list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/924481411/list.o.d" -o ${OBJECTDIR}/_ext/924481411/list.o ../Common/FreeRTOS-Kernel/list.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1346478737/port.o: ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c  .generated_files/flags/Default/3760eb97ea75ce51cfff103c90761661a526f6a0 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1346478737" 
	@${RM} ${OBJECTDIR}/_ext/1346478737/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1346478737/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1346478737/port.o.d" -o ${OBJECTDIR}/_ext/1346478737/port.o ../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/1243373050/heap_4.o: ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c  .generated_files/flags/Default/42bcc612157b8100c3fef9f4f2e9770a8a721a2f .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1243373050" 
	@${RM} ${OBJECTDIR}/_ext/1243373050/heap_4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1243373050/heap_4.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1243373050/heap_4.o.d" -o ${OBJECTDIR}/_ext/1243373050/heap_4.o ../Common/FreeRTOS-Kernel/portable/MemMang/heap_4.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/protocol.o: ../Common/Protocol/protocol.c  .generated_files/flags/Default/8e3093821aad8934d917c1c04cb4419506e500a3 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/protocol.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/protocol.o.d" -o ${OBJECTDIR}/_ext/179706861/protocol.o ../Common/Protocol/protocol.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/ring.o: ../Common/Protocol/ring.c  .generated_files/flags/Default/8b92f7c85fab84ae76a92f1447242bc723a8d92e .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/ring.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/ring.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/ring.o.d" -o ${OBJECTDIR}/_ext/179706861/ring.o ../Common/Protocol/ring.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/_ext/179706861/monitor.o: ../Common/Protocol/monitor.c  .generated_files/flags/Default/4138eb03035bf541c2cbc82720d066595b30eb85 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/179706861" 
	@${RM} ${OBJECTDIR}/_ext/179706861/monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/179706861/monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/179706861/monitor.o.d" -o ${OBJECTDIR}/_ext/179706861/monitor.o ../Common/Protocol/monitor.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/Target/ATParser.o: Target/ATParser.c  .generated_files/flags/Default/398d9e24d775fc289e7ac47ab5d8dc04bbebbba .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Target" 
	@${RM} ${OBJECTDIR}/Target/ATParser.o.d 
	@${RM} ${OBJECTDIR}/Target/ATParser.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/Target/ATParser.o.d" -o ${OBJECTDIR}/Target/ATParser.o Target/ATParser.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
${OBJECTDIR}/Target/test.o: Target/test.c  .generated_files/flags/Default/2cef0161d45a7e010b285071cf2429eaa1fe4af8 .generated_files/flags/Default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Target" 
	@${RM} ${OBJECTDIR}/Target/test.o.d 
	@${RM} ${OBJECTDIR}/Target/test.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb -I "usb" -I "hpl/port" -I "hpl/osc32kctrl" -I "hpl/oscctrl" -I "./." -I "hpl/cmcc" -I "usb/class/cdc" -I "hal/utils/include" -I "hal/include" -I "hpl/pm" -I "usb/class/cdc/device" -I "hri" -I "hpl/gclk" -I "hpl/mclk" -I "hpl/core" -I "hpl/ramecc" -I "usb/device" -I "hpl/dmac" -I "examples" -I "Target" -I "hpl/usb" -I "config" -I "CMSIS/Core/Include" -I "stdio_redirect" -I "../Common/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I "../Common/FreeRTOS-Kernel/include" -I "../Common/FreeRTOS-Kernel" -I "../Common/Protocol" -I "../Common/SymRuntime"  -I "samd51a/include" -I "CMSIS/Core/Include"  -O1 -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/Target/test.o.d" -o ${OBJECTDIR}/Target/test.o Target/test.c  -DXPRJ_Default=$(CND_CONF)  $(COMPARISON_BUILD)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -g3
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__    -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}/SAMD51.${IMAGE_TYPE}.map"  -o ${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1  -L"samd51a/gcc/gcc" -Wl,--gc-sections   -T"samd51a/gcc/gcc/samd51j19a_flashMPU.ld" 
	
	
	
	
	
	
else
${DISTDIR}/SAMD51.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__$(MP_PROCESSOR_OPTION)__    -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}/SAMD51.${IMAGE_TYPE}.map"  -o ${DISTDIR}/SAMD51.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Default=$(CND_CONF)  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)  -L"samd51a/gcc/gcc" -Wl,--gc-sections   -T"samd51a/gcc/gcc/samd51j19a_flashMPU.ld" 
	
	${MP_CC_DIR}/arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/SAMD51.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/SAMD51.${IMAGE_TYPE}.hex"
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
