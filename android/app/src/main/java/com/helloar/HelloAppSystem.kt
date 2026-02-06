package com.helloar

import com.facebook.jni.HybridData
import com.facebook.soloader.SoLoader
import androidx.annotation.Keep
import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.content.res.AssetManager
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

  external fun setAssetManager(assetManager: AssetManager)

  // View lifecycle coordination
  external fun onARViewMounted()
  external fun onARViewUnmounted()

  // Explicit teardown (session is otherwise persistent)
  external fun destroySession()

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

  // Flattened config setters (no JSON)
  external fun setSessionType(sessionType: String)
  external fun setDepthMode(depthMode: String)
  external fun setCloudAnchorMode(cloudAnchorMode: String)
  external fun setInstantPlacementMode(instantPlacementMode: String)
  external fun setLightEstimationMode(lightEstimationMode: String)
  external fun setPlaneDetectionMode(planeDetectionMode: String)
  external fun setFocusMode(focusMode: String)

  external fun setDebugShowPlanes(enabled: Boolean)
  external fun setDebugShowPointCloud(enabled: Boolean)
  external fun setDebugShowWorldOrigin(enabled: Boolean)
  external fun setDebugShowDepthMap(enabled: Boolean)

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