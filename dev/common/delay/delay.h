// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/delay/delay.h
\brief      Busy-wait delay counted in CPU cycles.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  DelayCycles() counts with the DWT cycle counter, so its duration does not
  depend on the compiler or the optimisation level, only on the CPU clock.
  It needs no setup.

\section delay_h_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/*!
\brief  Busy-wait for a number of CPU cycles.

\details
  Duration is cycles / CPU clock: 8000000 cycles last 0.5 s at 16 MHz.
  Up to 2^32 - 1 cycles, about 268 s at 16 MHz or 25 s at 168 MHz.

\param  cycles  Number of CPU cycles to wait.
*/
void DelayCycles(uint32_t cycles);

#endif
