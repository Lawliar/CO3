#
# Generated Makefile - do not edit!
#
#
# This file contains information about the location of compilers and other tools.
# If you commmit this file into your revision control server, you will be able to 
# to checkout the project and build it from the command line with make. However,
# if more than one person works on the same project, then this file might show
# conflicts since different users are bound to have compilers in different places.
# In that case you might choose to not commit this file and let MPLAB X recreate this file
# for each user. The disadvantage of not commiting this file is that you must run MPLAB X at
# least once so the file gets created and the project can be built. Finally, you can also
# avoid using this file at all if you are only building from the command line with make.
# You can invoke make with the values of the macros:
# $ makeMP_CC="/opt/microchip/mplabc30/v3.30c/bin/pic30-gcc" ...  
#
PATH_TO_IDE_BIN=/opt/microchip/mplabx/v6.15/mplab_platform/platform/../mplab_ide/modules/../../bin/
# Adding MPLAB X bin directory to path.
PATH:=/opt/microchip/mplabx/v6.15/mplab_platform/platform/../mplab_ide/modules/../../bin/:$(PATH)
# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="/opt/microchip/mplabx/v6.15/sys/java/zulu8.64.0.19-ca-fx-jre8.0.345-linux_x64/bin/"
OS_CURRENT="$(shell uname -s)"
MP_CC="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin/arm-none-eabi-gcc"
MP_CPPC="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin/arm-none-eabi-g++"
# MP_BC is not defined
MP_AS="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin/arm-none-eabi-as"
MP_LD="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin/arm-none-eabi-ld"
MP_AR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin/arm-none-eabi-ar"
DEP_GEN=${MP_JAVA_PATH}java -jar "/opt/microchip/mplabx/v6.15/mplab_platform/platform/../mplab_ide/modules/../../bin/extractobjectdependencies.jar"
MP_CC_DIR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin"
MP_CPPC_DIR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin"
# MP_BC_DIR is not defined
MP_AS_DIR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin"
MP_LD_DIR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin"
MP_AR_DIR="/home/lcm/github/toolchains/arm/gcc-linaro12.3.1-0x20020000/bin"
DFP_DIR=/opt/microchip/mplabx/v6.15/packs/Microchip/SAMD51_DFP/3.7.237
CMSIS_DIR=/opt/microchip/mplabx/v6.15/packs/arm/CMSIS/5.4.0
