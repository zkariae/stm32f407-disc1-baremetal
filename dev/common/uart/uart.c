// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/uart/uart.c
\brief      USART transmit driver, 8N1.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  API documented in uart.h. STM32F4 register layout (SR and DR, RM0090
  section 30.6). 8N1 is the reset configuration of CR1 and CR2, and the
  default 16x oversampling (OVER8 = 0) makes BRR equal to pclk / baud.

\section uart_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include "uart/uart.h"

#include <stdint.h>

#include "mcu.h"
#include "gpio/gpio.h"

void UartInit(const Uart *uart, uint32_t pclk_hz, uint32_t baud)
{
  *uart->enr |= uart->enr_bit;
  (void)*uart->enr; // read back: the clock reaches the USART

  GpioEnableClock(uart->tx_port);
  GpioSetAf(uart->tx_port, uart->tx_pin, uart->tx_af);

  // Mantissa and fraction together: pclk / baud, rounded to nearest
  USART_BRR(uart->base).raw = (pclk_hz + baud / 2U) / baud;
  USART_CR1(uart->base).bitfield.ue = 1;
  USART_CR1(uart->base).bitfield.te = 1; // sends an idle frame first
}

void UartPutc(const Uart *uart, char c)
{
  while (!USART_SR(uart->base).bitfield.txe) {}
  USART_DR(uart->base) = (uint8_t)c;
}

void UartPuts(const Uart *uart, const char *s)
{
  while (*s) {
    if (*s == '\n') {
      UartPutc(uart, '\r');
    }
    UartPutc(uart, *s++);
  }
}

void UartPutDec(const Uart *uart, uint32_t v)
{
  char buf[10]; // 4294967295 has 10 digits
  int i = 0;

  if (v == 0) {
    UartPutc(uart, '0');
    return;
  }
  while (v > 0) {
    buf[i++] = (char)('0' + (v % 10U));
    v /= 10U;
  }
  while (i > 0) {
    UartPutc(uart, buf[--i]);
  }
}
