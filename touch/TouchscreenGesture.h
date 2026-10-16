/*
 * Copyright (C) 2019-2022 The LineageOS Project
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

#include <aidl/vendor/lineage/touch/BnTouchscreenGesture.h>

namespace aidl::vendor::lineage::touch::samsung {
class TouchscreenGesture : public BnTouchscreenGesture {
  public:
    static bool isSupported();
    ndk::ScopedAStatus getSupportedGestures(std::vector<Gesture>* gestures) override;
    ndk::ScopedAStatus setGestureEnabled(const Gesture& gesture, bool enabled) override;
};
}  // namespace aidl::vendor::lineage::touch::samsung
