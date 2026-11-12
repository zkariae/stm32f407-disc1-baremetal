// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/delay/delay.c
\brief      Busy-wait delay counted in CPU cycles.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  API documented in delay.h. On the Cortex-M4 the DWT needs no unlock
  (no DWT_LAR access, unlike the Cortex-M7).

\section delay_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include "delay/delay.h"

#include <stdint.h>

#include "mcu.h"

void DelayCycles(uint32_t cycles)
{
  // Start the cycle counter; harmless if it already runs
  DEMCR.bitfield.trcena = 1;
  DWT_CTRL.bitfield.cyccntena = 1;

  // Unsigned difference: still right when DWT_CYCCNT wraps around
  uint32_t start = DWT_CYCCNT;
  while ((DWT_CYCCNT - start) < cycles) {}
}
