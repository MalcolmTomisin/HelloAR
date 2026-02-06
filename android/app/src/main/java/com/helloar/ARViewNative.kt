package com.helloar

import androidx.annotation.Keep
import com.facebook.jni.HybridData
import com.facebook.soloader.SoLoader

@Keep
class ARViewNative(ownerView: ARView) {

  @Keep
  private val mHybridData: HybridData = initHybrid(ownerView)

  // NOTE: fbjni infers this parameter as java.lang.Object when registering the native
  // method (because the C++ signature uses alias_ref<JObject>). Keep this as Any to
  // avoid runtime NoSuchMethodError during native registration.
  private external fun initHybrid(ownerView: Any): HybridData

  external fun onSurfaceCreated()
  external fun onSurfaceChanged(width: Int, height: Int)
  external fun onSurfaceDestroyed()

  // GLSurfaceView.Renderer callbacks (GL thread)
  external fun onGlSurfaceCreated()
  external fun onGlSurfaceChanged(width: Int, height: Int)
  external fun onGlDrawFrame()

  // Gestures
  external fun onGestureTap(x: Float, y: Float)

  external fun setPaused(paused: Boolean)
  external fun setPlaneDetectionEnabled(enabled: Boolean)
  external fun setLightEstimationEnabled(enabled: Boolean)

  external fun setSessionType(sessionType: String)
  external fun setDepthMode(depthMode: String)
  external fun setCloudAnchorMode(cloudAnchorMode: String)
  external fun setInstantPlacementMode(instantPlacementMode: String)
  external fun setLightEstimationMode(lightEstimationMode: String)
  external fun setPlaneDetectionMode(planeDetectionMode: String)
  external fun setFocusMode(focusMode: String)

  // Camera config (flattened)
  external fun setCameraFacing(cameraFacing: String)
  external fun setCameraTargetFps(cameraTargetFps: String)
  external fun setCameraDepthSensorUsage(cameraDepthSensorUsage: String)

  external fun setDebugShowPlanes(enabled: Boolean)
  external fun setDebugShowPointCloud(enabled: Boolean)
  external fun setDebugShowWorldOrigin(enabled: Boolean)
  external fun setDebugShowDepthMap(enabled: Boolean)

  companion object {
    init {
      // Ensure JNI symbols are available even if HelloAppSystem.instance is never touched.
      SoLoader.loadLibrary("appmodules")
    }
  }
}
