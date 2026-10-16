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

#include "AdaptiveBacklight.h"

#include <android-base/file.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <unistd.h>

namespace aidl::vendor::lineage::livedisplay::samsung {
static constexpr const char* kBacklightPath = "/sys/class/lcd/panel/alpm";

bool AdaptiveBacklight::isSupported() {
    return access(kBacklightPath, R_OK | W_OK) == 0;
}

ndk::ScopedAStatus AdaptiveBacklight::getEnabled(bool* enabled) {
    std::string value;
    int contents = 0;
    if (!::android::base::ReadFileToString(kBacklightPath, &value) ||
        !::android::base::ParseInt(::android::base::Trim(value), &contents)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    *enabled = contents > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus AdaptiveBacklight::setEnabled(bool enabled) {
    if (!::android::base::WriteStringToFile(enabled ? "2" : "0", kBacklightPath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    return ndk::ScopedAStatus::ok();
}
}  // namespace aidl::vendor::lineage::livedisplay::samsung
