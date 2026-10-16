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
