# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: Copyright (c) 2022-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

this_makefile_path := $(abspath $(shell dirname $(lastword $(MAKEFILE_LIST))))

ifeq ($(CONFIG_TEGRA_KLEAF_BUILD),y)
include $(this_makefile_path)/Makefile.kleaf
endif

$(info ) # blank line
$(info === nvidia-oot config variables)

# Include kernel specific config
$(info kernel_name = $(kernel_name))
ifneq ($(kernel_name),)
kernel_config := $(this_makefile_path)/configs/Makefile.config.$(kernel_name)
ifneq ($(wildcard $(kernel_config)),)
$(info kernel_config = $(kernel_config))
include $(kernel_config)
endif
endif

# Include system specific config
$(info system_type = $(system_type))
ifneq ($(system_type),)
system_config := $(this_makefile_path)/configs/Makefile.config.$(system_type)
ifneq ($(wildcard $(system_config)),)
$(info system_config = $(system_config))
include $(system_config)
endif
endif

LINUXINCLUDE += -I$(srctree.nvconftest)
LINUXINCLUDE += -I$(srctree.nvidia-oot)/include

subdir-ccflags-y += -Werror
subdir-ccflags-y += -Wmissing-prototypes
# Demote a warning emitted from an upstream kernel header (of.h
# _OF_DECLARE_STUB pointer comparison) that would otherwise fail the -Werror
# build. The kernel source itself is not modified.
subdir-ccflags-y += -Wno-error=compare-distinct-pointer-types
# Force-include a compat header providing strncpy(), which the target kernel
# no longer declares (and no longer exports). See linux/strncpy_compat.h.
subdir-ccflags-y += -include $(srctree.nvidia-oot)/include/linux/strncpy_compat.h

ifeq ($(CONFIG_TEGRA_SYSTEM_TYPE_ACK),y)
subdir-ccflags-y += -DCONFIG_TEGRA_SYSTEM_TYPE_ACK
subdir-ccflags-y += -Wno-sometimes-uninitialized
subdir-ccflags-y += -Wno-parentheses-equality
subdir-ccflags-y += -Wno-enum-conversion
subdir-ccflags-y += -Wno-implicit-fallthrough
endif

obj-m += drivers/

ifdef CONFIG_SND_SOC
obj-m += sound/soc/tegra/
obj-m += sound/tegra-safety-audio/
obj-m += sound/soc/tegra-virt-alt/
endif
