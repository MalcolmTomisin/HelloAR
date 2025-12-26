#include "ARViewNativeWrapper.h"

namespace helloar {

using namespace facebook::jni;

ARViewNativeWrapper::ARViewNativeWrapper(alias_ref<JObject> ownerView)
    : ownerView_(make_global(ownerView)) {}

local_ref<ARViewNativeWrapper::jhybriddata> ARViewNativeWrapper::initHybrid(
    alias_ref<jclass> /*clazz*/,
    alias_ref<JObject> ownerView) {
  return makeCxxInstance(ownerView);
}

void ARViewNativeWrapper::onSurfaceCreated(alias_ref<JObject> /*surface*/) {
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

void ARViewNativeWrapper::setDebugShowPlanes(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowPointCloud(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowWorldOrigin(jboolean /*enabled*/) {}
void ARViewNativeWrapper::setDebugShowDepthMap(jboolean /*enabled*/) {}

void ARViewNativeWrapper::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", ARViewNativeWrapper::initHybrid),

      makeNativeMethod("onSurfaceCreated", "(Landroid/view/Surface;)V", ARViewNativeWrapper::onSurfaceCreated),
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

      makeNativeMethod("setDebugShowPlanes", "(Z)V", ARViewNativeWrapper::setDebugShowPlanes),
      makeNativeMethod("setDebugShowPointCloud", "(Z)V", ARViewNativeWrapper::setDebugShowPointCloud),
      makeNativeMethod("setDebugShowWorldOrigin", "(Z)V", ARViewNativeWrapper::setDebugShowWorldOrigin),
      makeNativeMethod("setDebugShowDepthMap", "(Z)V", ARViewNativeWrapper::setDebugShowDepthMap),
  });
}

} // namespace helloar
