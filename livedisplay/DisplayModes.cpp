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

#include "DisplayModes.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <fstream>
#include <limits>
#include <unistd.h>

namespace aidl::vendor::lineage::livedisplay::samsung {
static constexpr const char* kModePath = "/sys/class/mdnie/mdnie/mode";
static constexpr const char* kModeMaxPath = "/sys/class/mdnie/mdnie/mode_max";
static constexpr const char* kDefaultPath = "/data/vendor/display/.displaymodedefault";

const std::map<int32_t, std::string> DisplayModes::kModeMap = {{2, "Natural"}, {4, "Vivid"}};

DisplayModes::DisplayModes() {
    std::ifstream defaultFile(kDefaultPath);
    int32_t value;
    if (defaultFile >> value && kModeMap.count(value)) {
        mDefaultModeId = value;
        if (!setDisplayMode(value, false).isOk()) {
            LOG(WARNING) << "Could not restore the default display mode";
        }
    }
}

bool DisplayModes::isSupported() {
    return access(kModePath, R_OK | W_OK) == 0;
}

ndk::ScopedAStatus DisplayModes::getDisplayModes(std::vector<DisplayMode>* modes) {
    std::ifstream maxModeFile(kModeMaxPath);
    int32_t maxMode;
    if (!(maxModeFile >> maxMode)) maxMode = std::numeric_limits<int32_t>::max();
    modes->clear();
    for (const auto& [id, name] : kModeMap) {
        if (id < maxMode) {
            DisplayMode mode;
            mode.id = id;
            mode.name = name;
            modes->push_back(mode);
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getCurrentDisplayMode(DisplayMode* mode) {
    std::ifstream modeFile(kModePath);
    int32_t value;
    if (!(modeFile >> value) || !kModeMap.count(value)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    mode->id = value;
    mode->name = kModeMap.at(value);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getDefaultDisplayMode(DisplayMode* mode) {
    mode->id = mDefaultModeId;
    mode->name = kModeMap.at(mDefaultModeId);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    if (!kModeMap.count(modeID)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    if (!::android::base::WriteStringToFile(std::to_string(modeID), kModePath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (makeDefault) {
        if (!::android::base::WriteStringToFile(std::to_string(modeID), kDefaultPath)) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
        }
        mDefaultModeId = modeID;
    }
    return ndk::ScopedAStatus::ok();
}
}  // namespace aidl::vendor::lineage::livedisplay::samsung
