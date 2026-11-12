// SPDX-License-Identifier: MIT
/*!
****************************
\file       01-blink/main.c
\brief      Blink BOARD_LED_LD4 with a cycle-counted delay.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  Bare-metal loop that:
  1) Enables the GPIOD clock and configures PD12 (BOARD_LED_LD4) as an
     output, LED off.
  2) Toggles it every 0.5 s, the delay counted by the DWT cycle counter.
  Runs on the 16 MHz HSI reset clock; no PLL, no interrupts.

\section blink_main_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include <stdint.h>

#include "board.h"
#include "delay/delay.h"
#include "led/led.h"

#define CPU_HZ 16000000U ///< HSI, the clock at reset.

/*!
\brief  Application entry point: blink BOARD_LED_LD4 forever.
\return Never returns.
*/
int main(void)
{
  LedInit(&BOARD_LED_LD4);

  while (1) {
    LedToggle(&BOARD_LED_LD4);
    DelayCycles(CPU_HZ / 2U); // 0.5 s
  }
}
