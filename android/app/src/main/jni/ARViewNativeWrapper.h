#pragma once

#include <fbjni/fbjni.h>

#include <string>

namespace helloar {

class ARViewNativeWrapper : public facebook::jni::HybridClass<ARViewNativeWrapper> {
 public:
  static constexpr auto kJavaDescriptor = "Lcom/helloar/ARViewNative;";

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jclass> clazz,
      facebook::jni::alias_ref<facebook::jni::JObject> ownerView);

    void onSurfaceCreated();
  void onSurfaceChanged(jint width, jint height);
  void onSurfaceDestroyed();

  void onGlSurfaceCreated();
  void onGlSurfaceChanged(jint width, jint height);
  void onGlDrawFrame();

  void onGestureTap(jfloat x, jfloat y);

  void setPaused(jboolean paused);
  void setPlaneDetectionEnabled(jboolean enabled);
  void setLightEstimationEnabled(jboolean enabled);

  void setSessionType(facebook::jni::alias_ref<facebook::jni::JString> sessionType);
  void setDepthMode(facebook::jni::alias_ref<facebook::jni::JString> depthMode);
  void setCloudAnchorMode(facebook::jni::alias_ref<facebook::jni::JString> cloudAnchorMode);
  void setInstantPlacementMode(
      facebook::jni::alias_ref<facebook::jni::JString> instantPlacementMode);
  void setLightEstimationMode(
      facebook::jni::alias_ref<facebook::jni::JString> lightEstimationMode);
  void setPlaneDetectionMode(
      facebook::jni::alias_ref<facebook::jni::JString> planeDetectionMode);
  void setFocusMode(facebook::jni::alias_ref<facebook::jni::JString> focusMode);

  void setCameraFacing(facebook::jni::alias_ref<facebook::jni::JString> cameraFacing);
  void setCameraTargetFps(facebook::jni::alias_ref<facebook::jni::JString> cameraTargetFps);
  void setCameraDepthSensorUsage(
      facebook::jni::alias_ref<facebook::jni::JString> cameraDepthSensorUsage);

  void setDebugShowPlanes(jboolean enabled);
  void setDebugShowPointCloud(jboolean enabled);
  void setDebugShowWorldOrigin(jboolean enabled);
  void setDebugShowDepthMap(jboolean enabled);

  static void registerNatives();

 private:
  friend HybridBase;
  explicit ARViewNativeWrapper(facebook::jni::alias_ref<facebook::jni::JObject> ownerView);

    void applyCameraConfigIfPossible();

  facebook::jni::global_ref<facebook::jni::JObject> ownerView_;

    // Camera config preferences (flattened strings from JS)
    std::string cameraFacing_ = "back";
    std::string cameraTargetFps_ = "30";
    std::string cameraDepthSensorUsage_ = "useIfAvailable";
};

} // namespace helloar
