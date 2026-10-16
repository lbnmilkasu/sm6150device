# SPDX-FileCopyrightText: 2026 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0

# LineageOS framework integrations. Other ROMs must provide these interfaces
# and their SELinux policy or supply their own product integration.
DEVICE_PACKAGE_OVERLAYS += device/samsung/sm6150-common/overlay-lineage

PRODUCT_PACKAGES += \
    SamsungDoze \
    vendor.lineage.health-service.default \
    vendor.lineage.livedisplay-service.samsung-sm6150 \
    vendor.lineage.touch-service.sm6150

# Samsung slate mode: 0 allows charging, 1 disables it. This is a charging
# toggle, not a bypass path. Keep the device's polarity when using the new API.
$(call soong_config_set,lineage_health,charging_control_charging_path,/sys/class/power_supply/battery/batt_slate_mode)
$(call soong_config_set,lineage_health,charging_control_charging_enabled,0)
$(call soong_config_set,lineage_health,charging_control_charging_disabled,1)
$(call soong_config_set_bool,lineage_health,charging_control_supports_bypass,false)
