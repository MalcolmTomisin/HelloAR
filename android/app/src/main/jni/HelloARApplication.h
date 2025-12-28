#pragma once

#include <memory>
#include <string>
#include "ARSessionManager.h"
#include "AndroidPlatformServices.h"
#include "utils.h"

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
            activity_resumed_ = false;
            ArSessionManager::Instance().Pause();
        }
        void OnResume(JNIEnv *env, jobject context, jobject activity, jboolean hasCameraPermission)
        {
            activity_resumed_ = true;

            if (active_view_count_ > 0 && !hasCameraPermission)
            {
                // Camera permission is required for ARCore. Do not create/resume the session.
                LOGI("AR Session not started: missing camera permission.");
                return;
            }
            if (!ArSessionManager::Instance().IsInitialized() && active_view_count_ > 0)
            {
                auto canProceed = platformServices_->checkARCoreInstallation(env, context, activity);
                if (!canProceed)
                {
                    return;
                }
                LOGI("Creating AR Session");
                ArSessionManager::Instance().Create(env, context);
                return;
            }

            // Only run the session when there is an active AR view.
            if (active_view_count_ > 0 && ArSessionManager::Instance().IsInitialized())
            {
                ArSessionManager::Instance().Resume();
                return;
            }

            LOGI("AR Session not started: no active AR views.");
        }

        void OnARViewMounted()
        {
            active_view_count_++;
            if (activity_resumed_ && ArSessionManager::Instance().IsInitialized())
            {
                ArSessionManager::Instance().Resume();
            }
        }

        void OnARViewUnmounted()
        {
            if (active_view_count_ > 0)
            {
                active_view_count_--;
            }

            if (active_view_count_ == 0)
            {
                ArSessionManager::Instance().Pause();
            }
        }

        void DestroySession()
        {
            // Client-requested explicit end of session.
            ArSessionManager::Instance().Pause();
            ArSessionManager::Instance().Destroy();
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
        int active_view_count_ = 0;
        bool activity_resumed_ = false;
    };
}