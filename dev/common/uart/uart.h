// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/uart/uart.h
\brief      USART transmit API, 8N1.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  Each USART is described by a Uart structure (instance, clock enable and TX
  pin), defined by the application. Output functions block until the data
  is accepted by the transmitter.

\section uart_h_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#ifndef UART_H
#define UART_H

#include <stdint.h>

/// A USART with its transmit pin.
typedef struct {
  uint32_t base;          ///< USART base address (USARTx_BASE).
  volatile uint32_t *enr; ///< RCC enable register of the USART (&RCC_APBxENR.raw).
  uint32_t enr_bit;       ///< Enable bit in that register.
  uint32_t tx_port;       ///< TX pin port base address.
  uint32_t tx_pin;        ///< TX pin number.
  uint32_t tx_af;         ///< TX pin alternate function.
} Uart;

/*!
\brief  Enable the USART and its TX pin, 8N1, transmitter only.
\param  uart     USART to configure.
\param  pclk_hz  Clock of the bus the USART is on, in Hz.
\param  baud     Baud rate; the divider is rounded to nearest.
*/
void UartInit(const Uart *uart, uint32_t pclk_hz, uint32_t baud);

/*!
\brief  Send one character.
\param  uart  USART to use.
\param  c     Character to send.
*/
void UartPutc(const Uart *uart, char c);

/*!
\brief  Send a string, expanding '\\n' to CRLF.
\param  uart  USART to use.
\param  s     NUL-terminated string.
*/
void UartPuts(const Uart *uart, const char *s);

/*!
\brief  Print an unsigned decimal value.
\param  uart  USART to use.
\param  v     Value to print.
*/
void UartPutDec(const Uart *uart, uint32_t v);

#endif
