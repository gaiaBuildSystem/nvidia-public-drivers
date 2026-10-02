# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: Copyright (c) 2022-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

#
# Makefile for Hypervisor interface
#
# Always build the real tegra_hv module. The NV_OOT_TEGRA_HV_SKIP_BUILD flag
# used to swap it for a 21-line no-op (tegra_hv_dummy.o) that exports nothing;
# but ~30 other modules (nvadsp, pva, nvmap, tegra_virt_storage, tegra_hv_net,
# nvethernet, nvsciipc, ...) call the tegra_hv IVC API exported by the real
# driver (tegra_hv_ivc_*, is_tegra_hypervisor_mode, tegra_hv_mempool_*), so the
# dummy breaks modpost for all of them. The real driver's probe() is a safe
# no-op on baremetal (it checks is_tegra_hypervisor_mode() before touching any
# hardware), so building it unconditionally is correct for both hv and
# non-hv targets.
obj-m		+= tegra_hv.o
obj-m		+= tegra_hv_pm_ctl.o
ifneq ($(NV_OOT_HVC_SYSFS_SKIP_BUILD),y)
obj-m		+= hvc_sysfs.o
endif
obj-m		+= ivc-cdev.o
obj-m		+= userspace_ivc_mempool.o
obj-m		+= tegra_hv_vcpu_yield.o

