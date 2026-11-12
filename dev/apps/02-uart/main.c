// SPDX-License-Identifier: MIT
/*!
****************************
\file       02-uart/main.c
\brief      Print a banner and an uptime counter on the USART2 console.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  1) Configures USART2 TX on PA2 (AF7), 115200 8N1, clocked from APB1 at
     16 MHz (HSI reset clock): BRR = 139, baud rate error -0.08 %.
  2) Prints a banner, then an uptime counter every second while
     BOARD_LED_LD4 blinks.
  PA2 goes to the RX input of a 3.3 V USB-serial adapter, GND to GND.

\section uart_main_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include <stdint.h>

#include "board.h"
#include "delay/delay.h"
#include "led/led.h"
#include "uart/uart.h"

#define CPU_HZ   16000000U ///< HSI, the clock at reset.
#define APB1_HZ  CPU_HZ    ///< No APB1 prescaler at reset.
#define BAUDRATE 115200U

/// Console: USART2 TX on PA2, to the USB-serial adapter.
static const Uart console = {
    .base = USART2_BASE,
    .enr = &RCC_APB1ENR.raw,
    .enr_bit = 1U << 17, // USART2EN
    .tx_port = GPIOA_BASE,
    .tx_pin = CONSOLE_TX_PIN,
    .tx_af = GPIO_AF_USART2,
};

/*!
\brief  Print the banner and the console settings.
*/
static void print_banner(void)
{
  UartPuts(&console, "\n");
  UartPuts(&console, "STM32F407VG - bare metal, no HAL\n");
  UartPuts(&console, "--------------------------------\n");

  UartPuts(&console, "SYSCLK   : ");
  UartPutDec(&console, CPU_HZ / 1000000U);
  UartPuts(&console, " MHz (HSI)\n");

  UartPuts(&console, "Baudrate : ");
  UartPutDec(&console, BAUDRATE);
  UartPuts(&console, "\n\n");
}

/*!
\brief  Print the uptime line.
\param  seconds  Seconds since boot.
*/
static void print_uptime(uint32_t seconds)
{
  UartPuts(&console, "uptime ");
  UartPutDec(&console, seconds);
  UartPuts(&console, " s\n");
}

/*!
\brief  Application entry point: print the banner, then an uptime counter.
\return Never returns.
*/
int main(void)
{
  UartInit(&console, APB1_HZ, BAUDRATE);
  LedInit(&BOARD_LED_LD4);

  print_banner();

  uint32_t seconds = 0;
  while (1) {
    LedToggle(&BOARD_LED_LD4);
    DelayCycles(CPU_HZ / 2U); // 0.5 s
    LedToggle(&BOARD_LED_LD4);
    DelayCycles(CPU_HZ / 2U);

    print_uptime(++seconds);
  }
}
