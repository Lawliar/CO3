/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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

/**
 * @file    TIMv1/stm32_tim1_16_17.inc
 * @brief   Shared TIM1, TIM16, TIM17 handler.
 *
 * @addtogroup STM32_TIM1_TIM16_TIM17_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_TIM1)
#error "STM32_HAS_TIM1 not defined in registry"
#endif

#if !defined(STM32_HAS_TIM16)
#error "STM32_HAS_TIM16 not defined in registry"
#endif

#if !defined(STM32_HAS_TIM17)
#error "STM32_HAS_TIM17 not defined in registry"
#endif

/* Driver checks for robustness, undefined USE macros are defaulted to
   FALSE. This makes this module independent from drivers implementation.*/
#if !defined(STM32_GPT_USE_TIM1)
#define STM32_GPT_USE_TIM1                  FALSE
#endif
#if !defined(STM32_ICU_USE_TIM1)
#define STM32_ICU_USE_TIM1                  FALSE
#endif
#if !defined(STM32_PWM_USE_TIM1)
#define STM32_PWM_USE_TIM1                  FALSE
#endif
#if !defined(STM32_ST_USE_TIM1)
#define STM32_ST_USE_TIM1                   FALSE
#endif

#if !defined(STM32_GPT_USE_TIM16)
#define STM32_GPT_USE_TIM16                 FALSE
#endif
#if !defined(STM32_ICU_USE_TIM16)
#define STM32_ICU_USE_TIM16                 FALSE
#endif
#if !defined(STM32_PWM_USE_TIM16)
#define STM32_PWM_USE_TIM16                 FALSE
#endif
#if !defined(STM32_ST_USE_TIM16)
#define STM32_ST_USE_TIM16                  FALSE
#endif

#if !defined(STM32_GPT_USE_TIM17)
#define STM32_GPT_USE_TIM17                 FALSE
#endif
#if !defined(STM32_ICU_USE_TIM17)
#define STM32_ICU_USE_TIM17                 FALSE
#endif
#if !defined(STM32_PWM_USE_TIM17)
#define STM32_PWM_USE_TIM17                 FALSE
#endif
#if !defined(STM32_ST_USE_TIM17)
#define STM32_ST_USE_TIM17                  FALSE
#endif

#if STM32_HAS_TIM1 || STM32_HAS_TIM16 || STM32_HAS_TIM17

/* Priority settings checks.*/
#if !defined(STM32_IRQ_TIM1_BRK_PRIORITY)
#error "STM32_IRQ_TIM1_BRK_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(STM32_IRQ_TIM1_UP_TIM16_PRIORITY)
#error "STM32_IRQ_TIM1_UP_TIM16_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY)
#error "STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(STM32_IRQ_TIM1_CC_PRIORITY)
#error "STM32_IRQ_TIM1_CC_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_BRK_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_BRK_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_UP_TIM16_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_UP_TIM16_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_CC_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_CC_PRIORITY"
#endif

#endif /* STM32_HAS_TIM1 || STM32_HAS_TIM16 || STM32_HAS_TIM17 */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void tim1_tim16_tim17_irq_init(void) {
#if defined(STM32_TIM1_IS_USED)
  nvicEnableVector(STM32_TIM1_BRK_NUMBER,
                   STM32_IRQ_TIM1_BRK_PRIORITY);
#endif
#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM16_IS_USED)
  nvicEnableVector(STM32_TIM1_UP_TIM16_NUMBER,
                   STM32_IRQ_TIM1_UP_TIM16_PRIORITY);
#endif
#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM17_IS_USED)
  nvicEnableVector(STM32_TIM1_TRGCO_TIM17_NUMBER,
                   STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY);
#endif
#if defined(STM32_TIM1_IS_USED)
  nvicEnableVector(STM32_TIM1_CC_NUMBER,
                   STM32_IRQ_TIM1_CC_PRIORITY);
#endif
}

static inline void tim1_tim16_tim17_irq_deinit(void) {
#if defined(STM32_TIM1_IS_USED)
  nvicDisableVector(STM32_TIM1_BRK_NUMBER);
#endif
#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM16_IS_USED)
  nvicDisableVector(STM32_TIM1_UP_TIM16_NUMBER);
#endif
#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM17_IS_USED)
  nvicDisableVector(STM32_TIM1_TRGCO_TIM17_NUMBER);
#endif
#if defined(STM32_TIM1_IS_USED)
  nvicDisableVector(STM32_TIM1_CC_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(STM32_TIM1_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   TIM1-BRK interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_BRK_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM16_IS_USED)  ||         \
    defined(__DOXYGEN__)
/**
 * @brief   TIM1-UP, TIM16 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_UP_TIM16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if STM32_GPT_USE_TIM1
  gpt_lld_serve_interrupt(&GPTD1);
#endif
#if STM32_GPT_USE_TIM16
  gpt_lld_serve_interrupt(&GPTD16);
#endif
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM1
  icu_lld_serve_interrupt(&ICUD1);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM1
  pwm_lld_serve_interrupt(&PWMD1);
#endif
#if STM32_PWM_USE_TIM16
  pwm_lld_serve_interrupt(&PWMD16);
#endif
#endif
#if 1
#if STM32_ST_USE_TIM1
  st_lld_serve_interrupt();
#endif
#if STM32_ST_USE_TIM16
  st_lld_serve_interrupt();
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if defined(STM32_TIM1_IS_USED) || defined(STM32_TIM17_IS_USED)  ||         \
    defined(__DOXYGEN__)
/**
 * @brief   TIM1-TRG-COM, TIM17 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_TRGCO_TIM17_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if STM32_GPT_USE_TIM17
  gpt_lld_serve_interrupt(&GPTD17);
#endif
#endif
#if HAL_USE_ICU
  /* Not used by ICU.*/
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM17
  pwm_lld_serve_interrupt(&PWMD17);
#endif
#endif
#if 1
#if STM32_ST_USE_TIM17
  st_lld_serve_interrupt();
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if defined(STM32_TIM1_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   TIM1-CC interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
  /* Not used by GPT.*/
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM1
  icu_lld_serve_interrupt(&ICUD1);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM1
  pwm_lld_serve_interrupt(&PWMD1);
#endif
#endif
#if 1
  /* Not used by ST.*/
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
