#pragma once

#include <memory>
#include <string>
#include "ARSessionManager.h"
#include "AndroidPlatformServices.h"

namespace helloar
{
    class HelloARApplication
    {
    public:
        HelloARApplication(std::unique_ptr<AndroidPlatformServices> platformServices)
            : platformServices_(std::move(platformServices)) {}
        ~HelloARApplication()
        {
            ArSessionManager::Instance().Destroy();
        }
        void OnPause()
        {
            ArSessionManager::Instance().Pause();
        }
        void OnResume(JNIEnv *env, jobject context, jobject activity)
        {
            if (!ArSessionManager::Instance().IsInitialized())
            {
                auto canProceed = platformServices_->checkARCoreInstallation(env, context, activity);
                if (!canProceed)
                {
                    return;
                }
                ArSessionManager::Instance().Create(env, context);
            }
        }

        void OnSurfaceCreated(JNIEnv * /*env*/, jint /*reactTag*/, jobject /*surface*/) {}
        void OnSurfaceChanged(jint /*reactTag*/, jint /*width*/, jint /*height*/) {}
        void OnSurfaceDestroyed(jint /*reactTag*/) {}

        void OnGlSurfaceCreated(jint /*reactTag*/) {}
        void OnGlSurfaceChanged(jint /*reactTag*/, jint /*width*/, jint /*height*/) {}
        void OnGlDrawFrame(jint /*reactTag*/) {}
        void OnGestureTap(jint /*reactTag*/, jfloat /*x*/, jfloat /*y*/) {}

        void SetPaused(jint /*reactTag*/, jboolean /*paused*/) {}
        void SetPlaneDetectionEnabled(jint /*reactTag*/, jboolean /*enabled*/) {}
        void SetLightEstimationEnabled(jint /*reactTag*/, jboolean /*enabled*/) {}

        void SetSessionType(jint /*reactTag*/, const std::string & /*sessionType*/) {}
        void SetDepthMode(jint /*reactTag*/, const std::string & /*depthMode*/) {}
        void SetCloudAnchorMode(jint /*reactTag*/, const std::string & /*cloudAnchorMode*/) {}
        void SetInstantPlacementMode(jint /*reactTag*/, const std::string & /*instantPlacementMode*/) {}
        void SetLightEstimationMode(jint /*reactTag*/, const std::string & /*lightEstimationMode*/) {}
        void SetPlaneDetectionMode(jint /*reactTag*/, const std::string & /*planeDetectionMode*/) {}
        void SetFocusMode(jint /*reactTag*/, const std::string & /*focusMode*/) {}

        void SetDebugShowPlanes(jint /*reactTag*/, jboolean /*enabled*/) {}
        void SetDebugShowPointCloud(jint /*reactTag*/, jboolean /*enabled*/) {}
        void SetDebugShowWorldOrigin(jint /*reactTag*/, jboolean /*enabled*/) {}
        void SetDebugShowDepthMap(jint /*reactTag*/, jboolean /*enabled*/) {}

    private:
        std::unique_ptr<AndroidPlatformServices> platformServices_;
    };
}