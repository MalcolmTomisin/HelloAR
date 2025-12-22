package com.helloar

import android.content.Context
import android.view.SurfaceHolder
import android.view.SurfaceView


class ARView(context: Context) : SurfaceView(context), SurfaceHolder.Callback {


    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        // PASS THE SURFACE TO C++ IMMEDIATELY
        HelloAppSystem.instance.onSurfaceCreated(holder.surface)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
        HelloAppSystem.instance.onSurfaceChanged(w, h)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        HelloAppSystem.instance.onSurfaceDestroyed()
    }
    
    // Props from React are passed directly to C++ via JNI
    fun setPaused(paused: Boolean) {
        HelloAppSystem.instance.setPaused(paused)
    }
}