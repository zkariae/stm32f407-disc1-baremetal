// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/led/led.c
\brief      LED on a GPIO pin, either polarity.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  API documented in led.h.

\section led_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include "led/led.h"

#include <stdint.h>

#include "gpio/gpio.h"

void LedInit(const Led *led)
{
  GpioEnableClock(led->port);

  // Off level first, so the LED does not flash when the pin becomes an output
  if (led->active_high) {
    GpioClear(led->port, led->pin);
  } else {
    GpioSet(led->port, led->pin);
  }

  GpioSetOutput(led->port, led->pin);
}

void LedToggle(const Led *led)
{
  GpioToggle(led->port, led->pin);
}
