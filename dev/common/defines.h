// SPDX-License-Identifier: MIT
/*!
****************************
\file       common/defines.h
\brief      Project-wide definitions, independent of the board.

\author     Z. BEN-AKKA
\version    1.0
\date       12/11/2026

\details
  Return codes shared by every module. Registers come from the board: mcu.h,
  and board wiring from board.h.

\section defines_h_version_history Version history
| Version | Date       | Author      | Description      |
|:-------:|:----------:|:------------|:-----------------|
| 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
****************************
*/

#ifndef DEFINES_H
#define DEFINES_H

/* Return codes */
#define SUCCESS 0x00 ///< Operation completed.
#define FAILURE 0x01 ///< Generic failure.
#define TIMEOUT 0x02 ///< Hardware did not respond in time.

#endif
