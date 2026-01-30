package com.helloar

import com.facebook.react.common.MapBuilder
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.annotations.ReactProp

class ARViewManager : SimpleViewManager<ARView>() {

  private companion object {
    private const val COMMAND_DESTROY_SESSION = 1
    private const val COMMAND_CAMERA_PERMISSION_GRANTED = 2
  }

  override fun getName(): String = "ARView"

  override fun createViewInstance(reactContext: ThemedReactContext): ARView {
    return ARView(reactContext)
  }

  override fun getCommandsMap(): MutableMap<String, Int> {
    return MapBuilder.of(
      "destroySession",
      COMMAND_DESTROY_SESSION,
      "cameraPermissionGranted",
      COMMAND_CAMERA_PERMISSION_GRANTED,
    )
  }

  override fun receiveCommand(root: ARView, commandId: Int, args: ReadableArray?) {
    when (commandId) {
      COMMAND_DESTROY_SESSION -> {
        HelloAppSystem.instance.destroySession()
      }

      COMMAND_CAMERA_PERMISSION_GRANTED -> {
        val activity = CurrentActivityTracker.getCurrentActivity() ?: return
        HelloAppSystem.instance.onResume(root.context.applicationContext, activity)
      }
    }
  }

  @ReactProp(name = "paused")
  fun setPaused(view: ARView, paused: Boolean) {
    view.setPaused(paused)
  }

  @ReactProp(name = "planeDetection")
  fun setPlaneDetection(view: ARView, enabled: Boolean?) {
    view.setPlaneDetectionEnabled(enabled)
  }

  @ReactProp(name = "lightEstimation")
  fun setLightEstimation(view: ARView, enabled: Boolean?) {
    view.setLightEstimationEnabled(enabled)
  }

  @ReactProp(name = "sessionType")
  fun setSessionType(view: ARView, sessionType: String?) {
    view.setSessionType(sessionType)
  }

  @ReactProp(name = "depthMode")
  fun setDepthMode(view: ARView, depthMode: String?) {
    view.setDepthMode(depthMode)
  }

  @ReactProp(name = "cloudAnchorMode")
  fun setCloudAnchorMode(view: ARView, mode: String?) {
    view.setCloudAnchorMode(mode)
  }

  @ReactProp(name = "instantPlacementMode")
  fun setInstantPlacementMode(view: ARView, mode: String?) {
    view.setInstantPlacementMode(mode)
  }

  @ReactProp(name = "lightEstimationMode")
  fun setLightEstimationMode(view: ARView, mode: String?) {
    view.setLightEstimationMode(mode)
  }

  @ReactProp(name = "planeDetectionMode")
  fun setPlaneDetectionMode(view: ARView, mode: String?) {
    view.setPlaneDetectionMode(mode)
  }

  @ReactProp(name = "focusMode")
  fun setFocusMode(view: ARView, mode: String?) {
    view.setFocusMode(mode)
  }

  @ReactProp(name = "cameraFacing")
  fun setCameraFacing(view: ARView, mode: String?) {
    view.setCameraFacing(mode)
  }

  @ReactProp(name = "cameraTargetFps")
  fun setCameraTargetFps(view: ARView, mode: String?) {
    view.setCameraTargetFps(mode)
  }

  @ReactProp(name = "cameraDepthSensorUsage")
  fun setCameraDepthSensorUsage(view: ARView, mode: String?) {
    view.setCameraDepthSensorUsage(mode)
  }

  @ReactProp(name = "debugShowPlanes")
  fun setDebugShowPlanes(view: ARView, enabled: Boolean?) {
    view.setDebugShowPlanes(enabled)
  }

  @ReactProp(name = "debugShowPointCloud")
  fun setDebugShowPointCloud(view: ARView, enabled: Boolean?) {
    view.setDebugShowPointCloud(enabled)
  }

  @ReactProp(name = "debugShowWorldOrigin")
  fun setDebugShowWorldOrigin(view: ARView, enabled: Boolean?) {
    view.setDebugShowWorldOrigin(enabled)
  }

  @ReactProp(name = "debugShowDepthMap")
  fun setDebugShowDepthMap(view: ARView, enabled: Boolean?) {
    view.setDebugShowDepthMap(enabled)
  }

  override fun getExportedCustomDirectEventTypeConstants(): MutableMap<String, Any> {
    return MapBuilder.builder<String, Any>()
      .put("topARCoreError", MapBuilder.of("registrationName", "onARCoreError"))
      .put(
        "topTrackingStateChange",
        MapBuilder.of("registrationName", "onTrackingStateChange"),
      )
      .put("topPlaneDetected", MapBuilder.of("registrationName", "onPlaneDetected"))
      .put("topPlaneUpdated", MapBuilder.of("registrationName", "onPlaneUpdated"))
      .build()
      .toMutableMap()
  }
}
