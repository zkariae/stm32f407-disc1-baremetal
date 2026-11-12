// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/rcc/rcc.h
\brief      Clock configuration: 168 MHz system clock.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  System: HSE / M (1 MHz VCO input) * N=336 / P=2 = 168 MHz; AHB /1, APB1 /4,
  APB2 /2. Every hardware wait is bounded: the function returns TIMEOUT
  instead of hanging when a clock does not start.

\section rcc_h_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#include "defines.h"

#define SYSCLK_HZ 168000000UL ///< SYSCLK and HCLK after RccInit168MHz().
#define APB1_HZ   42000000UL  ///< APB1 clock after RccInit168MHz().
#define APB2_HZ   84000000UL  ///< APB2 clock after RccInit168MHz().

/*!
\brief  Switch SYSCLK from HSI to the main PLL at 168 MHz.

\details
  Starts the HSE, sets 5 flash wait states and the bus prescalers, then
  switches to the PLL. On TIMEOUT SYSCLK stays on the 16 MHz HSI: SYSCLK_HZ,
  APB1_HZ and APB2_HZ do not apply.

\param  hse_hz  HSE frequency, a multiple of 1 MHz (crystal: 4 to 26 MHz).
\return SUCCESS, or TIMEOUT if the HSE, the PLL or the clock switch does not
        respond.
*/
int RccInit168MHz(uint32_t hse_hz);

#endif
