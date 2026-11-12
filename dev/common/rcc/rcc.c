// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/rcc/rcc.c
\brief      Clock configuration: 168 MHz system clock.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  Registers with several fields to change are read into a local copy, edited,
  then written once through .raw: reserved bits keep their value.

\section rcc_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include "rcc.h"

#include <stdint.h>

#include "defines.h"
#include "mcu.h"

#define PLL_N 336U ///< VCO = 1 MHz * 336 = 336 MHz.
#define PLL_Q 7U   ///< 336 / 7 = 48 MHz for USB.

#define RCC_TIMEOUT_LOOPS 1000000U ///< >= 60 ms at 16 MHz (HSE: 2 ms).

/*!
\brief  Wait until (*reg & mask) == value.
\return SUCCESS, or TIMEOUT after RCC_TIMEOUT_LOOPS polls.
*/
static int wait_bits(volatile uint32_t *reg, uint32_t mask, uint32_t value)
{
  for (uint32_t i = 0; i < RCC_TIMEOUT_LOOPS; i++) {
    if ((*reg & mask) == value) {
      return SUCCESS;
    }
  }
  return TIMEOUT;
}

int RccInit168MHz(uint32_t hse_hz)
{
  T_rcc_pllcfgr pllcfgr;
  T_flash_acr acr;
  T_rcc_cfgr cfgr;

  RCC_CR.bitfield.hseon = 1;
  if (wait_bits(&RCC_CR.raw, 1U << 17, 1U << 17) != SUCCESS) // HSERDY
    return TIMEOUT;

  // PLLCFGR is only writable while the PLL is off
  pllcfgr.raw = RCC_PLLCFGR.raw;
  pllcfgr.bitfield.pllm = hse_hz / 1000000U; // 1 MHz VCO input
  pllcfgr.bitfield.plln = PLL_N;
  pllcfgr.bitfield.pllp = RCC_PLLP_DIV2;
  pllcfgr.bitfield.pllsrc = RCC_PLLSRC_HSE;
  pllcfgr.bitfield.pllq = PLL_Q;
  RCC_PLLCFGR.raw = pllcfgr.raw;

  RCC_CR.bitfield.pllon = 1;

  // Wait states and prescalers before the switch, never after
  acr.raw = FLASH_ACR.raw;
  acr.bitfield.latency = FLASH_LATENCY_5WS;
  acr.bitfield.prften = 1;
  acr.bitfield.icen = 1;
  acr.bitfield.dcen = 1;
  FLASH_ACR.raw = acr.raw;
  if (wait_bits(&FLASH_ACR.raw, 7U, FLASH_LATENCY_5WS) != SUCCESS) // LATENCY
    return TIMEOUT;

  cfgr.raw = RCC_CFGR.raw;
  cfgr.bitfield.hpre = RCC_HPRE_DIV1;  // 168 MHz
  cfgr.bitfield.ppre1 = RCC_PPRE_DIV4; //  42 MHz
  cfgr.bitfield.ppre2 = RCC_PPRE_DIV2; //  84 MHz
  RCC_CFGR.raw = cfgr.raw;

  if (wait_bits(&RCC_CR.raw, 1U << 25, 1U << 25) != SUCCESS) // PLLRDY
    return TIMEOUT;

  RCC_CFGR.bitfield.sw = RCC_SYSCLK_PLL;
  return wait_bits(&RCC_CFGR.raw, 3U << 2, RCC_SYSCLK_PLL << 2); // SWS confirms
}
