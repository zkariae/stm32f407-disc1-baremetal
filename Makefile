# SPDX-License-Identifier: MIT
# /*!
# ****************************
# \file       Makefile
# \brief      Build and flash project images for a board.
#
# \author     Z. BEN-AKKA
# \version    1.1
# \date       12/11/2026
#
# \details
#   Says what to build: the applications, their sources and modules, and the
#   project-wide compiler options. How to build and program the images is the
#   board's: $(BOARD)/board.mk provides the toolchain, the memories and the
#   recipes (board_compile, board_link, board_flash...) that the rules below
#   only call. The rules for every image come from the IMAGE template.
#     make 01-blink            build one application
#     make 01-blink-flash      build and flash it with the ST-LINK
#     make BOARD=<name> ...    build for another board folder
#     make help                everything else
#
# \section root_makefile_version_history Version history
# | Version | Date       | Author      | Description      |
# |:-------:|:----------:|:------------|:-----------------|
# | 1.0     | 12/11/2026 | Z. BEN-AKKA | Initial version. |
# | 1.1     | 12/11/2026 | Z. BEN-AKKA | Add 02-uart.     |
# ****************************
# */

####################################################################
# Board
####################################################################

# Board to build for: a board folder (submodule) at the repository root
BOARD     ?= stm32f407-disc1
BOARD_DIR := $(BOARD)

DEV      := dev
APPS_DIR := $(DEV)/apps
BUILD    := work/build/$(BOARD)

# Project-wide compiler options, added to the board's (CPU, -I for mcu.h and
# board.h): warnings, optimisation, the drivers, header dependency tracking
PROJECT_CFLAGS := -Wall -Wextra -O2 -g3 -I$(DEV)/common -MMD -MP

ifeq ($(wildcard $(BOARD_DIR)/board.mk),)
$(error $(BOARD_DIR)/board.mk not found: run "git submodule update --init", or set BOARD)
endif
include $(BOARD_DIR)/board.mk

.DEFAULT_GOAL := help

####################################################################
# Applications
####################################################################

# One image each, named after the application. <image>_MODULES lists the
# dev/common/<module>/<module>.c files it links, on top of the board sources.
# <image>_MEMORY picks one of the board's memories (default: the board's).
SINGLE := 01-blink 02-uart
$(foreach a,$(SINGLE),$(eval $(a)_IMAGES := $(a)))
$(foreach a,$(SINGLE),$(eval $(a)_SRCS := $(APPS_DIR)/$(a)/main.c))

01-blink_MODULES := delay gpio led
02-uart_MODULES  := delay gpio led uart

APPS   := $(SINGLE)
IMAGES := $(foreach a,$(APPS),$($(a)_IMAGES))

####################################################################
# Templates
####################################################################

# All sources of image $(1): board sources, its own files, its common modules
image_srcs = $(BOARD_SRCS) $($(1)_SRCS) $(foreach m,$($(1)_MODULES),$(DEV)/common/$(m)/$(m).c)

# $(1) = image. Every recipe is the board's, given the memory the image lives in.
define IMAGE
$(1)_MEMORY   ?= $(BOARD_DEFAULT_MEMORY)
$(1)_LDSCRIPT  = $$(MEMORY_$$($(1)_MEMORY)_LDSCRIPT)
$(1)_OUT      := $(BUILD)/$(1)
$(1)_ELF      := $(BUILD)/$(1)/$(1).elf
$(1)_BIN      := $(BUILD)/$(1)/$(1).bin
$(1)_OBJS     := $(patsubst %.c,$(BUILD)/$(1)/%.o,$(notdir $(call image_srcs,$(1))))

$(BUILD)/$(1):
	mkdir -p $$@

$$($(1)_ELF): $$($(1)_OBJS) $$($(1)_LDSCRIPT)
	$$(call board_link,$$@,$$($(1)_OBJS),$$($(1)_MEMORY),$(BUILD)/$(1)/$(1).map)
	$$(call board_size,$$@)

$$($(1)_BIN): $$($(1)_ELF)
	$$(call board_bin,$$@,$$<)

-include $$($(1)_OBJS:.o=.d)

.PHONY: $(1)
$(1): $$($(1)_BIN)

.PHONY: $(1)-flash
$(1)-flash: $$($(1)_BIN)
	$$(call board_flash,$$<,$$($(1)_MEMORY))

.PHONY: $(1)-clean
$(1)-clean:
	rm -rf $(BUILD)/$(1)

.PHONY: $(1)-size
$(1)-size: $$($(1)_ELF)
	$$(call board_size,$$<)

.PHONY: $(1)-disasm
$(1)-disasm: $$($(1)_ELF)
	$$(call board_disasm,$$<)

.PHONY: $(1)-sections
$(1)-sections: $$($(1)_ELF)
	$$(call board_sections,$$<)
endef

# $(1) = image, $(2) = source file
define OBJECT
$(BUILD)/$(1)/$(patsubst %.c,%.o,$(notdir $(2))): $(2) | $(BUILD)/$(1)
	$$(call board_compile,$$@,$$<,$$($(1)_CFLAGS))
endef

$(foreach i,$(IMAGES),$(eval $(call IMAGE,$(i))))
$(foreach i,$(IMAGES),$(foreach s,$(call image_srcs,$(i)),$(eval $(call OBJECT,$(i),$(s)))))

####################################################################
# Everything at once
####################################################################

.PHONY: all
all: $(APPS)

.PHONY: clean
clean:
	rm -rf $(BUILD)

.PHONY: list
list:
	@for a in $(APPS); do echo "  $$a"; done

.PHONY: reset
reset:
	$(call board_reset)

.PHONY: help
help:
	@echo "Usage: make [BOARD=<name>] <target>"
	@echo ""
	@echo "Per application (01-blink, 02-uart):"
	@echo "  <app>                build it"
	@echo "  <app>-flash          build and flash with the ST-LINK"
	@echo "  <app>-clean          remove its build outputs"
	@echo "  <app>-size           flash and RAM usage"
	@echo "  <app>-disasm         disassembly"
	@echo "  <app>-sections       section addresses"
	@echo ""
	@echo "Also:"
	@echo "  all                  build every application"
	@echo "  clean                remove work/build/$(BOARD)"
	@echo "  list                 list applications"
	@echo "  reset                reset the board"
	@echo ""
	@echo "Board: $(BOARD) ($(BOARD_DIR)/board.mk), memories: $(BOARD_MEMORIES)"
	@echo "Applications: $(APPS)"
