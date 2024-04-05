//a copy of this file must be renamed to ProtocolConfig.h  and added to the includes of this project,
// do not use this file in the includes this is  just a template

#ifndef PROTOCOLCONFIG_H_
#define PROTOCOLCONFIG_H_

#define RX_BUFFER_STARTING_POINT 4
#define TX_BUFFER_STARTING_POINT 1

#define RX_BUFFER_SIZE 1036
#define TX_BUFFER_SIZE 256 

// 1. if you want to use the MCU-side shadow memory or not

//#define CO3_NO_SHADOW


#if defined CO3_NO_SHADOW
#else
#define SYM_RAM_ADDR_START           0x24000000 // the start of the RAM that you want to shadow-mapping to (i.e., this is not the shadow memory itself, but the RAM desginated to the eApp that you want to maintain the symbolic states for)
#define SYM_RAM_SIZE                 1024*512 // the size of the desginated RAM used by the eApp
#define SYM_SHADOW_RAM_START         0x30000000 // the start address of the shadow memory itself
#define SYM_SHADOW_RAM_LENGTH        1024*64  // the size of the shadow memory
#define SYM_SHADOW_RAM_OFFSET        0x2B800000 // the offset used by the formula mapping the address of the RAM its shadow memory ( ADDR >> 3 + OFFSET = Shadow_Addr)
#endif



// 6. APP-specific MACRO
#define CO3_TEST_COMMANDLINE
//#define CO3_TEST_CGC
//#define CO3_TEST_MODBUSDMA
//#define CO3_TEST_MIDIDMA
//#define CO3_TEST_SHELLYDIMMER
//#define CO3_TEST_ATPARSER




#endif /* PROTOCOLCONFIG_H_ */
