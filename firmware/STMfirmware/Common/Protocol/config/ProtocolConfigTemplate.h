/*
 * ProtocolConfig.h
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */

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

// see if you wanna use serial port over usb
//#define USE_SERIAL_OVER_USB

#endif /* PROTOCOLCONFIG_H_ */
