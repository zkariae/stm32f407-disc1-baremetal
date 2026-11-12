// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/gpio/gpio.c
\brief      GPIO pin configuration and output control.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  API documented in gpio.h. The pin is only known at run time, so the
  registers are accessed through .raw with shifts, not through bit fields.

\section gpio_c_version_history Version history
| Version | Date       | Author      | Description            |
|:-------:|:----------:|:------------|:-----------------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version.       |
****************************
*/

#include "gpio/gpio.h"

#include <stdint.h>

#include "mcu.h"

void GpioEnableClock(uint32_t port)
{
  // Ports are 0x400 apart from GPIOA, and their enable bits follow the same order
  RCC_AHB1ENR.raw |= 1U << ((port - GPIOA_BASE) / 0x400U);
  (void)RCC_AHB1ENR.raw; // read back: the clock reaches the port
}

void GpioSetOutput(uint32_t port, uint32_t pin)
{
  uint32_t shift = pin * 2U; // two mode bits per pin

  GPIO_MODER(port).raw = (GPIO_MODER(port).raw & ~(3U << shift)) | (GPIO_MODE_OUTPUT << shift);
}

void GpioSet(uint32_t port, uint32_t pin)
{
  GPIO_BSRR(port) = 1U << pin;
}

void GpioClear(uint32_t port, uint32_t pin)
{
  GPIO_BSRR(port) = 1U << (pin + 16U);
}

void GpioToggle(uint32_t port, uint32_t pin)
{
  // Set if low, reset if high: one BSRR write never disturbs the other pins
  if (GPIO_ODR(port) & (1U << pin)) {
    GPIO_BSRR(port) = 1U << (pin + 16U);
  } else {
    GPIO_BSRR(port) = 1U << pin;
  }
}
