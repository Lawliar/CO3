/*
 * ProtocolConfig.h
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */

#ifndef PROTOCOLCONFIG_H_
#define PROTOCOLCONFIG_H_

#define AFL_BUFFER_STARTING_POINT 4
#define REPORTING_BUFFER_STARTING_POINT 1
#define MAX_BUFFER_INPUT 1036
#define MAX_BUFFER_OUTPUT 128
#define MAX_USB_FRAME 64
#define MAX_USB_FUNCTIONS 8

#define SYM_RAM_ADDR_START           0x24000000
#define SYM_RAM_SIZE                 1024*512
#define SYM_SHADOW_RAM_START         0x30000000
#define SYM_SHADOW_RAM_LENGTH        1024*64
#define SYM_SHADOW_RAM_OFFSET        0x2B800000

#define SYM_FLASH_ADDR_START         0x08000000
#define SYM_FLASH_SIZE               1024*2048

#define SYM_PERIPHERAL_ADDR_START    0x40000000
#define SYM_PERIPHERAL_SIZE          0x1FFFFFFF

#define CO3_USE_USB

#define CO3_USE_FREERTOS
#endif /* PROTOCOLCONFIG_H_ */
