*****************************************************************************
** ChibiOS/HAL + ChibiOS/EX - SPI + ADXL355 demo for STM32F4xx.            **
*****************************************************************************

** TARGET **

The demo runs on an ADI EVAL-SDP-CK1Z board. It has been tested 
connecting an external the EVAL-ADXL355.

** The Demo **

The application demonstrates the use of the STM32F4xx SPI driver in order
to acquire data from ADXL355 using ChibiOS/EX.

** Board Setup **

With reference to the ADI UG-1030 (EVAL-ADXL354/EVAL-ADXL355 User Guide) and
to the Schematic of the SDP-K1 the following connection
are needed:
 ---------------------------------------------
|     EVAL-ADXL355     |        SDP-K1        |
|---------------------------------------------|
|                      |                      |
|         P1.1         |      ARD_IOREF       |
|         P1.3         |       ARD_3V3        |
|         P1.5         |       ARD_GND        |
|                      |                      |
|         P2.2         |       ARD_D10        |
|         P2.4         |       ARD_D13        |
|         P2.5         |       ARD_D12        |
|         P2.6         |       ARD_D11        |
 ---------------------------------------------
** Build Procedure **

The demo has been tested using the free Codesourcery GCC-based toolchain
and YAGARTO.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com