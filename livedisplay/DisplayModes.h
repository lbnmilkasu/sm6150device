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

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnDisplayModes.h>
#include <map>
#include <string>

namespace aidl::vendor::lineage::livedisplay::samsung {
class DisplayModes : public BnDisplayModes {
  public:
    DisplayModes();
    static bool isSupported();
    ndk::ScopedAStatus getDisplayModes(std::vector<DisplayMode>* modes) override;
    ndk::ScopedAStatus getCurrentDisplayMode(DisplayMode* mode) override;
    ndk::ScopedAStatus getDefaultDisplayMode(DisplayMode* mode) override;
    ndk::ScopedAStatus setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    int32_t mDefaultModeId = 4;
    static const std::map<int32_t, std::string> kModeMap;
};
}  // namespace aidl::vendor::lineage::livedisplay::samsung
