// SPDX-License-Identifier: MIT
/*!
****************************
\file       03-clock/main.c
\brief      Run at 168 MHz from the HSE and show it on the USART2 console.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  1) Switches SYSCLK to the main PLL at 168 MHz (HSE 8 MHz), APB1 at 42 MHz.
  2) Configures USART2 TX on PA2 (AF7), 115200 8N1, from APB1: BRR = 365,
     baud rate error -0.11 %. If the clock setup fails, everything stays on
     the 16 MHz HSI and the banner says so.
  3) Prints a banner, then an uptime counter every second while
     BOARD_LED_LD4 blinks: a readable console and a 1 s period prove the
     clock tree.
  PA2 goes to the RX input of a 3.3 V USB-serial adapter, GND to GND.

\section clock_main_c_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#include <stdint.h>

#include "board.h"
#include "defines.h"
#include "delay/delay.h"
#include "led/led.h"
#include "rcc/rcc.h"
#include "uart/uart.h"

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
\brief  Print the banner and the clock settings.
\param  clock    Result of RccInit168MHz().
\param  cpu_hz   SYSCLK frequency.
\param  apb1_hz  APB1 frequency.
*/
static void print_banner(int clock, uint32_t cpu_hz, uint32_t apb1_hz)
{
  UartPuts(&console, "\n");
  UartPuts(&console, "STM32F407VG - bare metal, no HAL\n");
  UartPuts(&console, "--------------------------------\n");

  UartPuts(&console, "SYSCLK   : ");
  UartPutDec(&console, cpu_hz / 1000000U);
  if (clock == SUCCESS)
    UartPuts(&console, " MHz (HSE + PLL)\n");
  else
    UartPuts(&console, " MHz (HSI), clock setup failed\n");

  UartPuts(&console, "APB1     : ");
  UartPutDec(&console, apb1_hz / 1000000U);
  UartPuts(&console, " MHz\n");

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
\brief  Application entry point: switch to 168 MHz, then an uptime counter.
\return Never returns.
*/
int main(void)
{
  const int clock = RccInit168MHz(HSE_HZ);
  const uint32_t cpu_hz = (clock == SUCCESS) ? SYSCLK_HZ : HSI_HZ;
  const uint32_t apb1_hz = (clock == SUCCESS) ? APB1_HZ : HSI_HZ;

  UartInit(&console, apb1_hz, BAUDRATE);
  LedInit(&BOARD_LED_LD4);

  print_banner(clock, cpu_hz, apb1_hz);

  uint32_t seconds = 0;
  while (1) {
    LedToggle(&BOARD_LED_LD4);
    DelayCycles(cpu_hz / 2U); // 0.5 s
    LedToggle(&BOARD_LED_LD4);
    DelayCycles(cpu_hz / 2U);

    print_uptime(++seconds);
  }
}
