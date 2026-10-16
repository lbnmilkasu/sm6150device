/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SunlightEnhancement.h"

#include <android-base/file.h>
#include <android-base/strings.h>
#include <unistd.h>

namespace aidl::vendor::lineage::livedisplay::samsung {
static constexpr const char* kBrightnessPath = "/sys/class/backlight/panel0-backlight/brightness";

bool SunlightEnhancement::isSupported() {
    return access(kBrightnessPath, R_OK | W_OK) == 0;
}

ndk::ScopedAStatus SunlightEnhancement::getEnabled(bool* enabled) {
    std::string brightness;
    if (!::android::base::ReadFileToString(kBrightnessPath, &brightness)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    *enabled = ::android::base::Trim(brightness) == "365";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    if (enabled) {
        // Repeated enables must not replace the saved brightness with the boost value.
        if (previous_brightness.empty() &&
            !::android::base::ReadFileToString(kBrightnessPath, &previous_brightness)) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
        }
        if (!::android::base::WriteStringToFile("365", kBrightnessPath, true)) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
        }
    } else if (!previous_brightness.empty()) {
        if (!::android::base::WriteStringToFile(previous_brightness, kBrightnessPath, true)) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
        }
        previous_brightness.clear();
    }
    return ndk::ScopedAStatus::ok();
}
}  // namespace aidl::vendor::lineage::livedisplay::samsung
