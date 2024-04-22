// This file is part of the CO3 runtime.
//
// The CO3 runtime is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The CO3 runtime is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with CO3. If not, see <https://www.gnu.org/licenses/>.


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