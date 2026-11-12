# stm32f407-disc1-baremetal

Register-level, bare-metal C applications for the **STM32F407G-DISC1**
discovery kit (STM32F407VG, Cortex-M4F): no HAL, no CMSIS.

The board side (registers, startup, linker script, build and flash recipes)
lives in its own repository,
[stm32f407-disc1](https://github.com/zkariae/stm32f407-disc1), included here
as a Git submodule. This repository says what to build: the generic drivers
and the applications.

## Layout

```
.
├── Makefile             what to build; recipes come from the board's board.mk
├── stm32f407-disc1/     board support (submodule): mcu.h, board.h, board.mk...
├── dev/
│   ├── .clang-format    code style
│   ├── common/          generic drivers, one folder per module (gpio, delay, led, uart)
│   └── apps/            one folder per application (01-blink, 02-uart)
└── work/                everything generated (builds), ignored by git
```

## Prerequisites

- `arm-none-eabi-gcc` (GNU Arm Embedded toolchain) and `make`
- OpenOCD, to program the board through its on-board ST-LINK
- `clang-format`, to format the sources
- For the console: a 3.3 V USB-serial adapter, its RX on PA2 and GND to GND,
  and a terminal at 115200 8N1 (the ST-LINK virtual COM port is not wired
  to the MCU on this board)

On WSL, attach the ST-LINK and the USB-serial adapter to Linux first, from a
Windows PowerShell: `usbipd attach --wsl --busid <BUSID>` (`usbipd list`
gives the bus IDs).

## Getting the sources

```sh
git clone --recursive git@github.com:zkariae/stm32f407-disc1-baremetal.git
```

After a plain clone, fetch the board submodule with:

```sh
git submodule update --init
```

## Usage

```sh
make help               # every target
make 01-blink           # build one application, in work/build/stm32f407-disc1/
make 01-blink-flash     # build it and flash it with the ST-LINK
```

## Applications

| Application | What it shows                                                     |
|:------------|:------------------------------------------------------------------|
| `01-blink`  | Blinks LD4, the green LED on PD12, on the reset clock             |
| `02-uart`   | Banner and uptime counter on USART2 (PA2, 115200 8N1), LD4 blinks |

## License

MIT, see [LICENSE](LICENSE).
