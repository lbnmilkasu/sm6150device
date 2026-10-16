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

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <cstdlib>
#include <string>

#include "AdaptiveBacklight.h"
#include "DisplayModes.h"
#include "SunlightEnhancement.h"

using namespace aidl::vendor::lineage::livedisplay::samsung;

template <typename T>
void registerService() {
    if (!T::isSupported()) {
        LOG(WARNING) << T::descriptor << " is unavailable on this device";
        return;
    }
    auto service = ndk::SharedRefBase::make<T>();
    const std::string instance = std::string(T::descriptor) + "/default";
    CHECK_EQ(AServiceManager_addService(service->asBinder().get(), instance.c_str()), STATUS_OK);
}

int main() {
    // Sysfs controls and saved state are serialized on the joining binder thread.
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    registerService<AdaptiveBacklight>();
    registerService<DisplayModes>();
    registerService<SunlightEnhancement>();
    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
