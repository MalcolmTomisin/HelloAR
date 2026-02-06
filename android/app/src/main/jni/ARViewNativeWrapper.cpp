#include "ARViewNativeWrapper.h"

#include "ARSessionManager.h"
#include "utils.h"

namespace helloar {

using namespace facebook::jni;

ARViewNativeWrapper::ARViewNativeWrapper(alias_ref<JObject> ownerView)
    : ownerView_(make_global(ownerView)) {}

local_ref<ARViewNativeWrapper::jhybriddata> ARViewNativeWrapper::initHybrid(
    alias_ref<jclass> /*clazz*/,
    alias_ref<JObject> ownerView) {
  return makeCxxInstance(ownerView);
}

void ARViewNativeWrapper::onSurfaceCreated() {
  // Intentionally no-op for now.
  // This is where you would attach the Surface/ANativeWindow to a renderer.
}

void ARViewNativeWrapper::onSurfaceChanged(jint /*width*/, jint /*height*/) {
  // Intentionally no-op for now.
}

void ARViewNativeWrapper::onSurfaceDestroyed() {
  // IMPORTANT: Do not destroy the global ARSession here.
  // The session should persist across navigation; only detach view resources.
}

void ARViewNativeWrapper::onGlSurfaceCreated() {
  // Intentionally no-op for now.
}

void ARViewNativeWrapper::onGlSurfaceChanged(jint /*width*/, jint /*height*/) {
  // Intentionally no-op for now.
}

void ARViewNativeWrapper::onGlDrawFrame() {
  // Intentionally no-op for now.
}

void ARViewNativeWrapper::onGestureTap(jfloat /*x*/, jfloat /*y*/) {
  // Intentionally no-op for now.
}

void ARViewNativeWrapper::setPaused(jboolean /*paused*/) {}
void ARViewNativeWrapper::setPlaneDetectionEnabled(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setLightEstimationEnabled(jboolean /*enabled*/) {}

void ARViewNativeWrapper::setSessionType(alias_ref<JString> /*sessionType*/) {}
void ARViewNativeWrapper::setDepthMode(alias_ref<JString> /*depthMode*/) {}
void ARViewNativeWrapper::setCloudAnchorMode(alias_ref<JString> /*cloudAnchorMode*/) {}
void ARViewNativeWrapper::setInstantPlacementMode(alias_ref<JString> /*instantPlacementMode*/) {}
void ARViewNativeWrapper::setLightEstimationMode(alias_ref<JString> /*lightEstimationMode*/) {}
void ARViewNativeWrapper::setPlaneDetectionMode(alias_ref<JString> /*planeDetectionMode*/) {}
void ARViewNativeWrapper::setFocusMode(alias_ref<JString> /*focusMode*/) {}

void ARViewNativeWrapper::applyCameraConfigIfPossible() {
  ArSession* session = ArSessionManager::Instance().Get();
  if (session == nullptr) {
    // Session not created yet; preferences will be applied when it exists.
    return;
  }

  // Build a filter from preferences.
  ArCameraConfigFilter* filter = nullptr;
  ArCameraConfigFilter_create(session, &filter);
  if (filter == nullptr) {
    LOGE("Failed to create ArCameraConfigFilter");
    return;
  }

  // Facing.
  ArCameraConfigFacingDirection facing = AR_CAMERA_CONFIG_FACING_DIRECTION_BACK;
  if (cameraFacing_ == "front") {
    facing = AR_CAMERA_CONFIG_FACING_DIRECTION_FRONT;
  }
  ArCameraConfigFilter_setFacingDirection(session, filter, facing);

  // Target FPS.
  uint32_t fpsMask = AR_CAMERA_CONFIG_TARGET_FPS_30;
  if (cameraTargetFps_ == "60") {
    fpsMask = AR_CAMERA_CONFIG_TARGET_FPS_60;
  }
  ArCameraConfigFilter_setTargetFps(session, filter, fpsMask);

  // Depth sensor usage.
  uint32_t depthMask = 0;
  if (cameraDepthSensorUsage_ == "doNotUse") {
    depthMask = AR_CAMERA_CONFIG_DEPTH_SENSOR_USAGE_DO_NOT_USE;
  } else if (cameraDepthSensorUsage_ == "requireAndUse") {
    depthMask = AR_CAMERA_CONFIG_DEPTH_SENSOR_USAGE_REQUIRE_AND_USE;
  } else {
    // useIfAvailable (default): allow both and let ARCore pick the best.
    depthMask = AR_CAMERA_CONFIG_DEPTH_SENSOR_USAGE_REQUIRE_AND_USE |
        AR_CAMERA_CONFIG_DEPTH_SENSOR_USAGE_DO_NOT_USE;
  }
  ArCameraConfigFilter_setDepthSensorUsage(session, filter, depthMask);

  // Get supported configs.
  ArCameraConfigList* list = nullptr;
  ArCameraConfigList_create(session, &list);
  if (list == nullptr) {
    ArCameraConfigFilter_destroy(filter);
    LOGE("Failed to create ArCameraConfigList");
    return;
  }

  ArSession_getSupportedCameraConfigsWithFilter(session, filter, list);

  int32_t size = 0;
  ArCameraConfigList_getSize(session, list, &size);
  if (size <= 0) {
    LOGE(
        "No supported camera configs for facing=%s fps=%s depth=%s",
        cameraFacing_.c_str(),
        cameraTargetFps_.c_str(),
        cameraDepthSensorUsage_.c_str());
    ArCameraConfigList_destroy(list);
    ArCameraConfigFilter_destroy(filter);
    return;
  }

  // Element 0 is documented as the best match for the filter.
  ArCameraConfig* best = nullptr;
  ArCameraConfig_create(session, &best);
  if (best == nullptr) {
    ArCameraConfigList_destroy(list);
    ArCameraConfigFilter_destroy(filter);
    LOGE("Failed to create ArCameraConfig");
    return;
  }
  ArCameraConfigList_getItem(session, list, 0, best);

  ArStatus status = ArSession_setCameraConfig(session, best);
  if (status == AR_ERROR_SESSION_NOT_PAUSED) {
    // ARCore requires the session to be paused when changing camera config.
    auto statusPause = ArSessionManager::Instance().Pause();
    if (statusPause != AR_SUCCESS) {
      LOGE("ArSession_pause failed: %d", static_cast<int>(statusPause));
      ArCameraConfig_destroy(best);
      ArCameraConfig_destroy(best);
      ArCameraConfigList_destroy(list);
      ArCameraConfigFilter_destroy(filter);
      return;
    }
    // ArSession_pause(session);
    status = ArSession_setCameraConfig(session, best);
    if (status == AR_SUCCESS) {
      // Resume only if we paused due to the config call.
      ArSessionManager::Instance().Resume();
    }
  }
  if (status != AR_SUCCESS) {
    LOGE("ArSession_setCameraConfig failed: %d", static_cast<int>(status));
  }

  ArCameraConfig_destroy(best);
  ArCameraConfig_destroy(best);
  ArCameraConfigList_destroy(list);
  ArCameraConfigFilter_destroy(filter);
}

void ARViewNativeWrapper::setCameraFacing(alias_ref<JString> cameraFacing) {
  cameraFacing_ = cameraFacing->toStdString();
  applyCameraConfigIfPossible();
}

void ARViewNativeWrapper::setCameraTargetFps(alias_ref<JString> cameraTargetFps) {
  cameraTargetFps_ = cameraTargetFps->toStdString();
  applyCameraConfigIfPossible();
}

void ARViewNativeWrapper::setCameraDepthSensorUsage(alias_ref<JString> cameraDepthSensorUsage) {
  cameraDepthSensorUsage_ = cameraDepthSensorUsage->toStdString();
  applyCameraConfigIfPossible();
}

void ARViewNativeWrapper::setDebugShowPlanes(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowPointCloud(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowWorldOrigin(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowDepthMap(jboolean /*enabled*/) {}

void ARViewNativeWrapper::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", ARViewNativeWrapper::initHybrid),

  makeNativeMethod("onSurfaceCreated", "()V", ARViewNativeWrapper::onSurfaceCreated),
      makeNativeMethod("onSurfaceChanged", "(II)V", ARViewNativeWrapper::onSurfaceChanged),
      makeNativeMethod("onSurfaceDestroyed", "()V", ARViewNativeWrapper::onSurfaceDestroyed),

      makeNativeMethod("onGlSurfaceCreated", "()V", ARViewNativeWrapper::onGlSurfaceCreated),
      makeNativeMethod("onGlSurfaceChanged", "(II)V", ARViewNativeWrapper::onGlSurfaceChanged),
      makeNativeMethod("onGlDrawFrame", "()V", ARViewNativeWrapper::onGlDrawFrame),

      makeNativeMethod("onGestureTap", "(FF)V", ARViewNativeWrapper::onGestureTap),

      makeNativeMethod("setPaused", "(Z)V", ARViewNativeWrapper::setPaused),
      makeNativeMethod("setPlaneDetectionEnabled", "(Z)V", ARViewNativeWrapper::setPlaneDetectionEnabled),
      makeNativeMethod("setLightEstimationEnabled", "(Z)V", ARViewNativeWrapper::setLightEstimationEnabled),

      makeNativeMethod("setSessionType", "(Ljava/lang/String;)V", ARViewNativeWrapper::setSessionType),
      makeNativeMethod("setDepthMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setDepthMode),
      makeNativeMethod("setCloudAnchorMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setCloudAnchorMode),
      makeNativeMethod("setInstantPlacementMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setInstantPlacementMode),
      makeNativeMethod("setLightEstimationMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setLightEstimationMode),
      makeNativeMethod("setPlaneDetectionMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setPlaneDetectionMode),
      makeNativeMethod("setFocusMode", "(Ljava/lang/String;)V", ARViewNativeWrapper::setFocusMode),

      makeNativeMethod("setCameraFacing", "(Ljava/lang/String;)V", ARViewNativeWrapper::setCameraFacing),
      makeNativeMethod("setCameraTargetFps", "(Ljava/lang/String;)V", ARViewNativeWrapper::setCameraTargetFps),
      makeNativeMethod("setCameraDepthSensorUsage", "(Ljava/lang/String;)V", ARViewNativeWrapper::setCameraDepthSensorUsage),

      makeNativeMethod("setDebugShowPlanes", "(Z)V", ARViewNativeWrapper::setDebugShowPlanes),
      makeNativeMethod("setDebugShowPointCloud", "(Z)V", ARViewNativeWrapper::setDebugShowPointCloud),
      makeNativeMethod("setDebugShowWorldOrigin", "(Z)V", ARViewNativeWrapper::setDebugShowWorldOrigin),
      makeNativeMethod("setDebugShowDepthMap", "(Z)V", ARViewNativeWrapper::setDebugShowDepthMap),
  });
}

} // namespace helloar
