# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

#
# Makefile for Extended IVC Driver and BPMP driver
#

# Always build the real ivc_ext (extended IVC) module. The
# NV_OOT_IVC_EXT_SKIP_BUILD flag used to swap it for ivc_ext_dummy.o, which
# exports nothing. But the camera rtcpu modules (capture-ivc, camchar,
# camera-diagnostics) and the AON driver call the legacy tegra_ivc_* API
# exported here, so the dummy breaks modpost for them. The real driver's probe
# is a safe no-op on baremetal, so building it unconditionally is correct.
obj-m		+= ivc_ext.o

ifneq ($(NV_OOT_TEGRA_BPMP_SKIP_BUILD),y)
tegra_bpmp-y	+= ../../clk/tegra/clk-bpmp.o
tegra_bpmp-y	+= ../../reset/tegra/reset-bpmp.o
tegra_bpmp-y	+= ../../soc/tegra/powergate-bpmp.o
tegra_bpmp-$(CONFIG_DEBUG_FS)	+= bpmp-debugfs.o
tegra_bpmp-y	+= bpmp-tegra186-hv.o
obj-m 		+= tegra_bpmp.o
else
obj-m 		+= tegra_bpmp.o
tegra_bpmp-objs	+= tegra_bpmp_dummy.o
endif
