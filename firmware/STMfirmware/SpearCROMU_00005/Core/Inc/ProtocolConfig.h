//a copy of this file must be renamed to ProtocolConfig.h  and added to the includes of this project,
// do not use this file in the includes this is  just a template

#ifndef PROTOCOLCONFIG_H_
#define PROTOCOLCONFIG_H_

#define AFL_BUFFER_STARTING_POINT 4
#define REPORTING_BUFFER_STARTING_POINT 1
#define MAX_BUFFER_INPUT 1036
#define MAX_BUFFER_OUTPUT 128
#define MAX_USB_FRAME 64
#define MAX_USB_FUNCTIONS 8

// 1. if you want to use the MCU-side shadow memory or not

//#define CO3_NO_MCU_SIDE_SHADOW


#if defined CO3_NO_MCU_SIDE_SHADOW
#else
#define SYM_RAM_ADDR_START           0x24000000 // the start of the RAM that you want to shadow-mapping to (i.e., this is not the shadow memory itself, but the RAM desginated to the eApp that you want to maintain the symbolic states for)
#define SYM_RAM_SIZE                 1024*512 // the size of the desginated RAM used by the eApp
#define SYM_SHADOW_RAM_START         0x30000000 // the start address of the shadow memory itself
#define SYM_SHADOW_RAM_LENGTH        1024*64  // the size of the shadow memory
#define SYM_SHADOW_RAM_OFFSET        0x2B800000 // the offset used by the formula mapping the address of the RAM its shadow memory ( ADDR >> 3 + OFFSET = Shadow_Addr)
#endif

#define SYM_FLASH_ADDR_START         0x08000000
#define SYM_FLASH_SIZE               1024*2048

#define SYM_PERIPHERAL_ADDR_START    0x40000000
#define SYM_PERIPHERAL_SIZE          0x1FFFFFFF


//2. which vendor is this board

#define CO3_USE_STM32
//#define CO3_USE_NXP
//#define CO3_USE_MICROCHIP

// 3. board specifications
#define CO3_USE_STM32_H743
//#define CO3_USE_NXP_K66F
//#define CO3_USE_MICROCHIP_SAMD51

// 4. which communication channel you would like to use
#define CO3_USE_USB
//#define CO3_USE_SERIAL

// 5. which RTOS you use
#define CO3_USE_FREERTOS  // one thing to notice is that FreeRTOS does not provide any HAL
//#define CO3_USE_CHIBIOS


// 6. APP-specific MACRO
//#define CO3_TEST_COMMANDLINE
#define CO3_TEST_CGC
//#define CO3_TEST_MODBUSDMA
//#define CO3_TEST_MIDIDMA
//#define CO3_TEST_SHELLYDIMMER
//#define CO3_TEST_ATPARSER




#endif /* PROTOCOLCONFIG_H_ */
