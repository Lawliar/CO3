/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdint.h>
#include "monitor.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  chThdSleepMilliseconds(500);
  SerialConfig sc_shift = {
  	        115200,
  	        USART_CR1_UE | USART_CR1_TE | USART_CR1_RE,
  	        USART_CR2_STOP1_BITS,
  	        0
  	};
    palSetPadMode(GPIOD, 6, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOD, 5, PAL_MODE_ALTERNATE(7));

    sdStart(&SD2, &sc_shift);
    int a = 4;
    while(1){
      sdWrite(&SD2, (uint8_t*)&a, 4);
      sdReadTimeout(&SD2, (uint8_t * )&a, sizeof(uint32_t) , TIME_INFINITE);
      chThdSleepMilliseconds(500);
    }

    app_main();
    while(1){}
}
