package com.helloar

import com.facebook.jni.HybridData
import com.facebook.soloader.SoLoader
import androidx.annotation.Keep
import android.app.Activity
import android.content.Context

@Keep
class HelloAppSystem { // <--- Changed from 'object' to 'class'

  // This is now a standard instance field, which JNI can find easily.
  @Keep
  private val mHybridData: HybridData = initHybrid()

  private external fun initHybrid(): HybridData
  external fun onPause()
  external fun onResume(context: Context, activity: Activity)

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