package com.helloar

import com.facebook.jni.HybridData
import com.facebook.soloader.SoLoader
import androidx.annotation.Keep
import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.view.Surface
import androidx.core.content.ContextCompat

@Keep
class HelloAppSystem { // <--- Changed from 'object' to 'class'

  // This is now a standard instance field, which JNI can find easily.
  @Keep
  private val mHybridData: HybridData = initHybrid()

  private external fun initHybrid(): HybridData
  external fun onPause()

  // Native entrypoint; call via Kotlin wrapper below.
  private external fun onResumeNative(context: Context, activity: Activity, hasCameraPermission: Boolean)

  fun onResume(context: Context, activity: Activity) {
    // Only check permission when an ARView is actually mounted, i.e. when the session
    // could realistically be created/resumed.
    val shouldCheckPermission = ARViewRegistry.hasActiveViews()
    val hasCameraPermission = if (shouldCheckPermission) {
      ContextCompat.checkSelfPermission(context, Manifest.permission.CAMERA) ==
          PackageManager.PERMISSION_GRANTED
    } else {
      true
    }

    if (shouldCheckPermission && !hasCameraPermission) {
      // Client is responsible for requesting permissions. We only report the error.
      ARViewRegistry.emitCameraPermissionDenied()
    }

    onResumeNative(context, activity, hasCameraPermission)
  }

  // View lifecycle coordination
  external fun onARViewMounted()
  external fun onARViewUnmounted()

  // Explicit teardown (session is otherwise persistent)
  external fun destroySession()

  external fun onSurfaceCreated(reactTag: Int, surface: Surface)
  external fun onSurfaceChanged(reactTag: Int, width: Int, height: Int)
  external fun onSurfaceDestroyed(reactTag: Int)

  // GLSurfaceView.Renderer callbacks (GL thread)
  external fun onGlSurfaceCreated(reactTag: Int)
  external fun onGlSurfaceChanged(reactTag: Int, width: Int, height: Int)
  external fun onGlDrawFrame(reactTag: Int)

  // Gestures
  external fun onGestureTap(reactTag: Int, x: Float, y: Float)

  external fun setPaused(reactTag: Int, paused: Boolean)
  external fun setPlaneDetectionEnabled(reactTag: Int, enabled: Boolean)
  external fun setLightEstimationEnabled(reactTag: Int, enabled: Boolean)

  // Flattened config setters (no JSON)
  external fun setSessionType(reactTag: Int, sessionType: String)
  external fun setDepthMode(reactTag: Int, depthMode: String)
  external fun setCloudAnchorMode(reactTag: Int, cloudAnchorMode: String)
  external fun setInstantPlacementMode(reactTag: Int, instantPlacementMode: String)
  external fun setLightEstimationMode(reactTag: Int, lightEstimationMode: String)
  external fun setPlaneDetectionMode(reactTag: Int, planeDetectionMode: String)
  external fun setFocusMode(reactTag: Int, focusMode: String)

  external fun setDebugShowPlanes(reactTag: Int, enabled: Boolean)
  external fun setDebugShowPointCloud(reactTag: Int, enabled: Boolean)
  external fun setDebugShowWorldOrigin(reactTag: Int, enabled: Boolean)
  external fun setDebugShowDepthMap(reactTag: Int, enabled: Boolean)

  // Singleton Pattern: Access this via HelloAppSystem.instance
  companion object {
    init {
      // Load library immediately when class is touched
      SoLoader.loadLibrary("appmodules")
    }

    val instance: HelloAppSystem by lazy {
      HelloAppSystem()
    }
  }
}