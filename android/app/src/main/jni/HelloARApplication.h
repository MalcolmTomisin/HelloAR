#pragma once

#include <memory>
#include <string>
#include <set>
#include <unordered_map>
#include "ARSessionManager.h"
#include "AndroidPlatformServices.h"
#include <android/asset_manager.h>
#include "utils.h"
#include "arcore_c_api.h"

#include "BackgroundRenderer.h"
#include "glm.h"
#include "PlaneRenderer.h"
#include "Texture.h"

namespace helloar
{
    class HelloARApplication
    {
    public:
        HelloARApplication(std::unique_ptr<AndroidPlatformServices> platformServices,
                           AAssetManager *assetManager)
            : platformServices_(std::move(platformServices)), asset_manager_(assetManager) {}
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

        void OnSurfaceCreated(JNIEnv * /*env*/) {}
        void OnSurfaceChanged(jint /*width*/, jint /*height*/) {}
        void OnSurfaceDestroyed() {}

        void OnGlSurfaceCreated() {}
        void OnGlSurfaceChanged(jint /*width*/, jint /*height*/) {}
        void OnGlDrawFrame() {}
        void OnGestureTap(jfloat /*x*/, jfloat /*y*/) {}

        void SetPaused(jboolean /*paused*/) {}
        void SetPlaneDetectionEnabled(jboolean /*enabled*/) {}
        void SetLightEstimationEnabled(jboolean /*enabled*/) {}

        void SetSessionType(const std::string & /*sessionType*/) {}
        void SetDepthMode(const std::string & /*depthMode*/) {}
        void SetCloudAnchorMode(const std::string & /*cloudAnchorMode*/) {}
        void SetInstantPlacementMode(const std::string & /*instantPlacementMode*/) {}
        void SetLightEstimationMode(const std::string & /*lightEstimationMode*/) {}
        void SetPlaneDetectionMode(const std::string & /*planeDetectionMode*/) {}
        void SetFocusMode(const std::string & /*focusMode*/) {}

        void SetDebugShowPlanes(jboolean /*enabled*/) {}
        void SetDebugShowPointCloud(jboolean /*enabled*/) {}
        void SetDebugShowWorldOrigin(jboolean /*enabled*/) {}
        void SetDebugShowDepthMap(jboolean /*enabled*/) {}

    private:
        std::unique_ptr<AndroidPlatformServices> platformServices_;
        int active_view_count_ = 0;
        bool activity_resumed_ = false;

        glm::mat3 GetTextureTransformMatrix(const ArSession *session,
                                            const ArFrame *frame);
        ArFrame *ar_frame_ = nullptr;

        bool install_requested_ = false;
        bool calculate_uv_transform_ = false;
        int width_ = 1;
        int height_ = 1;
        int display_rotation_ = 0;
        bool is_instant_placement_enabled_ = true;

        AAssetManager *const asset_manager_;

        // The anchors at which we are drawing android models using given colors.
        struct ColoredAnchor
        {
            ArAnchor *anchor;
            ArTrackable *trackable;
            float color[4];
        };

        std::vector<ColoredAnchor> anchors_;

        Texture depth_texture_;

        int32_t plane_count_ = 0;
    };
}