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

#include "TouchscreenGesture.h"
#include "samsung_touch.h"

#include <android-base/file.h>
#include <unistd.h>

namespace aidl::vendor::lineage::touch::samsung {
bool TouchscreenGesture::isSupported() {
    return access(TSP_CMD_NODE, W_OK) == 0;
}

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* gestures) {
    Gesture gesture;
    gesture.id = 0;
    gesture.name = "Single Tap";
    gesture.keycode = 0x1c7;
    *gestures = {gesture};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    if (gesture.id != 0) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    if (!::android::base::WriteStringToFile(enabled ? "singletap_enable,1" : "singletap_enable,0",
                                           TSP_CMD_NODE, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    return ndk::ScopedAStatus::ok();
}
}  // namespace aidl::vendor::lineage::touch::samsung
