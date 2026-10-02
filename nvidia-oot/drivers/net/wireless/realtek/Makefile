# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: Copyright (c) 2022-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

# The rtl8822ce and rtl8852ce modules are old Realtek *vendor* out-of-tree
# drivers. They are NOT part of the upstream kernel (upstream 7.2's
# drivers/net/wireless/realtek/ only ships rtl818x/rtlwifi/rtw89/rtl8xxxu), and
# they rely on the pre-6.x cfg80211 ops API (struct net_device *ndev based
# station/key/tx-power ops, no radio_idx/link_id, and net_device based
# cfg80211_new_sta/del_sta) which was removed in the target kernel. Porting
# them is out of scope for this kernel port, so they are excluded from the
# module build. Realtek rtl8822/rtl8852 hardware is instead supported upstream
# via the rtw88 driver.
ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
ifneq ($(NV_OOT_REALTEK_RTL8822CE_SKIP_BUILD),y)
# obj-m += rtl8822ce/
endif

ifneq ($(NV_OOT_REALTEK_RTL8852CE_SKIP_BUILD),y)
# obj-m += rtl8852ce/
endif
endif
